<?php

/**
 * @file
 */


/**
 * Import biblios.
 */
function biblio_ui_import_biblio($form, &$form_state) {
  $options = array();
  foreach (biblio_get_biblio_styles() as $plugin_name => $plugin) {
    if (empty($plugin['import'])) {
      continue;
    }

    $options[$plugin_name];
  }
  
  $form['biblio_style'] = array(
    '#type' => 'select',
    '#title' => t('Biblio style'),
    '#required' => TRUE,
    '#options' => $options,
  );
  return $form;


}