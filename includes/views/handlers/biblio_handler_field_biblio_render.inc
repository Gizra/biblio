<?php

/**
 * @file
 * Contains the message field handler.
 */

/**
 * Views field handler for rendering a bilbio.
 *
 * @ingroup views
 */
class biblio_handler_field_biblio_render extends views_handler_field {

  /**
   * Set default field name to render.
   */
  function option_definition() {
    $options = parent::option_definition();
    // @todo: have another default?
    $options['style_name'] = array('default' => 'citeproc');
    return $options;
  }

  /**
   * Provide form to select a field name to render.
   */
  function options_form(&$form, &$form_state) {
    $options = array();

    foreach (biblio_get_biblio_styles() as $plugin_name => $plugin) {
      $options[$plugin_name] = $plugin['title'];
    }

    // Get all the text fields attached to a message-type.
    $form['style_name'] = array(
      '#type' => 'select',
      '#title' => t('Style name'),
      '#description' => t('Select the style name to render.'),
      '#options' => $options,
      '#default_value' => $this->options['style_name'],
      '#required' => TRUE,
    );

    // @todo: Allow each plugin to define views options.
    parent::options_form($form, $form_state);
  }

  function render($values) {
    $field_alias = $this->field_alias;
    if (empty($values->$field_alias) || !$biblio = biblio_load($values->$field_alias)) {
      return;
    }

    $options = array();
    return $biblio->getText($this->options['style_name'], $options);
  }
}
