<?php
class biblio_ui_handler_export_area extends views_handler_area {
  function render($empty = FALSE) {
    $element = array(
      '#type' => 'fieldset',
      '#title' => t('Title'),
      '#value' => "Hello world",
    );
    $output = theme('biblio_ui_export_area', $element);
    return $output;
  }
}
