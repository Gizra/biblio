<?php

/**
 * @file
 * Extending Citeproc biblio style example.
 */

class BiblioStyleExampleCiteProc extends BiblioStyleCiteProc {

  public function render($options = array(), $langcode = NULL) {
    $output = parent::render($options, $langcode);

    $wrapper = entity_metadata_wrapper('biblio', $this->biblio);
    if (isset($wrapper->biblio_abstract) && $abstract = $wrapper->biblio_abstract->value()) {
      // Add the abstract.
      $output .= "<br />" . $abstract;
    }
    return $output;
  }
}
