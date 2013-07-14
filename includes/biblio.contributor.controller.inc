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

    if (isset($values['family']) && isset($values['given'])) {
      $wrapper = entity_metadata_wrapper('biblio_contributor', $this);

      // @todo: Add fields if not yet added.
      $wrapper->contributor_family = $values['family'];
      $wrapper->contributor_given = $values['given'];

      $this->name = implode(', ', $this->name);
      return;
    }
    elseif (!empty($values['name'])) {
      $this->name = $values['name'];
    }
  }

  public function save() {
    $wrapper = entity_metadata_wrapper('biblio_contributor', $this);

    if (!$wrapper->contributor_family->value() && !$wrapper->contributor_given->value()) {
      // Set the "given" snd "family" based in the name property.
      $name = str_replace(',', '', $this->name);
      $name = explode(' ', $name, 2);

      // Try to be smart and use words with a dot as the first name.
      if (strpos($name[0], '.') === TRUE) {
        $wrapper->contributor_family->set(!empty($name[1]) ? $name[1] : '');
        $wrapper->contributor_given->set($name[0]);
      }
      else {
        $wrapper->contributor_family->set($name[0]);
        $wrapper->contributor_given->set(!empty($name[1]) ? $name[1] : '');
      }
    }

    parent::save();
  }
}

