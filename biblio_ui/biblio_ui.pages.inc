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
    '#type' => 'select',
    '#options' => $options,
    '#ajax' => TRUE,
  );


  $form['biblio'] = array(
    '#type' => 'container',
    '#attached' => array(
      // We use Entity-reference classes to indent the handler settings.
      'css' => array(drupal_get_path('module', 'entityreference') . '/entityreference.admin.css'),
    ),
    '#weight' => 10,
    '#tree' => TRUE,
    '#process' => array(
      '_biblio_ui_form_process_merge_parent',
      '_biblio_ui_field_instance_settings_form',
      '_biblio_ui_field_settings_ajax_process',
    ),
    '#element_validate' => array('_biblio_ui_field_instance_settings_validate'),
  );

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
 * AJAX callback to attach the group membership type fields to the form.
 *
 * Since the controlling logic for populating the form is in the form builder
 * function, all we do here is select the element and return it to be updated.
 */
function biblio_ui_import_settings_ajax_callback(array $form, array &$form_state) {
  return $form['style_settings'];
}


function _biblio_ui_field_instance_settings_form($form, $form_state) {
  _biblio_ui_get_content_sync_elements($form, $form_state);
  if (!empty($form['biblio_settings'])) {
    $form['biblio_settings'] += array(
      '#type' => 'fieldset',
      '#title' => t('Biblio'),
      '#process' => array(
        '_biblio_ui_field_settings_ajax_process',
      ),
    );
  }

  return $form;
}


/**
 * Get the field or instance elements for the field configuration.
 */
function _biblio_ui_get_content_sync_elements(&$element, $form_state) {
  $element['biblio_settings'] = array(
    '#tree' => TRUE,
  );

  $plugin_name = $form_state['values']['plugin_name'];
  dpm($form_state);

  if (!$plugin = biblio_get_biblio_style($plugin_name)) {
    return;
  }

  dpm($plugin);
  return;

  dpm($plugin);

  // Get the handler's settings form.
  $class_name = ctools_plugin_load_class('biblio', 'biblio_style', $plugin_name, 'class');
  // @todo: Add broken class.
  $class_name = class_exists($class_name) ? $class_name : 'ContentSyncBaseBroken';
  $biblio_style = new $class_name($plugin);

  if ($plugin_elements = $biblio_style->settingsForm()) {
    $element['biblio_settings']['settings'] = $plugin_elements;

    $element['biblio_settings']['settings'] += array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('entityreference-settings'),
      ),
    );
  }
}

function _biblio_ui_form_process_merge_parent($element) {
  $parents = $element['#parents'];
  array_pop($parents);
  $element['#parents'] = $parents;
  return $element;
}


function _biblio_ui_field_instance_settings_validate($form, &$form_state) {
  // Store the new values in the form state.
  $instance = $form['#instance'];
  if (isset($form_state['values']['instance'])) {
    $instance = drupal_array_merge_deep($instance, $form_state['values']['instance']);
  }
  $form_state['content_sync']['instance'] = $instance;
}

function _biblio_ui_field_settings_ajax_process($form, $form_state) {
  _biblio_ui_field_settings_ajax_process_element($form, $form);
  return $form;
}


function _biblio_ui_field_settings_ajax_process_element(&$element, $main_form) {
  if (isset($element['#ajax']) && $element['#ajax'] === TRUE) {
    $element['#ajax'] = array(
      'callback' => 'biblio_ui_settings_ajax',
      'wrapper' => $main_form['#id'],
      'element' => $main_form['#array_parents'],
    );
  }

  foreach (element_children($element) as $key) {
    _biblio_ui_field_settings_ajax_process_element($element[$key], $main_form);
  }
}

/**
 * Ajax callback for the handler settings form.
 *
 * @see biblio_ui_field_settings_form()
 */
function biblio_ui_settings_ajax($form, $form_state) {
  $trigger = $form_state['triggering_element'];
  return drupal_array_get_nested_value($form, $trigger['#ajax']['element']);
}

/**
 * Submit handler for the non-JS case.
 */
function biblio_ui_settings_ajax_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}
