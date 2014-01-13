<?php

/**
 * @file
 * EndNote XML8 biblio style.
 */

class BiblioStyleEndNoteXML8 extends BiblioStyleEndNote implements BiblioStyleImportInterface {

  /**
   * @inheritdoc
   */
  public function importData($data, $options = array()) {
    $options += array(
      // Populate the pattern here, so BiblioStyleEndNoteXML7 can re-use
      // this class.
      'pattern' => '/<ref-type>(.*)<\/ref-type>/',
    );
    $match = array();

    // Get the Biblio type from the XML.
    preg_match($options['pattern'], $data, $match);
    if (!$type = intval($match[1])) {
      return;
    }

    $type = $this->getBiblioType($type);
    $this->biblio = $biblio = biblio_create($type);
    $this->wrapper = $wrapper = entity_metadata_wrapper('biblio', $this->biblio);

    $data = str_replace("\r\n", "\n", $data);

    $parser = drupal_xml_parser_create($data);
    xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, FALSE);
    xml_parser_set_option($parser, XML_OPTION_SKIP_WHITE, TRUE);
    xml_set_object($parser, $this);
    xml_set_element_handler($parser, 'startElement', 'endElement');
    xml_set_character_data_handler($parser, 'characterData');

    if (!xml_parse($parser, $data)) {
      $params = array(
        '@error' => xml_error_string(xml_get_error_code($parser)),
        '@line' => xml_get_current_line_number($parser),
      );
      return array(
        'error' => array(
          'data' => $data,
          'message' => t('XML error: @error at line @line', $params),
        ),
      );
    }

    xml_parser_free($parser);

