<?php

/**
 * @file
 * EndNote XML biblio style.
 */

class BiblioStyleEndNoteXML8 extends BiblioStyleEndNote {

  // @todo: Remove.
  public $biblio = NULL;

  /**
   * Import XML.
   */
  public function import($data, $options = array()) {
    $options += array(
      // Populate the pattern here, so BiblioStyleEndNoteXML7 can re-use
      // this class.
      'pattern' => '/<ref-type>(.*)<\/ref-type>/',
    );
    $match = array();

    // Get the Biblio type from the XML.
    preg_match($options['pattern'], $data, $match);
    if (!$type = intval($match[1])) {
      return;
    }

    $type = $this->getBiblioType($type);
    $this->biblio = biblio_create($type);

    $data = str_replace("\r\n", "\n", $data);

    $parser = drupal_xml_parser_create($data);
    xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, FALSE);
    xml_parser_set_option($parser, XML_OPTION_SKIP_WHITE, TRUE);
    xml_set_object($parser, $this);
    xml_set_element_handler($parser, 'startElement', 'endElement');
    xml_set_character_data_handler($parser, 'characterData');

    if (!xml_parse($parser, $data)) {
      $params = array(
        '@error' => xml_error_string(xml_get_error_code($parser)),
        '@line' => xml_get_current_line_number($parser),
      );
      drupal_set_message(t('XML error: @error at line @line'), $params);
    }

