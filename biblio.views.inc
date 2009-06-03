<?php
// $Id$
/**
 * Implementation of hook_views_handlers().
 */
function biblio_views_handlers() {
  return array(
    'handlers' => array(
      'biblio_handler_field' => array(
        'parent' => 'views_handler_field',
      ),
      'biblio_handler_citation' => array(
        'parent' => 'views_handler_field',
      ),
      'biblio_handler_field_contributor' => array(
        'parent' => 'biblio_handler_field',
      ),
      'biblio_handler_filter_contributor' => array(
        'parent' => 'views_handler_filter_many_to_one',
      ),
      'biblio_handler_filter_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
/*
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
  // everything belongs to the Biblio group
  $data['table']['group'] = t('Biblio');
//  // advertise this table as a possible base table
//  $data['table']['base'] = array(
//    'field' => 'vid',
//    'title' => t('Biblio'),
//    'help' => t("Biblio publications."),
//  );

  // add fields
  $data['citation'] = array(
    'title' => t('Biblio Citation'),
    'help'  => t("Display the complete citation for a given node"),
    'field' => array(
      'handler' => 'biblio_handler_citation',
    ),
  );
  $result = db_query('SELECT f.name,f.type,ftd.title,ft.ftdid FROM {biblio_fields} f
                      INNER JOIN {biblio_field_type} AS ft ON ft.fid = f.fid
                      INNER JOIN {biblio_field_type_data} ftd ON ft.ftdid = ftd.ftdid
                      WHERE ft.tid=0');

  while ($field = db_fetch_array($result)){
    $data[$field['name']] = array(
      'title'   => $field['title'],
      'help'    => "Display the " . $field['title'],
      'field'   => array('handler' => 'biblio_handler_field'),
      'sort'    => array('handler' => 'views_handler_sort'),
      'filter'  => array('handler' => 'views_handler_filter_string')
    );

    // for contrib_widgets we use a special handler:
    if ($field['type'] == 'contrib_widget') {
      $data[$field['name']]['field'] = array(
        'handler' => 'biblio_handler_field_contributor',
        'auth_category' => $field['ftdid'],
      );
      unset($data[$field['name']]['sort']);
      unset($data[$field['name']]['filter']);
    }
  }
  unset($data['biblio_keywords']['filter']);
  unset($data['biblio_keywords']['sort']);
  $data['biblio_year']['sort'] = array('handler' => 'views_handler_sort');
  $data['biblio_type'] = array(
    'filter'  => array('handler' => 'biblio_handler_filter_type'),
    'sort'    => array('handler' => 'views_handler_sort'),
    'title'   => t('Type of Publication'),
    'help'    => t('The type of publication: Journal, Book, Conference Paper, etc.')
  );

  $data['table']['join'] = array(
    'node' => array(
      // links directly to node via vid
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_revisions' => array(
      // links directly to node_revisions via vid
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    );

  $viewsdata['biblio'] = $data;

  /**************** biblio contributors table **************/
  $data = array();
  // everything belongs to the Biblio group
  $data['table']['group'] = t('Biblio');
  $data['table']['join'] = array(
    'node' => array(
      'left_table' => 'biblio',
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_revision' => array(
      'left_table' => 'biblio',
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    // This is provided for many_to_one argument
    'biblio' => array(
      'field' => 'vid',
      'left_field' => 'vid',
    ),
  );
  $data['cid'] = array(
    'title' => t('Contributors'),
    'help' => t('Filter by any kind of contributor.'),
    'filter' => array(
      'handler' => 'biblio_handler_filter_contributor',
    )
  );

  $viewsdata['biblio_contributor'] = $data;
/*
 * Describe the keyword table...
 */
  $data = array();
  $data['table']['group'] = t('Biblio');
  $data['table']['join'] = array(
    'node' => array(
      'left_table' => 'biblio',
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_revision' => array(
      'left_table' => 'biblio',
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    // This is provided for many_to_one argument
    'biblio' => array(
      'field' => 'vid',
      'left_field' => 'vid',
    ),
  );
  $data['kid'] = array(
    'title' => t('Keyword'),
    'help' => t('Filter by any keyword.'),
    'filter' => array(
      'handler' => 'biblio_handler_filter_keyword',
    )
  );

  $viewsdata['biblio_keyword'] = $data;
/*
 * Describe the keyword_data table
 */
  $data = array();
  $data['table']['group'] = t('Biblio');
  $data['table']['join'] = array(
    'node' => array(
      'left_table' => 'biblio_keyword',
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );
  $data['word'] = array(
    'title' => t('Keyword'),
    'help'  => t("Display the keyword from a Biblio node"),
    'field' => array(
      'handler' => 'biblio_handler_field',
    ),
  );
  $viewsdata['biblio_keyword_data'] = $data;
  return $viewsdata;
}

/**
 * Use hook_views_data_alter to add items to the node and node_revisions tables
 * This allows to join these tables, when biblio is the base table.
 */
//function biblio_views_data_alter(&$data) {
//  $data['node']['table']['join']['biblio'] = array(
//     'left_field' => 'vid',
//     'field' => 'vid',
//     'type' => 'INNER',
//     );
//  $data['node_revisions']['table']['join']['biblio'] = array(
//     'left_field' => 'vid',
//     'field' => 'vid',
//     'type' => 'INNER',
//     );
//  $data['users']['table']['join']['biblio'] = array(
//     'left_table' => 'node',
//     'left_field' => 'uid',
//     'field' => 'uid',
//     'type' => 'INNER', // all nodes have an author.
//  );
//}