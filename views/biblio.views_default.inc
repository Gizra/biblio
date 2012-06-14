<?php
/**
 * @file
 * Contains default views on behalf of the biblio module.
 */

/**
 * Implementation of hook_default_view_views().
 */
//function biblio_views_default_views() {
//  $view = new view;
//  $view->name = 'biblio_views';
//  $view->description = '';
//  $view->tag = '';
//  $view->view_php = '';
//  $view->base_table = 'node';
//  $view->is_cacheable = FALSE;
//  $view->api_version = 2;
//  $view->disabled = TRUE; /* Edit this to TRUE to make a default view disabled initially */
//  $handler = $view->new_display('default', 'Defaults', 'default');
//  $handler->override_option('fields', array(
//  'citation' => array(
//    'label' => '',
//    'alter' => array(
//      'alter_text' => 0,
//      'text' => '',
//      'make_link' => 0,
//      'path' => '',
//      'link_class' => '',
//      'alt' => '',
//      'prefix' => '',
//      'suffix' => '',
//      'help' => '',
//      'trim' => 0,
//      'max_length' => '',
//      'word_boundary' => 1,
//      'ellipsis' => 1,
//      'strip_tags' => 0,
//      'html' => 0,
//  ),
//    'style_name' => 'vancouver',
//    'exclude' => 0,
//    'id' => 'citation',
//    'table' => 'biblio',
//    'field' => 'citation',
//    'relationship' => 'none',
//  ),
//  ));
//  $handler->override_option('sorts', array(
//  'biblio_year' => array(
//    'order' => 'DESC',
//    'id' => 'biblio_year',
//    'table' => 'biblio',
//    'field' => 'biblio_year',
//    'relationship' => 'none',
//  ),
//  'name' => array(
//    'id' => 'name',
//    'table' => 'biblio_types',
//    'field' => 'name',
//  ),
//  ));
//  $handler->override_option('arguments', array(
//  'biblio_year' => array(
//    'id' => 'biblio_year',
//    'table' => 'biblio',
//    'field' => 'biblio_year',
//  ),
//  ));
//  $handler->override_option('filters', array(
//  'status' => array(
//    'operator' => '=',
//    'value' => '1',
//    'group' => '0',
//    'exposed' => FALSE,
//    'expose' => array(
//      'operator' => FALSE,
//      'label' => '',
//  ),
//    'id' => 'status',
//    'table' => 'node',
//    'field' => 'status',
//    'relationship' => 'none',
//  ),
//  'type' => array(
//    'operator' => 'in',
//    'value' => array(
//      'biblio' => 'biblio',
//  ),
//    'group' => '0',
//    'exposed' => FALSE,
//    'expose' => array(
//      'operator' => FALSE,
//      'label' => '',
//  ),
//    'id' => 'type',
//    'table' => 'node',
//    'field' => 'type',
//    'relationship' => 'none',
//  ),
//  ));
//  $handler->override_option('access', array(
//  'type' => 'none',
//  ));
//  $handler->override_option('cache', array(
//  'type' => 'none',
//  ));
//  $handler->override_option('header_format', '1');
//  $handler->override_option('header_empty', 0);
//  $handler->override_option('empty', 'Nothing found');
//  $handler->override_option('empty_format', '1');
//  $handler->override_option('use_pager', '1');
//  $handler->override_option('style_options', array(
//  'grouping' => 'name',
//  ));
//  $handler->override_option('row_options', array(
//  'inline' => array(),
//  'separator' => '',
//  ));
//  $handler = $view->new_display('page', 'Group by Year', 'page_1');
//  $handler->override_option('fields', array(
//  'citation' => array(
//    'label' => '',
//    'alter' => array(
//      'alter_text' => 0,
//      'text' => '',
//      'make_link' => 0,
//      'path' => '',
//      'link_class' => '',
//      'alt' => '',
//      'prefix' => '',
//      'suffix' => '',
//      'help' => '',
//      'trim' => 0,
//      'max_length' => '',
//      'word_boundary' => 1,
//      'ellipsis' => 1,
//      'strip_tags' => 0,
//      'html' => 0,
//  ),
//    'style_name' => 'vancouver',
//    'exclude' => 0,
//    'id' => 'citation',
//    'table' => 'biblio',
//    'field' => 'citation',
//    'relationship' => 'none',
//  ),
//  'biblio_year' => array(
//    'label' => '',
//    'alter' => array(
//      'alter_text' => 0,
//      'text' => '',
//      'make_link' => 0,
//      'path' => '',
//      'link_class' => '',
//      'alt' => '',
//      'prefix' => '',
//      'suffix' => '',
//      'help' => '',
//      'trim' => 0,
//      'max_length' => '',
//      'word_boundary' => 1,
//      'ellipsis' => 1,
//      'strip_tags' => 0,
//      'html' => 0,
//  ),
//    'biblio_label' => 0,
//    'exclude' => 1,
//    'id' => 'biblio_year',
//    'table' => 'biblio',
//    'field' => 'biblio_year',
//    'override' => array(
//      'button' => 'Override',
//  ),
//    'relationship' => 'none',
//  ),
//  ));
//  $handler->override_option('arguments', array(
//  'biblio_year' => array(
//    'default_action' => 'ignore',
//    'style_plugin' => 'default_summary',
//    'style_options' => array(),
//    'wildcard' => 'all',
//    'wildcard_substitution' => 'All',
//    'title' => '',
//    'breadcrumb' => '',
//    'default_argument_type' => 'fixed',
//    'default_argument' => '',
//    'validate_type' => 'none',
//    'validate_fail' => 'not found',
//    'glossary' => 0,
//    'limit' => '0',
//    'case' => 'none',
//    'path_case' => 'none',
//    'transform_dash' => 0,
//    'id' => 'biblio_year',
//    'table' => 'biblio',
//    'field' => 'biblio_year',
//    'validate_user_argument_type' => 'uid',
//    'validate_user_roles' => array(
//      '2' => 0,
//  ),
//    'override' => array(
//      'button' => 'Use default',
//  ),
//    'relationship' => 'none',
//    'default_options_div_prefix' => '',
//    'default_argument_user' => 0,
//    'default_argument_fixed' => '',
//    'default_argument_php' => '',
//    'validate_argument_node_type' => array(
//      'image' => 0,
//      'biblio' => 0,
//      'book' => 0,
//      'feed' => 0,
//      'page' => 0,
//      'story' => 0,
//      'test' => 0,
//  ),
//    'validate_argument_node_access' => 0,
//    'validate_argument_nid_type' => 'nid',
//    'validate_argument_vocabulary' => array(
//      '2' => 0,
//      '3' => 0,
//      '4' => 0,
//  ),
//    'validate_argument_type' => 'tid',
//    'validate_argument_transform' => 0,
//    'validate_user_restrict_roles' => 0,
//    'image_size' => array(
//      '_original' => '_original',
//      'thumbnail' => 'thumbnail',
//      'preview' => 'preview',
//  ),
//    'validate_argument_php' => '',
//  ),
//  ));
//  $handler->override_option('style_options', array(
//  'grouping' => 'biblio_year',
//  ));
//  $handler->override_option('path', 'view/biblio/year');
//  $handler->override_option('menu', array(
//  'type' => 'none',
//  'title' => '',
//  'description' => '',
//  'weight' => 0,
//  'name' => 'navigation',
//  ));
//  $handler->override_option('tab_options', array(
//  'type' => 'none',
//  'title' => '',
//  'description' => '',
//  'weight' => 0,
//  ));
//  $handler = $view->new_display('page', 'Group by Type', 'page_2');
//  $handler->override_option('fields', array(
//  'citation' => array(
//    'label' => '',
//    'alter' => array(
//      'alter_text' => 0,
//      'text' => '',
//      'make_link' => 0,
//      'path' => '',
//      'link_class' => '',
//      'alt' => '',
//      'prefix' => '',
//      'suffix' => '',
//      'help' => '',
//      'trim' => 0,
//      'max_length' => '',
//      'word_boundary' => 1,
//      'ellipsis' => 1,
//      'strip_tags' => 0,
//      'html' => 0,
//  ),
//    'style_name' => 'vancouver',
//    'exclude' => 0,
//    'id' => 'citation',
//    'table' => 'biblio',
//    'field' => 'citation',
//    'relationship' => 'none',
//  ),
//  'name' => array(
//    'label' => '',
//    'alter' => array(
//      'alter_text' => 0,
//      'text' => '',
//      'make_link' => 0,
//      'path' => '',
//      'link_class' => '',
//      'alt' => '',
//      'prefix' => '',
//      'suffix' => '',
//      'help' => '',
//      'trim' => 0,
//      'max_length' => '',
//      'word_boundary' => 1,
//      'ellipsis' => 1,
//      'strip_tags' => 0,
//      'html' => 0,
//  ),
//    'exclude' => 1,
//    'id' => 'name',
//    'table' => 'biblio_types',
//    'field' => 'name',
//    'relationship' => 'none',
//  ),
//  ));
//  $handler->override_option('sorts', array(
//  'name' => array(
//    'order' => 'ASC',
//    'id' => 'name',
//    'table' => 'biblio_types',
//    'field' => 'name',
//    'override' => array(
//      'button' => 'Use default',
//  ),
//    'relationship' => 'none',
//  ),
//  'biblio_year' => array(
//    'order' => 'DESC',
//    'id' => 'biblio_year',
//    'table' => 'biblio',
//    'field' => 'biblio_year',
//    'relationship' => 'none',
//  ),
//  ));
//  $handler->override_option('path', 'view/biblio/type');
//  $handler->override_option('menu', array(
//  'type' => 'none',
//  'title' => '',
//  'description' => '',
//  'weight' => 0,
//  'name' => 'navigation',
//  ));
//  $handler->override_option('tab_options', array(
//  'type' => 'none',
//  'title' => '',
//  'description' => '',
//  'weight' => 0,
//  ));
//  $handler = $view->new_display('page', 'Group by Title', 'page_3');
//  $handler->override_option('fields', array(
//  'citation' => array(
//    'label' => '',
//    'alter' => array(
//      'alter_text' => 0,
//      'text' => '',
//      'make_link' => 0,
//      'path' => '',
//      'link_class' => '',
//      'alt' => '',
//      'prefix' => '',
//      'suffix' => '',
//      'help' => '',
//      'trim' => 0,
//      'max_length' => '',
//      'word_boundary' => 1,
//      'ellipsis' => 1,
//      'strip_tags' => 0,
//      'html' => 0,
//  ),
//    'style_name' => 'vancouver',
//    'exclude' => 0,
//    'id' => 'citation',
//    'table' => 'biblio',
//    'field' => 'citation',
//    'relationship' => 'none',
//  ),
//  'biblio_year' => array(
//    'label' => 'Year of Publication',
//    'alter' => array(
//      'alter_text' => 0,
//      'text' => '',
//      'make_link' => 0,
//      'path' => '',
//      'link_class' => '',
//      'alt' => '',
//      'prefix' => '',
//      'suffix' => '',
//      'help' => '',
//      'trim' => 0,
//      'max_length' => '',
//      'word_boundary' => 1,
//      'ellipsis' => 1,
//      'strip_tags' => 0,
//      'html' => 0,
//  ),
//    'biblio_label' => 1,
//    'exclude' => 1,
//    'id' => 'biblio_year',
//    'table' => 'biblio',
//    'field' => 'biblio_year',
//    'override' => array(
//      'button' => 'Use default',
//  ),
//    'relationship' => 'none',
//  ),
//  ));
//  $handler->override_option('sorts', array(
//  'biblio_year' => array(
//    'order' => 'DESC',
//    'id' => 'biblio_year',
//    'table' => 'biblio',
//    'field' => 'biblio_year',
//    'relationship' => 'none',
//  ),
//  'title' => array(
//    'order' => 'ASC',
//    'id' => 'title',
//    'table' => 'node',
//    'field' => 'title',
//    'override' => array(
//      'button' => 'Use default',
//  ),
//    'relationship' => 'none',
//  ),
//  ));
//  $handler->override_option('style_options', array(
//  'grouping' => 'biblio_year',
//  ));
//  $handler->override_option('path', 'view/biblio/title');
//  $handler->override_option('menu', array(
//  'type' => 'none',
//  'title' => '',
//  'description' => '',
//  'weight' => 0,
//  'name' => 'navigation',
//  ));
//  $handler->override_option('tab_options', array(
//  'type' => 'none',
//  'title' => '',
//  'description' => '',
//  'weight' => 0,
//  ));
//  $handler = $view->new_display('page', 'Filter by Keyword ID', 'page_4');
//  $handler->override_option('arguments', array(
//  'kid' => array(
//    'default_action' => 'ignore',
//    'style_plugin' => 'default_summary',
//    'style_options' => array(),
//    'wildcard' => 'all',
//    'wildcard_substitution' => 'All',
//    'title' => 'Contains the keyword %1',
//    'breadcrumb' => '',
//    'default_argument_type' => 'fixed',
//    'default_argument' => '',
//    'validate_type' => 'none',
//    'validate_fail' => 'not found',
//    'break_phrase' => 1,
//    'add_table' => 0,
//    'require_value' => 0,
//    'reduce_duplicates' => 0,
//    'id' => 'kid',
//    'table' => 'biblio_keyword',
//    'field' => 'kid',
//    'validate_user_argument_type' => 'uid',
//    'validate_user_roles' => array(
//      '2' => 0,
//  ),
//    'override' => array(
//      'button' => 'Use default',
//  ),
//    'relationship' => 'none',
//    'default_options_div_prefix' => '',
//    'default_argument_user' => 0,
//    'default_argument_fixed' => '',
//    'default_argument_php' => '',
//    'validate_argument_node_type' => array(
//      'image' => 0,
//      'biblio' => 0,
//      'book' => 0,
//      'feed' => 0,
//      'page' => 0,
//      'story' => 0,
//      'test' => 0,
//  ),
//    'validate_argument_node_access' => 0,
//    'validate_argument_nid_type' => 'nid',
//    'validate_argument_vocabulary' => array(
//      '2' => 0,
//      '3' => 0,
//      '4' => 0,
//  ),
//    'validate_argument_type' => 'tid',
//    'validate_argument_transform' => 0,
//    'validate_user_restrict_roles' => 0,
//    'image_size' => array(
//      '_original' => '_original',
//      'thumbnail' => 'thumbnail',
//      'preview' => 'preview',
//  ),
//    'validate_argument_php' => '',
//  ),
//  ));
//  $handler->override_option('path', 'view/biblio/keyword');
//  $handler->override_option('menu', array(
//  'type' => 'none',
//  'title' => '',
//  'description' => '',
//  'weight' => 0,
//  'name' => 'navigation',
//  ));
//  $handler->override_option('tab_options', array(
//  'type' => 'none',
//  'title' => '',
//  'description' => '',
//  'weight' => 0,
//  ));
//  $handler = $view->new_display('page', 'Filter by Author ID', 'page_5');
//  $handler->override_option('arguments', array(
//  'cid' => array(
//    'default_action' => 'ignore',
//    'style_plugin' => 'default_summary',
//    'style_options' => array(),
//    'wildcard' => 'all',
//    'wildcard_substitution' => 'All',
//    'title' => '',
//    'breadcrumb' => '',
//    'default_argument_type' => 'fixed',
//    'default_argument' => '',
//    'validate_type' => 'none',
//    'validate_fail' => 'not found',
//    'break_phrase' => 1,
//    'add_table' => 0,
//    'require_value' => 0,
//    'reduce_duplicates' => 0,
//    'id' => 'cid',
//    'table' => 'biblio_contributor',
//    'field' => 'cid',
//    'validate_user_argument_type' => 'uid',
//    'validate_user_roles' => array(
//      '2' => 0,
//  ),
//    'override' => array(
//      'button' => 'Use default',
//  ),
//    'relationship' => 'none',
//    'default_options_div_prefix' => '',
//    'default_argument_user' => 0,
//    'default_argument_fixed' => '',
//    'default_argument_php' => '',
//    'validate_argument_node_type' => array(
//      'image' => 0,
//      'biblio' => 0,
//      'book' => 0,
//      'feed' => 0,
//      'page' => 0,
//      'story' => 0,
//      'test' => 0,
//  ),
//    'validate_argument_node_access' => 0,
//    'validate_argument_nid_type' => 'nid',
//    'validate_argument_vocabulary' => array(
//      '2' => 0,
//      '3' => 0,
//      '4' => 0,
//  ),
//    'validate_argument_type' => 'tid',
//    'validate_argument_transform' => 0,
//    'validate_user_restrict_roles' => 0,
//    'image_size' => array(
//      '_original' => '_original',
//      'thumbnail' => 'thumbnail',
//      'preview' => 'preview',
//  ),
//    'validate_argument_php' => '',
//  ),
//  ));
//  $handler->override_option('path', 'view/biblio/author');
//  $handler->override_option('menu', array(
//  'type' => 'none',
//  'title' => '',
//  'description' => '',
//  'weight' => 0,
//  'name' => 'navigation',
//  ));
//  $handler->override_option('tab_options', array(
//  'type' => 'none',
//  'title' => '',
//  'description' => '',
//  'weight' => 0,
//  ));
//  $handler = $view->new_display('page', 'Browse by Year', 'page_6');
//  $handler->override_option('fields', array(
//  'biblio_year' => array(
//    'label' => '',
//    'alter' => array(
//      'alter_text' => 0,
//      'text' => '',
//      'make_link' => 0,
//      'path' => '',
//      'link_class' => '',
//      'alt' => '',
//      'prefix' => '',
//      'suffix' => '',
//      'help' => '',
//      'trim' => 0,
//      'max_length' => '',
//      'word_boundary' => 1,
//      'ellipsis' => 1,
//      'strip_tags' => 0,
//      'html' => 0,
//  ),
//    'biblio_label' => 0,
//    'exclude' => 0,
//    'id' => 'biblio_year',
//    'table' => 'biblio',
//    'field' => 'biblio_year',
//    'override' => array(
//      'button' => 'Use default',
//  ),
//    'relationship' => 'none',
//  ),
//  ));
//  $handler->override_option('sorts', array());
//  $handler->override_option('arguments', array());
//  $handler->override_option('distinct', 0);
//  $handler->override_option('style_plugin', 'grid');
//  $handler->override_option('style_options', array(
//  'grouping' => '',
//  'columns' => '4',
//  'alignment' => 'horizontal',
//  ));
//  $handler->override_option('path', 'view/biblio/years');
//  $handler->override_option('menu', array(
//  'type' => 'none',
//  'title' => '',
//  'description' => '',
//  'weight' => 0,
//  'name' => 'navigation',
//  ));
//  $handler->override_option('tab_options', array(
//  'type' => 'none',
//  'title' => '',
//  'description' => '',
//  'weight' => 0,
//  ));
//
//  $views[$view->name] = $view;
//  return $views;
//}

