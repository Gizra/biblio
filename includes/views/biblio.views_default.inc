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
  $handler->display->display_options['title'] = 'Publications';
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
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Biblio: Render bilbio (Get text) */
  $handler->display->display_options['fields']['biblio_render']['id'] = 'biblio_render';
  $handler->display->display_options['fields']['biblio_render']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_render']['field'] = 'biblio_render';
  $handler->display->display_options['fields']['biblio_render']['label'] = '';
  $handler->display->display_options['fields']['biblio_render']['element_label_colon'] = FALSE;
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

  $view = new view();
  $view->name = 'biblio_issued';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'biblio';
  $view->human_name = 'Biblio Issued';
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
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'biblio_publication_year',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Biblio: Publication year */
  $handler->display->display_options['fields']['biblio_publication_year']['id'] = 'biblio_publication_year';
  $handler->display->display_options['fields']['biblio_publication_year']['table'] = 'field_data_biblio_publication_year';
  $handler->display->display_options['fields']['biblio_publication_year']['field'] = 'biblio_publication_year';
  $handler->display->display_options['fields']['biblio_publication_year']['label'] = '';
  $handler->display->display_options['fields']['biblio_publication_year']['exclude'] = TRUE;
  $handler->display->display_options['fields']['biblio_publication_year']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_publication_year']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Biblio: Render bilbio (Get text) */
  $handler->display->display_options['fields']['biblio_render']['id'] = 'biblio_render';
  $handler->display->display_options['fields']['biblio_render']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_render']['field'] = 'biblio_render';
  $handler->display->display_options['fields']['biblio_render']['label'] = '';
  $handler->display->display_options['fields']['biblio_render']['element_label_colon'] = FALSE;
  /* Sort criterion: Biblio: Publication year (biblio_publication_year) */
  $handler->display->display_options['sorts']['biblio_publication_year_value']['id'] = 'biblio_publication_year_value';
  $handler->display->display_options['sorts']['biblio_publication_year_value']['table'] = 'field_data_biblio_publication_year';
  $handler->display->display_options['sorts']['biblio_publication_year_value']['field'] = 'biblio_publication_year_value';
  $handler->display->display_options['sorts']['biblio_publication_year_value']['order'] = 'DESC';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'biblio-issued';

  $views['biblio_issued'] = $view;

  return $views;
}
