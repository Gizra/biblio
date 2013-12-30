<?php
class biblio_ui_handler_export_area extends views_handler_area {
  function render($empty = FALSE) {
// Here you just return a string of your content you want.
    if ($render = parent::render($empty)) {
      $element = array(
        '#type' => 'fieldset',
        '#title' => t('Title'),
        '#value' => $render,
      );
      $output = "hello world";//theme('fieldset', $element);
      return $output;
    }
  }
}
