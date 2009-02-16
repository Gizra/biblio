<?php

class biblio_handler_field_contributor extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['vid'] = array('table' => 'biblio', 'field' => 'vid');
    $this->auth_category = isset($this->definition['auth_category']) ? $this->definition['auth_category'] : 1;
    $this->style_name = isset($this->definition['style_name']) ? $this->definition['style_name'] : 'ieee';
  }

  function options(&$options) {
    parent::options($options);
    $options['label'] = '';
  }

  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['vid'];
  }

  function pre_render($values) {
    module_load_include('inc','biblio','biblio_style_'.$this->style_name);

    $vids = array();
    $this->items = array();

    foreach ($values as $result) {
      $vids[] = $result->{$this->aliases['vid']};
    }

    if ($vids) {
      $result = db_query('SELECT * FROM {biblio_contributor} bc
                          INNER JOIN {biblio_contributor_data} bcd ON bc.cid = bcd.cid
      						  WHERE vid IN ('. implode(',', $vids) .')'.
                         ' AND auth_category='.$this->auth_category.
                         ' ORDER BY bc.rank');
      while ($item = db_fetch_array($result)) {
        // Clean up the $item from vid.
        $vid = $item['vid']; unset($item['vid']);
        $this->items[$vid][] = $item;
      }
    }
  }

  function render($values) {
    $vid = $values->{$this->field_alias};
    if (!isset($this->items[$vid])) return NULL;
    return theme('biblio_authors', $this->items[$vid], 
                 $this->style_name, $this->auth_category);
  }
}