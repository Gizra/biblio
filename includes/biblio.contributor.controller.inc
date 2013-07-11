<?php

/**
 * Contributor class.
 */
class BiblioContributor extends Entity {

  public function __construct($values = array()) {
    parent::__construct($values, 'biblio_contributor');
    if (!empty($this->cid)) {
      return;
    }

    if (empty($values['name'])) {
      return;
    }

    $name = $values['name'];
    if (is_array($values['name'])) {
      $wrapper = entity_metadata_wrapper('biblio_contributor', $this);

      // @todo: Add fields if not yet added.
      if ($name['family']) {
        $wrapper->contributor_family = $name['family'];
      }

      if ($name['given']) {
        $wrapper->contributor_given = $name['given'];
      }

      $this->name = implode(', ', $this->name);
    }
    else {
      $this->name = $name;
    }
  }
}

