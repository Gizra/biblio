<?php
// $Id$
class biblio_handler_filter_type extends views_handler_filter_in_operator {

  function construct() {
    parent::construct();
    $this->definition['numeric'] = true;
  }

  function get_value_options() {
    if (!isset($this->value_options)) {
      $result = db_query("SELECT name, tid
              FROM {biblio_types} t
              WHERE t.tid > 0 AND t.visible=1
              ORDER by name");
      $this->value_title = t('Publication type');
      $options = array();
      while ($row = db_fetch_object($result)) {
        $options[$row->tid] =  $row->name;
      }
      $this->value_options = $options;
    }
  }
}
