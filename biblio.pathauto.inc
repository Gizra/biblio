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
  if ($op != 'settings') {
    return;
  }

  $settings = new stdClass();
  $settings->module = 'biblio';
  $settings->token_type = 'biblio';
  $settings->groupheader = t('Biblio paths');
  $settings->patterndescr = t('Default path pattern.');
  $settings->patterndefault = 'biblio/[biblio:title]';

  return $settings;
}
