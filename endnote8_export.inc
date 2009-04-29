<?PHP
// $Id$
function _endnote8_XML_export($results){
  $style_attr = 'face="normal" font="default" size="100%"';
  $xml = '<?xml version="1.0" encoding="UTF-8"?>';
  $xml .= "\n<xml><records>\n";
  if (!is_array($results)) {
    $result_array[] = $results;
  }
  else {
    $result_array = $results;
  }
  foreach ($result_array as $pub) {
    $xml .= "<record>\n";
    $xml .= '<source-app name="Biblio" version="6.x">Drupal-Biblio</source-app>'."\n";
    $xml .= "\t<ref-type>". _endnote8_type_map($pub->biblio_type) ."</ref-type>\n";

    //<!-- Author information -->
    $xml .= '<contributors>';
    if (count($pub->biblio_contributors[1])) {
      $xml .= '<authors>';
      foreach((array)$pub->biblio_contributors[1] as $auth) {
        $xml .= "<author><style $style_attr>";
        $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
        $xml .= '</style></author> ';
      }
      $xml .= '</authors>';
    }
    if (count($pub->biblio_contributors[2])) {
      $xml .= '<secondary-authors>';
      foreach((array)$pub->biblio_contributors[2] as $auth) {
        $xml .= "<author><style $style_attr>";
        $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
        $xml .= '</style></author> ';
      }
      $xml .= '</secondary-authors>';
    }
    if (count($pub->biblio_contributors[3])) {
      $xml .= '<tertiary-authors>';
      foreach((array)$pub->biblio_contributors[3] as $auth) {
        $xml .= "<author><style $style_attr>";
        $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
        $xml .= '</style></author> ';
      }
      $xml .= '</tertiary-authors>';
    }
    if (count($pub->biblio_contributors[4])) {
      $xml .= '<subsidiary-authors>';
      foreach((array)$pub->biblio_contributors[4] as $auth) {
        $xml .= "<author><style $style_attr>";
        $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
        $xml .= '</style></author> ';
      }
      $xml .= '</subsidiary-authors>';
    }
    if (count($pub->biblio_contributors[5])) {
      $xml .= '<translated-authors>';
      foreach((array)$pub->biblio_contributors[5] as $auth) {
        $xml .= "<author><style $style_attr>";
        $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
        $xml .= '</style></author> ';
      }
      $xml .= '</translated-authors>';
    }
    $xml .= '</contributors>';

    //$xml .= "<auth-address><style $style_attr>";
    // todo auth address here
    //$xml .= '</style></auth-address>';
    //$xml .= "<auth-affiliaton><style $style_attr>";
    // todo auth affiliation here
    //$xml .= '</style></auth-affiliaton>';

    //<!-- Title information -->
    $xml .= '<titles>';
    $xml .= (!empty ($pub->title)) ? "<title><style $style_attr>".htmlspecialchars($pub->title)."</style></title>" :'';
    $xml .= (!empty ($pub->biblio_secondary_title)) ? "<secondary-title><style $style_attr>".htmlspecialchars($pub->biblio_secondary_title)."</style></secondary-title>" :'';
    $xml .= (!empty ($pub->biblio_tertiary_title)) ? "<tertiary-title><style $style_attr>".htmlspecialchars($pub->biblio_tertiary_title)."</style></tertiary-title>" :'';
    $xml .= (!empty ($pub->biblio_alternate_title)) ? "<alt-title><style $style_attr>".htmlspecialchars($pub->biblio_alternate_title)."</style></alt-title>" :'';
    $xml .= (!empty ($pub->biblio_short_title)) ? "<short-title><style $style_attr>".htmlspecialchars($pub->biblio_short_title)."</style></short-title>" :'';
    $xml .= (!empty ($pub->biblio_translated_title)) ? "<translated-title><style $style_attr>".htmlspecialchars($pub->biblio_translated_title)."</style></translated-title>" :'';
    $xml .= '</titles>';
    //<!-- Periodical information -->
    //$xml .= '<periodical>';
    //$xml .=   "<full-title><style  $style_attr>";
    //$xml .= '  </style></full-title>';
    //$xml .=   "<abbr-1><style  $style_attr>";
    //$xml .= '  </style></abbr-1>';
    //$xml .=   "<abbr-2><style  $style_attr>";
    //$xml .= '  </style></abbr-2>';
    //$xml .=   "<abbr-3><style  $style_attr>";
    //$xml .= '  </style></abbr-3>';
    //$xml .= '</periodical>';

    //<!-- Page range information -->
    $xml .= (!empty ($pub->biblio_pages)) ? "<pages><style $style_attr>".htmlspecialchars($pub->biblio_pages)."</style></pages>":'';

    //<!-- Volume, issue, and edition information -->
    $xml .= (!empty ($pub->biblio_volume)) ? "<volume><style $style_attr>".htmlspecialchars($pub->biblio_volume)."</style></volume>":'';
    $xml .= (!empty ($pub->biblio_number)) ? "<number><style $style_attr>".htmlspecialchars($pub->biblio_number)."</style></number>":'';
    $xml .= (!empty ($pub->biblio_issue)) ? "<issue><style  $style_attr>".htmlspecialchars($pub->biblio_issue)."</style></issue>":'';
    //    $xml .= "<secondary-volume><style $style_attr> </style></secondary-volume>";
    //    $xml .= "<secondary-issue><style $style_attr> </style></secondary-issue>";
    $xml .= (!empty ($pub->biblio_number_of_volumes)) ? "<num-vols><style $style_attr>".htmlspecialchars($pub->biblio_number_of_volumes)."</style></num-vols>":'';
    $xml .= (!empty ($pub->biblio_edition)) ? "<edition><style $style_attr>".htmlspecialchars($pub->biblio_edition)."</style></edition>":'';
    $xml .= (!empty ($pub->biblio_section)) ? "<section><style $style_attr>".htmlspecialchars($pub->biblio_section)."</style></section>":'';
    $xml .= (!empty ($pub->biblio_reprint_edition)) ? "<reprint-edition><style $style_attr>".htmlspecialchars($pub->biblio_reprint_edition)."</style></reprint-edition>":'';
    /*
     <!ELEMENT reprint-status EMPTY>
     <!ATTLIST reprint-status
     date CDATA #IMPLIED
     status (in-file | no-file | on-request) #REQUIRED
     >
     */
    $kw_array = array();
    if (!empty($pub->biblio_keywords)) {
      foreach($pub->biblio_keywords as $term) {
        $kw_array[] = trim($term);
      }
    }
    if (!empty($kw_array)) {
      $kw_array = array_unique($kw_array);
      $xml .= '<keywords>';
      foreach($kw_array as $word) {
        $xml .= "<keyword><style  $style_attr>". htmlspecialchars(trim($word)) . "</style></keyword>";
      }
      $xml .= "</keywords>";
    }

    //<!-- Date information -->
    if (!empty($pub->biblio_year) || !empty($pub->biblio_date) ) {
      $xml .= '<dates>';
      $xml .=  (!empty($pub->biblio_year)) ? "<year><style  $style_attr>". htmlspecialchars($pub->biblio_year)."</style></year>":'';
      $xml .=  (!empty($pub->biblio_date)) ? "<date><style  $style_attr>". htmlspecialchars($pub->biblio_date)."</style></date>":'';
      $xml .= '</dates>';
    }
    //    <!-- Publisher information -->
    $xml .= (!empty ($pub->biblio_place_published)) ? "<pub-location><style $style_attr>". htmlspecialchars($pub->biblio_place_published)."</style></pub-location>":'';
    $xml .= (!empty ($pub->biblio_publisher)) ? "<publisher><style $style_attr>". htmlspecialchars($pub->biblio_publisher)."</style></publisher>":'';
    //    $xml .= "<orig-pub><style $style_attr> </style></orig-pub>";

    //    <!-- Identifying numbers -->
    $xml .= (!empty ($pub->biblio_isbn)) ? "<isbn><style $style_attr>". htmlspecialchars($pub->biblio_isbn)."</style></isbn>":'';
    $xml .= (!empty ($pub->biblio_accession_number)) ? "<accession-num><style $style_attr>". htmlspecialchars($pub->biblio_accession_number)."</style></accession-num>":'';
    $xml .= (!empty ($pub->biblio_call_number)) ? "<call-num><style $style_attr>". htmlspecialchars($pub->biblio_call_number)."</style></call-num>":'';
    //    $xml .= "<report-id><style $style_attr> </style></report-id>";
    //    $xml .= "<coden><style $style_attr> </style></coden>";
    //    $xml .= "<electronic-resource-num><style $style_attr> </style></electronic-resource-num>";

    //    <!-- Abstract, notes, images, etc. -->

    $xml .= (!empty ($pub->biblio_abst_e)) ? "<abstract><style $style_attr>". htmlspecialchars($pub->biblio_abst_e)."</style></abstract>":'';
    $xml .= (!empty ($pub->biblio_label)) ? "<label><style $style_attr>". htmlspecialchars($pub->biblio_label)."</style></label>":'';
    //  $xml .= "<image><style $style_attr> </style></image>";
    //  $xml .= "<caption><style $style_attr> </style></caption>";
    $xml .= (!empty ($pub->biblio_notes)) ? "<notes><style $style_attr>". htmlspecialchars($pub->biblio_notes)."</style></notes>":'';
    //    $xml .= "<research-notes><style $style_attr> </style></research-notes>";

    //    <!-- Other detailed information on item -->

    $xml .= (!empty ($pub->biblio_type_of_work)) ? "<work-type><style $style_attr>". htmlspecialchars($pub->biblio_type_of_work)."</style></work-type>":'';
    //    <!ELEMENT reviewed-item (#PCDATA | style)*>
    //    <!ELEMENT availability (#PCDATA | style)*>
    //    <!ELEMENT remote-source (#PCDATA | style)*>
    //    <!ELEMENT meeting-place (#PCDATA | style)*>
    //    <!ELEMENT work-location (#PCDATA | style)*>
    //    <!ELEMENT work-extent (#PCDATA | style)*>
    //    <!ELEMENT pack-method (#PCDATA | style)*>
    //    <!ELEMENT size (#PCDATA | style)*>
    //    <!ELEMENT repro-ratio (#PCDATA | style)*>
    $xml .= (!empty ($pub->biblio_remote_db_name)) ? "<remote-database-name><style $style_attr>". htmlspecialchars($pub->biblio_remote_db_name)."</style></remote-database-name>":'';
    $xml .= (!empty ($pub->biblio_remote_db_provider)) ? "<remote-database-provider><style $style_attr>". htmlspecialchars($pub->biblio_remote_db_provider)."</style></remote-database-provider>":'';
    $xml .= (!empty ($pub->biblio_lang)) ? "<language><style $style_attr>". htmlspecialchars($pub->biblio_lang)."</style></language>":'';

    //    <!-- URL information -->
    // TODO: fix URLS
    if (!empty($pub->biblio_url)) {
      $xml .= '<urls>';
      $xml .= "<url><style $style_attr>".urlencode($pub->biblio_url)."</style></url>";
      $xml .= '</urls>';      
    }
    //    <!ELEMENT urls (web-urls?, pdf-urls?, text-urls?, related-urls?, image-urls?)>
    //    <!ELEMENT web-urls (url+)>
    //    <!ELEMENT pdf-urls (url+)>
    //    <!ELEMENT text-urls (url+)>
    //    <!ELEMENT related-urls (url+)>
    //    <!ELEMENT image-urls (url+)>
    //    <!ELEMENT url (#PCDATA | style)*>
    //    <!ATTLIST url
    //    has-ut (yes | no) #IMPLIED
    //    ppv-app CDATA #IMPLIED
    //    ppv-ref (yes | no) #IMPLIED
    //    ppv-ut CDATA #IMPLIED
    //    >

    //    <!-- Information about record -->
    $xml .= (!empty($pub->biblio_access_date)) ? "<access-date><style $style_attr>$pub->biblio_access_date</style></access-date>":'';
 //   $xml .= "<modified-date><style $style_attr> </style></modified-date>";

    //    <!-- Custom and miscellaneous fields -->

    $xml .= (!empty($pub->biblio_custom1)) ? "<custom1><style $style_attr>". htmlspecialchars($pub->biblio_custom1)."</style></custom1>" : '';
    $xml .= (!empty($pub->biblio_custom2)) ? "<custom2><style $style_attr>". htmlspecialchars($pub->biblio_custom2)."</style></custom2>" : '';
    $xml .= (!empty($pub->biblio_custom3)) ? "<custom3><style $style_attr>". htmlspecialchars($pub->biblio_custom3)."</style></custom3>" : '';
    $xml .= (!empty($pub->biblio_custom4)) ? "<custom4><style $style_attr>". htmlspecialchars($pub->biblio_custom4)."</style></custom4>" : '';
    $xml .= (!empty($pub->biblio_custom5)) ? "<custom5><style $style_attr>". htmlspecialchars($pub->biblio_custom5)."</style></custom5>" : '';
    $xml .= (!empty($pub->biblio_custom6)) ? "<custom6><style $style_attr>". htmlspecialchars($pub->biblio_custom6)."</style></custom6>" : '';
    $xml .= (!empty($pub->biblio_custom7)) ? "<custom7><style $style_attr>". htmlspecialchars($pub->biblio_custom7)."</style></custom7>" : '';
    //    <!ELEMENT misc1 (#PCDATA | style)*>
    //    <!ELEMENT misc2 (#PCDATA | style)*>
    //    <!ELEMENT misc3 (#PCDATA | style)*>
    //    <!-- Styled text handling -->
    //    <!ELEMENT style (#PCDATA)>
    //    <!ATTLIST style
    //    color CDATA #IMPLIED
    //    face CDATA "normal"
    //    font CDATA "default"
    //    size CDATA #IMPLIED
    //    >
    $xml .='</record>';
  }
  $xml .= '</records></xml>';
  
  return $xml;
}
function _endnote8_type_map($bibliotype) {
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
  return ($en8_type = array_search($bibliotype, $map)) ? $en8_type : 13; //return the biblio type or 129 (Misc) if type not found
}