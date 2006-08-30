<?php
// $Id$ 
function _endnote7_XML_export($result){

  $xml = '<?xml version="1.0" encoding="UTF-8"?><XML><RECORDS>';

  while ($rec = db_fetch_object($result)){

    $xml .= "<RECORD>";
    switch ($rec->biblio_type) {
        case 1: $reftype = 0;
          break; // journal
        case 2: // 2,3 and 4
        case 3: // are all
        case 4: $reftype = 3;
          break; // conference proceedings 
        case 5: $reftype = 10;
          break; // report
        case 6: $reftype = 7;
          break; // book section
        case 7: $reftype = 2;
          break; // thesis
        case 8: $reftype = 15;
          break; // patent
        case 9:
        default:
          $reftype = 31;
          break; // generic
      } 
    $xml .= "<REFERENCE_TYPE>$reftype</REFERENCE_TYPE>";
    $xml .= "<AUTHORS>";
      $author_array = explode(";", $rec->biblio_authors);
      foreach($author_array as $auth) {
        $xml .= "<AUTHOR>".trim($auth)."</AUTHOR>";
      } 
    $xml .= "</AUTHORS>";
    $xml .= (!empty($rec->biblio_year))?"<YEAR>$rec->biblio_year</YEAR>":"";
    $xml .= (!empty($rec->title))?"<TITLE>$rec->title</TITLE>":"";
    if (!empty($rec->biblio_secondary_authors)) {
      $xml .= "<SECONDARY_AUTHORS>";
        $author_array = explode(";", $rec->biblio_secondary_authors);
        foreach($author_array as $auth) {
          $xml .= "<SECONDARY_AUTHOR>".utf8_decode(trim($auth))."</SECONDARY_AUTHOR>";
        } 
      $xml .= "</SECONDARY_AUTHORS>";
    }
    $xml .= (!empty($rec->biblio_secondary_title))?"<SECONDARY_TITLE>$rec->biblio_secondary_title</SECONDARY_TITLE>":"";
    $xml .= (!empty($rec->biblio_place_published))?"<PLACE_PUBLISHED>$rec->biblio_place_published</PLACE_PUBLISHED>":"";
    $xml .= (!empty($rec->biblio_publisher))?"<PUBLISHER>$rec->biblio_publisher</PUBLISHER>":"";
    $xml .= (!empty($rec->biblio_volume))?"<VOLUME>$rec->biblio_volume</VOLUME>":"";
//        <NUMBER_OF_VOLUMES>number of volumes</NUMBER_OF_VOLUMES>
    $xml .= (!empty($rec->biblio_issue))?"<NUMBER>$rec->biblio_issue</NUMBER>":"";
    $xml .= (!empty($rec->biblio_pages))?"<PAGES>$rec->biblio_pages</PAGES>":"";
//        <SECTION>section</SECTION>
    if (!empty($rec->biblio_tertiary_authors)) {
      $xml .= "<TERTIARY_AUTHORS>";
        $author_array = explode(";", $rec->biblio_tertiary_authors);
        foreach($author_array as $auth) {
           $xml .= "<TERTIARY_AUTHOR>".trim($auth)."</TERTIARY_AUTHOR>";
        } 
      $xml .= "</TERTIARY_AUTHORS>";

    } 
    $xml .= (!empty($rec->biblio_tertiary_title))?"<TERTIARY_TITLE>$rec->biblio_tertiary_title</TERTIARY_TITLE>":"";
    $xml .= (!empty($rec->biblio_edition))?"<EDITION>$rec->biblio_edition</EDITION>":"";
    $xml .= (!empty($rec->biblio_date))?"<DATE>$rec->biblio_date</DATE>":"";
    //    <TYPE_OF_WORK>type of work</TYPE_OF_WORK>
    //    <SUBSIDIARY_AUTHORS>
    //        <SUBSIDIARY_AUTHOR>lastname1, firstname1</SUBSIDIARY_AUTHOR>
    //        <SUBSIDIARY_AUTHOR>lastname2, firstname2</SUBSIDIARY_AUTHOR>
    //    </SUBSIDIARY_AUTHORS>
    //    <SHORT_TITLE>short title</SHORT_TITLE>
    //    <ALTERNATE_TITLE>alternate title</ALTERNATE_TITLE>
    $xml .= (!empty($rec->biblio_isbn))?"<ISBN>$rec->biblio_isbn</ISBN>":"";
    //    <ORIGINAL_PUB>original publication</ORIGINAL_PUB>
    //    <REPRINT_EDITION>reprint edition</REPRINT_EDITION>
    //    <REVIEWED_ITEM>reviewed item</REVIEWED_ITEM>
    //    <CUSTOM1>custom 1</CUSTOM1>
    //    <CUSTOM2>custom 2</CUSTOM2>
    //    <CUSTOM3>custom 3</CUSTOM3>
    //    <CUSTOM4>custom 4</CUSTOM4>
    //    <CUSTOM5>custom 5</CUSTOM5>
    //    <CUSTOM6>custom 6</CUSTOM6>
    $xml .= (!empty($rec->biblio_accession_number))?"<ACCESSION_NUMBER>$rec->biblio_accession_number</ACCESSION_NUMBER>":"";
    $xml .= (!empty($rec->biblio_call_number))?"<CALL_NUMBER>$rec->biblio_call_number</CALL_NUMBER>":"";
    //    <LABEL>label</LABEL>
    if (!empty($rec->biblio_keywords)) {
      $xml .= "<KEYWORDS>";
      $splitchar = (strstr($rec->biblio_keywords, ";"))?";":" ";
      $kw_array = explode($splitchar, $rec->biblio_keywords);
      foreach($kw_array as $kw) {
        $xml .= "<KEYWORD>".trim($kw)."</KEYWORD>";
      } 
      $xml .="</KEYWORDS>";
    } 
    $xml .= (!empty($rec->biblio_abst_e))?"<ABSTRACT>".htmlentities($rec->biblio_abst_e)."</ABSTRACT>":"";
    $xml .= (!empty($rec->biblio_notes))?"<NOTES>$rec->biblio_notes</NOTES>":"";
    $xml .= (!empty($rec->biblio_url))?"<URL>".htmlentities($rec->biblio_url)."</URL>":"";
    //    <AUTHOR_ADDRESS>author address</AUTHOR_ADDRESS>
    //    <CAPTION>caption</CAPTION>
    $xml .= "</RECORD>";
  } //end while
    
  $xml .= "</RECORDS></XML>";

  return $xml;
}
