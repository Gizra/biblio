<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class BiblioMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['title']['setter callback'] = 'entity_property_verbatim_set';
    $properties['language']['setter callback'] = 'entity_property_verbatim_set';

    return $info;
  }
}
