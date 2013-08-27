<?php
/**
 * @file
 * Admin UI functionality file for the biblio UI module.
 */

/**
 * Page callback; Form for biblio settings.
 */
function biblio_ui_admin_form($form, &$form_state) {
  $form['biblio_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode'),
    '#description' => t('Disable biblio caching.'),
    '#default_value' => variable_get('biblio_debug'),
  );

  return system_settings_form($form);
}
