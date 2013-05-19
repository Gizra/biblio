<?php

/**
 * @file
 * Integration with the views module.
 */

/**
 * Implements hook_views_default_views().
 */
function biblio_ui_views_default_views() {
  $view = new view();
  $view->name = 'biblio';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'biblio';
  $view->human_name = 'Biblio';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Biblio';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer biblio';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'title' => 'title',
    'type' => 'type',
    'url' => 'url',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'url' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Biblio: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'biblio';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Biblio: Edit */
  $handler->display->display_options['fields']['edit']['id'] = 'edit';
  $handler->display->display_options['fields']['edit']['table'] = 'biblio';
  $handler->display->display_options['fields']['edit']['field'] = 'edit';
  $handler->display->display_options['fields']['edit']['exclude'] = TRUE;
  /* Field: Biblio: Delete */
  $handler->display->display_options['fields']['delete']['id'] = 'delete';
  $handler->display->display_options['fields']['delete']['table'] = 'biblio';
  $handler->display->display_options['fields']['delete']['field'] = 'delete';
  $handler->display->display_options['fields']['delete']['exclude'] = TRUE;
  /* Field: Biblio: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_biblio';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['url']['link_to_entity'] = 0;
  /* Field: Biblio: Label */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'biblio';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = '[url]';
  /* Field: Biblio: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'biblio';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'user/[uid]';
  /* Field: Biblio: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'biblio';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[edit]
[delete]';
  /* Sort criterion: Biblio: Changed */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'biblio';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  /* Filter criterion: Biblio: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'biblio';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Filter by type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => 0,
    3 => '3',
    1 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/biblio';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Biblio';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'devel';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'main-menu';

  $views['biblio'] = $view;

  return $views;
}

/**
 * Implements hook_views_data_alter().
 */
function biblio_ui_views_data_alter(&$data) {
  $biblio =& $data['biblio'];

  $biblio['created']['field']['handler'] = 'views_handler_field_date';
  $biblio['changed']['field']['handler'] = 'views_handler_field_date';
  $biblio['uid'] = array(
    'title' => t('Author uid'),
    'help' => t('The user authoring the content. If you need more fields than the uid add the content: author relationship'),
    'relationship' => array(
      'title' => t('Author'),
      'help' => t('Relate content to the user who created it.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('author'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );

  $biblio['delete'] = array(
    'title' => t('Delete'),
    'help' => t('Link to delete the biblio entry.'),
    'field' => array(
      'handler' => 'biblio_ui_delete_button',
    ),
  );
  $biblio['edit'] = array(
    'title' => t('Edit'),
    'help' => t('Link to edit the biblio entry.'),
    'field' => array(
      'handler' => 'biblio_ui_edit_button',
    ),
  );
}
