<?php

/**
 * @file
 * Extending Citeproc biblio style example.
 */

class BiblioStyleExampleCiteProc extends BiblioStyleCiteProc {

  public function render($options = array(), $langcode = NULL) {
    if (empty($this->biblio->title_no_url)) {
      // Convert the title to a URL referencing the bilbio.
      $url = entity_uri('biblio', $this->biblio);

      // @todo: Find a better way to fix the title.
      $this->biblio->title = l($this->biblio->title, $url['path'], $url['options']);
    }
    $output = parent::render($options, $langcode);

    $wrapper = entity_metadata_wrapper('biblio', $this->biblio);
    if (isset($wrapper->biblio_abstract) && $abstract = $wrapper->biblio_abstract->value()) {
      // Add the abstract.
      $output .= "<br />" . substr($abstract,0, 100);
    }

    return $output;
  }
}
