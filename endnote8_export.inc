<?PHP
// $Id$ 
function en8_XMLout($result){
  
  $sml = '<?xml version="1.0" encoding="UTF-8"?>';
  $xml .= '<records'>;
  while ($rec = db_get_object($result)){
    $xml .= '<record>';
//<!ELEMENT record (database?, source-app?, rec-number?, ref-type?, contributors?, auth-address?, auth-affiliaton?, titles?, periodical?, pages?, volume?, number?, issue?, secondary-volume?, secondary-issue?, num-vols?, edition?, section?, reprint-edition?, reprint-status?, keywords?, dates?, pub-location?, publisher?, orig-pub?, isbn?, accession-num?, call-num?, report-id?, coden?, electronic-resource-num?, abstract?, label?, image?, caption?, notes?, research-notes?, work-type?, reviewed-item?, availability?, remote-source?, meeting-place?, work-location?, work-extent?, pack-method?, size?, repro-ratio?, remote-database-name?, remote-database-provider?, language?, urls?, access-date?, modified-date?, custom1?, custom2?, custom3?, custom4?, custom5?, custom6?, custom7?, misc1?, misc2?, misc3?)>

/*
<!ELEMENT database (#PCDATA)>
<!ATTLIST database
	name CDATA #IMPLIED
	path CDATA #IMPLIED
>
<!ELEMENT source-app (#PCDATA)>
<!ATTLIST source-app
	name CDATA #IMPLIED
	version CDATA #IMPLIED
>
<!ELEMENT rec-number (#PCDATA)>
*/
  $xml .= '<ref-type name=" ">  </ref-type>';
//<!-- Author information -->
  $xml .= '<contributors>';
  $xml .= '<authors>';
  $xml .= '<author><style face="normal" font="default" size="100%">';
  $xml .= ; // insert author here.
  $xml .= '</style></author> ';    
  $xml .= '</authors>';
  $xml .= '<secondary-authors>';
  $xml .= '<author><style face="normal" font="default" size="100%">';
  $xml .= ; // insert author here.
  $xml .= '</style></author> ';    
  $xml .= '</secondary-authors>';
  $xml .= '<tertiary-authors>';
  $xml .= '<author><style face="normal" font="default" size="100%">';
  $xml .= ; // insert author here.
  $xml .= '</style></author> ';    
  $xml .= '</tertiary-authors>';
  $xml .= '<subsidiary-authors>';
  $xml .= '<author><style face="normal" font="default" size="100%">';
  $xml .= ; // insert author here.
  $xml .= '</style></author> ';    
  $xml .= '</subsidiary-authors>';
  $xml .= '<translated-authors>';
  $xml .= '<author><style face="normal" font="default" size="100%">';
  $xml .= ; // insert author here.
  $xml .= '</style></author> ';    
  $xml .= '</translated-authors>';
  $xml .= '</contributors>';

  $xml .= '<auth-address><style face="normal" font="default" size="100%">';
  // todo auth address here
  $xml .= '</style></auth-address>';    
  $xml .= '<auth-affiliaton><style face="normal" font="default" size="100%">';
  // todo auth affiliation here
  $xml .= '</style></auth-affiliaton>';    
  
  
//<!-- Title information -->
  $xml .= '<titles>';
  $xml .= '  <title><style face="normal" font="default" size="100%">';
  // todo 
  $xml .= '  </style></title>';    
  $xml .= '  <secondary-title><style face="normal" font="default" size="100%">';
  // todo 
  $xml .= '  </style></secondary-title>';    
  $xml .= '  <tertiary-title><style face="normal" font="default" size="100%">';
  // todo 
  $xml .= '  </style></tertiary-title>';    
  $xml .= '  <alt-title><style face="normal" font="default" size="100%">';
  // todo 
  $xml .= '  </style></alt-title>';    
  $xml .= '  <short-title><style face="normal" font="default" size="100%">';
  // todo 
  $xml .= '  </style></short-title>';    
  $xml .= '  <translated-title><style face="normal" font="default" size="100%">';
  // todo 
  $xml .= '  </style></translated-title>';    
  $xml .= '</titles>';
//<!-- Periodical information -->
  $xml .= '<periodical>';
  $xml .=   '<full-title><style face="normal" font="default" size="100%">';
  $xml .= '  </style></full-title>';      
  $xml .=   '<abbr-1><style face="normal" font="default" size="100%">';
  $xml .= '  </style></abbr-1>';      
  $xml .=   '<abbr-2><style face="normal" font="default" size="100%">';
  $xml .= '  </style></abbr-2>';      
  $xml .=   '<abbr-3><style face="normal" font="default" size="100%">';
  $xml .= '  </style></abbr-3>';      
  $xml .= '</periodical>';

//<!-- Page range information -->
  $xml .= '<pages><style face="normal" font="default" size="100%">';
  $xml .= '</style></pages>';

//<!-- Volume, issue, and edition information -->
  $xml .= '<volume><style face="normal" font="default" size="100%">';
  $xml .= '</style></volume>';
  $xml .= '<number><style face="normal" font="default" size="100%">';
  $xml .= '</style></number>';
  $xml .= '<issue><style face="normal" font="default" size="100%">';
  $xml .= '</style></issue>';
  $xml .= '<secondary-volume><style face="normal" font="default" size="100%">';
  $xml .= '</style></secondary-volume>';
  $xml .= '<secondary-issue><style face="normal" font="default" size="100%">';
  $xml .= '</style></secondary-issue>';
  $xml .= '<num-vols><style face="normal" font="default" size="100%">';
  $xml .= '</style></num-vols>';
  $xml .= '<edition><style face="normal" font="default" size="100%">';
  $xml .= '</style></edition>';
  $xml .= '<section><style face="normal" font="default" size="100%">';
  $xml .= '</style></section>';
  $xml .= '<reprint-edition><style face="normal" font="default" size="100%">';
  $xml .= '</style></reprint-edition>';
/*
<!ELEMENT reprint-status EMPTY>
<!ATTLIST reprint-status
	date CDATA #IMPLIED
	status (in-file | no-file | on-request) #REQUIRED
>
*/
//<!-- Keywords -->
  $xml .= '<keywords>';
  $xml .= '  <keyword><style face="normal" font="default" size="100%">';
  $xml .= '  </style></keyword>';
  $xml .= '</keywords>';
//<!-- Date information -->
  $xml .= '<dates>';
  $xml .= '  <year><style face="normal" font="default" size="100%">';
  $xml .= '  </style></year>';
<!ELEMENT pub-dates (date+)>
<!ELEMENT copyright-dates (date+)><style face="normal" font="default" size="100%">';
<!ELEMENT date (#PCDATA | style)*><style face="normal" font="default" size="100%">';
<!ATTLIST date
	day CDATA #IMPLIED
	julian CDATA #IMPLIED
	month CDATA #IMPLIED
	year CDATA #IMPLIED
>
  $xml .= '</dates>';

<!-- Publisher information -->
<!ELEMENT pub-location (#PCDATA | style)*>
<!ELEMENT publisher (#PCDATA | style)*>
<!ELEMENT orig-pub (#PCDATA | style)*>
<!-- Identifying numbers -->
<!ELEMENT isbn (#PCDATA | style)*>
<!ELEMENT accession-num (#PCDATA | style)*>
<!ELEMENT call-num (#PCDATA | style)*>
<!ELEMENT report-id (#PCDATA | style)*>
<!ELEMENT coden (#PCDATA | style)*>
<!ELEMENT electronic-resource-num (#PCDATA | style)*>
<!-- Abstract, notes, images, etc. -->
<!ELEMENT abstract (#PCDATA | style)*>
<!ELEMENT label (#PCDATA | style)*>
<!ELEMENT image (#PCDATA)>
<!ATTLIST image
	file CDATA #IMPLIED
	name CDATA #IMPLIED
>
<!ELEMENT caption (#PCDATA | style)*>
<!ELEMENT notes (#PCDATA | style)*>
<!ELEMENT research-notes (#PCDATA | style)*>
<!-- Other detailed information on item -->
<!ELEMENT work-type (#PCDATA | style)*>
<!ELEMENT reviewed-item (#PCDATA | style)*>
<!ELEMENT availability (#PCDATA | style)*>
<!ELEMENT remote-source (#PCDATA | style)*>
<!ELEMENT meeting-place (#PCDATA | style)*>
<!ELEMENT work-location (#PCDATA | style)*>
<!ELEMENT work-extent (#PCDATA | style)*>
<!ELEMENT pack-method (#PCDATA | style)*>
<!ELEMENT size (#PCDATA | style)*>
<!ELEMENT repro-ratio (#PCDATA | style)*>
<!ELEMENT remote-database-name (#PCDATA | style)*>
<!ELEMENT remote-database-provider (#PCDATA | style)*>
<!ELEMENT language (#PCDATA | style)*>
<!-- URL information -->
<!ELEMENT urls (web-urls?, pdf-urls?, text-urls?, related-urls?, image-urls?)>
<!ELEMENT web-urls (url+)>
<!ELEMENT pdf-urls (url+)>
<!ELEMENT text-urls (url+)>
<!ELEMENT related-urls (url+)>
<!ELEMENT image-urls (url+)>
<!ELEMENT url (#PCDATA | style)*>
<!ATTLIST url
	has-ut (yes | no) #IMPLIED
	ppv-app CDATA #IMPLIED
	ppv-ref (yes | no) #IMPLIED
	ppv-ut CDATA #IMPLIED
>
<!-- Information about record -->
<!ELEMENT access-date (#PCDATA | style)*>
<!ELEMENT modified-date (#PCDATA | style)*>
<!-- Custom and miscellaneous fields -->
<!ELEMENT custom1 (#PCDATA | style)*>
<!ELEMENT custom2 (#PCDATA | style)*>
<!ELEMENT custom3 (#PCDATA | style)*>
<!ELEMENT custom4 (#PCDATA | style)*>
<!ELEMENT custom5 (#PCDATA | style)*>
<!ELEMENT custom6 (#PCDATA | style)*>
<!ELEMENT custom7 (#PCDATA | style)*>
<!ELEMENT misc1 (#PCDATA | style)*>
<!ELEMENT misc2 (#PCDATA | style)*>
<!ELEMENT misc3 (#PCDATA | style)*>
<!-- Styled text handling -->
<!ELEMENT style (#PCDATA)>
<!ATTLIST style
	color CDATA #IMPLIED
	face CDATA "normal"
	font CDATA "default"
	size CDATA #IMPLIED
>
  $xml .='</record>';
  }
return $xml;
}
