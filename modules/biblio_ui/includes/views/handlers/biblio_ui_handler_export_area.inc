<?php
class yourmodule_handler_collapsible_area_text extends views_handler_area_text {
  function render($empty = FALSE) {
// Here you just return a string of your content you want.
    if ($render = parent::render($empty)) {
      $element = array(
        '#type' => 'fieldset',
        '#title' => t('Title'),
        '#value' => $render,
      );
      $output = theme('fieldset', $element);
      return $output;
    }
  }
}
