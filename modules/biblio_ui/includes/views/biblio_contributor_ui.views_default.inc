<?php

/**
 * @file
 * Export of views_default objects.
 */

/**
 * Implements hook_views_default_views().
 */
function biblio_contributor_ui_views_default_views() {
  $view = new view();
  $view->name = 'biblio_contributor';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'biblio_contributor';
  $view->human_name = 'Biblio contributor';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'cid' => 'cid',
    'firstname' => 'firstname',
    'lastname' => 'lastname',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'cid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'firstname' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'lastname' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Contributor: Contributor ID */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'biblio_contributor';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['exclude'] = TRUE;
  /* Field: Contributor: Firstname */
  $handler->display->display_options['fields']['firstname']['id'] = 'firstname';
  $handler->display->display_options['fields']['firstname']['table'] = 'biblio_contributor';
  $handler->display->display_options['fields']['firstname']['field'] = 'firstname';
  $handler->display->display_options['fields']['firstname']['exclude'] = TRUE;
  /* Field: Contributor: Lastname */
  $handler->display->display_options['fields']['lastname']['id'] = 'lastname';
  $handler->display->display_options['fields']['lastname']['table'] = 'biblio_contributor';
  $handler->display->display_options['fields']['lastname']['field'] = 'lastname';
  $handler->display->display_options['fields']['lastname']['exclude'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Author';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[firstname] [lastname]';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'biblio-contributor/[cid]';
  /* Field: Contributor: Delete */
  $handler->display->display_options['fields']['delete']['id'] = 'delete';
  $handler->display->display_options['fields']['delete']['table'] = 'biblio_contributor';
  $handler->display->display_options['fields']['delete']['field'] = 'delete';
  /* Field: Contributor: Edit */
  $handler->display->display_options['fields']['edit']['id'] = 'edit';
  $handler->display->display_options['fields']['edit']['table'] = 'biblio_contributor';
  $handler->display->display_options['fields']['edit']['field'] = 'edit';

  $views['biblio_contributor'] = $view;

  return $views;
}
