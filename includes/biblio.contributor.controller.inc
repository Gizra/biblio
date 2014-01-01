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
    $this->md5 = BiblioContributorUtility::generateBiblioContributorMd5($this);

    parent::save();
  }
}

