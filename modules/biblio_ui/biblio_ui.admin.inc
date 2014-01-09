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

  $options = array();
  foreach (biblio_get_biblio_styles() as $plugin_name => $plugin) {
    $options[$plugin_name] = $plugin['title'];
  }
  $form['biblio_default_style'] = array(
    '#type' => 'select',
    '#title' => t('Biblio default style'),
    '#description' => t('Select the default text style you would like to display in the rendered Biblio field.'),
    '#default_value' => variable_get('biblio_default_style', 'citeproc'),
    '#options' => $options,
  );

  return system_settings_form($form);
}
