<?php
// $Id$
/**
 * Implementation of hook_views_handlers().
 */
function biblio_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'biblio') . '/views',
    ),
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
      'biblio_handler_filter_biblio_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'biblio_handler_field_biblio_type' => array(
        'parent' => 'biblio_handler_field',
      ),
      'biblio_handler_filter_biblio_keyword_kid' => array(
        'parent' => 'views_handler_filter_many_to_one',
      ),
     'biblio_handler_field_biblio_keyword_kid' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
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
      'title'     => $field['title'],
      'help'      => "Display the " . $field['title'],
      'field'     => array('handler' => 'biblio_handler_field'),
      'sort'      => array('handler' => 'views_handler_sort'),
      'filter'    => array('handler' => 'views_handler_filter_string'),
      'argument'  => array('handler' => 'views_handler_argument_string')
    );

    // for contrib_widgets we use a special handler:
    if ($field['type'] == 'contrib_widget') {
      $data[$field['name']]['field'] = array(
        'handler' => 'biblio_handler_field_contributor',
        'auth_category' => $field['ftdid'],
      );
      unset($data[$field['name']]['sort']);
      unset($data[$field['name']]['filter']);
      unset($data[$field['name']]['argument']);
    }
  }

  unset($data['biblio_keywords']); // keywords are in a separate table which has it's own handlers

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
  
  /**************** biblio_types table *********************/

  $data = array();
  $data['table']['group'] = t('Biblio');
  $data['table']['join'] = array(
    'node' => array(
      'left_table' => 'biblio',
      'left_field' => 'biblio_type',
      'field' => 'tid',
    ),
    'node_revision' => array(
      'left_table' => 'biblio',
      'left_field' => 'biblio_type',
      'field' => 'tid',
    ),
  );
  $data['name'] =  array(
    'field'   => array('handler' => 'views_handler_field'),
    'sort'    => array('handler' => 'views_handler_sort'),
    'title'   => t('Type of Publication'),
    'help'    => t('The type of publication: Journal, Book, Conference Paper, etc.')
  );
  $data['tid'] =  array(
    'filter'  => array('handler' => 'biblio_handler_filter_biblio_type'),
    'title'   => t('Type of Publication'),
    'help'    => t('The type of publication: Journal, Book, Conference Paper, etc.')
  );
  
  $viewsdata['biblio_types'] = $data;
  
  /**************** biblio contributors table **************/
  
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
  $data['cid'] = array(
    'title' => t('Contributors'),
    'help' => t('Filter by any kind of contributor.'),
    'filter' => array(
      'handler' => 'biblio_handler_filter_contributor',
    )
  );

  $viewsdata['biblio_contributor'] = $data;

/***************** Describe the keyword table ***********/
  
  $data = array();
  $data['table']['group'] = t('Biblio');
  $data['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_revision' => array(
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
    'title' => t('Keyword ID'),
    'help' => t('The Biblio keyword ID'),
    'field' => array(
      'title' => t('All keywords'),
      'help' => t('Display all keywords associated with a node.'),
      'handler' => 'biblio_handler_field_biblio_keyword_kid',
      'skip base' => 'term_data',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_many_to_one',
      'name table' => 'biblio_keyword_data',
      'name field' => 'word',
      'empty name field' => t('No Keyword'),
      'numeric' => TRUE,
      'skip base' => 'biblio_keyword_data',
    ),
    'filter' => array(
      'title' => t('Keyword'),
      'handler' => 'biblio_handler_filter_biblio_keyword_kid',
      'numeric' => TRUE,
      'skip base' => 'biblio_keyword_data',
    ),
    );

  $viewsdata['biblio_keyword'] = $data;

/***************** Describe the keyword_data table ***********/

  $data = array();
  $data['table']['group'] = t('Biblio');
  $data['table']['join'] = array(
    'biblio_keyword' => array(
      'left_field' => 'kid',
      'field' => 'kid',
    ),
    'node' => array(
      'left_table' => 'biblio_keyword',
      'left_field' => 'kid',
      'field' => 'kid',
    ),
    );

  
  $viewsdata['biblio_keyword_data'] = $data;
  
  return $viewsdata;
}

