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
  /* Filter criterion: Biblio: Publication_type */
  $handler->display->display_options['filters']['publication_type']['id'] = 'publication_type';
  $handler->display->display_options['filters']['publication_type']['table'] = 'biblio';
  $handler->display->display_options['filters']['publication_type']['field'] = 'publication_type';
  $handler->display->display_options['filters']['publication_type']['value'] = array(
    'all' => 'all',
    'book' => 'book',
    'book_chapter' => 'book_chapter',
    'journal_article' => 'journal_article',
    'journal' => 'journal',
    'conference_paper' => 'conference_paper',
    'conference_proceedings' => 'conference_proceedings',
    'newspaper_article' => 'newspaper_article',
    'magazine_article' => 'magazine_article',
    'web_article' => 'web_article',
    'website' => 'website',
    'web_service' => 'web_service',
    'web_project_page' => 'web_project_page',
    'thesis' => 'thesis',
    'report' => 'report',
    'film' => 'film',
    'broadcast' => 'broadcast',
    'artwork' => 'artwork',
    'software' => 'software',
    'audiovisual' => 'audiovisual',
    'hearing' => 'hearing',
    'case' => 'case',
    'bill' => 'bill',
    'statute' => 'statute',
    'patent' => 'patent',
    'personal' => 'personal',
    'manuscript' => 'manuscript',
    'map' => 'map',
    'chart' => 'chart',
    'unpublished' => 'unpublished',
    'database' => 'database',
    'government_report' => 'government_report',
    'classical' => 'classical',
    'legal_ruling' => 'legal_ruling',
    'miscellaneous' => 'miscellaneous',
    'miscellaneous_section' => 'miscellaneous_section',
    'presentation' => 'presentation',
  );
  $handler->display->display_options['filters']['publication_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['publication_type']['expose']['operator_id'] = 'publication_type_op';
  $handler->display->display_options['filters']['publication_type']['expose']['label'] = 'Publication Type';
  $handler->display->display_options['filters']['publication_type']['expose']['use_operator'] = 1;
  $handler->display->display_options['filters']['publication_type']['expose']['operator'] = 'publication_type_op';
  $handler->display->display_options['filters']['publication_type']['expose']['identifier'] = 'publication_type';
  $handler->display->display_options['filters']['publication_type']['expose']['remember'] = 1;
  $handler->display->display_options['filters']['publication_type']['expose']['multiple'] = 1;
  $handler->display->display_options['filters']['publication_type']['expose']['reduce'] = 0;

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
  return $views;
}
