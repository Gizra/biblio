<?php

/**
 * Biblio class.
 */
class Biblio extends Entity {

  public function __construct($values = array()) {
    parent::__construct($values, 'biblio');
  }

  public function buildContent($view_mode = 'full', $langcode = NULL, $content = array()) {
    // Add the export if it is visible.
    $fields = field_extra_fields_get_display('biblio', $this->type, $view_mode);
    if (isset($fields['export']) && $fields['export']['visible']) {
      $variables = array();
      $variables['bid'] = $this->bid;

      $content['export'] = array(
        '#markup' => theme('biblio_ui_entity_export_area', $variables),
        '#weight' => $fields['export']['weight'],
      );
    }

    if (isset($fields['biblio_get_text']) && $fields['biblio_get_text']['visible']) {
      $options = variable_get('biblio_views_biblio_render_options', array());
      $content['biblio_get_text'] = array(
        '#markup' => $this->getText(variable_get('biblio_default_style', 'citeproc'), $options),
        '#weight' => $fields['biblio_get_text']['weight'],
      );
    }

    $content['#theme'] = 'biblio';

    // Calling parent::buildContent() won't work, since we need to send the
    // content as well.
    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }

  /**
   * Override Entity:save().
   *
   * Clear biblio cache.
   */
  public function save() {
    $wrapper = entity_metadata_wrapper('biblio', $this);

    if (!$wrapper->biblio_status->value() && $wrapper->biblio_year->value()) {
      // If Year is populated and Status is not, the status should be Published.
      $wrapper->biblio_status->set('published');
    }

    if ($wrapper->biblio_status->value() != 'published') {
      // Set the year to 0, to non-published publications, so it's easier to
      // group them by year.
      $wrapper->biblio_year->set(0);
    }

    $this->md5 = BiblioStyleBase::generateBiblioMd5($this);

    if (empty($this->_skip_cache)) {
      $this->cache = '';
      $this->cache_id = '';
    }

    if (!$wrapper->biblio_status->value() && $wrapper->biblio_year->value()) {
      // If Year is populated and Status is not, the status should be Published.
      $wrapper->biblio_status->set('published');
    }

    parent::save();
  }

  /**
   * Get the text, rendered using a Biblio style plugin.
   *
   * @param $style_name
   *   The Biblio style plugin.
   * @param $options
   *   Optional; Array with settings specific to the select style name.
   * @param $skip_cache
   *   Optional; Determine if cache values should be skipped . Defaults to
   *   FALSE.
   */
  public function getText($style_name, $options = array(), $skip_cache = FALSE) {
    if (!$plugin = biblio_get_biblio_style($style_name)) {
      // @todo: Add own exception.
      throw new Exception(format_string('Biblio style "@style" does not exist.', array('@style' => $style_name)));
    }

    foreach ($plugin['assets']['js'] as $js_file) {
      drupal_add_js($js_file);
    }
    foreach ($plugin['assets']['css'] as $css_file) {
      drupal_add_css($css_file);
    }

    if (!variable_get('biblio_debug') && $output = $this->getCache($style_name, $options, $skip_cache)) {
      return $output;
    }

    $plugin['options'] = drupal_array_merge_deep($plugin['options'], $options);

    $class = ctools_plugin_load_class('biblio', 'biblio_style', $style_name, 'class');

    $biblio_style = new $class($plugin, $this);
    $output = $biblio_style->render($options);

    // @todo: We do this because the title might have link. Is there a nicer way?
    $this->title = strip_tags($this->title);

    if ($skip_cache) {
      return $output;
    }

    // Cache the output.
    $this->cache = $output;
    $this->cache_id = md5(serialize(array($style_name, $options)));

    // Make sure the cache isn't cleared on save.
    $this->_skip_cache = TRUE;

    $wrapper = entity_metadata_wrapper('biblio', $this);

    if (isset($wrapper->biblio_first_letter)) {
      // Assign first letter to ease grouping.
      $trimmed = trim(strip_tags($output), ' “');
      $wrapper->biblio_first_letter->set(strtoupper(drupal_substr($trimmed, 0, 1)));
    }

    $this->save();

    return $output;

  }

  /**
   * Helper function to get cached values.
   *
   * @param $style_name
   * @param array $options
   * @param bool $skip_cache
   */
  private function getCache($style_name, $options = array(), $skip_cache = FALSE) {
    if ($skip_cache) {
      return;
    }

    if (empty($this->cache)) {
      return;
    }

    // Try to get the values from the cache.
    $cache_id = md5(serialize(array($style_name, $options)));
    if ($this->cache_id != $cache_id) {
      return;
    }

    return $this->cache;
  }

  /**
   * Build the uri of the biblio.
   */
  public function uri() {
    return array('path' => 'biblio/' . $this->bid);
  }

  /**
   * Add contributors to the field collection of a Biblio.
   *
   * @param $names
   *   String with name(s) of the contributors, or an array of Biblio
   *   contributor objects.
   * @param $role
   *   The role of the contributor (e.g "Author", "Editor").
   */
  public function addContributors($names, $role = 'Author') {
    $contributors = is_string($names) ? BiblioContributorUtility::getBiblioContributorsFromNames($names) : $names;

    foreach ($contributors as $contributor) {
      $field_collection = entity_create('field_collection_item', array('field_name' => 'contributor_field_collection'));
      $field_collection->setHostEntity('biblio', $this);
      $collection_wrapper = entity_metadata_wrapper('field_collection_item', $field_collection);
      $collection_wrapper->biblio_contributor->set($contributor);

      // @todo: Add reference to correct term.
      if ($term = taxonomy_get_term_by_name(ucfirst($role), 'biblio_roles')) {
        $term = reset($term);
      }
      else {
        // Create a new term.
        $vocabulary = taxonomy_vocabulary_machine_name_load('biblio_roles');
        $values = array(
          'name' => ucfirst($role),
          'vid' => $vocabulary->vid,
        );
        $term = entity_create('taxonomy_term', $values);
        taxonomy_term_save($term);
      }

      $collection_wrapper->biblio_contributor_role->set($term);
    }
  }
}
