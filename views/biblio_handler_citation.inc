<?php
// $Id$
class biblio_handler_citation extends views_handler_field {
  function init(&$view, $options) {
    parent::init($view, $options);
    $this->additional_fields['vid'] = array('table' => 'biblio', 'field' => 'vid');

  }
  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['vid'];
    // add the biblio_type field as tid
  }
  function option_definition() {
    $options = parent::option_definition();
    $options['style_name'] = array('default' => biblio_get_style());
    return $options;
  }
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    module_load_include('inc','biblio','biblio.admin');
    $form['style_name'] = array(
      '#type' => 'radios',
      '#title' => t('Style'),
      '#default_value' => $this->options['style_name'],
      '#options' => _biblio_admin_get_styles(),
      '#description' => t('Define the layout of author lists.')
    );
  }
  function pre_render($values) {
    module_load_include('inc','biblio','biblio.contributors');
    module_load_include('inc','biblio','biblio.keywords');

    module_load_include('inc', 'biblio', 'styles/biblio_style_'.$this->options['style_name']);
    $author_options_func = 'biblio_style_'.$this->options['style_name'].'_author_options';
    $this->author_options = $author_options_func();

    $vids = array();
    $this->items = array();

    foreach ($values as $result) {
      $vids[] = $result->{$this->aliases['vid']};
    }

    if (count($vids)) {
      $result = db_query('SELECT n.title, n.status, b.* FROM {node} n
                          LEFT JOIN {biblio} b on n.vid = b.vid
                          WHERE n.vid IN (:vids)',array(':vids' => implode(',', $vids)));
      foreach ($result as $item) {
        // Clean up the $item from vid.
        $vid = $item->vid; //unset($item['vid']);
        if (isset($item->biblio_year)) $item->biblio_year = _biblio_text_year($item->biblio_year);
        $item->biblio_contributors = biblio_load_contributors($item->vid);
        if (variable_get('biblio_hide_bibtex_braces', 0)) {
          $item->title = biblio_remove_brace($item->title);
        }

        $this->items[$vid] = $item;
      }
    }
  }

  function render($values) {
    $vid = $values->{$this->field_alias};
    if (!isset($this->items[$vid])) return NULL;

    $base = variable_get('biblio_base', 'biblio');
    return theme('biblio_entry', array('node' => $this->items[$vid], 'base' => $base, 'style_name' => $this->options['style_name'], 'inline' => $inline));
  }

}