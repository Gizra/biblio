<?php
class biblio_plugin_display_page extends views_plugin_display_page {
  function option_definition() {
    $options = parent::option_definition();

    $options['path']['default'] = 'biblio';
    $options['title']['default'] = 'Biblio';
    $options['style_plugin']['default'] = 'biblio_cite';
    $options['defaults']['default']['style_plugin'] = FALSE;
    $options['defaults']['default']['style_options'] = FALSE;
    $options['defaults']['default']['row_plugin'] = FALSE;
    $options['defaults']['default']['row_options'] = FALSE;
    $options['defaults']['default']['title'] = FALSE;

    return $options;
  }


  function query() {

    return parent::query();
  }

}