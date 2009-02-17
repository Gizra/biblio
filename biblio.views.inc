<?php

/**
 * Implementation of hook_views_handlers().
 */
function biblio_views_handlers() {
  return array(
    'handlers' => array(
      'biblio_handler_field' => array(
        'parent' => 'views_handler_field',
      ),
      'biblio_handler_field_contributor' => array(
        'parent' => 'biblio_handler_field',
      ),
/*
      'biblio_handler_filter_contributor' => array(
        'parent' => 'views_handler_filter_many_to_one',
      ),
      'biblio_handler_filter_role' => array(
        'parent' => 'views_handler_filter_many_to_one',
      ),
*/
    )
  );
}

/**
 * Implementation of hook_views_data().
 *
 * Exposes all fields to the views system.
 */
function biblio_views_data() {
  $viewsdata = array();

  /**************** biblio table **************/
  $data = array();
  // we make a new group
  $data['table']['group'] = t('Biblio');
  // advertise this table as a possible base table
  $data['table']['base'] = array(
    'field' => 'vid',
    'title' => t('Biblio'),
    'help' => t("Biblio publications."),
  );

  // add fields
  $result = db_query('SELECT f.name,f.type,ftd.title,ft.ftdid FROM {biblio_fields} f 
                      INNER JOIN {biblio_field_type} AS ft ON ft.fid = f.fid
                      INNER JOIN {biblio_field_type_data} ftd ON ft.ftdid = ftd.ftdid
                      WHERE ft.tid=0');
  while ($field = db_fetch_array($result)){
    $data[$field['name']] = array(
      'title' => $field['title'], 
      'help'  => "Display the " . $field['title'], 
      'field' => array(
        'handler' => 'biblio_handler_field',
      ),
    );
    // for contrib_widgets we use a special handler:
    if ($field['type'] == 'contrib_widget') {
      $data[$field['name']]['field'] = array(
        'handler' => 'biblio_handler_field_contributor',
        'auth_category' => $field['ftdid'],
      );    
    }
  }

  $viewsdata['biblio'] = $data;
  return $viewsdata;
}

/**
 * Use hook_views_data_alter to add items to the node and node_revisions tables
 * This allows to join these tables, when biblio is the base table.
 */
function biblio_views_data_alter(&$data) {
  $data['node']['table']['join']['biblio'] = array(
     'left_field' => 'vid',
     'field' => 'vid',
     'type' => 'INNER',
     );
  $data['node_revisions']['table']['join']['biblio'] = array(
     'left_field' => 'vid',
     'field' => 'vid',
     'type' => 'INNER',
     );
  $data['users']['table']['join']['biblio'] = array(
     'left_table' => 'node',
     'left_field' => 'uid',
     'field' => 'uid',
     'type' => 'INNER', // all nodes have an author.
  );
}