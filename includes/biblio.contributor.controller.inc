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
   * @todo: Re-build the contributor name.
   */
  public function save() {
    $this->name = $this->lastname ? $this->firstname . ' ' . $this->lastname : $this->firstname;

    $this->md5 = BiblioContributor::generateBiblioContributorMd5($this);

    parent::save();
  }
}

