<?php

/**
 * @file
 * EndNote XML biblio style.
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
      'field' => array(
        'ABSTRACT' => array('property' => 'biblio_abstract'),
        'ACCESSION_NUMBER' => array('property' => 'biblio_accession_number'),
        'ALTERNATE_TITLE' => array('property' => 'biblio_alternate_title'),
        'CALL_NUMBER' => array('property' => 'biblio_call_number'),
        'DATE' => array('property' => 'biblio_date'),
        'EDITION' => array('property' => 'biblio_edition'),
        'ISBN' => array('property' => 'biblio_isbn'),
        'ISSUE' => array('property' => 'biblio_issue'),
        'LABEL' => array('property' => 'biblio_label'),
        'NOTES' => array('property' => 'biblio_notes'),
        'NUMBER' => array('property' => 'biblio_number'),
        'NUMBER_OF_VOLUMES' => array('property' => 'biblio_number_of_volumes'),
        'ORIGINAL_PUB' => array('property' => 'biblio_original_publication'),
        'PAGES' => array('property' => 'biblio_pages'),
        'PLACE_PUBLISHED' => array('property' => 'biblio_place_published'),
        'PUBLISHER' => array('property' => 'biblio_publisher'),
        'REPRINT_EDITION' => array('property' => 'biblio_reprint_edition'),
        'SECONDARY_TITLE' => array('property' => 'biblio_secondary_title'),
        'SECTION' => array('property' => 'biblio_section'),
        'SHORT_TITLE' => array('property' => 'biblio_short_title'),
        'TERTIARY_TITLE' => array('property' => 'biblio_tertiary_title'),
        'TYPE_OF_WORK' => array('property' => 'biblio_type_of_work'),
        'URL' => array('property' => 'biblio_url'),
        'VOLUME' => array('property' => 'biblio_volume'),
        'YEAR' => array('property' => 'biblio_year'),
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
