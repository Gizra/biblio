<?php
// $Id$ 

function en7_startElement($parser, $name, $attrs) 
{

global $node_array,$node,$node_id, $element, $contributors, $titles, $periodical, $dates, $urls;
    switch ($name){
      case 'RECORD':
        $node_id = array_push($node_array, $node) - 1;
        break;

        
      default:
        $element = $name;

    }
}

function en7_endElement($parser, $name) 
{
    global $element, $contributors, $dates, $urls;
    switch ($name){
        
        
      default:
        $element = '';

    }
}

function en7_characterData($parser, $data) 
{
global $node_array,$node_id, $element, $contributors,   $dates, $urls;
 if (trim($data)){
 
  switch ($element) {
    case 'REFERENCE_TYPE': 
      switch ((int) $data) {
        case 0: // Journal Article
          $node_array[$node_id]['biblio_type'] = 1;
          break;
        case 2: // Thesis
          $node_array[$node_id]['biblio_type'] = 7;
          break;
        case 3: // Conference Proceeding
          $node_array[$node_id]['biblio_type'] = 2;
          break;
        case 7: // Book Section
          $node_array[$node_id]['biblio_type'] = 6;
          break;
        case 10: // Report
          $node_array[$node_id]['biblio_type'] = 5;
          break;
        case 15: // Patent
          $node_array[$node_id]['biblio_type'] = 8;
          break;
      } 
      break;
//Author information
      case 'AUTHOR':
        $node_array[$node_id]['biblio_authors'] .= (empty($node_array[$node_id]['biblio_authors'])) ? $data : "; $data";          
//         $node_array[$node_id]['biblio_authors'][] = $data;
         break;
      case 'SECONDARY_AUTHOR':
        $node_array[$node_id]['biblio_secondary_authors'] .= (empty($node_array[$node_id]['biblio_secondary_authors'])) ? $data : "; $data";          
//         $node_array[$node_id]['biblio_secondary_authors'][] = $data;
         break;
      case 'TERTIARY_AUTHOR':
        $node_array[$node_id]['biblio_tertiary_authors'] .= (empty($node_array[$node_id]['biblio_tertiary_authors'])) ? $data : "; $data";          
//         $node_array[$node_id]['biblio_tertiary_authors'][] = $data;
         break;
      case 'subsidiary-authors':
//         $node_array[$node_id]['biblio_subsidiary_authors'][] = $data;
        break; 
      case 'translated-authors':
//         $node_array[$node_id]['biblio_translated_authors'][] = $data;
        break; 
    case 'auth-address':
      break; 
    case 'auth-affiliaton':
      break;
// Title information  
      case 'TITLE':
        $node_array[$node_id]['title'] .= $data;
        break;      
      case 'SECONDARY_TITLE':
        $node_array[$node_id]['biblio_secondary_title'] .= $data;
        break;
      case 'TERTIARY_TITLE':
        $node_array[$node_id]['biblio_tertiary-title'] .= $data;
        break;
      case 'alt-title':
        break; 
      case 'short-title':
        break; 
      case 'translated-title':
        break;
// Periodical information
      case 'full-title':
        break;
      case 'abbr-1':
        break;
      case 'abbr-2':
        break;
      case 'abbr-3':
        $node_array[$node_id][$periodical] = $data;
        break;
    case 'PAGES': 
        $node_array[$node_id]['biblio_pages'] .= $data;
        break;    
//Volume, issue, and edition information
    case 'VOLUME': 
        $node_array[$node_id]['biblio_volume'] .= $data;
        break;    
    case 'NUMBER': 
        $node_array[$node_id]['biblio_number'] .= $data;
        break;    
    case 'ISSUE': 
        $node_array[$node_id]['biblio_issue'] .= $data;
        break;    
    case 'secondary-volume':
      break; 
    case 'secondary-issue': 
      break; 
    case 'num-vols': 
      break; 
    case 'edition': 
      break; 
    case 'section': 
      break; 
    case 'reprint-edition': 
      break; 
    case 'reprint-status':
      break; 
    case 'KEYWORD':
        $node_array[$node_id]['biblio_keywords'] .= (empty($node_array[$node_id]['biblio_keywords'])) ? $data : "; $data";          
 //       $node_array[$node_id]['biblio_keywords'][] = $data;
      break;

    case 'YEAR':
      $node_array[$node_id]['biblio_year'] .= $data;
      break;
    case 'DATE':
      $node_array[$node_id]['biblio_date'] .= $data;
      break;
    case 'PLACE_PUBLISHED': 
      $node_array[$node_id]['biblio_place_published'] .= $data;
      break;   
    case 'PUBLISHER': 
      $node_array[$node_id]['biblio_publisher'] .= $data;
      break;   
    case 'orig-pub': 
      break;
    case 'ISBN': 
      $node_array[$node_id]['biblio_isbn'] .= $data;
      break;   
    case 'AACCESSION_NUMBER': 
      $node_array[$node_id]['biblio_accession_number'] .= $data;
      break;   
    case 'CALL_NUMBER': 
      $node_array[$node_id]['biblio_call_number'] .= $data;
      break;   
    case 'ABSTRACT': 
      $node_array[$node_id]['biblio_abst_e'] .= $data;
      break; 
    case 'label': 
      break; 
    case 'image': 
      break; 
    case 'caption':
      break; 
    case 'NOTES': 
      $node_array[$node_id]['biblio_notes'] .= $data;
      break; 
    case 'research-notes': 
      $node_array[$node_id]['biblio_notes'] .= $data;
      break; 
    case 'URL':
          $node_array[$node_id]['biblio_url'] .= $data;
          break;
    case 'access-date': 
      break; 
    case 'modified-date': 
      break; 
    case 'custom1': 
      break; 
    case 'custom2': 
      break; 
    case 'custom3': 
      break; 
    case 'custom4': 
      break; 
    case 'custom5': 
      break; 
    case 'custom6': 
      break; 
    case 'custom7': 
      break; 
    case 'misc1': 
      break; 
    case 'misc2': 
      break; 
    case 'misc3':
      break; 
    }
 }
}
?>