    $biblios[] = $biblio;
    return array(
      'success' => $biblios,
    );
  }


  public function startElement($parser, $name, $attrs) {
    if ($name == 'style') {
      $this->font_attr = explode(' ', $attrs['face']);
      foreach ($this->font_attr as $font_attr) {
        switch ($font_attr) {
          case 'normal':
            // Do nothing.
            break;

          case 'bold':
            $this->characterData(NULL, '<b>');
            break;

          case 'italic':
            $this->characterData(NULL, '<i>');
            break;

          case 'underline':
            $this->characterData(NULL, '<u>');
            break;

          case 'superscript':
            $this->characterData(NULL, '<sup>');
            break;

          case 'subscript':
            $this->characterData(NULL, '<sub>');
            break;
        }
      }
    }
    else {
      $this->element = $name;
      if (strpos($name, 'authors') !== FALSE) {
        // Add the role of the contributor.
        $this->role = $name;
      }
    }
  }

  public function endElement($parser, $name) {
    if ($name == 'style') {
      foreach ($this->font_attr as $font_attr) {
        switch ($font_attr) {
          case 'normal':
            // Do nothing.
            break;

          case 'bold':
            $this->characterData(NULL, '</b>');
            break;

          case 'italic':
            $this->characterData(NULL, '</i>');
            break;

          case 'underline':
            $this->characterData(NULL, '</u>');
            break;

          case 'superscript':
            $this->characterData(NULL, '</sup>');
            break;

          case 'subscript':
            $this->characterData(NULL, '</sub>');
            break;
        }
      }
    }
    else {
      $this->element = '';
    }
  }

  /**
   * @todo: Import keywords.
   */
  public function characterData($parser, $data) {
    $map = $this->getMapping();

    $element = $this->element;

    if (!empty($map['field'][$element]['import_method'])) {
      // Prepare the data by striping any tags or white space.
      $data = explode("\n", $data);
      foreach ($data as $key => $value) {
        $data[$key] = trim(htmlspecialchars_decode($value));
      }
      $data = implode('', $data);

      if (!$data) {
        // No data given, it might have been a carriage return that was striped.
        return;
      }

      $method = $map['field'][$element]['import_method'];

      // Key might be a Biblio field, or the role of a contributor.
      $key = $element == 'author' ? $this->role : $map['field'][$element]['property'];

      $this->{$method}($this->wrapper, $key, $data);
    }
  }

  /**
   * Import generic property.
   *
   * @param EntityMetadataWrapper $wrapper
   *   The wrapped Biblio object.
   * @param $property
   *   The property name to import.
   * @param $data
   *   The data to import from.
   *
   */
  public function importGeneric(EntityMetadataWrapper $wrapper, $property, $data) {
    // @todo: Make more generic + configurable?
    if (!isset($wrapper->{$property})) {
      // Create field.
      biblio_create_field($property, 'biblio', $wrapper->getBundle());
    }
    $value = $wrapper->{$property}->value() . $data;
    $wrapper->{$property}->set($value);
  }

  /**
   * Import year and Biblio status.
   *
   * @param EntityMetadataWrapper $wrapper
   *   The wrapped Biblio object.
   * @param $property
   *   The property name to import.
   * @param $data
   *   The data to import from.
   */
  public function importYear(EntityMetadataWrapper $wrapper, $property, $data) {
    if (is_numeric($data)) {
      $wrapper->biblio_year->set($data);
      return;
    }

    // @todo: Get Biblio status valid options from field.
    $options = array(
      'in_press',
      'submitted',
    );

    $data = str_replace(' ', '_', strtolower($data));

    if (in_array($data, $options)) {
      $wrapper->biblio_status->set($data);
    }
  }

  /**
   * Import a Contributor.
   *
   * @param EntityMetadataWrapper $wrapper
   *   The wrapped Biblio object.
   * @param $role
   *   The role of the contributor.
   * @param $name
   *   The name of the contributor.
   */
  public function importContributor(EntityMetadataWrapper $wrapper, $role, $name) {
    $biblio = $wrapper->value();

    // Map the role to Biblio.
    $role = $role == 'authors' ? 'author' : str_replace('-authors', '', $role);

    // @todo: Check if roles make sense.
    switch ($role) {
      case 'authors':
        $role = 'Author';
        break;

      case 'secondary-authors':
        $role = 'Secondary Author';
        break;

      case 'tertiary-authors':
        $role = 'Tertiary Author';
        break;

      case 'subsidiary-authors':
        $role = 'Subsidiary Author';
        break;

      case 'translated-authors':
        $role = 'Translator';
    }

    $this->addContributors($name, $role);
  }


  /**
   * @inheritdoc
   *
   * @todo: Allow passing in options 'wrap' => TRUE, so we can remove the
   * <records> tag,
   */
  public function render($options = array(), $langcode = NULL) {
    $wrapper = entity_metadata_wrapper('biblio', $this->biblio);

    $writer = new XMLWriter();
    $writer->openMemory();
    $writer->startDocument('1.0','UTF-8');
    $writer->startElement("records");
    $writer->startElement("record");

    $this->addTitles($writer, $wrapper);

    // $writer->writeAttribute("ah", "OK");
    $writer->endElement();
    $writer->endElement();
    return $writer->outputMemory(true);
  }

  public function addTitles(XMLWriter $writer, EntityDrupalWrapper $wrapper) {
    $writer->startElement('titles');
    $writer->startElement('title');
    $writer->text($wrapper->label());
    $writer->endElement();
    $writer->endElement();
  }

  /**
   * @inheritdoc
   */
  public function getMapping() {
    $return = array(
      'type' => array(
        2 => 'artwork',
        3 => 'audiovisual',
        4 => 'bill',
        5 => 'book_chapter',
        6 => 'book',
        7 => 'case',
        9 => 'software',
        10 => 'conference_proceedings',
        12 => 'web_article',
        13 => 'miscellaneous',
        14 => 'hearing',
        17 => 'journal_article',
        19 => 'magazine_article',
        20 => 'map',
        21 => 'broadcast',
        23 => 'newspaper_article',
        25 => 'patent',
        26 => 'personal',
        27 => 'report',
        28 => 'miscellaneous',
        31 => 'statute',
        32 => 'thesis',
        34 => 'unpublished',
        36 => 'manuscript',
        37 => 'miscellaneous',
        38 => 'chart',
        39 => 'miscellaneous',
        43 => 'miscellaneous',
        44 => 'miscellaneous',
        45 => 'database',
        46 => 'government_report',
        47 => 'conference_paper',
        48 => 'miscellaneous',
        49 => 'classical',
        50 => 'legal_ruling',
        52 => 'miscellaneous',
        53 => 'miscellaneous',
        54 => 'miscellaneous',
      ),
      'field' => array(
        // Todo: Get the role from the XML tag.
        'author' => array(
          // We don't have a property for this key. The role name will be taken
          // from the parent tag (e.g. <authors>, <secondary-authors>).
          'import_method' => 'importContributor',
        ),
        'abbr-1' => array('property' => 'biblio_short_title'),
        'abstract' => array('property' => 'biblio_abstract'),
        'access-date' => array('property' => 'biblio_access_date'),
        'accession-num' => array('property' => 'biblio_accession_number'),
        'alt-title' => array('property' => 'biblio_alternate_title'),
        'auth-address' => array('property' => 'biblio_auth_address'),
        'call-num' => array('property' => 'biblio_call_number'),
        'edition' => array('property' => 'biblio_edition'),
        'full-title' => array('property' => 'biblio_secondary_title'),
        'isbn' => array('property' => 'biblio_isbn'),
        'issue' => array('property' => 'biblio_issue'),
        'label' => array('property' => 'biblio_label'),
        'language' => array('property' => 'biblio_language'),
        'notes' => array('property' => 'biblio_notes'),
        'num-vols' => array('property' => 'biblio_number_of_volumes'),
        'number' => array('property' => 'biblio_number'),
        'orig-pub' => array('property' => 'biblio_original_publication'),
        'pages' => array('property' => 'biblio_pages'),
        'pub-location' => array('property' => 'biblio_place_published'),
        'publisher' => array('property' => 'biblio_publisher'),
        'related-urls' => array('property' => 'biblio_url'),
        'remote-database-name' => array('property' => 'biblio_remote_db_name'),
        'remote-database-provider' => array('property' => 'biblio_remote_db_provider'),
        'reprint-edition' => array('property' => 'biblio_reprint_edition'),
        'research-notes' => array('property' => 'biblio_search_notes'),
        'secondary-title' => array('property' => 'biblio_secondary_title'),
        'section' => array('property' => 'biblio_section'),
        'short-title' => array('property' => 'biblio_short_title'),
        'tertiary-title' => array('property' => 'biblio_tertiary_title'),
        'title' => array('property' => 'title'),
        'translated-title' => array('property' => 'biblio_translated_title'),
        'url' => array('property' => 'biblio_url'),
        'volume' => array('property' => 'biblio_volume'),
        'work-type' => array('property' => 'biblio_type_of_work'),
        'year' => array(
          'property' => 'biblio_year',
          'import_method' => 'importYear',
        ),
      ),
    );


    // Add default values.
    foreach (array_keys($return['field']) as $key) {
      $return['field'][$key] += array(
        'import_method' => 'importGeneric',
      );
    }

    return $return;
  }
}