/**
 * Implements hook_views_default_views(). 
 */
function biblio_views_default_views() {
  $view = new view;
$view->name = 'biblio_admin';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'biblio';
$view->human_name = 'Biblio Administration Views';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'List of Biblio Entries';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'administer biblio';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '0';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 1;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'views_bulk_operations' => 'views_bulk_operations',
  'bid' => 'bid',
  'publication_type' => 'publication_type',
  'biblio_title' => 'biblio_title',
  'nothing' => 'nothing',
  'changed' => 'changed',
);
$handler->display->display_options['style_options']['default'] = 'changed';
$handler->display->display_options['style_options']['info'] = array(
  'views_bulk_operations' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'bid' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'publication_type' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'biblio_title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nothing' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'changed' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Field: Bulk operations: Biblio */
$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'biblio';
$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
$handler->display->display_options['fields']['views_bulk_operations']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['external'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['trim'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['alter']['html'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['views_bulk_operations']['element_default_classes'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['empty_zero'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['entity_load_capacity'] = '10';
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['operations'] = array(
  'action::views_bulk_operations_delete_item' => array(
    'selected' => 1,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::system_message_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_script_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_modify_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'settings' => array(
      'show_all_tokens' => 1,
      'display_values' => array(
        '_all_' => '_all_',
      ),
    ),
  ),
  'action::views_bulk_operations_argument_selector_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'settings' => array(
      'url' => '',
    ),
  ),
  'action::system_goto_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::system_send_email_action' => array(
    'selected' => 0,
    'use_queue' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
);
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['enable_select_all_pages'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_type'] = '0';
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_result'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['merge_single_action'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo']['force_single'] = 0;
/* Field: Biblio: Biblio ID */
$handler->display->display_options['fields']['bid']['id'] = 'bid';
$handler->display->display_options['fields']['bid']['table'] = 'biblio';
$handler->display->display_options['fields']['bid']['field'] = 'bid';
/* Field: Biblio: Publication_type */
$handler->display->display_options['fields']['publication_type']['id'] = 'publication_type';
$handler->display->display_options['fields']['publication_type']['table'] = 'biblio';
$handler->display->display_options['fields']['publication_type']['field'] = 'publication_type';
$handler->display->display_options['fields']['publication_type']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['publication_type']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['publication_type']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['publication_type']['alter']['external'] = 0;
$handler->display->display_options['fields']['publication_type']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['publication_type']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['publication_type']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['publication_type']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['publication_type']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['publication_type']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['publication_type']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['publication_type']['alter']['trim'] = 0;
$handler->display->display_options['fields']['publication_type']['alter']['html'] = 0;
$handler->display->display_options['fields']['publication_type']['element_label_colon'] = 1;
$handler->display->display_options['fields']['publication_type']['element_default_classes'] = 1;
$handler->display->display_options['fields']['publication_type']['hide_empty'] = 0;
$handler->display->display_options['fields']['publication_type']['empty_zero'] = 0;
$handler->display->display_options['fields']['publication_type']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['publication_type']['machine_name'] = 0;
/* Field: Biblio: Title */
$handler->display->display_options['fields']['biblio_title']['id'] = 'biblio_title';
$handler->display->display_options['fields']['biblio_title']['table'] = 'field_data_biblio_title';
$handler->display->display_options['fields']['biblio_title']['field'] = 'biblio_title';
$handler->display->display_options['fields']['biblio_title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['biblio_title']['alter']['make_link'] = 1;
$handler->display->display_options['fields']['biblio_title']['alter']['path'] = 'biblio/[bid]';
$handler->display->display_options['fields']['biblio_title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['biblio_title']['alter']['external'] = 0;
$handler->display->display_options['fields']['biblio_title']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['biblio_title']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['biblio_title']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['biblio_title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['biblio_title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['biblio_title']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['biblio_title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['biblio_title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['biblio_title']['alter']['html'] = 0;
$handler->display->display_options['fields']['biblio_title']['element_label_colon'] = 1;
$handler->display->display_options['fields']['biblio_title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['biblio_title']['hide_empty'] = 0;
$handler->display->display_options['fields']['biblio_title']['empty_zero'] = 0;
$handler->display->display_options['fields']['biblio_title']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['biblio_title']['field_api_classes'] = 0;
/* Field: Global: Custom text */
$handler->display->display_options['fields']['nothing']['id'] = 'nothing';
$handler->display->display_options['fields']['nothing']['table'] = 'views';
$handler->display->display_options['fields']['nothing']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing']['label'] = '';
$handler->display->display_options['fields']['nothing']['alter']['text'] = 'Edit';
$handler->display->display_options['fields']['nothing']['alter']['make_link'] = 1;
$handler->display->display_options['fields']['nothing']['alter']['path'] = 'biblio/[bid]/edit';
$handler->display->display_options['fields']['nothing']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['nothing']['alter']['external'] = 0;
$handler->display->display_options['fields']['nothing']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['nothing']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['nothing']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['nothing']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['nothing']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['nothing']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['nothing']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['nothing']['alter']['trim'] = 0;
$handler->display->display_options['fields']['nothing']['alter']['html'] = 0;
$handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['nothing']['element_default_classes'] = 1;
$handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
$handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
$handler->display->display_options['fields']['nothing']['hide_alter_empty'] = 0;
/* Field: Biblio: Changed */
$handler->display->display_options['fields']['changed']['id'] = 'changed';
$handler->display->display_options['fields']['changed']['table'] = 'biblio';
$handler->display->display_options['fields']['changed']['field'] = 'changed';
$handler->display->display_options['fields']['changed']['label'] = 'Last Edited';
$handler->display->display_options['fields']['changed']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['changed']['alter']['external'] = 0;
$handler->display->display_options['fields']['changed']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['changed']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['changed']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
$handler->display->display_options['fields']['changed']['alter']['html'] = 0;
$handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
$handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
$handler->display->display_options['fields']['changed']['hide_empty'] = 0;
$handler->display->display_options['fields']['changed']['empty_zero'] = 0;
$handler->display->display_options['fields']['changed']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['changed']['separator'] = '';
$handler->display->display_options['fields']['changed']['format_plural'] = 0;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'admin/structure/biblio2';
$handler->display->display_options['menu']['type'] = 'default tab';
$handler->display->display_options['menu']['title'] = 'Biblio view';
$handler->display->display_options['menu']['description'] = 'asdf';
$handler->display->display_options['menu']['weight'] = '5';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['tab_options']['title'] = 'a title?';
$handler->display->display_options['tab_options']['weight'] = '0';

$views[$view->name] = $view;
return $views;
}

/**
 * Implements hook_views_post_execute().
 */
function biblio_views_post_execute(&$view) {
//  dpm($view);
//  if ($view->name == 'biblio_admin') {
//    foreach ($view->result as $key => $row) {
//      if (isset($row->biblio_changed)) {
//        $view->result[$key]->biblio_changed = date('M d Y H:i:s', $row->biblio_changed);
//      }
//    }
//  }
//  dpm($view);
}
