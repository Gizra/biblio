<?php

/**
 * @file
 * EndNote XML biblio style.
 */

class BiblioStyleEndNoteXML8 extends BiblioStyleEndNote {

  // @todo: Remove.
  public $biblio = NULL;

  /**
   * Import XML.
   */
  public function import($data, $options = array()) {
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

    // @todo: Check md5.
    $wrapper->save();
    $biblios[] = $biblio;
    return array(
      'new' => $biblios,
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

  public function characterData($parser, $data) {
    $map = $this->getMapping();

    $element = $this->element;

    if (!empty($map['field'][$element]['import_method'])) {
      $method = $map['field'][$element]['import_method'];
      $property = $map['field'][$element]['property'];

      // Prepare the data by striping any tags or white space.
      $data = explode("\n", $data);
      foreach ($data as $key => $value) {
        $data[$key] = trim(htmlspecialchars_decode($value));
      }
      $data = implode('', $data);

      $this->{$method}($this->wrapper, $property, $data);
    }

    return;

    switch ($element) {
      //Author information
      case 'author' :
        $this->contributors[$this->contrib_count]['name'] .= $data;
        break;
      case 'keyword' :
        $this->biblio->biblio_keywords[$this->keyword_count] .= $data;
        break;
      case 'date' :
        switch ($this->dates) {
          case 'pub-dates' :
            $this->biblio->biblio_date .= $data;
            break;
          case 'copyright-dates' :
            break;
        }
        break;
      case 'urls' :
      case 'url' :
        switch ($this->urls) {
          case 'web-urls' :
            $this->biblio->biblio_url .= $data;
            break;
          case 'pdf-urls' :
          case 'text-urls' :
          case 'image-urls' :
            break;
          case 'related-urls' :
        }
        break;
    }
  }

  /**
   * Generic import entry.
   */
  public function importEntryGeneric(EntityMetadataWrapper $wrapper, $property, $data) {
    if (!$data) {
      // No data given, it might have been a carriage return that was striped.
      return;
    }

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
   */
  public function importEntryYear(EntityMetadataWrapper $wrapper, $property, $data) {
    if (!$data) {
      // No data given, it might have been a carriage return that was striped.
      return;
    }

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


  public function field_map() {}

  public function type_map() {}


  /**
   * Render tagged.
   */
  public function render($options = array(), $langcode = NULL) {

  }

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
        'pub-dates' => array('property' => 'biblio_date'),
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
        'volume' => array('property' => 'biblio_volume'),
        'work-type' => array('property' => 'biblio_type_of_work'),
        'year' => array(
          'property' => 'biblio_year',
          'import_method' => 'importEntryYear',
        ),
      ),
    );


    // Add default values.
    foreach (array_keys($return['field']) as $key) {
      $return['field'][$key] += array(
        'import_method' => 'importEntryGeneric',
      );
    }

    return $return;
  }
}
