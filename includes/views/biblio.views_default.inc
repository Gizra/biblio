<?php

/**
 * @file
 * Export of views_default objects.
 */

/**
 * Implements hook_views_default_views().
 */
function biblio_views_default_views() {
  $views = array();

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
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'biblio_in_press',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Biblio: Publication Cache */
  $handler->display->display_options['fields']['biblio_cache']['id'] = 'biblio_cache';
  $handler->display->display_options['fields']['biblio_cache']['table'] = 'field_data_biblio_cache';
  $handler->display->display_options['fields']['biblio_cache']['field'] = 'biblio_cache';
  $handler->display->display_options['fields']['biblio_cache']['label'] = '';
  $handler->display->display_options['fields']['biblio_cache']['element_label_colon'] = FALSE;
  /* Field: Biblio: In Press */
  $handler->display->display_options['fields']['biblio_in_press']['id'] = 'biblio_in_press';
  $handler->display->display_options['fields']['biblio_in_press']['table'] = 'field_data_biblio_in_press';
  $handler->display->display_options['fields']['biblio_in_press']['field'] = 'biblio_in_press';
  $handler->display->display_options['fields']['biblio_in_press']['exclude'] = TRUE;
  /* Sort criterion: Biblio: Issued (biblio_issued) */
  $handler->display->display_options['sorts']['biblio_issued_value']['id'] = 'biblio_issued_value';
  $handler->display->display_options['sorts']['biblio_issued_value']['table'] = 'field_data_biblio_issued';
  $handler->display->display_options['sorts']['biblio_issued_value']['field'] = 'biblio_issued_value';
  $handler->display->display_options['sorts']['biblio_issued_value']['order'] = 'DESC';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'biblio';

  $views['biblio'] = $view;

  return $views;
}
