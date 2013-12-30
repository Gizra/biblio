<?php

/**
 * Implements hook_views_data()
 */
function biblio_ui_views_data() {
  $data['biblio']['created']['field']['handler'] = 'views_handler_field_date';
  $data['biblio']['changed']['field']['handler'] = 'views_handler_field_date';

  $data['biblio_contributor']['created']['field']['handler'] = 'views_handler_field_date';
  $data['biblio_contributor']['changed']['field']['handler'] = 'views_handler_field_date';

  $data['biblio']['delete'] = array(
    'title' => t('Delete'),
    'help' => t('Link to delete the biblio entry.'),
    'field' => array(
      'handler' => 'biblio_ui_handler_delete',
    ),
  );

  $data['biblio']['edit'] = array(
    'title' => t('Edit'),
    'help' => t('Link to edit the biblio entry.'),
    'field' => array(
    'handler' => 'biblio_ui_handler_edit',
    ),
  );

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

  $data['bibilio']['biblio_export'] = array(
    'title' => t('Biblio export UI'),
    'help' => t('Provide UI for exporting Biblio.'),
    'area'=> array(
      'hanlder' => 'biblio_ui_handler_export_area',
    ),
  );
  return $data;
}



