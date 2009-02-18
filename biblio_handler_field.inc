<?php

class biblio_handler_field extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields += array(
      'biblio_type' => 'biblio_type',
    );
    $result = db_query("SELECT bft.tid, bftd.title FROM {biblio_field_type} bft
												INNER JOIN {biblio_fields} bf ON bft.fid=bf.fid AND bf.name='%s'
												INNER JOIN {biblio_field_type_data} bftd ON bftd.ftdid=bft.ftdid", 
                        $this->field);
    while($label = db_fetch_object($result)) {
      $this->labels[$label->tid] = $label->title;
    }
  }
    
  function option_definition() {
    $options = parent::option_definition();
    $options['biblio_label'] = array('default' => TRUE);
    return $options;
  }
  function options_form(&$form, &$form_state) {
    $form['biblio_label'] = array(
      '#type' => 'checkbox',
      '#title' => 'Use label specific to biblio type',
      '#description' => 'Check this option to use the type-specific field labels as defined in '
      .l(t('biblio settings'),'admin/settings/biblio/fields/type'),
      '#default_value' => $this->options['biblio_label'],
    );
    parent::options_form(&$form, &$form_state);
    $form['label'] += array(
      '#process' => array('views_process_dependency'),
			'#dependency' => array(
        'edit-options-biblio-label' => array(0),
      ),
    );
  }
  
  function render($values) {
//    $tid = $values->biblio_type;
//    $this->options['label'] = isset($this->labels[$tid]) ? $this->labels[$tid] : $this->labels[0]; 
    return parent::render($values);
  }
}