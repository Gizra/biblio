<?php
// $Id$
function en8_startElement($parser, $name, $attrs) {
  global $node, $element, $contributors, $contrib_count, $titles, $periodical, $dates, $urls, $keyword_count;
  switch ($name) {
    case 'record' :
      $node = array();
      break;
    case 'style' :
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
    default :
      $element = $name;
  }
}
function en8_endElement($parser, $name) {
  global $node, $nids, $element, $terms, $batch_proc, $session_id, $contributors, $contrib_count, $dates, $urls, $keyword_count;
  switch ($name) {
    case 'record' :
      $element = $contributors = $contrib_count = $dates = $urls = '';
      if (!empty($terms)) $node['taxonomy'] = array_merge($terms,$node['taxonomy']);
      $nids[] = biblio_save_node($node, $batch_proc, $session_id);
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
       $node['biblio_type'] = en8_type_map($node['biblio_type']);
       break;
    case 'style' :
      break;
    default :
  }
  $element = '';
  
}
function en8_characterData($parser, $data) {
  global $node, $element, $contributors, $contrib_count, $dates, $urls, $keyword_count;
  if (trim($data)) {
    switch ($element) {
      case 'source-app' :
        break;
      case 'rec-number' :
        break;
      case 'ref-type' :
        $node['biblio_type'] .= $data;
        break;

        //Author information
      case 'author' :        
        switch ($contributors) {
          case 'authors' :
            $node['biblio_contributors'][1][$contrib_count]['name'] .= $data;
            $node['biblio_contributors'][1][$contrib_count]['auth_type'] = _biblio_get_auth_type(1, $node['biblio_type']);
            break;
          case 'secondary-authors' :
            $node['biblio_contributors'][2][$contrib_count]['name'] .= $data;
            $node['biblio_contributors'][2][$contrib_count]['auth_type'] = _biblio_get_auth_type(2, $node['biblio_type']);
            break;
          case 'tertiary-authors' :
            $node['biblio_contributors'][3][$contrib_count]['name'] .= $data;
            $node['biblio_contributors'][3][$contrib_count]['auth_type'] = _biblio_get_auth_type(3, $node['biblio_type']);
            break;
          case 'subsidiary-authors' :
            $node['biblio_contributors'][4][$contrib_count]['name'] .= $data;
            $node['biblio_contributors'][4][$contrib_count]['auth_type'] = _biblio_get_auth_type(4, $node['biblio_type']);
            break;
          case 'translated-authors' :
            $node['biblio_contributors'][5][$contrib_count]['name'] .= $data;
            $node['biblio_contributors'][5][$contrib_count]['auth_type'] = _biblio_get_auth_type(5, $node['biblio_type']);
            break;
        }
        break;
      case 'auth-address' :
        $node['biblio_auth_address'] .= $data;
        break;
      case 'auth-affiliaton' :
        break;
        // Title information
      case 'title' :
        $node['title'] .= $data;
        break;
      case 'secondary-title' :
        $node['biblio_secondary_title'] .= $data;
        break;
      case 'tertiary-title' :
        $node['biblio_tertiary_title'] .= $data;
        break;
      case 'alt-title' :
        $node['biblio_alternate_title'] .= $data;
        break;
      case 'short-title' :
        $node['biblio_short_title'] .= $data;
        break;
      case 'translated-title' :
        $node['biblio_translated_title'] .= $data;
        break;
        // Periodical information
      case 'full-title' :
        break;
      case 'abbr-1' :
        break;
      case 'abbr-2' :
        break;
      case 'abbr-3' :
        $node[$periodical] .= $data;
        break;
      case 'pages' :
        $node['biblio_pages'] .= $data;
        break;
        //Volume, issue, and edition information
      case 'volume' :
        $node['biblio_volume'] .= $data;
        break;
      case 'number' :
        $node['biblio_number'] .= $data;
        break;
      case 'issue' :
        $node['biblio_issue'] .= $data;
        break;
      case 'secondary-volume' :
        break;
      case 'secondary-issue' :
        break;
      case 'num-vols' :
        $node['biblio_number_of_volumes'] .= $data;
        break;
      case 'edition' :
        $node['biblio_edition'] .= $data;
        break;
      case 'section' :
        $node['biblio_section'] .= $data;
        break;
      case 'reprint-edition' :
        $node['biblio_reprint_edition'] .= $data;
        break;
      case 'reprint-status' :
        break;
      case 'keyword' :
        $vid = variable_get('biblio_keyword_vocabulary', 0);
        if ($vid  && variable_get('biblio_keyword_freetagging', 0)) { 
           //TODO: this will potentially cause problems exta commas in the tags string if all the data is not stored in one pass.
          $node['taxonomy']['tags'][$vid] .= (empty($node['taxonomy']['tags'][$vid])) ? "\"$data\"" : ",\"$data\"";
        }
        $node['biblio_keywords'][$keyword_count] .= $data;
        break;
      case 'dates' :
        switch ($dates) {
          case 'year' :
            $node['biblio_year'] .= $data;
            break;
        }
        break;
      case 'date' :
        switch ($dates) {
          case 'pub-dates' :
            $node['biblio_date'] .= $data;
            break;
          case 'copyright-dates' :
            break;
        }
        break;
      case 'pub-location' :
        $node['biblio_place_published'] .= $data;
        break;
      case 'publisher' :
        $node['biblio_publisher'] .= $data;
        break;
      case 'orig-pub' :
        break;
      case 'isbn' :
        $node['biblio_isbn'] .= $data;
        break;
      case 'accession-num' :
        $node['biblio_accession_number'] .= $data;
        break;
      case 'call-num' :
        $node['biblio_call_number'] .= $data;
        break;
      case 'report-id' :
        break;
      case 'coden' :
        break;
      case 'electronic-resource-num' :
        break;
      case 'abstract' :
        $node['biblio_abst_e'] .= $data;
        break;
      case 'label' :
        $node['biblio_label'] .= $data;
        break;
      case 'image' :
        break;
      case 'caption' :
        break;
      case 'notes' :
        $node['biblio_notes'] .= $data;
        break;
      case 'research-notes' :
        $node['biblio_notes'] .= $data;
        break;
      case 'work-type' :
        $node['biblio_type_of_work'] .= $data;
        break;
      case 'reviewed-item' :
        break;
      case 'availability' :
        break;
      case 'remote-source' :
        break;
      case 'meeting-place' :
        break;
      case 'work-location' :
        break;
      case 'work-extent' :
        break;
      case 'pack-method' :
        break;
      case 'size' :
        break;
      case 'repro-ratio' :
        break;
      case 'remote-database-name' :
        $node['biblio_remote_db_name'] .= $data;
        break;
      case 'remote-database-provider' :
        $node['biblio_remote_db_provider'] .= $data;
        break;
      case 'language' :
        $node['biblio_lang'] .= $data;
        break;
      case 'urls' :
      case 'url' :
        switch ($urls) {
          case 'web-urls' :
          case 'pdf-urls' :
          case 'text-urls' :
          case 'image-urls' :
            break;
          case 'related-urls' :
            $node['biblio_url'] .= $data;
            break;
        }
        break;
      case 'access-date' :
        $node['biblio_access_date'] .= $data;
        break;
      case 'modified-date' :
        break;
      case 'custom1' :
        $node['biblio_custom1'] .= $data;
        break;
      case 'custom2' :
        $node['biblio_custom2'] .= $data;
        break;
      case 'custom3' :
        $node['biblio_custom3'] .= $data;
        break;
      case 'custom4' :
        $node['biblio_custom4'] .= $data;
        break;
      case 'custom5' :
        $node['biblio_custom5'] .= $data;
        break;
      case 'custom6' :
        $node['biblio_custom6'] .= $data;
        break;
      case 'custom7' :
        $node['biblio_custom7'] .= $data;
        break;
      case 'misc1' :
        break;
      case 'misc2' :
        break;
      case 'misc3' :
        break;
    }
  }
}

