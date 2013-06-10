<?php

/**
 * @file
 * Provides support for the Views module.
 */
/**
 * "Bilbio" entity Views definition.
 */
class BiblioViewsController extends EntityDefaultViewsController {

  /**
   * Override views_data().
   */
  public function views_data() {
    $data = parent::views_data();

    $data['biblio']['biblio_render']['field'] = array(
      'title' => 'Render bilbio (Get text)',
      'real field' => 'bid',
      'help' => t('Render the Bilbio as a specific style.'),
      'handler' => 'biblio_handler_field_biblio_render',
    );

    return $data;
  }
}