function buildXML() {
  $xml = new SimpleXMLElement('<records/>');
  $output = $xml->asXML();
  dpm($output);
}

function _endnote8_XML_export($node, $part = 'record') {
  $style_attr = 'face="normal" font="default" size="100%"';
  switch ($part) {
    case 'begin':
      $xml = '<?xml version="1.0" encoding="UTF-8"?>';
      $xml .= "<xml><records>";
      break;
    case 'record':
      $xml = "<record>";
      $xml .= '<source-app name="Biblio" version="7.x">Drupal-Biblio</source-app>';
      $xml .= "<ref-type>" . _endnote8_type_map($node->biblio_type) . "</ref-type>";
      unset($node->biblio_type);
      //<!-- Author information -->
      $xml .= en8_add_contributors($node);
      $xml .= en8_add_titles($node);
      $xml .= en8_add_keywords($node);
      $xml .= en8_add_dates($node);
      $xml .= en8_add_urls($node);

      foreach ($node as $key => $value) {
        $entag = en8_field_map($key);
        if (!empty($entag) && !empty($value)) {
          $xml .=  "<" . $entag . '><style face="normal" font="default" size="100%">' . htmlspecialchars($value) . "</style></$entag>";
        }
      }
      $xml .= "</record>";
      break;
    case 'end':
      $xml = '</records></xml>';
      break;
  }
  return $xml;
}

function en8_encode_font_faces(&$node) {
  $search = array('<b>', '<i>', '<u>', '<sup>', '<sub>', '</b>', '</i>', '</u>', '</sup>', '</sub>');
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
  foreach ($node as $key => $value) {
    $node->$key = str_ireplace($search, $replace, $value);
  }
}