function en8_type_map($en8type){
  static $map = array();
  if (empty($map)) {
    $map = array(
    2 => 112, // artwork
    3 => 114, // Audio Visual
    4 => 117, // bill
    5 => 101, // Book Section
    6 => 100, // Book
    7 => 116, // case
    9 => 113, // software
    17 => 102, // Journal Article
    10 => 104, // Conference Proceeding
    12 => 107, // web page
    13 => 129, // Generic
    14 => 115, // hearing
    19 => 106, // magazine_article
    20 => 122, // map
    21 => 110, // film
    21 => 111, // broadcast
    23 => 105, // newspaper_article
    25 => 119, // patent
    26 => 120, // personal communication
    27 => 109, // Report
    28 => 129, // Edited Book
    31 => 118, // statute
    32 => 108, // Thesis
    34 => 124, // unpublished
    36 => 121, // manuscript
    37 => 129, // figure
    38 => 123, // chart
    39 => 129, // equation
    43 => 129, // electronic article
    44 => 129, // electronic book
    45 => 125, // online database
    46 => 126, // government_document
    47 => 103, // conference_paper
    48 => 129, // online multimedia
    49 => 127, // Classical Work
    50 => 128, // legal_ruling
    52 => 129, // Dictionary
    53 => 129, // Encyclopedia
    54 => 129, // Grant
    );
  }
  return (isset($map[$en8type]))?$map[$en8type]:129; //return the biblio type or 129 (Misc) if type not found
}