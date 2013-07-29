<?php

/**
 * Contributor class.
 */
class BiblioContributor extends Entity {

  public function __construct($values = array()) {
    parent::__construct($values, 'biblio_contributor');
  }

  /**
   * Overrides Entity::save().
   *
   * Re-build the contributor name.
   */
  public function save() {
    $this->name = $this->lastname . ' ' . $this->firstname;
    parent::save();
  }
}