function en8_add_titles(&$node) {
  $xml = '<titles>';
  $xml .= (!empty ($node->title)) ? '<title><style face="normal" font="default" size="100%">' . htmlspecialchars($node->title) . "</style></title>" :'';
  $xml .= (!empty ($node->biblio_secondary_title)) ? '<secondary-title><style face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_secondary_title) . "</style></secondary-title>" :'';
  $xml .= (!empty ($node->biblio_tertiary_title)) ? '<tertiary-title><style face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_tertiary_title) . "</style></tertiary-title>" :'';
  $xml .= (!empty ($node->biblio_alternate_title)) ? '<alt-title><style face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_alternate_title) . "</style></alt-title>" :'';
  $xml .= (!empty ($node->biblio_short_title)) ? '<short-title><style face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_short_title) . "</style></short-title>" :'';
  $xml .= (!empty ($node->biblio_translated_title)) ? '<translated-title><style face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_translated_title) . "</style></translated-title>" :'';
  $xml .= '</titles>';
  unset($node->title);
  unset($node->biblio_secondary_title);
  unset($node->biblio_tertiary_title);
  unset($node->biblio_alternate_title);
  unset($node->biblio_short_title);
  unset($node->biblio_translated_title);

  return $xml;
}
function en8_add_urls(&$node) {
  global $base_path;
  $xml = '';
  // TODO: fix URLS
  if (!empty($node->biblio_url)) {
    $xml .= "<web-urls>";
    $xml .= '<url><style face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_url) . "</style></url>";
    $xml .= "</web-urls>";
  }
  unset($node->biblio_url);
  if (!empty ($node->upload) && count($node->upload['und'])  && user_access('view uploaded files')) {
    $xml .= "<related-urls>";
    foreach ($node->upload['und'] as $file) {
      $xml .= '<url><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(file_create_url($file['uri']));
      $xml .= "</style></url>";
    }
    $xml .= "</related-urls>";
  }
  unset($node->upload['und']);
  if (!empty($xml)) return "<urls>$xml</urls>";
  return ;
}

function en8_add_dates(&$node) {
  $xml = '';
  if (!empty($node->biblio_year) || !empty($node->biblio_date) ) {
    $xml .= '<dates>';
    $xml .=  (!empty($node->biblio_year)) ? '<year><style  face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_year) . "</style></year>":'';
    $xml .=  (!empty($node->biblio_date)) ? '<pub-dates><date><style  face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_date) . "</style></date></pub-dates>":'';
    $xml .= "</dates>";
  }
  unset($node->biblio_year);
  unset($node->biblio_date);
  return $xml;
}

function en8_add_keywords(&$node) {
  $kw_array = array();
  $xml = '';
  if (!empty($node->biblio_keywords)) {
    foreach ($node->biblio_keywords as $term) {
      $kw_array[] = trim($term);
    }
  }
  if (!empty($kw_array)) {
    $kw_array = array_unique($kw_array);
    $xml .= '<keywords>';
    foreach ($kw_array as $word) {
      $xml .= '<keyword><style  face="normal" font="default" size="100%">' . htmlspecialchars(trim($word)) . "</style></keyword>";
    }
    $xml .= "</keywords>";
  }
  unset($node->biblio_keywords);
  return $xml;
}

function en8_add_contributors(&$node) {
  $xml = '<contributors>';
  $authors = biblio_get_contributor_category($node->biblio_contributors, 1);
  if (!empty($authors)) {
    $xml .= "<authors>";
    foreach ($authors as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</authors>";
  }
  $authors = biblio_get_contributor_category($node->biblio_contributors, 2);
  if (!empty($authors)) {
    $xml .= "<secondary-authors>";
    foreach ($authors as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</secondary-authors>";
  }
  $authors = biblio_get_contributor_category($node->biblio_contributors, 3);
  if (!empty($authors)) {
    $xml .= "<tertiary-authors>";
    foreach ($authors as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</tertiary-authors>";
  }
  $authors = biblio_get_contributor_category($node->biblio_contributors, 4);
  if (!empty($authors)) {
    $xml .= "<subsidiary-authors>";
    foreach ($authors as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</subsidiary-authors>";
  }
  $authors = biblio_get_contributor_category($node->biblio_contributors, 5);
  if (!empty($authors)) {
    $xml .= "<translated-authors>";
    foreach ($authors as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</translated-authors>";
  }
  $xml .= '</contributors>';
  unset($node->biblio_contributors);
  return $xml;
}

function en8_field_map($biblio_field) {
  static $fmap = array();
  if (empty($fmap)) {
    $fmap = biblio_get_map('field_map', 'endnote8');
  }
  return ($en8_field = array_search($biblio_field, $fmap)) ? $en8_field : '';
}

function _endnote8_type_map($bibliotype) {
  static $map = array();
  if (empty($map)) {
    $map = biblio_get_map('type_map', 'endnote8');
  }
  return ($en8_type = array_search($bibliotype, $map)) ? $en8_type : 13; //return the biblio type or 129 (Misc) if type not found
}