    xml_parser_free($parser);
  }


  function startElement($parser, $name, $attrs) {
    switch ($name) {
      case 'style' :
        $this->font_attr = explode(' ', $attrs['face']);
        foreach ($this->font_attr as $fatt) {
          switch ($fatt) {
            case 'normal':
              break;
            case 'bold':
              $this->characterData(NULL, '<b>');
              break;
            case 'italic':
              $this->characterData(NULL, '<i>');
              break;
            case 'underline':
              $this->characterData(NULL, '<u>');
              break;
            case 'superscript':
              $this->characterData(NULL, '<sup>');
              break;
            case 'subscript':
              $this->characterData(NULL, '<sub>');
              break;
          }
        }
        break;

      case 'keywords' :
        $this->keyword_count = 0;
        break;

      case 'authors' :
      case 'secondary-authors' :
      case 'tertiary-authors' :
      case 'subsidiary-authors' :
      case 'translated-authors' :
        $this->contributors_type = $name;
        $this->contributors = array();
        $this->contrib_count = 0;
        break;

      case 'author' :
        $this->contributors[$this->contrib_count]['name'] = '';
        $this->element = $name;
        break;

      case 'year' :
      case 'pub-dates' :
      case 'copyright-dates' :
        $this->dates = $name;
        break;

      case 'web-urls' :
      case 'pdf-urls' :
      case 'text-urls' :
      case 'related-urls' :
      case 'image-urls' :
        $this->urls = $name;
        break;

      case 'keyword':
        $this->biblio->biblio_keywords[$this->keyword_count] = '';
        $this->element = $name;
        break;

      default :
        $this->element = $name;
    }
  }

  function endElement($parser, $name) {
    //    global $this->biblio, $nids, $this->element, $terms, $batch_proc, $session_id, $this->contributors_type, $this->contrib_count, $this->dates, $this->urls, $this->keyword_count, $this->font_attr;
    switch ($name) {
      case 'record' :
        dpm($this->biblio);

        // @todo.
        break;

        $this->element = $this->contributors_type = $this->contrib_count = $this->dates = $this->urls = '';
        $this->biblio->biblio_xml_md5 = md5(serialize($this->biblio));
        if ( !($dup = $this->biblio_xml_check_md5($this->biblio->biblio_xml_md5)) ) {
          biblio_save_node($this->biblio, $this->terms, $this->batch_proc, $this->session_id);
          if (!empty($this->biblio->nid)) $this->nids[] = $this->biblio->nid;
        }
        else {
          $this->dups[] = $dup;
        }
        break;
      case 'authors' :
      case 'secondary-authors' :
      case 'tertiary-authors' :
      case 'subsidiary-authors' :
      case 'translated-authors' :
        $this->contributors_type = '';
        foreach ($this->contributors as $contributor) {
          $this->biblio->biblio_contributors[] = $contributor;
        }
        break;
      case 'author' :
        switch ($this->contributors_type) {
          case 'authors' :
            $this->contributors[$this->contrib_count]['auth_category'] = 1;
            $this->contributors[$this->contrib_count]['auth_type'] =  1;
            break;
          case 'secondary-authors' :
            $this->contributors[$this->contrib_count]['auth_category'] = 2;
            $this->contributors[$this->contrib_count]['auth_type'] = 2;
            break;
          case 'tertiary-authors' :
            $this->contributors[$this->contrib_count]['auth_category'] = 3;
            $this->contributors[$this->contrib_count]['auth_type'] = 3;
            break;
          case 'subsidiary-authors' :
            $this->contributors[$this->contrib_count]['auth_category'] = 4;
            $this->contributors[$this->contrib_count]['auth_type'] = 4;
            break;
          case 'translated-authors' :
            $this->contributors[$this->contrib_count]['auth_category'] = 5;
            $this->contributors[$this->contrib_count]['auth_type'] = 5;
            break;
        }
        $this->contrib_count++;
        break;
      case 'keyword' :
        $this->keyword_count++;
        break;
      case 'year' :
      case 'pub-dates' :
      case 'copyright-dates' :
        $this->dates = '';
        break;
      case 'web-urls' :
      case 'pdf-urls' :
      case 'text-urls' :
      case 'related-urls' :
      case 'image-urls' :
        $this->urls = '';
        break;
      case 'ref-type':
        $this->biblio->biblio_type = $this->type_map($this->biblio->biblio_type);
        $this->element = '';
        break;
      case 'style' :
        foreach ($this->font_attr as $fatt) {
          switch ($fatt) {
            case 'normal':
              break;
            case 'bold':
              $this->characterData(NULL, '</b>');
              break;
            case 'italic':
              $this->characterData(NULL, '</i>');
              break;
            case 'underline':
              $this->characterData(NULL, '</u>');
              break;
            case 'superscript':
              $this->characterData(NULL, '</sup>');
              break;
            case 'subscript':
              $this->characterData(NULL, '</sub>');
              break;
          }
        }
        $this->font_attr = array();
        break;
      default :
        $this->element = '';
    }


  }

  function characterData($parser, $data) {
    // @todo: Do it only for text fields.
    // first replace any carriage returns with html line breaks
    $data = str_replace("\n", "<br/>", $data);
    if (trim(htmlspecialchars_decode($data))) {
      switch ($this->element) {
        //Author information
        case 'author' :
          $this->contributors[$this->contrib_count]['name'] .= $data;
          break;
        case 'keyword' :
          $this->biblio->biblio_keywords[$this->keyword_count] .= $data;
          break;
        case 'dates' :
          switch ($this->dates) {
            case 'year' :
              $this->biblio->biblio_year .= $data;
              break;
          }
          break;
        case 'date' :
          switch ($this->dates) {
            case 'pub-dates' :
              $this->biblio->biblio_date .= $data;
              break;
            case 'copyright-dates' :
              break;
          }
          break;
        case 'urls' :
        case 'url' :
          switch ($this->urls) {
            case 'web-urls' :
              $this->biblio->biblio_url .= $data;
              break;
            case 'pdf-urls' :
            case 'text-urls' :
            case 'image-urls' :
              break;
            case 'related-urls' :
          }
          break;
        case 'title':
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
        2 => 'artwork',
        3 => 'audio_visual',
        4 => 'bill',
        5 => 'book_chapter',
        6 => 'book',
        7 => 'case',
        9 => 'software',
        10 => 'conference_proceeding',
        12 => 'web_article',
        13 => 'generic',
        14 => 'hearing',
        17 => 'journal_article',
        19 => 'magazine_article',
        20 => 'map',
        21 => 'broadcast',
        23 => 'newspaper_article',
        25 => 'patent',
        26 => 'personal_communication',
        27 => 'report',
        28 => 'edited_book',
        31 => 'statute',
        32 => 'thesis',
        34 => 'unpublished',
        36 => 'manuscript',
        37 => 'figure',
        38 => 'chart',
        39 => 'equation',
        43 => 'electronic_article',
        44 => 'electronic_book',
        45 => 'online_database',
        46 => 'government_document',
        47 => 'conference_paper',
        48 => 'online_multimedia',
        49 => 'classical_work',
        50 => 'legal_ruling',
        52 => 'dictionary',
        53 => 'encyclopedia',
        54 => 'grant',
      ),
      'fields' => array(
        'abbr-1' => 'biblio_short_title',
        'abstract' => 'biblio_abst_e',
        'access-date' => 'biblio_access_date',
        'accession-num' => 'biblio_accession_number',
        'alt-title' => 'biblio_alternate_title',
        'auth-address' => 'biblio_auth_address',
        'call-num' => 'biblio_call_number',
        'custom1' => 'biblio_custom1',
        'custom2' => 'biblio_custom2',
        'custom3' => 'biblio_custom3',
        'custom4' => 'biblio_custom4',
        'custom5' => 'biblio_custom5',
        'custom6' => 'biblio_custom6',
        'custom7' => 'biblio_custom7',
        'edition' => 'biblio_edition',
        'full-title' => 'biblio_secondary_title',
        'isbn' => 'biblio_isbn',
        'issue' => 'biblio_issue',
        'label' => 'biblio_label',
        'language' => 'biblio_language',
        'notes' => 'biblio_notes',
        'num-vols' => 'biblio_number_of_volumes',
        'number' => 'biblio_number',
        'orig-pub' => 'biblio_original_publication',
        'pages' => 'biblio_pages',
        'pub-dates' => 'biblio_date',
        'pub-location' => 'biblio_place_published',
        'publisher' => 'biblio_publisher',
        // TODO:  Check if we need to write this to the type key or into external
        //        field.
        'ref-type' => 'type',
        'related-urls' => 'biblio_url',
        'remote-database-name' => 'biblio_remote_db_name',
        'remote-database-provider' => 'biblio_remote_db_provider',
        'reprint-edition' => 'biblio_reprint_edition',
        'research-notes' => 'biblio_search_notes',
        'secondary-title' => 'biblio_secondary_title',
        'section' => 'biblio_section',
        'short-title' => 'biblio_short_title',
        'tertiary-title' => 'biblio_tertiary_title',
        'translated-title' => 'biblio_translated_title',
        'volume' => 'biblio_volume',
        'work-type' => 'biblio_type_of_work',
        'year' => 'biblio_year',
      ),
    );

    return $return;
  }
}
