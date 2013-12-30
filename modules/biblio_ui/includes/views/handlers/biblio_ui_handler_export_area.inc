<?php
class biblio_ui_handler_export_area extends views_handler_area {
  function render($empty = FALSE) {
    $element = array(
      '#type' => 'fieldset',
      '#title' => t('Title'),
      '#value' => "Hello world",
    );
    $output = theme('fieldset', $element);
    return $output;
  }
}
