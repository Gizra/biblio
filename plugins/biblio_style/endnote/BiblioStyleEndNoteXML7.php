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
        0 => 'Journal Article',
        1 => 'Book',
        2 => 'Thesis',
        3 => 'Conference Proceedings',
        4 => 'Personal Communication',
        5 => 'NewsPaper Article',
        6 => 'Computer Program',
        7 => 'Book Section',
        8 => 'Magazine Article',
        9 => 'Edited Book',
        10 => 'Report',
        11 => 'Map',
        12 => 'Audiovisual Material',
        13 => 'Artwork',
        15 => 'Patent',
        16 => 'Electronic Source',
        17 => 'Bill',
        18 => 'Case',
        19 => 'Hearing',
        20 => 'Manuscript',
        21 => 'Film or Broadcast',
        22 => 'Statute',
        26 => 'Chart or Table',
        31 => 'Generic',
      ),
      'fields' => array(
        'ABSTRACT' => 'biblio_abst_e',
        'ACCESSION_NUMBER' => 'biblio_accession_number',
        'ALTERNATE_TITLE' => 'biblio_alternate_title',
        'CALL_NUMBER' => 'biblio_call_number',
        'CUSTOM1' => 'biblio_custom1',
        'CUSTOM2' => 'biblio_custom2',
        'CUSTOM3' => 'biblio_custom3',
        'CUSTOM4' => 'biblio_custom4',
        'CUSTOM5' => 'biblio_custom5',
        'CUSTOM6' => 'biblio_custom6',
        'DATE' => 'biblio_date',
        'EDITION' => 'biblio_edition',
        'ISBN' => 'biblio_isbn',
        'ISSUE' => 'biblio_issue',
        'LABEL' => 'biblio_label',
        'NOTES' => 'biblio_notes',
        'NUMBER' => 'biblio_number',
        'NUMBER_OF_VOLUMES' => 'biblio_number_of_volumes',
        'ORIGINAL_PUB' => 'biblio_original_publication',
        'PAGES' => 'biblio_pages',
        'PLACE_PUBLISHED' => 'biblio_place_published',
        'PUBLISHER' => 'biblio_publisher',
        // TODO:  Check if we need to write this to the type key or into external
        //        field.
        'REFERENCE_TYPE' => 'type',
        'REPRINT_EDITION' => 'biblio_reprint_edition',
        'SECONDARY_TITLE' => 'biblio_secondary_title',
        'SECTION' => 'biblio_section',
        'SHORT_TITLE' => 'biblio_short_title',
        'TERTIARY_TITLE' => 'biblio_tertiary_title',
        'TYPE_OF_WORK' => 'biblio_type_of_work',
        'URL' => 'biblio_url',
        'VOLUME' => 'biblio_volume',
        'YEAR' => 'biblio_year',
      ),
    );

    return $return;
  }
}
