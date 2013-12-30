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
  /* Header: Biblio: Biblio export UI */
  $handler->display->display_options['header']['biblio_export']['id'] = 'biblio_export';
  $handler->display->display_options['header']['biblio_export']['table'] = 'biblio';
  $handler->display->display_options['header']['biblio_export']['field'] = 'biblio_export';
  $handler->display->display_options['header']['biblio_export']['label'] = '';
  $handler->display->display_options['header']['biblio_export']['empty'] = TRUE;
  /* Field: Biblio: Render bilbio (Get text) */
  $handler->display->display_options['fields']['biblio_render']['id'] = 'biblio_render';
  $handler->display->display_options['fields']['biblio_render']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_render']['field'] = 'biblio_render';
  $handler->display->display_options['fields']['biblio_render']['label'] = '';
  $handler->display->display_options['fields']['biblio_render']['element_label_colon'] = FALSE;
  /* Sort criterion: Biblio: Year of Publication (biblio_year) */
  $handler->display->display_options['sorts']['biblio_year_value']['id'] = 'biblio_year_value';
  $handler->display->display_options['sorts']['biblio_year_value']['table'] = 'field_data_biblio_year';
  $handler->display->display_options['sorts']['biblio_year_value']['field'] = 'biblio_year_value';
  $handler->display->display_options['sorts']['biblio_year_value']['order'] = 'DESC';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'biblio';

  $views['biblio'] = $view;

  $view = new view();
  $view->name = 'biblio_year';
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
      'field' => 'biblio_status',
      'rendered' => 0,
      'rendered_strip' => 0,
    ),
    1 => array(
      'field' => 'biblio_year',
      'rendered' => 0,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Biblio: Publication status */
  $handler->display->display_options['fields']['biblio_status']['id'] = 'biblio_status';
  $handler->display->display_options['fields']['biblio_status']['table'] = 'field_data_biblio_status';
  $handler->display->display_options['fields']['biblio_status']['field'] = 'biblio_status';
  $handler->display->display_options['fields']['biblio_status']['label'] = '';
  $handler->display->display_options['fields']['biblio_status']['exclude'] = TRUE;
  $handler->display->display_options['fields']['biblio_status']['element_label_colon'] = FALSE;
  /* Field: Biblio: Year of Publication */
  $handler->display->display_options['fields']['biblio_year']['id'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['table'] = 'field_data_biblio_year';
  $handler->display->display_options['fields']['biblio_year']['field'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['label'] = '';
  $handler->display->display_options['fields']['biblio_year']['exclude'] = TRUE;
  $handler->display->display_options['fields']['biblio_year']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_year']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Biblio: Render bilbio (Get text) */
  $handler->display->display_options['fields']['biblio_render']['id'] = 'biblio_render';
  $handler->display->display_options['fields']['biblio_render']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_render']['field'] = 'biblio_render';
  $handler->display->display_options['fields']['biblio_render']['label'] = '';
  $handler->display->display_options['fields']['biblio_render']['element_label_colon'] = FALSE;
  /* Sort criterion: Biblio: Year of Publication (biblio_year) */
  $handler->display->display_options['sorts']['biblio_year_value']['id'] = 'biblio_year_value';
  $handler->display->display_options['sorts']['biblio_year_value']['table'] = 'field_data_biblio_year';
  $handler->display->display_options['sorts']['biblio_year_value']['field'] = 'biblio_year_value';
  $handler->display->display_options['sorts']['biblio_year_value']['order'] = 'DESC';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'biblio-issued';

  $views['biblio_year'] = $view;

  $view = new view();
  $view->name = 'biblio_summary';
  $view->description = 'Show publications grouped by the first letter.';
  $view->tag = 'default';
  $view->base_table = 'biblio';
  $view->human_name = 'Biblio summary';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Biblio summary';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'biblio_first_letter',
      'rendered' => 0,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Biblio: Publication summary character */
  $handler->display->display_options['fields']['biblio_first_letter']['id'] = 'biblio_first_letter';
  $handler->display->display_options['fields']['biblio_first_letter']['table'] = 'field_data_biblio_first_letter';
  $handler->display->display_options['fields']['biblio_first_letter']['field'] = 'biblio_first_letter';
  $handler->display->display_options['fields']['biblio_first_letter']['label'] = '';
  $handler->display->display_options['fields']['biblio_first_letter']['exclude'] = TRUE;
  $handler->display->display_options['fields']['biblio_first_letter']['element_label_colon'] = FALSE;
  /* Field: Biblio: Render bilbio (Get text) */
  $handler->display->display_options['fields']['biblio_render']['id'] = 'biblio_render';
  $handler->display->display_options['fields']['biblio_render']['table'] = 'biblio';
  $handler->display->display_options['fields']['biblio_render']['field'] = 'biblio_render';
  $handler->display->display_options['fields']['biblio_render']['label'] = '';
  $handler->display->display_options['fields']['biblio_render']['element_label_colon'] = FALSE;
  /* Sort criterion: Biblio: Publication summary character (biblio_first_letter) */
  $handler->display->display_options['sorts']['biblio_first_letter_value']['id'] = 'biblio_first_letter_value';
  $handler->display->display_options['sorts']['biblio_first_letter_value']['table'] = 'field_data_biblio_first_letter';
  $handler->display->display_options['sorts']['biblio_first_letter_value']['field'] = 'biblio_first_letter_value';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'biblio-summary';

  $views['biblio_summary'] = $view;

  return $views;
}
