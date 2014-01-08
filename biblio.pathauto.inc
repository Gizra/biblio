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

      return (object) $settings;
    default:
      break;
  }
}
