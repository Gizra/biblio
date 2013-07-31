<?php

/**
 * @file
 * Extending Citeproc biblio style example.
 */

class BiblioStyleExampleCiteProc extends BiblioStyleCiteProc {

  public function render($options = array(), $langcode = NULL) {
    $output = parent::render($options, $langcode);

    if (empty($this->biblio->title_no_url)) {
      // Convert the title to a URL referencing the bilbio.
      $url = entity_uri('biblio', $this->biblio);
      $output = str_replace($this->biblio->title, l($this->biblio->title, $url['path'], $url['options']), $output);
    }

    $wrapper = entity_metadata_wrapper('biblio', $this->biblio);
    if (isset($wrapper->biblio_abstract) && $abstract = $wrapper->biblio_abstract->value()) {
      // Add the abstract.
      $id = $wrapper->getIdentifier();
      $output .= '<br/><span class="show-abstract" bid="' . $id .'">ABSTRACT</span></br>';
      $output .= '<div style="display: none;" class="bid-' . $id .'">' . substr($abstract, 0, 100) . '</div>';
    }

    return $output;
  }
}