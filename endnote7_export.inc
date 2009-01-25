<?php
// $Id$
function _endnote7_XML_export($results) {
  $xml = '<?xml version="1.0" encoding="UTF-8"?>';
  $xml .= "\n<XML><RECORDS>\n";
  if (!is_array($results)) {
    $result_array[] = $results;
  }
  else {
    $result_array = $results;
  }
  foreach ($result_array as $pub) {
    $xml .= "<RECORD>\n";
    switch ($pub->biblio_type) {
      case 1 :
        $reftype = 0;
      case 102 :
        $reftype = 0;
        break; // journal
      case 2 : // 2,3 and 4
      case 3 : // are all
      case 4 : // are all
      case 103 :
        $reftype = 3;
        break; // conference proceedings
      case 5 :
      case 109 :
        $reftype = 10;
        break; // report
      case 5 :
      case 100 :
        $reftype = 7;
        break; // book section
      case 7 :
      case 108 :
        $reftype = 2;
        break; // thesis
      case 8 :
      case 119 :
        $reftype = 15;
        break; // patent
      case 9 :
      case 129 :
      default :
        $reftype = 31;
        break; // generic
    }
    $xml .= "\t<REFERENCE_TYPE>$reftype</REFERENCE_TYPE>\n";
    $a = $e = $t = '';
    foreach ((array)$pub->biblio_contributors[1] as $auth) {
      $a .= "\t\t<AUTHOR>". trim($auth['name']) ."</AUTHOR>\n";
    }
    foreach ((array)$pub->biblio_contributors[2] as $auth) {
      $e .= "\t\t<SECONDARY_AUTHOR>". (trim($auth['name'])) ."</SECONDARY_AUTHOR>\n";
    }
    foreach ((array)$pub->biblio_contributors[2] as $auth) {
      $t .= "\t\t<TERTIARY_AUTHOR>". (trim($auth['name'])) ."</TERTIARY_AUTHOR>\n";
    }
    if (!empty ($a))
      $xml .= "\t<AUTHORS>\n$a\t</AUTHORS>\n";
    if (!empty ($e))
      $xml .= "\t<SECONDARY_AUTHORS>\n$e\t</SECONDARY_AUTHORS>\n";
    if (!empty ($t))
      $xml .= "\t<TERTIARY_AUTHORS>\n$t\t</TERTIARY_AUTHORS>\n";
    $xml .= (!empty ($pub->biblio_year)) ? "\t<YEAR>$pub->biblio_year</YEAR>\n" : "";
    $xml .= (!empty ($pub->title)) ? "\t<TITLE>$pub->title</TITLE>\n" : "";
    $xml .= (!empty ($pub->biblio_secondary_title)) ? "\t<SECONDARY_TITLE>". htmlentities($pub->biblio_secondary_title) ."</SECONDARY_TITLE>\n" : "";
    $xml .= (!empty ($pub->biblio_place_published)) ? "\t<PLACE_PUBLISHED>". htmlentities($pub->biblio_place_published) ."</PLACE_PUBLISHED>\n" : "";
    $xml .= (!empty ($pub->biblio_publisher)) ? "\t<PUBLISHER>". htmlentities($pub->biblio_publisher) ."</PUBLISHER>\n" : "";
    $xml .= (!empty ($pub->biblio_volume)) ? "\t<VOLUME>$pub->biblio_volume</VOLUME>\n" : "";
    //        <NUMBER_OF_VOLUMES>number of volumes</NUMBER_OF_VOLUMES>
    $xml .= (!empty ($pub->biblio_issue)) ? "\t<NUMBER>$pub->biblio_issue</NUMBER>\n" : "";
    $xml .= (!empty ($pub->biblio_pages)) ? "\t<PAGES>$pub->biblio_pages</PAGES>\n" : "";
    //        <SECTION>section</SECTION>
    $xml .= (!empty ($pub->biblio_tertiary_title)) ? "\t<TERTIARY_TITLE>$pub->biblio_tertiary_title</TERTIARY_TITLE>\n" : "";
    $xml .= (!empty ($pub->biblio_edition)) ? "\t<EDITION>$pub->biblio_edition</EDITION>\n" : "";
    $xml .= (!empty ($pub->biblio_date)) ? "\t<DATE>$pub->biblio_date</DATE>\n" : "";
    //    <TYPE_OF_WORK>type of work</TYPE_OF_WORK>
    //    <SUBSIDIARY_AUTHORS>
    //        <SUBSIDIARY_AUTHOR>lastname1, firstname1</SUBSIDIARY_AUTHOR>
    //        <SUBSIDIARY_AUTHOR>lastname2, firstname2</SUBSIDIARY_AUTHOR>
    //    </SUBSIDIARY_AUTHORS>
    //    <SHORT_TITLE>short title</SHORT_TITLE>
    //    <ALTERNATE_TITLE>alternate title</ALTERNATE_TITLE>
    $xml .= (!empty ($pub->biblio_isbn)) ? "\t<ISBN>$pub->biblio_isbn</ISBN>\n" : "";
    //    <ORIGINAL_PUB>original publication</ORIGINAL_PUB>
    //    <REPRINT_EDITION>reprint edition</REPRINT_EDITION>
    //    <REVIEWED_ITEM>reviewed item</REVIEWED_ITEM>
    //    <CUSTOM1>custom 1</CUSTOM1>
    //    <CUSTOM2>custom 2</CUSTOM2>
    //    <CUSTOM3>custom 3</CUSTOM3>
    //    <CUSTOM4>custom 4</CUSTOM4>
    //    <CUSTOM5>custom 5</CUSTOM5>
    //    <CUSTOM6>custom 6</CUSTOM6>
    $xml .= (!empty ($pub->biblio_accession_number)) ? "\t<ACCESSION_NUMBER>$pub->biblio_accession_number</ACCESSION_NUMBER>\n" : "";
    $xml .= (!empty ($pub->biblio_call_number)) ? "\t<CALL_NUMBER>$pub->biblio_call_number</CALL_NUMBER>\n" : "";
    //    <LABEL>label</LABEL>
    if (!empty($pub->terms)){
      $xml .= "\t<KEYWORDS>\n";
      $kw='';
      foreach($pub->terms as $term){
        $xml .= "\t\t<KEYWORD>". trim($term->name) ."</KEYWORD>\n";
      }
      $xml .= "\t</KEYWORDS>\n";
      if (!empty($pub->biblio_keywords)) unset($pub->biblio_keywords);
    }
      
    if (!empty ($pub->biblio_keywords)) {
      $xml .= "\t<KEYWORDS>\n";
      $splitchar = (strstr($pub->biblio_keywords, ";")) ? ";" : " ";
      $kw_array = explode($splitchar, $pub->biblio_keywords);
      foreach ($kw_array as $kw) {
        $xml .= "\t\t<KEYWORD>". trim($kw) ."</KEYWORD>\n";
      }
      $xml .= "\t</KEYWORDS>\n";
    }
    $xml .= (!empty ($pub->biblio_abst_e)) ? "\t<ABSTRACT>". htmlentities($pub->biblio_abst_e) ."</ABSTRACT>\n" : "";
    $xml .= (!empty ($pub->biblio_notes)) ? "\t<NOTES>$pub->biblio_notes</NOTES>\n" : "";
    $xml .= (!empty ($pub->biblio_url)) ? "\t<URL>". htmlentities($pub->biblio_url) ."</URL>\n" : "";
    //    <AUTHOR_ADDRESS>author address</AUTHOR_ADDRESS>
    //    <CAPTION>caption</CAPTION>
    $xml .= "</RECORD>\n";
  } //end while
  $xml .= "</RECORDS></XML>";
  return $xml;
}