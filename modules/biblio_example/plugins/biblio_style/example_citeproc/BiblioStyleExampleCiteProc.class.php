<?php

/**
 * @file
 * Extending Citeproc biblio style example.
 */

class BiblioStyleExampleCiteProc extends BiblioStyleCiteProc {

  public function render($options = array(), $langcode = NULL) {
    $output = parent::render($options, $langcode);

    // Get citation.
    if (empty($this->biblio->title_no_url)) {
      // Convert the title to a URL referencing the bilbio.
      $url = entity_uri('biblio', $this->biblio);
      $citation = str_replace($this->biblio->title, l($this->biblio->title, $url['path'], $url['options']), $output);
    }

    // Get abstract.
    $wrapper = entity_metadata_wrapper('biblio', $this->biblio);
    $abstract = isset($wrapper->biblio_abstract) ? $wrapper->biblio_abstract->value() : '';

    $items = array();
    foreach ($wrapper->biblio_pdf->value() as $pdf_file) {
      $items[] = file_create_url($pdf_file['uri']);
    }

    $variables = array(
      'bid' => $wrapper->getIdentifier(),
      'image' => theme('image_style'),
      'citation' => $citation,
      'abstract' => $abstract,
      'pdf_list' => theme('item_list', array('items' => $items)),
    );

    return theme('biblio_example_citeproc', $variables);
  }
}