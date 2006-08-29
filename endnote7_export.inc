<?php
// $Id$ 
function _endnote7_XML_export($result){



  $xml = '<?xml version="1.0"?>
            <XML>   
            <RECORDS>';

  while ($rec = db_get_object($result)){

    $xml .= '<RECORD>';
        <REFERENCE_TYPE>31</REFERENCE_TYPE>
    $xml .= '<AUTHORS>';
            <AUTHOR>lastname1, firstname1</AUTHOR>
            <AUTHOR>lastname1, firstname1</AUTHOR>
    $xml .= '</AUTHORS>';
    $xml .= ($rec->biblio_year)?"<YEAR>$rec->biblio_year</YEAR>":"";
    $xml .= ($rec->title)?"<TITLE>$rec->title</TITLE>":"";
        <SECONDARY_AUTHORS>
            <SECONDARY_AUTHOR>lastname1, firstname1</SECONDARY_AUTHOR>
            <SECONDARY_AUTHOR>lastname1, firstname1</SECONDARY_AUTHOR>
        </SECONDARY_AUTHORS>
    $xml .= ($rec->biblio_secondary_title)?"<SECONDARY_TITLE>$rec->biblio_secondary_title</SECONDARY_TITLE>":"";
    $xml .= ($rec->biblio_place_published)?"<PLACE_PUBLISHED>$rec->biblio_place_published</PLACE_PUBLISHED>":"";
    $xml .= ($rec->biblio_publisher)?"<PUBLISHER>$rec->biblio_publisher</PUBLISHER>":"";
        <VOLUME>volume</VOLUME>
        <NUMBER_OF_VOLUMES>number of volumes</NUMBER_OF_VOLUMES>
        <NUMBER>number</NUMBER>
        <PAGES>pages</PAGES>
        <SECTION>section</SECTION>
        <TERTIARY_AUTHORS>
            <TERTIARY_AUTHOR>lastname1, firstname1</TERTIARY_AUTHOR>
            <TERTIARY_AUTHOR>lastname1, firstname1</TERTIARY_AUTHOR>
        </TERTIARY_AUTHORS>
        <TERTIARY_TITLE>this is te tertiary title</TERTIARY_TITLE>
        <EDITION>edition</EDITION>
        <DATE>date</DATE>
        <TYPE_OF_WORK>type of work</TYPE_OF_WORK>
        <SUBSIDIARY_AUTHORS>
            <SUBSIDIARY_AUTHOR>lastname1, firstname1</SUBSIDIARY_AUTHOR>
            <SUBSIDIARY_AUTHOR>lastname2, firstname2</SUBSIDIARY_AUTHOR>
        </SUBSIDIARY_AUTHORS>
        <SHORT_TITLE>short title</SHORT_TITLE>
        <ALTERNATE_TITLE>alternate title</ALTERNATE_TITLE>
        <ISBN>isbn and issn</ISBN>
        <ORIGINAL_PUB>original publication</ORIGINAL_PUB>
        <REPRINT_EDITION>reprint edition</REPRINT_EDITION>
        <REVIEWED_ITEM>reviewed item</REVIEWED_ITEM>
        <CUSTOM1>custom 1</CUSTOM1>
        <CUSTOM2>custom 2</CUSTOM2>
        <CUSTOM3>custom 3</CUSTOM3>
        <CUSTOM4>custom 4</CUSTOM4>
        <CUSTOM5>custom 5</CUSTOM5>
        <CUSTOM6>custom 6</CUSTOM6>
        <ACCESSION_NUMBER>accession number</ACCESSION_NUMBER>
        <CALL_NUMBER>call number</CALL_NUMBER>
        <LABEL>label</LABEL>
        <KEYWORDS>
            <KEYWORD>keyword1</KEYWORD>
            <KEYWORD>keyword2</KEYWORD>
        </KEYWORDS>
        <ABSTRACT>abstract</ABSTRACT>
        <NOTES>notes</NOTES>
        <URL>this is the url to the source</URL>
        <AUTHOR_ADDRESS>author address</AUTHOR_ADDRESS>
        <CAPTION>caption</CAPTION>
    </RECORD>
</RECORDS>
</XML>

  return $xml;
}
