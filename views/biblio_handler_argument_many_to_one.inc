<?php

/**
 * Argument handler for 'associated tables'. (Used for types, authors, keywords)
 */
class biblio_handler_argument_many_to_one extends views_handler_argument_many_to_one {

  /**
   * Override the behavior of title_query(). Get the corresponding names (instead of the ID values).
   */
  function title_query() {
    $names = array();
    $result = db_query('SELECT ' . $this->name_field . ' FROM {' . $this->name_table
      . '} WHERE ' . $this->real_field . ' IN ('. db_placeholders($this->value) .')', $this->value);
    while ($row = db_fetch_object($result)) {
      $names[] = $row->{$this->name_field};
    }
    return !empty($names) ? $names : array(t('Invalid input'));
  }
}

