<?php

/**
 * @file
 */


/**
 * Import biblios.
 */
function biblio_ui_import_biblio($form, &$form_state) {
  $form['import_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Import text'),
    '#required' => TRUE,
  );

  $options = array('' => '-' . t('Select') . '-');
  foreach (biblio_get_biblio_styles() as $plugin_name => $plugin) {
    if (empty($plugin['import'])) {
      continue;
    }

    $class_name = ctools_plugin_load_class('biblio', 'biblio_style', $plugin_name, 'class');
    if (!class_exists($class_name)) {
      continue;
    }

    $options[$plugin_name] = $plugin['title'];
  }

  $form['plugin_name'] = array(
    '#title' => t('Biblio style'),
    '#description' => t('Select the format of the imported text'),
    '#type' => 'select',
    '#options' => $options,
    '#ajax' => array(
      'callback' => 'biblio_ui_import_settings_ajax_callback',
      'wrapper' => 'biblio-ui-style-settings',
    ),
  );

  $form['style_settings'] = array(
    '#prefix' => '<div id="biblio-ui-style-settings">',
    '#suffix' => '</div>',
    '#parents' => array('style_settings'),
    '#type' => 'fieldset',
  );

  if (!empty($form_state['values']['plugin_name'])) {
    $form['style_settings'] += _biblio_ui_add_import_settings($form_state['values']['plugin_name']);
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Helper function; Add the selected Biblio style own settings.
 *
 * @param $form
 * @param $form_state
 */
function _biblio_ui_add_import_settings($plugin_name) {
  $plugin = biblio_get_biblio_style($plugin_name);
  $class_name = ctools_plugin_load_class('biblio', 'biblio_style', $plugin_name, 'class');
  if (!class_exists($class_name)) {
    return;
  }

  $biblio_style = new $class_name($plugin);
  return $biblio_style->settingsForm();
}

/**
 * AJAX callback to attach the group membership type fields to the form.
 *
 * Since the controlling logic for populating the form is in the form builder
 * function, all we do here is select the element and return it to be updated.
 */
function biblio_ui_import_settings_ajax_callback(array $form, array &$form_state) {
  return $form['style_settings'];
}
