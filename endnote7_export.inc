<?php
// $Id$ 
function _endnote7_XML_export($result){

  $xml = '<?xml version="1.0" encoding="UTF-8"?>';
  $xml .= "\n<XML><RECORDS>\n";

  while ($rec = db_fetch_object($result)){

    $xml .= "<RECORD>\n";
    switch ($rec->biblio_type) {
        case 1: $reftype = 0;
        case 102: $reftype = 0;
          break; // journal
        case 2: // 2,3 and 4
        case 3: // are all
        case 4: // are all
        case 103: $reftype = 3;
          break; // conference proceedings 
        case 5: 
        case 109: $reftype = 10;
          break; // report
        case 5:
        case 100: $reftype = 7;
          break; // book section
        case 7:
        case 108: $reftype = 2;
          break; // thesis
        case 8:
        case 119: $reftype = 15;
          break; // patent
        case 9:
        case 129:
        default:
          $reftype = 31;
          break; // generic
      } 
    $xml .= "\t<REFERENCE_TYPE>$reftype</REFERENCE_TYPE>\n";
    $xml .= "\t<AUTHORS>\n";
      $author_array = explode(";", $rec->biblio_authors);
      foreach($author_array as $auth) {
        $xml .= "\t\t<AUTHOR>".trim($auth)."</AUTHOR>\n";
      } 
    $xml .= "\t</AUTHORS>\n";
    $xml .= (!empty($rec->biblio_year))?"\t<YEAR>$rec->biblio_year</YEAR>\n":"";
    $xml .= (!empty($rec->title))?"\t<TITLE>$rec->title</TITLE>\n":"";
    if (!empty($rec->biblio_secondary_authors)) {
      $xml .= "\t<SECONDARY_AUTHORS>\n";
        $author_array = explode(";", $rec->biblio_secondary_authors);
        foreach($author_array as $auth) {
          $xml .= "\t\t<SECONDARY_AUTHOR>".utf8_decode(trim($auth))."</SECONDARY_AUTHOR>\n";
        } 
      $xml .= "\t</SECONDARY_AUTHORS>\n";
    }
    $xml .= (!empty($rec->biblio_secondary_title))?"\t<SECONDARY_TITLE>". htmlentities($rec->biblio_secondary_title) ."</SECONDARY_TITLE>\n":"";
    $xml .= (!empty($rec->biblio_place_published))?"\t<PLACE_PUBLISHED>". htmlentities($rec->biblio_place_published) ."</PLACE_PUBLISHED>\n":"";
    $xml .= (!empty($rec->biblio_publisher))?"\t<PUBLISHER>". htmlentities($rec->biblio_publisher) ."</PUBLISHER>\n":"";
    $xml .= (!empty($rec->biblio_volume))?"\t<VOLUME>$rec->biblio_volume</VOLUME>\n":"";
//        <NUMBER_OF_VOLUMES>number of volumes</NUMBER_OF_VOLUMES>
    $xml .= (!empty($rec->biblio_issue))?"\t<NUMBER>$rec->biblio_issue</NUMBER>\n":"";
    $xml .= (!empty($rec->biblio_pages))?"\t<PAGES>$rec->biblio_pages</PAGES>\n":"";
//        <SECTION>section</SECTION>
    if (!empty($rec->biblio_tertiary_authors)) {
      $xml .= "\t<TERTIARY_AUTHORS>\n";
        $author_array = explode(";", $rec->biblio_tertiary_authors);
        foreach($author_array as $auth) {
           $xml .= "\t\t<TERTIARY_AUTHOR>".trim($auth)."</TERTIARY_AUTHOR>\n";
        } 
      $xml .= "\t</TERTIARY_AUTHORS>\n";

    } 
    $xml .= (!empty($rec->biblio_tertiary_title))?"\t<TERTIARY_TITLE>$rec->biblio_tertiary_title</TERTIARY_TITLE>\n":"";
    $xml .= (!empty($rec->biblio_edition))?"\t<EDITION>$rec->biblio_edition</EDITION>\n":"";
    $xml .= (!empty($rec->biblio_date))?"\t<DATE>$rec->biblio_date</DATE>\n":"";
    //    <TYPE_OF_WORK>type of work</TYPE_OF_WORK>
    //    <SUBSIDIARY_AUTHORS>
    //        <SUBSIDIARY_AUTHOR>lastname1, firstname1</SUBSIDIARY_AUTHOR>
    //        <SUBSIDIARY_AUTHOR>lastname2, firstname2</SUBSIDIARY_AUTHOR>
    //    </SUBSIDIARY_AUTHORS>
    //    <SHORT_TITLE>short title</SHORT_TITLE>
    //    <ALTERNATE_TITLE>alternate title</ALTERNATE_TITLE>
    $xml .= (!empty($rec->biblio_isbn))?"\t<ISBN>$rec->biblio_isbn</ISBN>\n":"";
    //    <ORIGINAL_PUB>original publication</ORIGINAL_PUB>
    //    <REPRINT_EDITION>reprint edition</REPRINT_EDITION>
    //    <REVIEWED_ITEM>reviewed item</REVIEWED_ITEM>
    //    <CUSTOM1>custom 1</CUSTOM1>
    //    <CUSTOM2>custom 2</CUSTOM2>
    //    <CUSTOM3>custom 3</CUSTOM3>
    //    <CUSTOM4>custom 4</CUSTOM4>
    //    <CUSTOM5>custom 5</CUSTOM5>
    //    <CUSTOM6>custom 6</CUSTOM6>
    $xml .= (!empty($rec->biblio_accession_number))?"\t<ACCESSION_NUMBER>$rec->biblio_accession_number</ACCESSION_NUMBER>\n":"";
    $xml .= (!empty($rec->biblio_call_number))?"\t<CALL_NUMBER>$rec->biblio_call_number</CALL_NUMBER>\n":"";
    //    <LABEL>label</LABEL>
    if (!empty($rec->biblio_keywords)) {
      $xml .= "\t<KEYWORDS>\n";
      $splitchar = (strstr($rec->biblio_keywords, ";"))?";":" ";
      $kw_array = explode($splitchar, $rec->biblio_keywords);
      foreach($kw_array as $kw) {
        $xml .= "\t\t<KEYWORD>".trim($kw)."</KEYWORD>\n";
      } 
      $xml .="\t</KEYWORDS>\n";
    } 
    $xml .= (!empty($rec->biblio_abst_e))?"\t<ABSTRACT>". htmlentities($rec->biblio_abst_e) ."</ABSTRACT>\n":"";
    $xml .= (!empty($rec->biblio_notes))?"\t<NOTES>$rec->biblio_notes</NOTES>\n":"";
    $xml .= (!empty($rec->biblio_url))?"\t<URL>". htmlentities($rec->biblio_url) ."</URL>\n":"";
    //    <AUTHOR_ADDRESS>author address</AUTHOR_ADDRESS>
    //    <CAPTION>caption</CAPTION>
    $xml .= "</RECORD>\n";
  } //end while
    
  $xml .= "</RECORDS></XML>";

  return $xml;
}
