<?php

/**
 * @file
 * EndNote tagged biblio style.
 */

class BiblioStyleEndNote extends BiblioStyleBase {

  public function settingsForm() {
    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#required' => TRUE,
      '#options' => array(
        'tagged' => t('Tagged'),
        'xml' => t('XML'),
      ),
      '#default_value' => 'tagged',

    );

    return $form;
  }

  /**
   * @todo: Add import options defaults.
   */
  public function import($data, $options = array()) {
    $options += array(
      'type' => 'tagged',
    );
    if ($options['type'] == 'tagged') {
      return $this->importTagged($data, $options);
    }
  }

  public function importTagged($data, $options = array()) {
    $biblios = array();

    $data = str_replace("\r\n", "\n", $data);
    $data = explode("\n", $data);

    foreach ($data as $row) {
      if (empty($row)) {
        // Empty line.
        continue;
      }
      $tag = substr($row, 0, 2);
      $value = substr($row, 3);

      if ($tag == '%0') {
        $type = strtolower(str_replace(array(' ', '-'), '_', $value));

        $biblio = biblio_create($type);
        $wrapper = entity_metadata_wrapper('biblio', $biblio);

        continue;
      }

      $map = $this->getMapping();
      if (empty($map[$tag])) {
        continue;
      }

      $method = $map[$tag]['import_method'];
      $this->{$method}($wrapper, $tag, $value);
    }

    // @todo: Check md5.
    $wrapper->save();
    $biblios[] = $biblio;
    return $biblios;
  }

  private function importEntryGeneric($wrapper, $tag, $value) {
    $map = $this->getMapping();
    $key = $map[$tag]['property'];
    $wrapper->{$key}->set($value);
  }

  /**
   * Create Biblio Contributor entities.
   */
  private function importEntryContributors($wrapper, $tag, $value) {
    switch ($tag) {
      case '%A':
        $type = 'author';
        break;

      case '%E':
        $type = 'editor';
        break;

      case '%Y':
        // @todo: Find the role.
        break;

      case '%?':
        // @todo: Find the role.
        break;
    }

    $biblio = $wrapper->value();

    // @todo: Add $this->getBiblioContributorsFromNames() to get
    // new $biblio_contributors or existing.

    // split names.
    $names = preg_split("/(and|&)/i", trim($value));
    foreach ($names as $name) {
      // Try to extract the given and family name.
      // @todo: Fix this preg_split.
      $sub_name = preg_split("/{|}/i", $name);
      $values = array('given' =>$sub_name[0]);
      if (!empty($sub_name[1])) {
        $values['family'] = $sub_name[1];
      }

      $biblio_contributor = biblio_contributor_create($values);
      $biblio_contributor->save();

      // Create contributors field collections.
      $field_collection = entity_create('field_collection_item', array('field_name' => 'contributor_field_collection'));
      $field_collection->setHostEntity('biblio', $biblio);
      $collection_wrapper = entity_metadata_wrapper('field_collection_item', $field_collection);
      $collection_wrapper->biblio_contributor->set($biblio_contributor);

      // @todo: Add reference to correct term.
      $term = taxonomy_get_term_by_name(ucfirst($type), 'biblio_roles');
      $term = reset($term);

      $collection_wrapper->biblio_contributor_role->set($term);

      $collection_wrapper->save();
    }
  }

  /**
   * @inheritdoc
   */
  public function render($options = array(), $langcode = NULL) {
    $output = array();

    // We clone the biblio, as we might change the values.
    $biblio = clone $this->biblio;
    $wrapper = entity_metadata_wrapper('biblio', $biblio);

    $output[] = "%0 " . $biblio->type;

    foreach ($this->getMapping() as $tag => $tag_info) {
      $method = $tag_info['render_method'];
      $this->{$method}($output, $wrapper, $tag);
    }

    dpm($output);
    return implode("\r\n", $output);


    switch ($biblio->biblio_type) {
      case 100 :
      case 101 :
      case 103 :
      case 104 :
      case 105 :
      case 108 :
      case 119 :
        if (!empty($biblio->biblio_secondary_title))
          $output[] = "%B " . trim($node->biblio_secondary_title) . "\r\n";
        break;
      case 102 :
        if (!empty($node->biblio_secondary_title))
          $output[] = "%J " . trim($node->biblio_secondary_title) . "\r\n";
        break; // journal
    }
    if (isset($node->biblio_year) && $node->biblio_year < 9998)  $output[] = "%D " . trim($node->biblio_year) . "\r\n";
    if (!empty($node->title))  $output[] = "%T " . trim($node->title) . "\r\n";

    foreach (biblio_get_contributor_category($node->biblio_contributors, 1) as $auth) {
      $output[] = "%A " . trim($auth['name']) . "\r\n";
    }
    foreach (biblio_get_contributor_category($node->biblio_contributors, 2) as $auth) {
      $output[] = "%E " . trim($auth['name']) . "\r\n";
    }
    foreach (biblio_get_contributor_category($node->biblio_contributors, 3) as $auth) {
      $output[] = "%Y " . trim($auth['name']) . "\r\n";
    }
    foreach (biblio_get_contributor_category($node->biblio_contributors, 4) as $auth) {
      $output[] = "%? " . trim($auth['name']) . "\r\n";
    }

    $kw_array = array();
    if (!empty($node->terms)) {
      foreach ($node->terms as $term) {
        $kw_array[] = $term->name;
      }
    }
    if (!empty($node->biblio_keywords)) {
      foreach ($node->biblio_keywords as $term) {
        $kw_array[] = $term;
      }
    }
    if (!empty($kw_array)) {
      $kw_array = array_unique($kw_array);
      foreach ($kw_array as $term) {
        $output[] = "%K " . trim($term) . "\r\n";
      }
    }
    $abst = "";
    if (!empty($node->biblio_abst_e))  $abst .= trim($node->biblio_abst_e);
    if ($abst) {
      $search = array("/\r/", "/\n/");
      $replace = " ";
      $abst = preg_replace($search, $replace, $abst);
      $output[] = "%X " . $abst . "\r\n";
    }
    $skip_fields = array('biblio_year',  'biblio_abst_e', 'biblio_abst_f', 'biblio_type' );
    $fields = drupal_schema_fields_sql('biblio');
    $fields = array_diff($fields, $skip_fields);
    foreach ($fields as $field) {
      if (!empty($node->$field)) {
        $output[] = _biblio_tagged_format_entry($field, $node->$field);
      }
    }
    if (!empty ($node->upload) && count($node->upload['und']) && user_access('view uploaded files')) {
      foreach ($node->upload['und'] as $file) {
        $output[] = "%> " . file_create_url($file['uri']) . "\r\n"; // insert file here.
      }
    }
  }

  /**
   * Generic entry render.
   *
   * @param array $output
   * @param EntityMetadataWrapper $wrapper
   * @param $tag
   */
  private function renderEntryGeneric(&$output = array(), EntityMetadataWrapper $wrapper, $tag) {
    $map = $this->getMapping();
    $property = $map['property'];
    if (!$value = $wrapper->{$property}->value()) {
      return;
    }

    $output[] = "%{$tag} " . $value;
  }

  private function formatEntryKeywords(&$output = array(), EntityMetadataWrapper $wrapper, $tag) {
    foreach ($wrapper->biblio_keywords as $sub_wrapper) {
      $output[] = "%K " . $sub_wrapper->label();
    }
  }




  public function getMapping() {
    $return = array(
      '%A' => array('import_method' => 'importEntryContributors'),
      '%B' => array('property' => 'biblio_secondary_title'),
      '%C' => array('property' => 'biblio_place_published'),
      '%D' => array('property' => 'biblio_year'),
      '%E' => array('import_method' => 'importEntryContributors'),
      '%F' => array('property' => 'biblio_label'),
      '%G' => array('property' => 'language'),
      '%I' => array('property' => 'biblio_publisher'),
      '%J' => array('property' => 'biblio_secondary_title'),
      '%K' => array(
        'property' => 'biblio_keywords',
        'render_method' => 'formatEntryKeywords',
      ),
      '%L' => array('property' => 'biblio_call_number'),
      '%M' => array('property' => 'biblio_accession_number'),
      '%N' => array('property' => 'biblio_issue'),
      '%P' => array('property' => 'biblio_pages'),
      '%R' => array('property' => 'biblio_doi'),
      '%S' => array('property' => 'biblio_tertiary_title'),
      '%T' => array('property' => 'title'),
      '%U' => array('property' => 'biblio_url'),
      '%V' => array('property' => 'biblio_volume'),
      '%X' => array('property' => 'biblio_abstract'),
      '%Y' => array('import_method' => 'importEntryContributors'),
      '%X' => array('property' => 'biblio_notes'),
      '%1' => array('property' => 'biblio_custom1'),
      '%2' => array('property' => 'biblio_custom2'),
      '%3' => array('property' => 'biblio_custom3'),
      '%4' => array('property' => 'biblio_custom4'),
      '%#' => array('property' => 'biblio_custom5'),
      '%$' => array('property' => 'biblio_custom6'),
      '%]' => array('property' => 'biblio_custom7'),
      '%6' => array('property' => 'biblio_number_of_volumes'),
      '%7' => array('property' => 'biblio_edition'),
      '%8' => array('property' => 'biblio_date'),
      '%9' => array('property' => 'biblio_type_of_work'),
      '%?' => array('import_method' => 'importEntryContributors'),
      '%@' => array('property' => 'biblio_isbn'),
      '%<' => array('property' => 'biblio_research_notes'),
      '%!' => array('property' => 'biblio_short_title'),
      '%&' => array('property' => 'biblio_section'),
      '%(' => array('property' => 'biblio_original_publication'),
      '%)' => array('property' => 'biblio_reprint_edition'),
      '%*' => array('property' => ''),
      '%+' => array('property' => ''),
    );

    // Assign default import method.
    foreach ($return as $key => $value) {
      if (empty($value['import_method'])) {
        $return[$key]['import_method'] = 'importEntryGeneric';
      }

      if (empty($value['render_method'])) {
        $return[$key]['render_method'] = 'renderEntryGeneric';
      }
    }

    return $return;

  }

}
