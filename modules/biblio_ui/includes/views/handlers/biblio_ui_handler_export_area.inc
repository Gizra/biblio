<?php
/**
 * @file
 * Contains biblio_ui_handler_export_area
 */
class biblio_ui_handler_export_area extends views_handler_area {
  function option_definition() {
    $options = parent::option_definition();

    $options['max_items'] = array('default' => 0);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['max_items'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum items'),
      '#description' => t('Limit number of exported items (0 for unlimited).'),
      '#default_value' => $this->options['max_items'],
    );
  }

  function render($empty = FALSE) {
    $variables = array();
    $variables['view_name'] = $this->view->name;
    $variables['display_name'] = $this->view->current_display;
    $variables['max_items'] = $this->options['max_items'];
    return theme('biblio_ui_view_export_area', $variables);
  }
}
