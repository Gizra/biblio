<?php
// $Id$
/*
 * @file endnote_xml_parser.inc
 *
 */
class EndNoteXMLParser {
  private $parser;
  private $format;
  private $node;
  private $keyword_count;
  private $contributors;
  private $contrib_count;
  private $titles;
  private $periodical;
  private $dates;
  private $urls;
  private $font_attr;
  private $session_id;
  private $batch_proc;
  private $terms;
  private $nids;
  private $dups;

  function parse($file, $terms, $batch, $session_id) {
    $this->terms = $terms;
    $this->batch_proc = $batch;
    $this->session_id = $session_id;
    $this->nids = array();
    $this->dups = array();

    if (!($fp = fopen($file->filepath, "r"))) {
      drupal_set_message("could not open XML input", 'error');
      return;
    }
    $data = fread($fp, 2048);
    if (strpos($data, 'record') !== NULL && strpos($data, 'ref-type') !== NULL) {
      $this->format = 'endnote8';
    }
    elseif (strpos($data, 'RECORD') !== NULL && strpos($data, 'REFERENCE_TYPE') !== NULL) {
      $this->format = 'endnote7';
    }
    if ($this->format) {
      $this->parser = drupal_xml_parser_create($data);
      xml_parser_set_option($this->parser, XML_OPTION_CASE_FOLDING, false);
      xml_parser_set_option($this->parser, XML_OPTION_SKIP_WHITE, true);
      xml_set_object($this->parser, $this);
      xml_set_element_handler($this->parser, $this->format .'_startElement', $this->format .'_endElement');
      xml_set_character_data_handler($this->parser, $this->format .'_characterData');

      xml_parse($this->parser, $data, feof($fp));

      while ($data = fread($fp, 2048)) {
        // $data = fread($fp, 2048);
        set_time_limit(30);
        if (!xml_parse($this->parser, $data, feof($fp))) {
          drupal_set_message(sprintf("XML error: %s at line %d",
          xml_error_string(xml_get_error_code($this->parser)),
          xml_get_current_line_number($this->parser)), 'error');
        }
      }
      xml_parser_free($this->parser);
    }
    fclose($fp);

    return array($this->nids, $this->dups);

  }

