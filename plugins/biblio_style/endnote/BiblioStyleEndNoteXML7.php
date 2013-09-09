<?php

/**
 * @file
 * EndNote XML7 biblio style.
 */

class BiblioStyleEndNoteXML7 extends BiblioStyleEndNoteXML8 {

  // @todo: Remove.
  public $biblio = NULL;

  /**
   * Import XML.
   */
  public function import($data, $options = array()) {
    $options['pattern'] = '/<REFERENCE_TYPE>(.*)<\/REFERENCE_TYPE>/';
    return parent::import($data, $options);
  }

  /**
   * Render tagged.
   *
   * @todo: Remove option to render with XML7.
   */
  public function render($options = array(), $langcode = NULL) {

  }

  public function getMapping() {
    $return = array(
      'type' => array(
        0 => 'journal_article',
        1 => 'book',
        2 => 'thesis',
        3 => 'conference_proceedings',
        4 => 'personal',
        5 => 'newspaper_article',
        6 => 'software',
        7 => 'book_chapter',
        8 => 'magazine_article',
        9 => 'edited',
        10 => 'report',
        11 => 'map',
        12 => 'audiovisual',
        13 => 'artwork',
        15 => 'patent',
        16 => 'web_article',
        17 => 'bill',
        18 => 'case',
        19 => 'hearing',
        20 => 'manuscript',
        21 => 'film',
        22 => 'statute',
        26 => 'chart',
        31 => 'miscellaneous',
      ),
    );

    $parent_map = parent::getMapping();

    $info = array(
      'ABSTRACT' => 'abstract',
      'ACCESSION_NUMBER' => 'accession-num',
      'ALTERNATE_TITLE' => 'alt-title',
      'CALL_NUMBER' => 'call-num',
      'EDITION' => 'edition',
      'ISBN' => 'isbn',
      'ISSUE' => 'issue',
      'LABEL' => 'label',
      'NOTES' => 'notes',
      'NUMBER' => 'number',
      'NUMBER_OF_VOLUMES' => 'num-vols',
      'ORIGINAL_PUB' => 'orig-pub',
      'PAGES' => 'pages',
      'PLACE_PUBLISHED' => 'pub-location',
      'PUBLISHER' => 'publisher',
      'REPRINT_EDITION' => 'reprint-edition',
      'SECONDARY_TITLE' => 'secondary-title',
      'SECTION' => 'section',
      'SHORT_TITLE' => 'short-title',
      'TERTIARY_TITLE' => 'tertiary-title',
      'TYPE_OF_WORK' => 'work-type',
      'URL' => 'url',
      'VOLUME' => 'volume',
      'YEAR' => 'year',
    );

    foreach ($info as $xml7 => $xml8) {
      // Map the field info using the XML7 key but with the XML8 definitions.
      $return['field'][$xml7] = $parent_map['field'][$xml8];
    }

    return $return;
  }
}
