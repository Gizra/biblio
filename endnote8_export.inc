<?PHP
// $Id$
function en8_XMLout($results){
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
    $xml .= '<authors>';
    foreach((array)$rec->biblio_contributors[1] as $auth) {
      $xml .= "<author><style $style_attr>";
      $xml .= trim($auth['name']); // insert author here.
      $xml .= '</style></author> ';
    }
    $xml .= '</authors>';

    $xml .= '<secondary-authors>';
    foreach((array)$rec->biblio_contributors[2] as $auth) {
      $xml .= "<author><style $style_attr>";
      $xml .= trim($auth['name']); // insert author here.
      $xml .= '</style></author> ';
    }
    $xml .= '</secondary-authors>';

    $xml .= '<tertiary-authors>';
    foreach((array)$rec->biblio_contributors[3] as $auth) {
      $xml .= "<author><style $style_attr>";
      $xml .= trim($auth['name']); // insert author here.
      $xml .= '</style></author> ';
    }
    $xml .= '</tertiary-authors>';

    $xml .= '<subsidiary-authors>';
    foreach((array)$rec->biblio_contributors[4] as $auth) {
      $xml .= "<author><style $style_attr>";
      $xml .= trim($auth['name']); // insert author here.
      $xml .= '</style></author> ';
    }
    $xml .= '</subsidiary-authors>';

    $xml .= '<translated-authors>';
    foreach((array)$rec->biblio_contributors[5] as $auth) {
      $xml .= "<author><style $style_attr>";
      $xml .= trim($auth['name']); // insert author here.
      $xml .= '</style></author> ';
    }
    $xml .= '</translated-authors>';
    $xml .= '</contributors>';

    $xml .= "<auth-address><style $style_attr>";
    // todo auth address here
    $xml .= '</style></auth-address>';
    $xml .= "<auth-affiliaton><style $style_attr>";
    // todo auth affiliation here
    $xml .= '</style></auth-affiliaton>';

    //<!-- Title information -->
    $xml .= '<titles>';
    $xml .= (!empty ($pub->title)) ? "<title><style $style_attr>$pub->title)</style></title>" :'';
    $xml .= (!empty ($pub->biblio_secondary_title)) ? "<secondary-title><style $style_attr>$pub->biblio_secondary_title)</style></secondary-title>" :'';
    $xml .= (!empty ($pub->biblio_tertiary_title)) ? "<tertiary-title><style $style_attr>$pub->biblio_tertiary_title)</style></tertiary-title>" :'';
    $xml .= (!empty ($pub->biblio_alternate_title)) ? "<alt-title><style $style_attr>$pub->biblio_alternate_title)</style></alt-title>" :'';
    $xml .= (!empty ($pub->biblio_short_title)) ? "<short-title><style $style_attr>$pub->biblio_short_title)</style></short-title>" :'';
    $xml .= (!empty ($pub->biblio_translated_title)) ? "<translated-title><style $style_attr>$pub->biblio_translated_title)</style></translated-title>" :'';
    $xml .= '</titles>';
    //<!-- Periodical information -->
    $xml .= '<periodical>';
    $xml .=   "<full-title><style  $style_attr>";
    $xml .= '  </style></full-title>';
    $xml .=   "<abbr-1><style  $style_attr>";
    $xml .= '  </style></abbr-1>';
    $xml .=   "<abbr-2><style  $style_attr>";
    $xml .= '  </style></abbr-2>';
    $xml .=   "<abbr-3><style  $style_attr>";
    $xml .= '  </style></abbr-3>';
    $xml .= '</periodical>';

    //<!-- Page range information -->
    $xml .= "<pages><style $style_attr> </style></pages>";

    //<!-- Volume, issue, and edition information -->
    $xml .= "<volume><style $style_attr> </style></volume>";
    $xml .= "<number><style $style_attr> </style></number>";
    $xml .= "<issue><style  $style_attr> </style></issue>";
    $xml .= "<secondary-volume><style $style_attr> </style></secondary-volume>";
    $xml .= "<secondary-issue><style $style_attr> </style></secondary-issue>";
    $xml .= "<num-vols><style $style_attr> </style></num-vols>";
    $xml .= "<edition><style $style_attr> </style></edition>";
    $xml .= "<section><style $style_attr> </style></section>";
    $xml .= "<reprint-edition><style $style_attr> </style></reprint-edition>";
    /*
     <!ELEMENT reprint-status EMPTY>
     <!ATTLIST reprint-status
     date CDATA #IMPLIED
     status (in-file | no-file | on-request) #REQUIRED
     >
     */
    //<!-- Keywords --> 
    //TODO: FIX KEYWORDS
    $xml .= '<keywords>';
    $xml .= '  <keyword><style  $style_attr>';
    $xml .= '  </style></keyword>';
    $xml .= '</keywords>';
    //<!-- Date information -->
    $xml .= '<dates>';
    $xml .= '  <year><style  $style_attr>';
    $xml .= '  </style></year>';
    //    <!ELEMENT pub-dates (date+)> 
    //TODO: FIX DATES
    //    <!ELEMENT copyright-dates (date+)><style  $style_attr>';
    //<!ELEMENT date (#PCDATA | style)*><style  $style_attr>';
    //    <!ATTLIST date
    //    day CDATA #IMPLIED
    //    julian CDATA #IMPLIED
    //    month CDATA #IMPLIED
    //    year CDATA #IMPLIED
    //    >
    $xml .= '</dates>';

    //    <!-- Publisher information -->
    $xml .= "<pub-location><style $style_attr> </style></pub-location>";
    $xml .= "<publisher><style $style_attr> </style></publisher>";
    $xml .= "<orig-pub><style $style_attr> </style></publisher>";

    //    <!-- Identifying numbers -->
    $xml .= "<isbn><style $style_attr> </style></isbn>";
    $xml .= "<accession-num><style $style_attr> </style></accession-num>";
    $xml .= "<call-num><style $style_attr> </style></call-num>";
    $xml .= "<report-id><style $style_attr> </style></report-id>";
    $xml .= "<coden><style $style_attr> </style></coden>";
    $xml .= "<electronic-resource-num><style $style_attr> </style></electronic-resource-num>";

    //    <!-- Abstract, notes, images, etc. -->

    $xml .= "<abstract><style $style_attr> </style></abstract>";
    $xml .= "<label><style $style_attr> </style></label>";
    $xml .= "<image><style $style_attr> </style></image>";
    //    <!ATTLIST image
    //    file CDATA #IMPLIED
    //    name CDATA #IMPLIED
    //    >
    $xml .= "<caption><style $style_attr> </style></caption>";
    $xml .= "<notes><style $style_attr> </style></notes>";
    $xml .= "<research-notes><style $style_attr> </style></research-notes>";

    //    <!-- Other detailed information on item -->

    //    <!ELEMENT work-type (#PCDATA | style)*>
    //    <!ELEMENT reviewed-item (#PCDATA | style)*>
    //    <!ELEMENT availability (#PCDATA | style)*>
    //    <!ELEMENT remote-source (#PCDATA | style)*>
    //    <!ELEMENT meeting-place (#PCDATA | style)*>
    //    <!ELEMENT work-location (#PCDATA | style)*>
    //    <!ELEMENT work-extent (#PCDATA | style)*>
    //    <!ELEMENT pack-method (#PCDATA | style)*>
    //    <!ELEMENT size (#PCDATA | style)*>
    //    <!ELEMENT repro-ratio (#PCDATA | style)*>
    //    <!ELEMENT remote-database-name (#PCDATA | style)*>
    //    <!ELEMENT remote-database-provider (#PCDATA | style)*>
    //    <!ELEMENT language (#PCDATA | style)*>

    //    <!-- URL information --> 
    // TODO: fix URLS
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
    $xml .= "<access-date><style $style_attr> </style></access-date>";
    $xml .= "<modified-date><style $style_attr> </style></modified-date>";

    //    <!-- Custom and miscellaneous fields -->

    $xml .= "<custom1><style $style_attr> </style></custom1>";
    $xml .= "<custom2><style $style_attr> </style></custom2>";
    $xml .= "<custom3><style $style_attr> </style></custom3>";
    $xml .= "<custom4><style $style_attr> </style></custom4>";
    $xml .= "<custom5><style $style_attr> </style></custom5>";
    $xml .= "<custom6><style $style_attr> </style></custom6>";
    $xml .= "<custom7><style $style_attr> </style></custom7>";
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