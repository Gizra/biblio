<?php

class biblio_handler_field extends views_handler_field {
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
}