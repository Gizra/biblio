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

  function endNote7StartElement($parser, $name, $attrs) {
    switch ($name) {
      case 'RECORD' :
        $this->biblio = new stdClass();
        $this->biblio->biblio_contributors = array();
        $this->biblio->biblio_type = 102; // we set 102 here because the xml parser won't
        // process a value of 0 (ZERO) which is the
        // ref-type 102. if there is a non-zero value it will be overwritten
        $this->element = '';
        break;
      case 'AUTHORS':
      case 'SECONDARY_AUTHORS':
      case 'TERTIARY_AUTHORS':
      case 'SUBSIDIARY_AUTHORS':
        $this->contrib_count = 0;
        $this->contributors = array();
        break;
      case 'AUTHOR':
      case 'SECONDARY_AUTHOR':
      case 'TERTIARY_AUTHOR':
      case 'SUBSIDIARY_AUTHOR':
        $this->contributors[$this->contrib_count]['name'] = '';
        $this->element = $name;
        break;
      case 'KEYWORDS':
        $this->keyword_count = 0;
        break;
      case 'KEYWORD':
        $this->biblio->biblio_keywords[$this->keyword_count] = '';
        $this->element = $name;
        break;
      default:
        $this->element = $name;
    }
  }

  function endNote7EndElement($parser, $name) {
    switch ($name) {
      case 'RECORD' :
        $this->biblio->biblio_xml_md5 = md5(serialize($this->biblio));
        if ( !($dup = $this->biblio_xml_check_md5($this->biblio->biblio_xml_md5)) ) {
          biblio_save_node($this->biblio, $this->terms, $this->batch_proc, $this->session_id);
          if (!empty($this->biblio->nid)) $this->nids[] = $this->biblio->nid;
        }
        else {
          $this->dups[] = $dup;
        }
        break;
      case 'AUTHORS':
      case 'SECONDARY_AUTHORS':
      case 'TERTIARY_AUTHORS':
      case 'SUBSIDIARY_AUTHORS':
        $this->contributors_type = '';
        foreach ($this->contributors as $contributor) {
          $this->biblio->biblio_contributors[] = $contributor;
        }
        break;
      case 'AUTHOR':
        $this->contributors[$this->contrib_count]['auth_category'] = 1;
        $this->contributors[$this->contrib_count]['auth_type'] = 1;
        $this->contrib_count++;
        break;
      case 'SECONDARY_AUTHOR':
        $this->contributors[$this->contrib_count]['auth_category'] = 2;
        $this->contributors[$this->contrib_count]['auth_type'] = 2;
        $this->contrib_count++;
        break;
      case 'TERTIARY_AUTHOR':
        $this->contributors[$this->contrib_count]['auth_category'] = 3;
        $this->contributors[$this->contrib_count]['auth_type'] = 3;
        $this->contrib_count++;
        break;
      case 'SUBSIDIARY_AUTHOR':
        $this->contributors[$this->contrib_count]['auth_category'] = 4;
        $this->contributors[$this->contrib_count]['auth_type'] = 4;
        $this->contrib_count++;
        break;
      case 'KEYWORD':
        $this->keyword_count++;
        break;
      default:

    }
    $this->element = '';
  }

  function endNote7CharacterData($parser, $data) {
    if (trim($data)) {
      switch ($this->element) {
        case 'REFERENCE_TYPE':
          $this->biblio->biblio_type = $this->type_map($data);
          break;
        case 'AUTHOR':
        case 'SECONDARY_AUTHOR':
        case 'TERTIARY_AUTHOR':
        case 'SUBSIDIARY_AUTHOR':
          $this->contributors[$this->contrib_count]['name'] .= $data;
          break;
        case 'KEYWORD':
          $this->biblio->biblio_keywords[$this->keyword_count] .= $data;
          break;
        case 'TITLE':
          $this->biblio->title .= $data;
          break;
        default:
          if ($field = $this->field_map(trim($this->element))) {
            $this->biblio->$field .= $data;
          }
          else {
            if (!in_array($this->element, $this->unmapped)) {
              $this->unmapped[] = $this->element;
            }
          }
      }
    }
  }

  public function field_map() {}

  public function type_map() {}


  /**
   * Render tagged.
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
        4 => 'personal_communication',
        5 => 'newspaper_article',
        6 => 'computer_program',
        7 => 'book_section',
        8 => 'magazine_article',
        9 => 'edited_book',
        10 => 'report',
        11 => 'map',
        12 => 'audiovisual_material',
        13 => 'artwork',
        15 => 'patent',
        16 => 'electronic_source',
        17 => 'bill',
        18 => 'case',
        19 => 'hearing',
        20 => 'manuscript',
        21 => 'broadcast',
        22 => 'statute',
        26 => 'chart',
        31 => 'generic',
      ),
      'fields' => array(
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
        'REFERENCE_TYPE' => array('property' => 'type'),
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

    return $return;
  }
}
