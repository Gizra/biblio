<?php
/**
 * @file
 * Contains default views on behalf of the biblio module.
 */

/**
 * Implements hook_views_default_views().
 */
function biblio_views_default_views() {
  $view = new view;
  $view->name = 'biblio_main_view';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'biblio';
  $view->human_name = 'Biblio Main View';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Biblio';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access biblio content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = 1;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = 1;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '15';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '5, 15, 30, 50';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 1;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'biblio_year',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Biblio';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No Biblio records present';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: Biblio: Biblio ID */
  $handler->display->display_options['fields']['bid']['id'] = 'bid';
  $handler->display->display_options['fields']['bid']['table'] = 'biblio';
  $handler->display->display_options['fields']['bid']['field'] = 'bid';
  $handler->display->display_options['fields']['bid']['label'] = '';
  $handler->display->display_options['fields']['bid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['bid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['bid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['bid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['bid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['bid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['bid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['bid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['bid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['bid']['separator'] = '';
  $handler->display->display_options['fields']['bid']['format_plural'] = 0;
  /* Field: Biblio: Publication_type */
  $handler->display->display_options['fields']['publication_type']['id'] = 'publication_type';
  $handler->display->display_options['fields']['publication_type']['table'] = 'biblio';
  $handler->display->display_options['fields']['publication_type']['field'] = 'publication_type';
  $handler->display->display_options['fields']['publication_type']['label'] = '';
  $handler->display->display_options['fields']['publication_type']['exclude'] = TRUE;
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
  $handler->display->display_options['fields']['publication_type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['publication_type']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['publication_type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['publication_type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['publication_type']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['publication_type']['machine_name'] = 0;
  /* Field: Biblio: Primary Contributors */
  $handler->display->display_options['fields']['biblio_primary_contributors']['id'] = 'biblio_primary_contributors';
  $handler->display->display_options['fields']['biblio_primary_contributors']['table'] = 'field_data_biblio_primary_contributors';
  $handler->display->display_options['fields']['biblio_primary_contributors']['field'] = 'biblio_primary_contributors';
  $handler->display->display_options['fields']['biblio_primary_contributors']['label'] = '';
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['path'] = 'biblio/contributor/[biblio_primary_contributors-target_id]';
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['link_class'] = 'biblio-contributor';
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['element_type'] = '0';
  $handler->display->display_options['fields']['biblio_primary_contributors']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_primary_contributors']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['biblio_primary_contributors']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_primary_contributors']['empty'] = '[Anonymous]';
  $handler->display->display_options['fields']['biblio_primary_contributors']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_primary_contributors']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['biblio_primary_contributors']['group_rows'] = 1;
  $handler->display->display_options['fields']['biblio_primary_contributors']['delta_offset'] = '0';
  $handler->display->display_options['fields']['biblio_primary_contributors']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['field_api_classes'] = 0;
  /* Field: Biblio: Year of Publication */
  $handler->display->display_options['fields']['biblio_year']['id'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['table'] = 'field_data_biblio_year';
  $handler->display->display_options['fields']['biblio_year']['field'] = 'biblio_year';
  $handler->display->display_options['fields']['biblio_year']['label'] = '';
  $handler->display->display_options['fields']['biblio_year']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_year']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_year']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_year']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_year']['element_type'] = '0';
  $handler->display->display_options['fields']['biblio_year']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_year']['element_wrapper_type'] = 'span';
  $handler->display->display_options['fields']['biblio_year']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_year']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_year']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_year']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_year']['field_api_classes'] = 0;
  /* Field: Biblio: Title */
  $handler->display->display_options['fields']['biblio_title']['id'] = 'biblio_title';
  $handler->display->display_options['fields']['biblio_title']['table'] = 'field_data_biblio_title';
  $handler->display->display_options['fields']['biblio_title']['field'] = 'biblio_title';
  $handler->display->display_options['fields']['biblio_title']['label'] = '';
  $handler->display->display_options['fields']['biblio_title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_title']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['biblio_title']['alter']['path'] = 'biblio/[bid]';
  $handler->display->display_options['fields']['biblio_title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_title']['alter']['link_class'] = 'biblio-title';
  $handler->display->display_options['fields']['biblio_title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_title']['element_type'] = '0';
  $handler->display->display_options['fields']['biblio_title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_title']['element_wrapper_type'] = 'span';
  $handler->display->display_options['fields']['biblio_title']['element_wrapper_class'] = 'biblio-title';
  $handler->display->display_options['fields']['biblio_title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_title']['field_api_classes'] = 0;
  /* Field: Biblio: Secondary Title */
  $handler->display->display_options['fields']['biblio_title_secondary']['id'] = 'biblio_title_secondary';
  $handler->display->display_options['fields']['biblio_title_secondary']['table'] = 'field_data_biblio_title_secondary';
  $handler->display->display_options['fields']['biblio_title_secondary']['field'] = 'biblio_title_secondary';
  $handler->display->display_options['fields']['biblio_title_secondary']['label'] = '';
  $handler->display->display_options['fields']['biblio_title_secondary']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_title_secondary']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_title_secondary']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_title_secondary']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_title_secondary']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_title_secondary']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_title_secondary']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_title_secondary']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_title_secondary']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_title_secondary']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_title_secondary']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_title_secondary']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_title_secondary']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_title_secondary']['element_type'] = '0';
  $handler->display->display_options['fields']['biblio_title_secondary']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_title_secondary']['element_wrapper_type'] = 'span';
  $handler->display->display_options['fields']['biblio_title_secondary']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_title_secondary']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_title_secondary']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_title_secondary']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_title_secondary']['field_api_classes'] = 0;
  /* Field: Biblio: Volume */
  $handler->display->display_options['fields']['biblio_volume']['id'] = 'biblio_volume';
  $handler->display->display_options['fields']['biblio_volume']['table'] = 'field_data_biblio_volume';
  $handler->display->display_options['fields']['biblio_volume']['field'] = 'biblio_volume';
  $handler->display->display_options['fields']['biblio_volume']['label'] = '';
  $handler->display->display_options['fields']['biblio_volume']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['biblio_volume']['alter']['text'] = '[biblio_volume]: ';
  $handler->display->display_options['fields']['biblio_volume']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_volume']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_volume']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_volume']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_volume']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_volume']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_volume']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_volume']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_volume']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_volume']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_volume']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_volume']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_volume']['element_type'] = '0';
  $handler->display->display_options['fields']['biblio_volume']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_volume']['element_wrapper_type'] = 'span';
  $handler->display->display_options['fields']['biblio_volume']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_volume']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_volume']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_volume']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_volume']['field_api_classes'] = 0;
  /* Field: Biblio: Description  */
  $handler->display->display_options['fields']['biblio_pages']['id'] = 'biblio_pages';
  $handler->display->display_options['fields']['biblio_pages']['table'] = 'field_data_biblio_pages';
  $handler->display->display_options['fields']['biblio_pages']['field'] = 'biblio_pages';
  $handler->display->display_options['fields']['biblio_pages']['label'] = '';
  $handler->display->display_options['fields']['biblio_pages']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_pages']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_pages']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_pages']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_pages']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_pages']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_pages']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_pages']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_pages']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_pages']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_pages']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_pages']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_pages']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_pages']['element_type'] = '0';
  $handler->display->display_options['fields']['biblio_pages']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_pages']['element_wrapper_type'] = 'span';
  $handler->display->display_options['fields']['biblio_pages']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_pages']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_pages']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_pages']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_pages']['field_api_classes'] = 0;
  /* Sort criterion: Biblio: Year of Publication (biblio_year) */
  $handler->display->display_options['sorts']['biblio_year_value']['id'] = 'biblio_year_value';
  $handler->display->display_options['sorts']['biblio_year_value']['table'] = 'field_data_biblio_year';
  $handler->display->display_options['sorts']['biblio_year_value']['field'] = 'biblio_year_value';
  $handler->display->display_options['sorts']['biblio_year_value']['order'] = 'DESC';
  $handler->display->display_options['sorts']['biblio_year_value']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['biblio_year_value']['expose']['label'] = 'Year';
  /* Sort criterion: Biblio: Title (biblio_title) */
  $handler->display->display_options['sorts']['biblio_title_value']['id'] = 'biblio_title_value';
  $handler->display->display_options['sorts']['biblio_title_value']['table'] = 'field_data_biblio_title';
  $handler->display->display_options['sorts']['biblio_title_value']['field'] = 'biblio_title_value';
  $handler->display->display_options['sorts']['biblio_title_value']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['biblio_title_value']['expose']['label'] = 'Title';

  /* Display: Year */
  $handler = $view->new_display('page', 'Year', 'page');
  $handler->display->display_options['display_description'] = 'Grouped by year';
  $handler->display->display_options['path'] = 'biblio';

  $views[$view->name] = $view;

