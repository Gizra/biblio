<?php
// $Id$
function unixref_startElement($parser, $name, $attrs) {
  global $node, $element, $auth_category, $contrib_count, $org_count, $titles, $periodical, $dates, $urls, $keyword_count;
  switch ($name) {
    case 'doi_record' :
      $node = array();
      break;
    case 'book':
    case 'journal':
    case 'standard':
    case 'conference':
    case 'report-paper':
    case 'dissertation':
    case 'database':
    case 'sa_component':
      $node['biblio_type'] = _unixref_type_map($name);
      break;
    case 'person_name' :
      $auth_category = _unixref_get_contributor_category($attrs['contributor_role']);
      if (!isset($contrib_count))
        $contrib_count = 0;
      break;
    case 'organization':
      if (!isset($org_count))
        $org_count = 0;
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
function unixref_endElement($parser, $name) {
  global $node, $nids, $element, $terms, $batch_proc, $session_id, $save_node, $auth_category, $contrib_count, $org_count, $dates, $urls, $keyword_count;
  switch ($name) {
    case 'doi_record' :
      $nids[] = biblio_save_node($node, $batch, $session_id, $save_node);
      break;
    case 'person_name' :
      $node['biblio_contributors'][$auth_category][$contrib_count]['auth_type'] = _biblio_get_auth_type($auth_category, $node['biblio_type']);
      $node['biblio_contributors'][$auth_category][$contrib_count]['name'] = 
          $node['biblio_contributors'][$auth_category][$contrib_count]['lastname']; 
      if (isset($node['biblio_contributors'][$auth_category][$contrib_count]['firstname'])) {
         $node['biblio_contributors'][$auth_category][$contrib_count]['name'] .= 
            ', ' . $node['biblio_contributors'][$auth_category][$contrib_count]['firstname'];
      }
      
      $auth_category = '';
      $contrib_count++;
      break;
    case 'organization' :
      $node['biblio_contributors'][5][$org_count]['auth_type'] = _biblio_get_auth_type(5, $node['biblio_type']);
      $org_count++;
      break;
    case 'pages':
      if (isset($node['biblio_first_page'])) $node['biblio_pages'] = $node['biblio_first_page'];
      if (isset($node{'biblio_last_page'}))  $node['biblio_pages'] .= ' - ' . $node['biblio_last_page'];
      break;
    case 'publication_date':
      
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
    case 'style' :
      break;
    default :
  }
  $element = '';

}
function unixref_characterData($parser, $data) {
  global $node, $element, $auth_category, $contrib_count, $org_count, $dates, $urls, $keyword_count;
  if (trim($data)) {
    switch ($element) {
      case 'surname' :
        $node['biblio_contributors'][$auth_category][$contrib_count]['lastname'] .= $data;
        break;
      case 'given_name' :
        $node['biblio_contributors'][$auth_category][$contrib_count]['firstname'] .= $data;
        break;
      case 'suffix':
        $node['biblio_contributors'][$auth_category][$contrib_count]['suffix'] .= $data;
        break;
      case 'affiliation' :
        $node['biblio_contributors'][$auth_category][$contrib_count]['affiliation'] .= $data;
        break;
      case 'organization':
        $node['biblio_contributors'][5][$org_count]['name'] .= $data;
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
      default:
        if ($field = _unixref_field_map(trim($element))) {
          $node[$field] .= $data;
        }
        
    }
  }
}
/*
 * map a unixref XML field to a biblio field
 */
function _unixref_field_map($field) {
  static $field_map = array();
  if (empty($field_map)) {
    $field_map = array(
      'publisher_place'     => 'biblio_place_published',
      'publisher_name'      => 'biblio_publisher',
      'volume'              => 'biblio_volume',
      'number'              => 'biblio_number',
      'issue'               => 'biblio_issue',
      'edition_number'      => 'biblio_edition',
      'section'             => 'biblio_section',
      'doi'                 => 'biblio_doi',
      'title'               => 'title',
      'isbn'                => 'biblio_isbn',
      'issn'                => 'biblio_issn',
      'first_page'          => 'biblio_first_page',
      'last_page'           => 'biblio_last_page',
      // Journal metadata
      'full_title'          => 'biblio_secondary_title',
      'abbrev_title'        => 'biblio_short_title',
      // Conference metadata
      'conference_location' => 'biblio_place_published',
      'conference_name'     => 'biblio_secondary_title',
      'conference_acronym'  => 'biblio_short_title',
      // Proceedings metadata
      'proceedings_title'   => 'biblio_secondary_title',
      'year'                => 'biblio_year',
      'degree'              => 'biblio_type_of_work',
      'error'               => 'error',
    
    );
    
  }
  return (isset($field_map[$field])) ? $field_map[$field]: FALSE;
}

function _unixref_type_map($type){
  static $map = array();
  if (empty($map)) {
    $map = array(
    'error'        => 0,
    'book'         => 100, // Book
    'journal'      => 102, // Journal Article
    'standard'     => 129, // Generic
    'conference'   => 103, // conference_paper
    'report-paper' => 109, // Report
    'dissertation' => 108, // Thesis
    'database'     => 125, // online database
    'sa_component' => 129
   );
  }
  return (isset($map[$type]))?$map[$type]:129; //return the biblio type or 129 (Misc) if type not found
}
function _unixref_get_contributor_category($role) {
  if ($role == 'author') return 1;
  if ($role == 'editor') return 2;
  if ($role == 'chair') return 3;
  if ($role == 'translator') return 4;
  return NULL;
}