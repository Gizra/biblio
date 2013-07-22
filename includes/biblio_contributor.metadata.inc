<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the default OG membership metadata properties.
 */
class BiblioContributorMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    // Initialize values to prevent WSOD, on certain cases of clear cache.
    $properties = !empty($properties) ? $properties : array();

    $names = array(
      'name',
      'lastname',
      'firstname',
      'prefix',
      'suffix',
      'initials'

      // @todo: convert affiliation to reference?

      // @todo: make literal boolean.

    );

    foreach ($names as $name) {
      $properties[$name]['getter callback'] = 'entity_property_verbatim_get';
      $properties[$name]['setter callback'] = 'entity_property_verbatim_set';
    }

    return $info;
  }
}
