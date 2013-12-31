<?php
class biblio_ui_handler_export_area extends views_handler_area {
  function render($empty = FALSE) {
    $variables = array();
    $variables['view_name'] = $this->view->name;
    $variables['display_name'] = $this->view->current_display;
    return theme('biblio_ui_export_area', $variables);
  }
}
