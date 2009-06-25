<?PHP
// $Id$
function _endnote8_XML_export($results){
  $style_attr = 'face="normal" font="default" size="100%"';
  $xml = '<?xml version="1.0" encoding="UTF-8"?>';
  $xml .= "<xml><records>";
  if (!is_array($results)) {
    $result_array[] = $results;
  }
  else {
    $result_array = $results;
  }
  foreach ($result_array as $pub) {
    //en8_encode_font_faces($pub);
    $xml .= "<record>";
    $xml .= '<source-app name="Biblio" version="6.x">Drupal-Biblio</source-app>';
    $xml .= "\t<ref-type>". _endnote8_type_map($pub->biblio_type) ."</ref-type>";
    unset($pub->biblio_type);
    //<!-- Author information -->
    $xml .= en8_add_contributors($pub);
    $xml .= en8_add_titles($pub);
    $xml .= en8_add_keywords($pub);
    $xml .= en8_add_dates($pub);
    $xml .= en8_add_urls($pub);

    foreach($pub as $key => $value) {
      $entag = en8_field_map($key);
      if (!empty($entag)) {
        $xml .=  "<".$entag.'><style face="normal" font="default" size="100%">'.htmlspecialchars($value)."</style></$entag>";
      }
    }

    $xml .= "</record>";
  }
  $xml .= '</records></xml>';
  return $xml;
}

function en8_encode_font_faces(&$pub) {
  $search = array('<b>','<i>', '<u>', '<sup>', '<sub>','</b>','</i>', '</u>', '</sup>', '</sub>');
  $replace = array(
    '<style face="bold" font="default" size="100%">',
    '<style face="italic" font="default" size="100%">',
    '<style face="underline" font="default" size="100%">',
    '<style face="superscript" font="default" size="100%">',
    '<style face="subscript" font="default" size="100%">',
    '</sytle>',
    '</sytle>',
    '</sytle>',
    '</sytle>',
    '</sytle>',
  );
  foreach ($pub as $key => $value) {
    $pub->$key = str_ireplace($search, $replace, $value);
  }
}

function en8_add_titles(&$pub) {
  $xml .= '<titles>';
  $xml .= (!empty ($pub->title)) ? '<title><style face="normal" font="default" size="100%">'.htmlspecialchars($pub->title)."</style></title>" :'';
  $xml .= (!empty ($pub->biblio_secondary_title)) ? '<secondary-title><style face="normal" font="default" size="100%">'.htmlspecialchars($pub->biblio_secondary_title)."</style></secondary-title>" :'';
  $xml .= (!empty ($pub->biblio_tertiary_title)) ? '<tertiary-title><style face="normal" font="default" size="100%">'.htmlspecialchars($pub->biblio_tertiary_title)."</style></tertiary-title>" :'';
  $xml .= (!empty ($pub->biblio_alternate_title)) ? '<alt-title><style face="normal" font="default" size="100%">'.htmlspecialchars($pub->biblio_alternate_title)."</style></alt-title>" :'';
  $xml .= (!empty ($pub->biblio_short_title)) ? '<short-title><style face="normal" font="default" size="100%">'.htmlspecialchars($pub->biblio_short_title)."</style></short-title>" :'';
  $xml .= (!empty ($pub->biblio_translated_title)) ? '<translated-title><style face="normal" font="default" size="100%">'.htmlspecialchars($pub->biblio_translated_title)."</style></translated-title>" :'';
  $xml .= '</titles>';
  unset($pub->title);
  unset($pub->biblio_secondary_title);
  unset($pub->biblio_tertiary_title);
  unset($pub->biblio_alternate_title);
  unset($pub->biblio_short_title);
  unset($pub->biblio_translated_title);

  return $xml;
}
function en8_add_urls(&$pub) {
  $xml = '';
  // TODO: fix URLS
  if (!empty($pub->biblio_url)) {
    $xml .= "<urls><related-urls>";
    $xml .= '<url><style face="normal" font="default" size="100%">'.$pub->biblio_url."</style></url>";
    $xml .= "</related-urls></urls>";
  }
  unset($pub->biblio_url);
  return $xml;
}

function en8_add_dates(&$pub) {
  $xml = '';
  if (!empty($pub->biblio_year) || !empty($pub->biblio_date) ) {
    $xml .= '<dates>';
    $xml .=  (!empty($pub->biblio_year)) ? '<year><style  face="normal" font="default" size="100%">'. htmlspecialchars($pub->biblio_year)."</style></year>":'';
    $xml .=  (!empty($pub->biblio_date)) ? '<pub-dates><date><style  face="normal" font="default" size="100%">'. htmlspecialchars($pub->biblio_date)."</style></date></pub-dates>":'';
    $xml .= "</dates>";
  }
  unset($pub->biblio_year);
  unset($pub->biblio_date);
  return $xml;
}

function en8_add_keywords(&$pub) {
  $kw_array = array();
  $xml = '';
  if (!empty($pub->biblio_keywords)) {
    foreach($pub->biblio_keywords as $term) {
      $kw_array[] = trim($term);
    }
  }
  if (!empty($kw_array)) {
    $kw_array = array_unique($kw_array);
    $xml .= '<keywords>';
    foreach($kw_array as $word) {
      $xml .= '<keyword><style  face="normal" font="default" size="100%">'. htmlspecialchars(trim($word)) . "</style></keyword>";
    }
    $xml .= "</keywords>";
  }
  unset($pub->biblio_keywords);
  return $xml;
}

function en8_add_contributors(&$pub) {
  $xml .= '<contributors>';
  if (count($pub->biblio_contributors[1])) {
    $xml .= "<authors>";
    foreach((array)$pub->biblio_contributors[1] as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</authors>";
  }
  if (count($pub->biblio_contributors[2])) {
    $xml .= "<secondary-authors>";
    foreach((array)$pub->biblio_contributors[2] as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</secondary-authors>";
  }
  if (count($pub->biblio_contributors[3])) {
    $xml .= "<tertiary-authors>";
    foreach((array)$pub->biblio_contributors[3] as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</tertiary-authors>";
  }
  if (count($pub->biblio_contributors[4])) {
    $xml .= "<subsidiary-authors>";
    foreach((array)$pub->biblio_contributors[4] as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</subsidiary-authors>";
  }
  if (count($pub->biblio_contributors[5])) {
    $xml .= "<translated-authors>";
    foreach((array)$pub->biblio_contributors[5] as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</translated-authors>";
  }
  $xml .= '</contributors>';
  unset($pub->biblio_contributors);
  return $xml;
}

function en8_field_map($biblio_field) {
  static $fmap = array();
  if(empty($fmap)) {
    $fmap = array(
       'source-app'               => '',
       'rec-number'               => '',
       'ref-type'                 => 'biblio_type',
       'auth-address'             => 'biblio_auth_address',
       'auth-affiliaton'          => '',
       'title'                    => 'title',
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
  }
  return ($en8_field = array_search($biblio_field, $fmap)) ? $en8_field : '';
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