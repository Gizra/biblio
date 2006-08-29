<?php
//$file = "LIIreview.xml";

function en8_startElement($parser, $name, $attrs) 
{

global $node_array,$node,$node_id, $element, $contributors, $titles, $periodical, $dates, $urls;
    switch ($name){
      case 'record':
        $node_id = array_push($node_array, $node) - 1;
        break;

      case 'style':
        break;
        
      case 'authors': 
      case 'secondary-authors':
      case 'tertiary-authors':
      case 'subsidiary-authors':
      case 'translated-authors':
        $contributors = $name;
        break;
          
      case 'year':
      case 'pub-dates':
      case 'copyright-dates':
        $dates = $name;
        break;
        
      case 'web-urls':
      case 'pdf-urls':
      case 'text-urls':
      case 'related-urls':
      case 'image-urls':
        $urls = $name;
        break;
        
      default:
        $element = $name;

    }
}

function en8_endElement($parser, $name) 
{
    global $element, $contributors, $dates, $urls;
    switch ($name){
        
      case 'authors': 
      case 'secondary-authors':
      case 'tertiary-authors':
      case 'subsidiary-authors':
      case 'translated-authors':
        $contributors = '';
        $element = '';
        break;
        
          
      case 'year':
      case 'pub-dates':
      case 'copyright-dates':
        $dates = '';
        $element = '';
        break;
        
      case 'web-urls':
      case 'pdf-urls':
      case 'text-urls':
      case 'related-urls':
      case 'image-urls':
        $urls = '';
        $element = '';
        break;
        
      default:
        $element = '';

    }
}

