<?php
// $Id$
function biblio_get_type_map($file_format, $default = FALSE) {
  if ($default) {
    switch ($file_format) {
      case 'bibtex':
        return array(
                'article'       => 102,
                'book'          => 100,
                'booklet'       => 129,
                'conference'    => 103,
                'inbook'        => 101,
                'incollection'  => 101,
                'inproceedings' => 103,
                'manual'        => 129,
                'mastersthesis' => 108,
                'misc'          => 129,
                'phdthesis'     => 108,
                'proceedings'   => 104,
                'techreport'    => 129,
                'unpublished'   => 124,
        );
      case 'tagged':
        return array(
                "Journal Article"         => 102,
                "Conference Paper"        => 103,
                "Conference Proceedings"  => 104,
                "Report"                  => 109,
                "Book"                    => 100,
                "Edited Book"             => 100,
                "Book Section"            => 101,
                "Thesis"                  => 108,
                "Patent"                  => 119,
                "Generic"                 => 129,
                "Newspaper Article"       => 105,
                "Web Page"                => 107,
                "Film or Broadcast"       => 110,
                "Artwork"                 => 112,
                "Audiovisual Material"    => 114,
                "Hearing"                 => 115,
                "Case"                    => 116,
                "Bill"                    => 117,
                "Statute"                 => 118,
                "Personal Communication"  => 120,
                "Manuscript"              => 121,
                "Map"                     => 122,
                "Chart or Table"          => 123,
                "Unpublished Work"        => 124,
                "Online Database"         => 125,
                "Government Document"     => 126,
                "Classical Work"          => 127,
                "Legal Rule or Regulation" => 128,
        );
      case 'ris':
        return array(
                'ABST' => 129,
                'ADVS' => 114,
                'ART'  => 112,
                'BILL' => 117,
                'BOOK' => 100,
                'CASE' => 116,
                'CHAP' => 101,
                'COMP' => 113,
                'CONF' => 103,
                'CTLG' => 129,
                'DATA' => 125,
                'ELEC' => 129,
                'GEN'  => 129,
                'HEAR' => 115,
                'ICOMM' => 107,
                'INPR'  => 129,
                'JFULL' => 129,
                'JOUR'  => 102,
                'MAP'   => 122,
                'MGZN'  => 106,
                'MPCT'  => 110,
                'MUSIC' => 129,
                'NEWS'  => 105,
                'PAMP'  => 129,
                'PAT'   => 119,
                'PCOMM' => 120,
                'RPRT'  => 109,
                'SER'   => 100,
                'SLIDE' => 129,
                'SOUND' => 129,
                'STAT'  => 125,
                'THES'  => 108,
                'UNBILl' => 129,
                'UNPB'  => 124,
                'VIDEO' => 129,
        );
      case 'endnote7':
        return array(
                0 => 102, // Journal Article
                1 => 100, // Book
                2 => 108, // Thesis
                3 => 103, // Conference Proceedings
                4 => 120, // Personal Communication
                5 => 105, // NewsPaper Article
                6 => 113, // Computer Program
                7 => 101, // Book Section
                8 => 106, // Magazine Article
                9 => 100, // Edited Book
                10 => 109, // Report
                11 => 122, // Map
                12 => 114, // Audiovisual Material
                13 => 112, // Artwork
                15 => 119, // Patent
                16 => 107, // Electronic Source
                17 => 117, // Bill
                18 => 116, // Case
                19 => 115, // Hearing
                20 => 121, // Manuscript
                21 => 110, // Film or Broadcast
                22 => 118, // Statute
                26 => 123, // Chart or Table
                31 => 129 // Generic
        );
      case 'endnote8':
        return  array(
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
      case 'unixref':
        return array(
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
  }
  else {
    $map = variable_get('biblio_'.$file_format.'_type_map', array());
    if (empty($map)) $map = biblio_get_type_map($file_format, TRUE);
    return $map;

  }
}
function biblio_get_type_names($file_format, $default = FALSE) {
  if ($default) {
    switch ($file_format) {
      case 'bibtex':
        return array(
                'article'       => 'An article from a journal',
                'book'          => 'A book with an explicit publisher',
                'booklet'       => 'A work that is printed and bound, but without a named publisher or sponsoring institution',
                'conference'    => 'An article in a conference proceedings',
                'inbook'        => 'A part of a book, usually untitled. May be a chapter (or section or whatever) and/or a range of pages',
                'incollection'  => 'A part of a book having its own title',
                'inproceedings' => 'An article in a conference proceedings',
                'manual'        => 'Technical documentation',
                'mastersthesis' => 'A Master\'s thesis',
                'misc'          => 'For use when nothing else fits',
                'phdthesis'     => 'A Ph.D. thesis',
                'proceedings'   => 'The proceedings of a conference',
                'techreport'    => 'A report published by a school or other institution, usually numbered within a series',
                'unpublished'   => 'A document having an author and title, but not formally published',
        );
      case 'tagged':
        return array(
                "Journal Article"         => "Journal Article",
                "Conference Paper"        => "Conference Paper",
                "Conference Proceedings"  => "Conference Proceedings",
                "Report"                  => "Report",
                "Book"                    => "Book",
                "Edited Book"             => "Edited Book",
                "Book Section"            => "Book Section",
                "Thesis"                  => "Thesis",
                "Patent"                  => "Patent",
                "Generic"                 => "Generic",
                "Newspaper Article"       => "Newspaper Article",
                "Web Page"                => "Web Page",
                "Film or Broadcast"       => "Film or Broadcast",
                "Artwork"                 => "Artwork",
                "Audiovisual Material"    => "Audiovisual Material",
                "Hearing"                 => "Hearing",
                "Case"                    => "Case",
                "Bill"                    => "Bill",
                "Statute"                 => "Statute",
                "Personal Communication"  => "Personal Communication",
                "Manuscript"              => "Manuscript",
                "Map"                     => "Map",
                "Chart or Table"          => "Chart or Table",
                "Unpublished Work"        => "Unpublished Work",
                "Online Database"         => "Online Database",
                "Government Document"     => "Government Document",
                "Classical Work"          => "Classical Work",
                "Legal Rule or Regulation" => "Legal Rule or Regulation",
        );
      case 'ris':
        return array(
                'ABST'   => 'Abstract',
                'ADVS'   => 'Audiovisual material',
                'ART'    => 'Art Work',
                'BILL'   => 'Bill/Resolution',
                'BOOK'   => 'Book, Whole',
                'CASE'   => 'Case',
                'CHAP'   => 'Book chapter',
                'COMP'   => 'Computer program',
                'CONF'   => 'Conference proceeding',
                'CTLG'   => 'Catalog',
                'DATA'   => 'Data file',
                'ELEC'   => 'Electronic Citation',
                'GEN'    => 'Generic',
                'HEAR'   => 'Hearing',
                'ICOMM'  => 'Internet Communication',
                'INPR'   => 'In Press',
                'JFULL'  => 'Journal (full)',
                'JOUR'   => 'Journal',
                'MAP'    => 'Map',
                'MGZN'   => 'Magazine article',
                'MPCT'   => 'Motion picture',
                'MUSIC'  => 'Music score',
                'NEWS'   => 'Newspaper',
                'PAMP'   => 'Pamphlet',
                'PAT'    => 'Patent',
                'PCOMM'  => 'Personal communication',
                'RPRT'   => 'Report',
                'SER'    => 'Serial (Book, Monograph)',
                'SLIDE'  => 'Slide',
                'SOUND'  => 'Sound recording',
                'STAT'   => 'Statute',
                'THES'   => 'Thesis/Dissertation',
                'UNBILl' => 'Unenacted bill/resolution',
                'UNPB'   => 'Unpublished work',
                'VIDEO'  => 'Video recording',
        );
      case 'endnote7':
        return array(
                0 => 'Journal Article',
                1 => 'Book',
                2 => 'Thesis',
                3 => 'Conference Proceedings',
                4 => 'Personal Communication',
                5 => 'NewsPaper Article',
                6 => 'Computer Program',
                7 => 'Book Section',
                8 => 'Magazine Article',
                9 => 'Edited Book',
                10 => 'Report',
                11 => 'Map',
                12 => 'Audiovisual Material',
                13 => 'Artwork',
                15 => 'Patent',
                16 => 'Electronic Source',
                17 => 'Bill',
                18 => 'Case',
                19 => 'Hearing',
                20 => 'Manuscript',
                21 => 'Film or Broadcast',
                22 => 'Statute',
                26 => 'Chart or Table',
                31 => 'Generic',
        );

      case 'endnote8':
        return array(
                2 => 'Artwork',
                3 => 'Audio Visual',
                4 => 'Bill',
                5 => 'Book Section',
                6 => 'Book',
                7 => 'Case',
                9 => 'Software',
                17 => 'Journal Article',
                10 => 'Conference Proceeding',
                12 => 'Web page',
                13 => 'Generic',
                14 => 'Hearing',
                19 => 'Magazine Article',
                20 => 'Map',
                21 => 'Film',
                21 => 'Broadcast',
                23 => 'Newspaper Article',
                25 => 'Patent',
                26 => 'Personal Communication',
                27 => 'Report',
                28 => 'Edited Book',
                31 => 'Statute',
                32 => 'Thesis',
                34 => 'Unpublished',
                36 => 'Manuscript',
                37 => 'Figure',
                38 => 'Chart',
                39 => 'Equation',
                43 => 'Electronic Article',
                44 => 'Electronic Book',
                45 => 'Online Database',
                46 => 'Government Document',
                47 => 'Conference Paper',
                48 => 'Online Multimedia',
                49 => 'Classical Work',
                50 => 'Legal Ruling',
                52 => 'Dictionary',
                53 => 'Encyclopedia',
                54 => 'Grant',
        );
      case 'unixref':
        return array(
                'error'        => 'Error',
                'book'         => 'Book',
                'journal'      => 'Journal Article',
                'standard'     => 'Generic',
                'conference'   => 'Conference Paper',
                'report-paper' => 'Report',
                'dissertation' => 'Thesis',
                'database'     => 'Online database',
                'sa_component' => 'SA Component',
           );
    }
  }
  else {
    $names = variable_get('biblio_'.$file_format.'_type_names', array());
    if (empty($names))  $names = biblio_get_type_names($file_format, TRUE);
    return $names;
  }
}
function biblio_get_field_map($file_format, $default = FALSE) {
  if ($default) {
    switch ($file_format) {
      case 'bibtex':
      case 'tagged':
        return array(
                '%B' => 'biblio_secondary_title',
                '%C' => 'biblio_place_published',
                '%D' => 'biblio_year',
                '%F' => 'biblio_label',
                '%G' => 'biblio_lang',
                '%I' => 'biblio_publisher',
                '%J' => 'biblio_secondary_title',
                '%K' => 'biblio_keywords',
                '%L' => 'biblio_call_number',
                '%M' => 'biblio_accession_number',
                '%N' => 'biblio_issue',
                '%P' => 'biblio_pages',
                '%R' => 'biblio_doi',
                '%S' => 'biblio_tertiary_title',
                '%U' => 'biblio_url',
                '%V' => 'biblio_volume',
                '%1' => 'biblio_custom1',
                '%2' => 'biblio_custom2',
                '%3' => 'biblio_custom3',
                '%4' => 'biblio_custom4',
                '%#' => 'biblio_custom5',
                '%$' => 'biblio_custom6',
                '%]' => 'biblio_custom7',
                '%6' => 'biblio_number_of_volumes',
                '%7' => 'biblio_edition',
                '%8' => 'biblio_date',
                '%9' => 'biblio_type_of_work',
                '%?' => '',
                '%@' => 'biblio_isbn',
                '%<' => 'biblio_research_notes',
                '%!' => '',
                '%&' => '',
                '%(' => '',
                '%)' => '',
                '%*' => '',
                '%+' => '',
        );
      case 'ris':
        return array(
                'ID'  =>  '',                     //- Reference ID (not imported to reference software)
                'T1'  =>  '',                     //- Primary title
                'TI'  =>  '',                     //- Book title
                'CT'  =>  '',                     //- Title of unpublished reference
                'A1'  =>  '',                     //- Primary author
                'A2'  =>  '',                     //- Secondary author (each name on separate line)
                'AU'  =>  '',                     //- Author (syntax. Last name, First name, Suffix)
                'Y1'  =>  '',                     //- Primary date
                'PY'  =>  '',                     //- Publication year (YYYY/MM/DD)
                'N1'  => 'biblio_notes',          //- Notes
                'KW'  =>  '',                     //- Keywords (each keyword must be on separate line preceded KW -)
                'RP'  =>  '',                     //- Reprint status (IN FILE, NOT IN FILE, ON REQUEST (MM/DD/YY))
                'SP'  =>  '',                     //- Start page number
                'EP'  =>  '',                     //- Ending page number
                'JF'  => 'biblio_secondary_title',//- Periodical full name
                'JO'  => 'biblio_short_title',    //- Periodical standard abbreviation
                'JA'  => 'biblio_secondary_title',//- Periodical in which article was published
                'J1'  =>  'biblio_short_title',   //- Periodical name //- User abbreviation 1
                'J2'  =>  'biblio_short_title',   //- Periodical name - User abbreviation 2
                'VL'  => 'biblio_volume',         //- Volume number
                'IS'  => 'biblio_issue',          //- Issue number
                'CP'  => 'biblio_issue',          //- Issue number
                'T2'  => 'biblio_secondary_title',//- Title secondary
                'CY'  => 'biblio_place_published',//- City of Publication
                'PB'  => 'biblio_publisher',      //- Publisher
                'U1'  => 'biblio_custom1',        //- User definable 1
                'U2'  => 'biblio_custom2',        //- User definable 2
                'U3'  => 'biblio_custom3',        //- User definable 3
                'U4'  => 'biblio_custom4',        //- User definable 4
                'U5'  => 'biblio_custom5',        //- User definable 5
                'T3'  => 'biblio_tertiary_title', //- Title series
                'AB'  => 'biblio_abst_e',         //- Abstract
                'N2'  => 'biblio_abst_e',         //- Abstract
                'SN'  => 'biblio_isbn',           //- ISSN/ISBN (e.g. ISSN XXXX-XXXX)
                'AV'  =>  '',                     //- Availability
                'M1'  =>  '',                     //- Misc. 1
                'M3'  =>  '',                     //- Misc. 3
                'AD'  =>  '',                     //- Address
                'UR'  => 'biblio_url',            //- Web/URL
                'L1'  =>  '',                     //- Link to PDF
                'L2'  =>  '',                     //- Link to Full-text
                'L3'  =>  '',                     //- Related records
                'L4'  =>  '',                     //- Images
                'ER'  =>  '',                     //- End of Reference (must be the last tag)
        );
      case 'endnote7':
        return array(
                'AUTHOR'            => 1,            //
                'SECONDARY_AUTHOR'  => 2,  // The 4 author fields map back to the author category
                'TERTIARY_AUTHOR'   => 3,   // rather than a field name.
                'SUBSIDIARY_AUTHOR' => 4, //
                'REFERENCE_TYPE'    => 'biblio_type',
                'REFNUM'            => '',
                'YEAR'              => 'biblio_year',
                'SECONDARY_TITLE'   => 'biblio_secondary_title',
                'PLACE_PUBLISHED'   => 'biblio_place_published',
                'PUBLISHER'         => 'biblio_publisher',
                'VOLUME'            => 'biblio_volume',
                'ISSUE'             => 'biblio_issue',
                'NUMBER_OF_VOLUMES' => 'biblio_number_of_volumes',
                'NUMBER'            => 'biblio_number',
                'PAGES'             => 'biblio_pages',
                'SECTION'           => 'biblio_section',
                'TERTIARY_TITLE'    => 'biblio_tertiary_title',
                'EDITION'           => 'biblio_edition',
                'DATE'              => 'biblio_date',
                'TYPE_OF_WORK'      => 'biblio_type_of_work',
                'SHORT_TITLE'       => 'biblio_short_title',
                'ALTERNATE_TITLE'   => 'biblio_alternate_title',
                'ISBN'              => 'biblio_isbn',
                'ORIGINAL_PUB'      => 'biblio_original_publication',
                'REPRINT_EDITION'   => 'biblio_reprint_edition',
                'REVIEWED_ITEM'     => '',
                'CUSTOM1'           => 'biblio_custom1',
                'CUSTOM2'           => 'biblio_custom2',
                'CUSTOM3'           => 'biblio_custom3',
                'CUSTOM4'           => 'biblio_custom4',
                'CUSTOM5'           => 'biblio_custom5',
                'CUSTOM6'           => 'biblio_custom6',
                'ACCESSION_NUMBER'  => 'biblio_accession_number',
                'CALL_NUMBER'       => 'biblio_call_number',
                'LABEL'             => 'biblio_label',
                'KEYWORD'           => 'biblio_keywords',
                'ABSTRACT'          => 'biblio_abst_e',
                'NOTES'             => 'biblio_notes',
                'URL'               => 'biblio_url',
                'AUTHOR_ADDRESS'    => '',
                'IMAGE'             => '',
                'CAPTION'           => '',
         );

      case 'endnote8':
        return array(
                 'source-app'               => '',
                 'rec-number'               => '',
                 'ref-type'                 => 'biblio_type',
                 'auth-address'             => 'biblio_auth_address',
                 'auth-affiliaton'          => '',
                 'title'                    => '', //handled separately
                 'secondary-title'          => 'biblio_secondary_title',
                 'tertiary-title'           => 'biblio_tertiary_title',
                 'alt-title'                => 'biblio_alternate_title',
                 'short-title'              => 'biblio_short_title',
                 'translated-title'         => 'biblio_translated_title',
                 'full-title'               => '',
                 'abbr-1'                   => '',
                 'abbr-2'                   => '',
                 'abbr-3'                   => '',
                 'pages'                    => 'biblio_pages',
                 'volume'                   => 'biblio_volume',
                 'number'                   => 'biblio_number',
                 'issue'                    => 'biblio_issue',
                 'secondary-volume'         => '',
                 'secondary-issue'          => '',
                 'num-vols'                 => 'biblio_number_of_volumes',
                 'edition'                  => 'biblio_edition',
                 'section'                  => 'biblio_section',
                 'reprint-edition'          => 'biblio_reprint_edition',
                 'reprint-status'           => '',
                 'year'                     => 'biblio_year',
                 'pub-dates'                => 'biblio_date',
                 'copyright-dates'          => '',
                 'pub-location'             => 'biblio_place_published',
                 'publisher'                => 'biblio_publisher',
                 'orig-pub'                 => 'biblio_original_publication',
                 'isbn'                     => 'biblio_isbn',
                 'accession-num'            => 'biblio_accession_number',
                 'call-num'                 => 'biblio_call_number',
                 'report-id'                => '',
                 'coden'                    => '',
                 'electronic-resource-num'  => '',
                 'abstract'                 => 'biblio_abst_e',
                 'label'                    => 'biblio_label',
                 'image'                    => '',
                 'caption'                  => '',
                 'notes'                    => 'biblio_notes',
                 'research-notes'           => 'biblio_research_notes',
                 'work-type'                => 'biblio_type_of_work',
                 'reviewed-item'            => '',
                 'availability'             => '',
                 'remote-source'            => '',
                 'meeting-place'            => '',
                 'work-location'            => '',
                 'work-extent'              => '',
                 'pack-method'              => '',
                 'size'                     => '',
                 'repro-ratio'              => '',
                 'remote-database-name'     => 'biblio_remote_db_name',
                 'remote-database-provider' => 'biblio_remote_db_provider',
                 'language'                 => 'biblio_lang',
                 'web-urls'                 => '',
                 'pdf-urls'                 => '',
                 'text-urls'                => '',
                 'image-urls'               => '',
                 'related-urls'             => 'biblio_url',
                 'access-date'              => 'biblio_access_date',
                 'modified-date'            => '',
                 'custom1'                  => 'biblio_custom1',
                 'custom2'                  => 'biblio_custom2',
                 'custom3'                  => 'biblio_custom3',
                 'custom4'                  => 'biblio_custom4',
                 'custom5'                  => 'biblio_custom5',
                 'custom6'                  => 'biblio_custom6',
                 'custom7'                  => 'biblio_custom7',
                 'misc1'                    => '',
                 'misc2'                    => '',
                 'misc3'                    => '',
              );

      case 'unixref':
        return array(
                'publisher_place'     => 'biblio_place_published',
                'publisher_name'      => 'biblio_publisher',
                'volume'              => 'biblio_volume',
                'number'              => 'biblio_number',
                'issue'               => 'biblio_issue',
                'edition_number'      => 'biblio_edition',
                'section'             => 'biblio_section',
                'doi'                 => 'doi',
                'title'               => 'title',
                'isbn'                => 'isbn',
                'issn'                => 'issn',
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
                'year'                => 'year',
                'month'               => 'month',
                'day'                 => 'day',
                'degree'              => 'biblio_type_of_work',
                'error'               => 'error',
                'language'            => 'biblio_lang',
        );

    }
  }
  else {
    $map = variable_get('biblio_'.$file_format.'_field_map', array());
    if (empty($map)) $map = biblio_get_field_map($file_format, TRUE);
    return $map;

  }
}
