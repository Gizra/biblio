<?php
/**
 * @file
 * integration with the views module.
 */

/**
 * Implements hook_views_data()
 */
function biblio_contributor_ui_views_data() {
  $data['biblio_contributor']['created']['field']['handler'] = 'views_handler_field_date';
  $data['biblio_contributor']['changed']['field']['handler'] = 'views_handler_field_date';

  $data['biblio_contributor']['delete'] = array(
    'title' => t('Delete'),
    'help' => t('Link to delete the biblio contributor entry.'),
    'field' => array(
      'handler' => 'biblio_contributor_ui_handler_delete',
    ),
  );

  $data['biblio_contributor']['edit'] = array(
    'title' => t('Edit'),
    'help' => t('Link to edit the biblio contributor entry.'),
    'field' => array(
      'handler' => 'biblio_contributor_ui_handler_edit',
    ),
  );

  return $data;
}


