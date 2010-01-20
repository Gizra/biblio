<?php
// $Id$
function en8_startElement($parser, $name, $attrs) {
  global $node, $element, $contributors, $contrib_count, $titles, $periodical, $dates, $urls, $keyword_count, $font_attr;
  switch ($name) {
    case 'record' :
      $node = new stdClass();
      $node->biblio_contributors = array();
      break;
    case 'style' :
      $font_attr = explode(' ',$attrs['face']);
      foreach ($font_attr as $fatt) {
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
      $keyword_count = 0;
      break;
    case 'authors' :
    case 'secondary-authors' :
    case 'tertiary-authors' :
    case 'subsidiary-authors' :
    case 'translated-authors' :
      $contributors = $name;
      if (!isset($contrib_count))
        $contrib_count = 0;
      break;
    case 'author' :
        switch ($contributors) {
          case 'authors' :
            if (!isset($node->biblio_contributors[1][$contrib_count]['name'])) $node->biblio_contributors[1][$contrib_count]['name'] = '';
             break;
          case 'secondary-authors' :
            $node->biblio_contributors[2][$contrib_count]['name'] = '';
            break;
          case 'tertiary-authors' :
            $node->biblio_contributors[3][$contrib_count]['name'] = '';
            break;
          case 'subsidiary-authors' :
            $node->biblio_contributors[4][$contrib_count]['name'] = '';
            break;
          case 'translated-authors' :
            $node->biblio_contributors[5][$contrib_count]['name'] = '';
            break;
        }
      $element = $name;
      break;
    case 'year' :
    case 'pub-dates' :
    case 'copyright-dates' :
      $dates = $name;
      break;
    case 'web-urls' :
    case 'pdf-urls' :
    case 'text-urls' :
    case 'related-urls' :
    case 'image-urls' :
      $urls = $name;
      break;
    case 'keyword':
      $node->biblio_keywords[$keyword_count] = '';
      $element = $name;
     break;
    case 'title':
      $node->title = '';
      $element = $name;
      break;

    default :
      $element = $name;
  }
}
function en8_endElement($parser, $name) {
  global $node, $nids, $element, $terms, $batch_proc, $session_id, $contributors, $contrib_count, $dates, $urls, $keyword_count, $font_attr;
  switch ($name) {
    case 'record' :
      $element = $contributors = $contrib_count = $dates = $urls = '';
      if (!empty($terms)) {
        if (!isset($node->taxonomy)) $node->taxonomy = array();
        $node->taxonomy = array_merge($terms, $node->taxonomy);
      }
      biblio_save_node($node, $batch_proc, $session_id);
      if (!empty($node->nid)) $nids[] = $node->nid;
      break;
    case 'authors' :
    case 'secondary-authors' :
    case 'tertiary-authors' :
    case 'subsidiary-authors' :
    case 'translated-authors' :
      $contributors = '';
      break;
    case 'author' :
      $contrib_count++;
      break;
    case 'keyword' :
      $keyword_count++;
      break;
    case 'year' :
    case 'pub-dates' :
    case 'copyright-dates' :
      $dates = '';
      break;
    case 'web-urls' :
    case 'pdf-urls' :
    case 'text-urls' :
    case 'related-urls' :
    case 'image-urls' :
      $urls = '';
      break;
    case 'ref-type':
       $node->biblio_type = en8_parser_type_map($node->biblio_type);
        $element = '';
       break;
    case 'style' :
      foreach ($font_attr as $fatt) {
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
      $font_attr = array();
      break;
    default :
      $element = '';
  }


}
function en8_characterData($parser, $data) {
  global $node, $element, $contributors, $contrib_count, $dates, $urls, $keyword_count;
  if (trim($data)) {
    switch ($element) {
        //Author information
      case 'author' :
        switch ($contributors) {
          case 'authors' :
            $node->biblio_contributors[1][$contrib_count]['name'] .= $data;
            $node->biblio_contributors[1][$contrib_count]['auth_type'] = _biblio_get_auth_type(1, $node->biblio_type);
            break;
          case 'secondary-authors' :
            $node->biblio_contributors[2][$contrib_count]['name'] .= $data;
            $node->biblio_contributors[2][$contrib_count]['auth_type'] = _biblio_get_auth_type(2, $node->biblio_type);
            break;
          case 'tertiary-authors' :
            $node->biblio_contributors[3][$contrib_count]['name'] .= $data;
            $node->biblio_contributors[3][$contrib_count]['auth_type'] = _biblio_get_auth_type(3, $node->biblio_type);
            break;
          case 'subsidiary-authors' :
            $node->biblio_contributors[4][$contrib_count]['name'] .= $data;
            $node->biblio_contributors[4][$contrib_count]['auth_type'] = _biblio_get_auth_type(4, $node->biblio_type);
            break;
          case 'translated-authors' :
            $node->biblio_contributors[5][$contrib_count]['name'] .= $data;
            $node->biblio_contributors[5][$contrib_count]['auth_type'] = _biblio_get_auth_type(5, $node->biblio_type);
            break;
        }
        break;
      case 'keyword' :
        $node->biblio_keywords[$keyword_count] .= $data;
        break;
      case 'title':
        $node->title .= $data;
        break;
      case 'dates' :
        switch ($dates) {
          case 'year' :
            $node->biblio_year .= $data;
            break;
        }
        break;
      case 'date' :
        switch ($dates) {
          case 'pub-dates' :
            $node->biblio_date .= $data;
            break;
          case 'copyright-dates' :
            break;
        }
        break;
      case 'urls' :
      case 'url' :
        switch ($urls) {
          case 'web-urls' :
            $node->biblio_url .= $data;
            break;
          case 'pdf-urls' :
          case 'text-urls' :
          case 'image-urls' :
            break;
          case 'related-urls' :
        }
        break;

      default:
        if ($field = en8_parser_field_map(trim($element))) {
          $node->$field .= $data;
        }
    }
  }
}

function en8_parser_field_map($enfield) {
  static $fmap = array();
  if(empty($fmap)) {
    module_load_include('inc', 'biblio', 'biblio.type.mapper');
    $fmap = biblio_get_field_map('endnote8');
  }
  return (!empty($fmap[$enfield])) ? $fmap[$enfield] : '';

}

function en8_parser_type_map($en8type){
  static $map = array();
  if (empty($map)) {// first check to see if there are user settings else load the defaults
    module_load_include('inc', 'biblio', 'biblio.type.mapper');
    $map = biblio_get_type_map('endnote8');
  }
  return (isset($map[$en8type]))?$map[$en8type]:129; //return the biblio type or 129 (Misc) if type not found
}
