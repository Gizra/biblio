<?php

/**
 * Biblio class.
 */
class Biblio extends Entity {

  public function __construct($values = array()) {
    parent::__construct($values, 'biblio');
  }

  /**
   * Get the text, rendered using a Biblio style plugin.
   *
   * @param $style_name
   *   The Biblio style plugin.
   */
  public function getText($style_name, $options = array()) {
    if (!$plugin = biblio_get_biblio_style($style_name)) {
      // @todo: Add own exception.
      throw new Exception(format_string('Biblio style "@style" does not exist.'), array('@style' => $style_name));
    }

    $plugin['options'] = drupal_array_merge_deep($plugin['options'], $options);

    $class = ctools_plugin_load_class('biblio', 'biblio_style', $style_name, 'class');
    $biblio_style = new $class($plugin, $this);
    return $biblio_style->render();
  }
}
