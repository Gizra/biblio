<?php
/**
 * @file
 * Pathauto integration for Biblio modules.
 *
 * @ingroup pathauto
 */

/**
 * Implements hook_pathauto().
 */
function biblio_pathauto($op) {
  switch ($op) {
    case 'settings':
      $settings = array();
      $settings['module'] = 'biblio';
      $settings['token_type'] = 'biblio';
      $settings['groupheader'] = t('Biblio paths');
      $settings['patterndescr'] = t('Default path pattern.');
      $settings['patterndefault'] = 'biblio/[biblio:title]';
      // todo: Handle when updating a biblio.
//      $settings['batch_update_callback'] = 'node_pathauto_bulk_update_batch_process';
//      $settings['batch_file'] = drupal_get_path('module', 'pathauto') . '/pathauto.pathauto.inc';

      return (object) $settings;
    default:
      break;
  }
}
