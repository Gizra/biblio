<?php

/**
 * @file
 * EndNote tagged biblio style.
 */

class BiblioStyleEndNoteXML extends BiblioStyleBase {

  /**
   * Import XML.
   */
  public function import($data, $options = array()) {
    if (strpos($data, 'record') !== FALSE && strpos($data, 'ref-type') !== FALSE) {
      $format = 'endNote8';
    }
    elseif (strpos($data, 'RECORD') !== FALSE && strpos($data, 'REFERENCE_TYPE') !== FALSE) {
      $format = 'endNote7';
    }

    if (empty($format)) {
      return;
    }

    $data = str_replace("\r\n", "\n", $data);

    $parser = drupal_xml_parser_create($data);
    xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, FALSE);
    xml_parser_set_option($parser, XML_OPTION_SKIP_WHITE, TRUE);
    xml_set_object($parser, $this);
    xml_set_element_handler($parser, $format . 'StartElement', $format . 'EndElement');
    xml_set_character_data_handler($parser, $format . 'CharacterData');

    if (!xml_parse($parser, $data)) {
      $params = array(
        '@error' => xml_error_string(xml_get_error_code($parser)),
        '@line' => xml_get_current_line_number($parser),
      );
      drupal_set_message(t('XML error: @error at line @line'), $params);
    }

    xml_parser_free($parser);
  }


  function endNote8StartElement($parser, $name, $attrs) {
    switch ($name) {
      case 'record' :
        $this->biblio = new stdClass();
        $this->biblio->biblio_contributors = array();
        break;

      case 'style' :
        $this->font_attr = explode(' ', $attrs['face']);
        foreach ($this->font_attr as $fatt) {
          switch ($fatt) {
            case 'normal':
              break;
            case 'bold':
              $this->endNote8CharacterData(NULL, '<b>');
              break;
            case 'italic':
              $this->endNote8CharacterData(NULL, '<i>');
              break;
            case 'underline':
              $this->endNote8CharacterData(NULL, '<u>');
              break;
            case 'superscript':
              $this->endNote8CharacterData(NULL, '<sup>');
              break;
            case 'subscript':
              $this->endNote8CharacterData(NULL, '<sub>');
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

  function endNote8EndElement($parser, $name) {
    //    global $this->biblio, $nids, $this->element, $terms, $batch_proc, $session_id, $this->contributors_type, $this->contrib_count, $this->dates, $this->urls, $this->keyword_count, $this->font_attr;
    switch ($name) {
      case 'record' :
        dpm($this);

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
              $this->endNote8CharacterData(NULL, '</b>');
              break;
            case 'italic':
              $this->endNote8CharacterData(NULL, '</i>');
              break;
            case 'underline':
              $this->endNote8CharacterData(NULL, '</u>');
              break;
            case 'superscript':
              $this->endNote8CharacterData(NULL, '</sup>');
              break;
            case 'subscript':
              $this->endNote8CharacterData(NULL, '</sub>');
              break;
          }
        }
        $this->font_attr = array();
        break;
      default :
        $this->element = '';
    }


  }

  function endNote8CharacterData($parser, $data) {
    // first replace any carriage returns with html line breaks
    $data = str_ireplace("\n", "<br/>", $data);
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
      '%A' => array(
        'import_method' => 'importEntryContributors',
        'render_method' => 'renderEntryContributors',
        'role' => 'Author',
        'execute_once' => TRUE,
      ),
      '%B' => array(
        'property' => 'biblio_secondary_title',
        'render_method' => 'renderEntrySecondaryTitle',
        'execute_once' => TRUE,
      ),
      '%C' => array('property' => 'biblio_place_published'),
      '%D' => array('property' => 'biblio_year'),
      '%E' => array(
        'import_method' => 'importEntryContributors',
        'render_method' => 'renderEntryContributors',
        'role' => 'Editor',
        'execute_once' => TRUE,
      ),
      '%F' => array('property' => 'biblio_label'),
      '%G' => array('property' => 'language'),
      '%I' => array('property' => 'biblio_publisher'),
      '%J' => array(
        'property' => 'biblio_secondary_title',
        'render_method' => 'renderEntrySecondaryTitle',
        'execute_once' => TRUE,
      ),
      '%K' => array(
        'property' => 'biblio_keywords',
        'render_method' => 'renderEntryKeywords',
      ),
      '%L' => array('property' => 'biblio_call_number'),
      '%M' => array('property' => 'biblio_accession_number'),
      '%N' => array('property' => 'biblio_issue'),
      '%P' => array('property' => 'biblio_pages'),
      '%R' => array('property' => 'biblio_doi'),
      '%S' => array('property' => 'biblio_tertiary_title'),
      '%T' => array('property' => 'title'),
      '%U' => array('property' => 'biblio_url'),
      '%V' => array('property' => 'biblio_volume'),
      '%X' => array('property' => 'biblio_abstract'),
      '%Y' => array(
        'import_method' => 'importEntryContributors',
        'render_method' => 'renderEntryContributors',
        // @todo: Fix role.
        'role' => '%Y',
        'execute_once' => TRUE,
      ),
      '%X' => array('property' => 'biblio_notes'),
      '%1' => array('property' => 'biblio_custom1'),
      '%2' => array('property' => 'biblio_custom2'),
      '%3' => array('property' => 'biblio_custom3'),
      '%4' => array('property' => 'biblio_custom4'),
      '%#' => array('property' => 'biblio_custom5'),
      '%$' => array('property' => 'biblio_custom6'),
      '%]' => array('property' => 'biblio_custom7'),
      '%6' => array('property' => 'biblio_number_of_volumes'),
      '%7' => array('property' => 'biblio_edition'),
      '%8' => array('property' => 'biblio_date'),
      '%9' => array('property' => 'biblio_type_of_work'),
      '%?' => array(
        'import_method' => 'importEntryContributors',
        'render_method' => 'renderEntryContributors',
        // @todo: Fix role.
        'role' => '%?',
        'execute_once' => TRUE,
      ),
      '%@' => array('property' => 'biblio_isbn'),
      '%<' => array('property' => 'biblio_research_notes'),
      '%>' => array(
        'property' => 'biblio_pdf',
        // @todo: We can try and download the file.
        'import_method' => FALSE,
        'render_method' => 'renderEntryFile'
      ),
      '%!' => array('property' => 'biblio_short_title'),
      '%&' => array('property' => 'biblio_section'),
      '%(' => array('property' => 'biblio_original_publication'),
      '%)' => array('property' => 'biblio_reprint_edition'),
    );

    // Assign default import method.
    foreach ($return as $key => $value) {
      $return[$key] += array(
        'import_method' => 'importEntryGeneric',
        'render_method' => 'renderEntryGeneric',
        'execute_once' => FALSE,
      );
    }

    return $return;
  }
}