  function endnote8_startElement($parser, $name, $attrs) {
    switch ($name) {
      case 'record' :
        $this->node = new stdClass();
        $this->node->biblio_contributors = array();
        break;
      case 'style' :
        $this->font_attr = explode(' ', $attrs['face']);
        foreach ($this->font_attr as $fatt) {
          switch ($fatt) {
            case 'normal':
              break;
            case 'bold':
              en8_characterData(NULL, '<b>');
              break;
            case 'italic':
              en8_characterData(NULL, '<i>');
              break;
            case 'underline':
              en8_characterData(NULL, '<u>');
              break;
            case 'superscript':
              en8_characterData(NULL, '<sup>');
              break;
            case 'subscript':
              en8_characterData(NULL, '<sub>');
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
        $this->contributors = $name;
        if (!isset($this->contrib_count))
        $this->contrib_count = 0;
        break;
      case 'author' :
        switch ($this->contributors) {
          case 'authors' :
            if (!isset($this->node->biblio_contributors[1][$this->contrib_count]['name'])) $this->node->biblio_contributors[1][$this->contrib_count]['name'] = '';
            break;
          case 'secondary-authors' :
            $this->node->biblio_contributors[2][$this->contrib_count]['name'] = '';
            break;
          case 'tertiary-authors' :
            $this->node->biblio_contributors[3][$this->contrib_count]['name'] = '';
            break;
          case 'subsidiary-authors' :
            $this->node->biblio_contributors[4][$this->contrib_count]['name'] = '';
            break;
          case 'translated-authors' :
            $this->node->biblio_contributors[5][$this->contrib_count]['name'] = '';
            break;
        }
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
        $this->node->biblio_keywords[$this->keyword_count] = '';
        $this->element = $name;
        break;

      default :
        $this->element = $name;
    }
  }

  function endnote8_endElement($parser, $name) {
    //    global $this->node, $nids, $this->element, $terms, $batch_proc, $session_id, $this->contributors, $this->contrib_count, $this->dates, $this->urls, $this->keyword_count, $this->font_attr;
    switch ($name) {
      case 'record' :
        $this->element = $this->contributors = $this->contrib_count = $this->dates = $this->urls = '';
        $this->node->biblio_xml_md5 = md5(serialize($this->node));
        if ( !($dup = $this->biblio_xml_check_md5($this->node->biblio_xml_md5)) ) {
          if (!empty($this->terms)) {
            if (!isset($this->node->taxonomy)) $this->node->taxonomy = array();
            $this->node->taxonomy = array_merge($this->terms, $this->node->taxonomy);
          }
          biblio_save_node($this->node, $this->batch_proc, $this->session_id);
          if (!empty($this->node->nid)) $this->nids[] = $this->node->nid;
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
        $this->contributors = '';
        break;
      case 'author' :
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
        $this->node->biblio_type = $this->type_map($this->node->biblio_type);
        $this->element = '';
        break;
      case 'style' :
        foreach ($this->font_attr as $fatt) {
          switch ($fatt) {
            case 'normal':
              break;
            case 'bold':
              en8_characterData(NULL, '</b>');
              break;
            case 'italic':
              en8_characterData(NULL, '</i>');
              break;
            case 'underline':
              en8_characterData(NULL, '</u>');
              break;
            case 'superscript':
              en8_characterData(NULL, '</sup>');
              break;
            case 'subscript':
              en8_characterData(NULL, '</sub>');
              break;
          }
        }
        $this->font_attr = array();
        break;
      default :
        $this->element = '';
    }


  }

  function endnote8_characterData($parser, $data) {
    //    global $this->node, $this->element, $this->contributors, $this->contrib_count, $this->dates, $this->urls, $this->keyword_count;
    if (trim($data)) {
      switch ($this->element) {
        //Author information
        case 'author' :
          switch ($this->contributors) {
            case 'authors' :
              $this->node->biblio_contributors[1][$this->contrib_count]['name'] .= $data;
              $this->node->biblio_contributors[1][$this->contrib_count]['auth_type'] = _biblio_get_auth_type(1, $this->node->biblio_type);
              break;
            case 'secondary-authors' :
              $this->node->biblio_contributors[2][$this->contrib_count]['name'] .= $data;
              $this->node->biblio_contributors[2][$this->contrib_count]['auth_type'] = _biblio_get_auth_type(2, $this->node->biblio_type);
              break;
            case 'tertiary-authors' :
              $this->node->biblio_contributors[3][$this->contrib_count]['name'] .= $data;
              $this->node->biblio_contributors[3][$this->contrib_count]['auth_type'] = _biblio_get_auth_type(3, $this->node->biblio_type);
              break;
            case 'subsidiary-authors' :
              $this->node->biblio_contributors[4][$this->contrib_count]['name'] .= $data;
              $this->node->biblio_contributors[4][$this->contrib_count]['auth_type'] = _biblio_get_auth_type(4, $this->node->biblio_type);
              break;
            case 'translated-authors' :
              $this->node->biblio_contributors[5][$this->contrib_count]['name'] .= $data;
              $this->node->biblio_contributors[5][$this->contrib_count]['auth_type'] = _biblio_get_auth_type(5, $this->node->biblio_type);
              break;
          }
          break;
        case 'keyword' :
          $this->node->biblio_keywords[$this->keyword_count] .= $data;
          break;
        case 'dates' :
          switch ($this->dates) {
            case 'year' :
              $this->node->biblio_year .= $data;
              break;
          }
          break;
        case 'date' :
          switch ($this->dates) {
            case 'pub-dates' :
              $this->node->biblio_date .= $data;
              break;
            case 'copyright-dates' :
              break;
          }
          break;
        case 'urls' :
        case 'url' :
          switch ($this->urls) {
            case 'web-urls' :
              $this->node->biblio_url .= $data;
              break;
            case 'pdf-urls' :
            case 'text-urls' :
            case 'image-urls' :
              break;
            case 'related-urls' :
          }
          break;
        case 'title':
          $this->node->title .= $data;
          break;
        default:
          if ($field = $this->field_map(trim($this->element))) {
            $this->node->$field .= $data;
          }
      }
    }
  }

  function endnote7_startElement($parser, $name, $attrs) {
    switch ($name) {
      case 'RECORD' :
        $this->node = new stdClass();
        $this->node->biblio_contributors = array();
        $this->node->biblio_type = 102; // we set 102 here because the xml parser won't
        // process a value of 0 (ZERO) which is the
        // ref-type 102. if there is a non-zero value it will be overwritten
        $this->element = '';
        break;
      case 'AUTHORS':
      case 'SECONDARY_AUTHORS':
      case 'TERTIARY_AUTHORS':
      case 'SUBSIDIARY_AUTHORS':
        $this->contrib_count = 0;
        break;
      case 'KEYWORDS':
        $this->keyword_count = 0;
        break;
      default:
        $this->element = $name;
    }
  }

  function endnote7_endElement($parser, $name) {
    switch ($name) {
      case 'RECORD' :
        $this->node->biblio_xml_md5 = md5(serialize($this->node));
        if ( !($dup = $this->biblio_xml_check_md5($this->node->biblio_xml_md5)) ) {
          if (!empty($this->terms)) {
            if (!isset($this->node->taxonomy)) $this->node->taxonomy = array();
            $this->node->taxonomy = array_merge($this->terms, $this->node->taxonomy);
          }
          biblio_save_node($this->node, $this->batch_proc, $this->session_id);
          if (!empty($this->node->nid)) $this->nids[] = $this->node->nid;
        }
        else {
          $this->dups[] = $dup;
        }
        break;
      case 'AUTHOR':
      case 'SECONDARY_AUTHOR':
      case 'TERTIARY_AUTHOR':
      case 'SUBSIDIARY_AUTHOR':
        $auth_category = $this->field_map($name);
        $this->node->biblio_contributors[$auth_category][$this->contrib_count]['auth_type'] = _biblio_get_auth_type($auth_category, $this->node->biblio_type);
        $this->contrib_count++;
        break;
      case 'KEYWORD':
        $this->keyword_count++;
        break;
      default:

    }
    $this->element = '';
  }

  function endnote7_characterData($parser, $data) {
    if (trim($data)) {
      switch ($this->element) {
        case 'REFERENCE_TYPE':
          $this->node->biblio_type = $this->type_map($data);
          break;
        case 'AUTHOR':
          $this->node->biblio_contributors[1][$this->contrib_count]['name'] .= $data;
          break;
        case 'SECONDARY_AUTHOR':
          $this->node->biblio_contributors[2][$this->contrib_count]['name'] .= $data;
          break;
        case 'TERTIARY_AUTHOR':
          $this->node->biblio_contributors[3][$this->contrib_count]['name'] .= $data;
          break;
        case 'SUBSIDIARY_AUTHOR':
          $this->node->biblio_contributors[4][$this->contrib_count]['name'] .= $data;
          break;
        case 'KEYWORD':
          $this->node->biblio_keywords[$this->keyword_count] .= $data;
          break;
        case 'TITLE':
          $this->node->title .= $data;
          break;
          default:
          if ($field = $this->field_map(trim($this->element))) {
            $this->node->$field .= $data;
          }
      }
    }
  }

  function field_map($enfield) {
    static $fmap = array();
    if (empty($fmap)) {
      $fmap = biblio_get_map('field_map', $this->format);
    }
    return (!empty($fmap[$enfield])) ? $fmap[$enfield] : '';
  }

  function type_map($entype) {
    static $map = array();
    if (empty($map)) {
      $map = biblio_get_map('type_map', $this->format);
    }
    return (isset($map[$entype]))?$map[$entype]:129; //return the biblio type or 129 (Misc) if type not found
  }

  function biblio_xml_check_md5($md5) {
    static $xml_md5s = array();
    if (empty($xml_md5s)) {
      $result = db_query("SELECT * FROM {biblio_xml} ");
      foreach ($result as $row) {
        $xml_md5s[$row->biblio_xml_md5] = $row->nid;
      }
    }
    if (isset($xml_md5s[$md5])) {
      return $xml_md5s[$md5];
    }
    else {
      $xml_md5s[$md5] = TRUE; // gaurd against duplicates in the same import
      return;
    }
  }

}