////////////////////////////////////////////////////////////////////////////////


  $view = new view;
  $view->name = 'biblio_keywords';
  $view->description = 'Shows all Biblio records that are associated with a given taxonomy term ID';
  $view->tag = 'biblio';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Biblio Keywords - Associated Biblio Records';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Biblio Keywords';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access biblio content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = 0;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = 1;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 1;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'name_2',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'name_2' => 'name_2',
    'biblio_title' => 'biblio_title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name_2' => array(
      'sortable' => 0,
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
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: Taxonomy term: Biblio using Keywords */
  $handler->display->display_options['relationships']['reverse_biblio_keywords_biblio']['id'] = 'reverse_biblio_keywords_biblio';
  $handler->display->display_options['relationships']['reverse_biblio_keywords_biblio']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['relationships']['reverse_biblio_keywords_biblio']['field'] = 'reverse_biblio_keywords_biblio';
  $handler->display->display_options['relationships']['reverse_biblio_keywords_biblio']['required'] = 0;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name_2']['id'] = 'name_2';
  $handler->display->display_options['fields']['name_2']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name_2']['field'] = 'name';
  $handler->display->display_options['fields']['name_2']['label'] = 'Biblios with keyword';
  $handler->display->display_options['fields']['name_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name_2']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name_2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name_2']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name_2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name_2']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name_2']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name_2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name_2']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name_2']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name_2']['link_to_taxonomy'] = 0;
  $handler->display->display_options['fields']['name_2']['convert_spaces'] = 0;
  /* Field: Biblio: Title */
  $handler->display->display_options['fields']['biblio_title']['id'] = 'biblio_title';
  $handler->display->display_options['fields']['biblio_title']['table'] = 'field_data_biblio_title';
  $handler->display->display_options['fields']['biblio_title']['field'] = 'biblio_title';
  $handler->display->display_options['fields']['biblio_title']['relationship'] = 'reverse_biblio_keywords_biblio';
  $handler->display->display_options['fields']['biblio_title']['label'] = 'Biblio';
  $handler->display->display_options['fields']['biblio_title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_title']['alter']['make_link'] = 0;
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
  /* Field: Biblio: Keywords */
  $handler->display->display_options['fields']['biblio_keywords']['id'] = 'biblio_keywords';
  $handler->display->display_options['fields']['biblio_keywords']['table'] = 'field_data_biblio_keywords';
  $handler->display->display_options['fields']['biblio_keywords']['field'] = 'biblio_keywords';
  $handler->display->display_options['fields']['biblio_keywords']['relationship'] = 'reverse_biblio_keywords_biblio';
  $handler->display->display_options['fields']['biblio_keywords']['label'] = 'All Keywords';
  $handler->display->display_options['fields']['biblio_keywords']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['biblio_keywords']['alter']['path'] = 'biblio/keywords/[biblio_keywords-tid]';
  $handler->display->display_options['fields']['biblio_keywords']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_keywords']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_keywords']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_keywords']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_keywords']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_keywords']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['biblio_keywords']['group_rows'] = 1;
  $handler->display->display_options['fields']['biblio_keywords']['delta_offset'] = '0';
  $handler->display->display_options['fields']['biblio_keywords']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['biblio_keywords']['field_api_classes'] = 0;
  /* Contextual filter: Taxonomy term: Term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['tid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['tid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['tid']['not'] = 0;
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'biblio_keywords' => 'biblio_keywords',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'biblio/keywords/%';


  $views[$view->name] = $view;



////////////////////////////////////////////////////////////////////////////////


  $view = new view;
  $view->name = 'biblio_contributors_list';
  $view->description = 'List of all Biblio Contributors';
  $view->tag = 'biblio';
  $view->base_table = 'biblio_contributor';
  $view->human_name = 'Biblio Contributors List';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Biblio Contributors';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access biblio content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Contributor: Contributor ID */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'biblio_contributor';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['cid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['cid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['cid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['cid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['cid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['cid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['cid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['cid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['cid']['separator'] = '';
  $handler->display->display_options['fields']['cid']['format_plural'] = 0;
  /* Field: Contributor: Author Name */
  $handler->display->display_options['fields']['biblio_contributor_name']['id'] = 'biblio_contributor_name';
  $handler->display->display_options['fields']['biblio_contributor_name']['table'] = 'field_data_biblio_contributor_name';
  $handler->display->display_options['fields']['biblio_contributor_name']['field'] = 'biblio_contributor_name';
  $handler->display->display_options['fields']['biblio_contributor_name']['label'] = '';
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['path'] = 'biblio/contributor/[cid]/publications';
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_contributor_name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['field_api_classes'] = 0;
  /* Sort criterion: Contributor: Last Name (biblio_contributor_last_name) */
  $handler->display->display_options['sorts']['biblio_contributor_last_name_value']['id'] = 'biblio_contributor_last_name_value';
  $handler->display->display_options['sorts']['biblio_contributor_last_name_value']['table'] = 'field_data_biblio_contributor_last_name';
  $handler->display->display_options['sorts']['biblio_contributor_last_name_value']['field'] = 'biblio_contributor_last_name_value';
  $handler->display->display_options['sorts']['biblio_contributor_last_name_value']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['biblio_contributor_last_name_value']['expose']['label'] = 'Last Name';
  /* Sort criterion: Contributor: First Name (biblio_contributor_first_name) */
  $handler->display->display_options['sorts']['biblio_contributor_first_name_value']['id'] = 'biblio_contributor_first_name_value';
  $handler->display->display_options['sorts']['biblio_contributor_first_name_value']['table'] = 'field_data_biblio_contributor_first_name';
  $handler->display->display_options['sorts']['biblio_contributor_first_name_value']['field'] = 'biblio_contributor_first_name_value';
  $handler->display->display_options['sorts']['biblio_contributor_first_name_value']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['biblio_contributor_first_name_value']['expose']['label'] = 'First Name';
  /* Sort criterion: Contributor: Contributor ID */
  $handler->display->display_options['sorts']['cid']['id'] = 'cid';
  $handler->display->display_options['sorts']['cid']['table'] = 'biblio_contributor';
  $handler->display->display_options['sorts']['cid']['field'] = 'cid';
  $handler->display->display_options['sorts']['cid']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['cid']['expose']['label'] = 'Contributor ID';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'biblio/contributors';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Contributors';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;

  $views[$view->name] = $view;



////////////////////////////////////////////////////////////////////////////////



  $view = new view;
  $view->name = 'biblio_keywords_all';
  $view->description = 'List of all Biblio Keywords';
  $view->tag = 'biblio';
  $view->base_table = 'taxonomy_term_data';
  $view->human_name = 'Biblio Keywords';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Biblio Keywords';
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access biblio content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Taxonomy term: Biblio using Keywords */
  $handler->display->display_options['relationships']['reverse_biblio_keywords_biblio']['id'] = 'reverse_biblio_keywords_biblio';
  $handler->display->display_options['relationships']['reverse_biblio_keywords_biblio']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['relationships']['reverse_biblio_keywords_biblio']['field'] = 'reverse_biblio_keywords_biblio';
  $handler->display->display_options['relationships']['reverse_biblio_keywords_biblio']['required'] = 0;
  /* Field: Taxonomy term: Term ID */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['label'] = '';
  $handler->display->display_options['fields']['tid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['tid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['tid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['tid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['tid']['separator'] = '';
  $handler->display->display_options['fields']['tid']['format_plural'] = 0;
  /* Field: Taxonomy term: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'biblio/keywords/[tid]';
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name']['link_to_taxonomy'] = 0;
  $handler->display->display_options['fields']['name']['convert_spaces'] = 0;
  /* Field: COUNT(Biblio: Biblio ID) */
  $handler->display->display_options['fields']['bid']['id'] = 'bid';
  $handler->display->display_options['fields']['bid']['table'] = 'biblio';
  $handler->display->display_options['fields']['bid']['field'] = 'bid';
  $handler->display->display_options['fields']['bid']['relationship'] = 'reverse_biblio_keywords_biblio';
  $handler->display->display_options['fields']['bid']['group_type'] = 'count';
  $handler->display->display_options['fields']['bid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['bid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['bid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['bid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['bid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['bid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['bid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['bid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['bid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['bid']['separator'] = '';
  $handler->display->display_options['fields']['bid']['format_plural'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[name] ([bid])';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = 0;
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
  /* Sort criterion: Taxonomy term: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  $handler->display->display_options['sorts']['name']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['name']['expose']['label'] = 'Name';
  /* Filter criterion: Taxonomy vocabulary: Machine name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['value'] = array(
    'biblio_keywords' => 'biblio_keywords',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'biblio/keywords';

  $views[$view->name] = $view;



////////////////////////////////////////////////////////////////////////////////


  $view = new view;
  $view->name = 'biblio_admin';
  $view->description = 'List of all Biblio records - emulates the admin/content page';
  $view->tag = 'biblio, administration';
  $view->base_table = 'biblio';
  $view->human_name = 'Biblio Admin - Biblio List';
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
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
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
  /* Field: Biblio: Publication_type */
  $handler->display->display_options['fields']['publication_type']['id'] = 'publication_type';
  $handler->display->display_options['fields']['publication_type']['table'] = 'biblio';
  $handler->display->display_options['fields']['publication_type']['field'] = 'publication_type';
  $handler->display->display_options['fields']['publication_type']['label'] = 'Publication Type';
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
  /* Field: Biblio: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'biblio';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = 'Author';
  $handler->display->display_options['fields']['uid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uid']['hide_alter_empty'] = 1;
  /* Field: Biblio: Changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'biblio';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
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
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Edit';
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
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Delete';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Delete';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'biblio/[bid]/delete';
  $handler->display->display_options['fields']['nothing_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['nothing_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing_1']['hide_alter_empty'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/structure/biblio-list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Biblio view';
  $handler->display->display_options['menu']['description'] = 'asdf';
  $handler->display->display_options['menu']['weight'] = '5';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['title'] = 'a title?';
  $handler->display->display_options['tab_options']['weight'] = '0';

  $views[$view->name] = $view;




////////////////////////////////////////////////////////////////////////////////


  $view = new view;
  $view->name = 'biblio_contributors_list_2';
  $view->description = 'Displays all Biblios with a given contributor';
  $view->tag = 'default';
  $view->base_table = 'biblio';
  $view->human_name = 'Biblio Contributors List 2';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Biblios by Contributor';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'biblio_contributor_name',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'bid' => 'bid',
    'biblio_title' => 'biblio_title',
    'cid' => 'cid',
    'biblio_contributor_name' => 'biblio_contributor_name',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'bid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'biblio_title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'biblio_contributor_name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['biblio_primary_contributors_target_id']['id'] = 'biblio_primary_contributors_target_id';
  $handler->display->display_options['relationships']['biblio_primary_contributors_target_id']['table'] = 'field_data_biblio_primary_contributors';
  $handler->display->display_options['relationships']['biblio_primary_contributors_target_id']['field'] = 'biblio_primary_contributors_target_id';
  $handler->display->display_options['relationships']['biblio_primary_contributors_target_id']['required'] = 0;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['biblio_secondary_contributors_target_id']['id'] = 'biblio_secondary_contributors_target_id';
  $handler->display->display_options['relationships']['biblio_secondary_contributors_target_id']['table'] = 'field_data_biblio_secondary_contributors';
  $handler->display->display_options['relationships']['biblio_secondary_contributors_target_id']['field'] = 'biblio_secondary_contributors_target_id';
  $handler->display->display_options['relationships']['biblio_secondary_contributors_target_id']['required'] = 0;
  /* Field: Biblio: Biblio ID */
  $handler->display->display_options['fields']['bid']['id'] = 'bid';
  $handler->display->display_options['fields']['bid']['table'] = 'biblio';
  $handler->display->display_options['fields']['bid']['field'] = 'bid';
  $handler->display->display_options['fields']['bid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['bid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['bid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['bid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['bid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['bid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['bid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['bid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['bid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['bid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['bid']['format_plural'] = 0;
  /* Field: Biblio: Title */
  $handler->display->display_options['fields']['biblio_title']['id'] = 'biblio_title';
  $handler->display->display_options['fields']['biblio_title']['table'] = 'field_data_biblio_title';
  $handler->display->display_options['fields']['biblio_title']['field'] = 'biblio_title';
  $handler->display->display_options['fields']['biblio_title']['label'] = 'Biblio';
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
  /* Field: Contributor: Contributor ID */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'biblio_contributor';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['relationship'] = 'biblio_primary_contributors_target_id';
  $handler->display->display_options['fields']['cid']['label'] = '';
  $handler->display->display_options['fields']['cid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['cid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['cid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['cid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['cid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['cid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['cid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['cid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['cid']['separator'] = '';
  $handler->display->display_options['fields']['cid']['format_plural'] = 0;
  /* Field: Contributor: Author Name */
  $handler->display->display_options['fields']['biblio_contributor_name']['id'] = 'biblio_contributor_name';
  $handler->display->display_options['fields']['biblio_contributor_name']['table'] = 'field_data_biblio_contributor_name';
  $handler->display->display_options['fields']['biblio_contributor_name']['field'] = 'biblio_contributor_name';
  $handler->display->display_options['fields']['biblio_contributor_name']['relationship'] = 'biblio_primary_contributors_target_id';
  $handler->display->display_options['fields']['biblio_contributor_name']['label'] = 'Biblios by Contributor';
  $handler->display->display_options['fields']['biblio_contributor_name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['field_api_classes'] = 0;
  /* Field: Biblio: Authors */
  $handler->display->display_options['fields']['biblio_primary_contributors']['id'] = 'biblio_primary_contributors';
  $handler->display->display_options['fields']['biblio_primary_contributors']['table'] = 'field_data_biblio_primary_contributors';
  $handler->display->display_options['fields']['biblio_primary_contributors']['field'] = 'biblio_primary_contributors';
  $handler->display->display_options['fields']['biblio_primary_contributors']['label'] = 'All Primary Contributors';
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['biblio_primary_contributors']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_primary_contributors']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_primary_contributors']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['biblio_primary_contributors']['group_rows'] = 1;
  $handler->display->display_options['fields']['biblio_primary_contributors']['delta_offset'] = '0';
  $handler->display->display_options['fields']['biblio_primary_contributors']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['biblio_primary_contributors']['field_api_classes'] = 0;
  /* Contextual filter: Contributor: Contributor ID */
  $handler->display->display_options['arguments']['cid']['id'] = 'cid';
  $handler->display->display_options['arguments']['cid']['table'] = 'biblio_contributor';
  $handler->display->display_options['arguments']['cid']['field'] = 'cid';
  $handler->display->display_options['arguments']['cid']['relationship'] = 'biblio_primary_contributors_target_id';
  $handler->display->display_options['arguments']['cid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['cid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['cid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['cid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['cid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['cid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['cid']['not'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'biblio/contributor/%/publications';

  $views[$view->name] = $view;




////////////////////////////////////////////////////////////////////////////////


  $view = new view;
  $view->name = 'biblio_admin_contributors';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'biblio_contributor';
  $view->human_name = 'Biblio Admin - Contributors';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Biblio Admin - Contributors';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = 0;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = 1;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Contributor: Contributor ID */
  $handler->display->display_options['fields']['cid']['id'] = 'cid';
  $handler->display->display_options['fields']['cid']['table'] = 'biblio_contributor';
  $handler->display->display_options['fields']['cid']['field'] = 'cid';
  $handler->display->display_options['fields']['cid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['cid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['cid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['cid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['cid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['cid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['cid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['cid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['cid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['cid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['cid']['separator'] = '';
  $handler->display->display_options['fields']['cid']['format_plural'] = 0;
  /* Field: Contributor: Author Name */
  $handler->display->display_options['fields']['biblio_contributor_name']['id'] = 'biblio_contributor_name';
  $handler->display->display_options['fields']['biblio_contributor_name']['table'] = 'field_data_biblio_contributor_name';
  $handler->display->display_options['fields']['biblio_contributor_name']['field'] = 'biblio_contributor_name';
  $handler->display->display_options['fields']['biblio_contributor_name']['label'] = '';
  $handler->display->display_options['fields']['biblio_contributor_name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['biblio_contributor_name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['biblio_contributor_name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['biblio_contributor_name']['field_api_classes'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'biblio/contributor/[cid]/edit';
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
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = '';
  $handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'delete';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'biblio/contributor/[cid]/delete';
  $handler->display->display_options['fields']['nothing_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing_1']['hide_alter_empty'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = '';
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = '[biblio_contributor_name] [[nothing]][[nothing_1]]';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing_2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing_2']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing_2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing_2']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing_2']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing_2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing_2']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing_2']['hide_alter_empty'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['path'] = 'admin/config/content/biblio/views/admin-contributors';

  $views[$view->name] = $view;


////////////////////////////////////////////////////////////////////////////////
  return $views;
}
