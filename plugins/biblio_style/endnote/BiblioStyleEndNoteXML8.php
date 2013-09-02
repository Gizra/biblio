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
        '17' => 'journal_article',
      ),
    );

    return $return;
  }
}