function en8_characterData($parser, $data) 
{
global $node_array,$node_id, $element, $contributors,   $dates, $urls;
 if (trim($data)){
  switch ($element) {
    case 'source-app':
      break;
    case 'rec-number':
      break; 
    case 'ref-type': 
    switch ((int) $data) {
      case 8: // Journal Article
      case 11:
      case 15:
      case 16:
      case 17: 
      case 18:
      case 22:
      case 24:
      case 29:
      case 30: 
      case 33:
        $node_array[$node_id]['biblio_type'] = 1;
        break;
      case 32: // Thesis
        $node_array[$node_id]['biblio_type'] = 7;
        break;
      case 10: // Conference Proceeding
        $node_array[$node_id]['biblio_type'] = 2;
        break;
      case 5: // Book Section
        $node_array[$node_id]['biblio_type'] = 6;
        break;
      case 27: // Report
        $node_array[$node_id]['biblio_type'] = 5;
        break;
      case 25: // Patent
        $node_array[$node_id]['biblio_type'] = 8;
        break;
      default:
         $node_array[$node_id]['biblio_type'] = 9;
         break;     
 /*
 				'book'			=>	6,
				'book_article'		=>	5,
				'journal_article'	=>	8, // also 11, 15, 16, 17, 18, 22, 24, 29, 30, 33
				'newspaper_article'	=>	23,
				'magazine_article'	=>	19,
				'proceedings_article'	=>	10,
				'thesis'		=>	32,
				'web_article'	=>	12,
				'film'			=>	21,
				'broadcast'		=>	21,
				'artwork'		=>	2,
				'software'		=>	9,
				'audiovisual'	=>	3,
				'report'		=>	27,
				'hearing'		=>	14,
				'case'			=>	7,
				'bill'			=>	4,
				'statute'		=>	31,
				'patent'		=>	25,
				'personal'		=>	26,
				'manuscript'	=>	36,
				'map'			=>	20,
				'chart'			=>	38,
				'unpublished'	=>	34,
				'database'		=>	45,
				'government_report'	=>	46,
				'conference_paper'	=>	47,
				'classical'		=>	49,
				'legal_ruling'	=>	50,
				'miscellaneous'	=>	13, // Generic
				'miscellaneous_section'	=>	13, // Generic

 */       
    } 
      break;
//Author information
    case 'author': 
      switch ($contributors){
      case 'authors':
        $node_array[$node_id]['biblio_authors'] .= (empty($node_array[$node_id]['biblio_authors'])) ? $data : "; $data";          
         //$node_array[$node_id]['biblio_authors'][] = $data;
         break;
      case 'secondary-authors':
         $node_array[$node_id]['biblio_secondary_authors'] .= (empty($node_array[$node_id]['biblio_secondary_authors'])) ? $data : "; $data";
//         $node_array[$node_id]['biblio_secondary_authors'][] = $data;
         break;
      case 'tertiary-authors':
         $node_array[$node_id]['biblio_tertiary_authors'] .= (empty($node_array[$node_id]['biblio_tertiary_authors'])) ? $data : "; $data";
//         $node_array[$node_id]['biblio_tertiary_authors'][] = $data;
         break;
      case 'subsidiary-authors':
         $node_array[$node_id]['biblio_subsidiary_authors'] .= (empty($node_array[$node_id]['biblio_subsidiary_authors'])) ? $data : "; $data";
//         $node_array[$node_id]['biblio_subsidiary_authors'][] = $data;
        break; 
      case 'translated-authors':
         $node_array[$node_id]['biblio_translated_authors'] .= (empty($node_array[$node_id]['biblio_translated_authors'])) ? $data : "; $data";
//         $node_array[$node_id]['biblio_translated_authors'][] = $data;
        break; 
      }
      break; 
    case 'auth-address':
      break; 
    case 'auth-affiliaton':
      break;
// Title information  
      case 'title':
        $node_array[$node_id]['title'] .= $data;
        break;      
      case 'secondary-title':
        $node_array[$node_id]['biblio_secondary_title'] .= $data;
        break;
      case 'tertiary-title':
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
        $node_array[$node_id][$periodical] .= $data;
        break;
    case 'pages': 
        $node_array[$node_id]['biblio_pages'] .= $data;
        break;    
//Volume, issue, and edition information
    case 'volume': 
        $node_array[$node_id]['biblio_volume'] .= $data;
        break;    
    case 'number': 
        $node_array[$node_id]['biblio_number'] .= $data;
        break;    
    case 'issue': 
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
    case 'keyword':
        $node_array[$node_id]['biblio_keywords'] .= (empty($node_array[$node_id]['biblio_keywords'])) ? $data : "; $data";
//        $node_array[$node_id]['biblio_keywords'][] = $data;
        break;

    case 'dates':
      switch($dates){
        case 'year':
          $node_array[$node_id]['biblio_year'] .= $data;
          break;
        case 'pub-dates':
          $node_array[$node_id]['biblio_date'] .= $data;
          break;
        case 'copyright-dates':
       }
       break;
    case 'pub-location': 
      $node_array[$node_id]['biblio_place_published'] .= $data;
      break;   
    case 'publisher': 
      $node_array[$node_id]['biblio_publisher'] .= $data;
      break;   
    case 'orig-pub': 
      break;
    case 'isbn': 
      $node_array[$node_id]['biblio_isbn'] .= $data;
      break;   
    case 'accession-num': 
      $node_array[$node_id]['biblio_accession_number'] .= $data;
      break;   
    case 'call-num': 
      $node_array[$node_id]['biblio_call_number'] .= $data;
      break;   
    case 'report-id': 
      break; 
    case 'coden': 
      break; 
    case 'electronic-resource-num': 
      break; 
    case 'abstract': 
      $node_array[$node_id]['biblio_abst_e'] .= $data;
      break; 
    case 'label': 
      break; 
    case 'image': 
      break; 
    case 'caption':
      break; 
    case 'notes': 
      $node_array[$node_id]['biblio_notes'] .= $data;
      break; 
    case 'research-notes': 
      $node_array[$node_id]['biblio_notes'] .= $data;
      break; 
    case 'work-type': 
      break; 
    case 'reviewed-item': 
      break; 
    case 'availability': 
      break; 
    case 'remote-source': 
      break; 
    case 'meeting-place': 
      break; 
    case 'work-location': 
      break; 
    case 'work-extent': 
      break; 
    case 'pack-method': 
      break; 
    case 'size': 
      break; 
    case 'repro-ratio': 
      break; 
    case 'remote-database-name': 
      break; 
    case 'remote-database-provider': 
      break; 
    case 'language': 
      break; 
    case 'urls': 
    case 'url':
      switch ($urls){
        case 'web-urls':
          $node_array[$node_id]['biblio_url'] .= $data;
          break;
        case 'pdf-urls':
        case 'text-urls':
        case 'related-urls':
        case 'image-urls':
      }
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
