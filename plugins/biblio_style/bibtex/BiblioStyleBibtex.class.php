<?php

/**
 * @file
 * BibTeX style.
 */

class BiblioStyleBibtex extends BiblioStyleBase {

  /**
   * Import BibTeX entries.
   *
   * @todo: Deal with duplication.
   *
   * @param $data
   * @param string $type
   * @return array
   */
  public function import($data, $type = 'text') {
    $bibtex = new PARSEENTRIES();

    if ($type == 'file') {
      $bibtex->openBib($data);
    }
    else {
      $bibtex->loadBibtexString($data);
    }

    $bibtex->extractEntries();

    if (!$bibtex->count) {
      return;
    }

    $entries = $bibtex->getEntries();

    $map = $this->getMapping();

    // Array of Biblios.
    $biblios = array();

    foreach ($entries as $entry) {

      // @todo: Why does the original return a number?
      $biblio = biblio_create(strtolower($entry['bibtexEntryType']));

      $wrapper = entity_metadata_wrapper('biblio', $biblio);

      foreach (array_keys($map['field']) as $key) {
        if (in_array($key, array('author', 'editor'))) {
          continue;
        }
        $this->importEntry($wrapper, $key, $entry);
      }

      $this->ImportEntryContributors($wrapper, $entry);

      // @todo: Check if the Biblio doesn't already exist, and if so, load it.
      $wrapper->save();

      $biblios[] = $wrapper->value();

      /*
      $node = new stdClass();
      $node->biblio_contributors = array();
      switch ($entry['bibtexEntryType']) {
        case 'mastersthesis':
          $node->biblio_type_of_work = 'masters';
          break;
        case 'phdthesis':
          $node->biblio_type_of_work = 'phd';
          break;
      }

      if (!empty($entry['keywords'])) {
        if (strpos($entry['keywords'], ';')) {
          $entry['keywords'] = str_replace(';', ',', $entry['keywords']);
        }
        $node->biblio_keywords = explode(',', $entry['keywords']);
      }

      $node->biblio_bibtex_md5      = md5(serialize($node));
      $node->biblio_import_type     = 'bibtex';

      if (!($dup = biblio_bibtex_check_md5($node->biblio_bibtex_md5))) {
        if ($save) {
          biblio_save_node($node, $terms, $batch, $session_id, $save);
          $nids[] = (!empty($node->nid))? $node->nid : NULL;
        }
        else { // return the whole node if we are not saveing to the DB (used for the paste function on the input form)
          $nids[] = $node;
        }
      }
      else {
        $dups[] = $dup;
      }

      */
    }
    return $biblios;
  }


  private function importEntry($wrapper, $key, $entry) {
    if (empty($entry[$key])) {
      return;
    }

    $map = $this->getMapping();
    $map = $map['field'];

    $property_name = $map[$key]['property'];

    biblio_create_field($property_name, $wrapper->type(), $wrapper->getBundle());

    if (!isset($wrapper->{$property_name})) {
      return;
    }

    $method = $map[$key]['import_method'];

    $value = $this->{$method}($key, $entry);

    // @todo: Make title writable.
    $wrapper_info = $wrapper->{$property_name}->info();
    if (empty($wrapper_info['setter callback'])) {
      return;
    }

    $wrapper->{$property_name}->set($value);
  }

  /**
   * Get the value of an entry.
   *
   * @param $key
   * @param $entry
   */
  private function getEntryValue($key, $entry) {
    return !empty($entry[$key]) ? $entry[$key] : NULL;
  }

  /**
   * Get the value of a publisher.
   *
   * @param $key
   * @param $entry
   */
  private function getEntryValuePublisher($key, $entry) {
    if (!empty($entry['organization'])) {
      return $entry['organization'];
    }

    if (!empty($entry['school'])) {
      return $entry['school'];
    }

    if (!empty($entry['institution'])) {
      return $entry['institution'];
    }

    return !empty($entry['publisher']) ? $entry['publisher'] : NULL;
  }

  /**
   * Get the value of a secondary title.
   */
  private function getEntryValueSecondaryTitle($key, $entry) {
    if (!empty($entry['series']) && empty($entry['booktitle'])) {
      return $entry['series'];
    }

    if (!empty($entry['booktitle'])) {
      return $entry['booktitle'];
    }

    return !empty($entry['journal']) ? $entry['journal'] : NULL;
  }

  /**
   * Get the value of a tertiary title.
   */
  private function getEntryValueTertiaryTitle($key, $entry) {
    return !empty($entry['series']) && !empty($entry['booktitle']) ? $entry['series'] : NULL;
  }

  /**
   * Create Biblio Contributor entities.
   */
  private function ImportEntryContributors($wrapper, $entry) {
    foreach (array('author', 'editor') as $type) {
      if (empty($entry[$type])) {
        continue;
      }

      $biblio = $wrapper->value();

      // split names.
      $names = preg_split("/(and|&)/i", trim($entry[$type]));
      foreach ($names as $name) {
        // Try to extract the given and family name.
        // @todo: Fix this preg_split.
        $sub_name = preg_split("/{|}/i", $name);

        $biblio_contributor = biblio_contributor_create(array('given' =>$sub_name[0], 'family' => $sub_name[1]));
        $biblio_contributor->save();

        // Create contributors field collections.
        $field_collection = entity_create('field_collection_item', array('field_name' => 'contributor_field_collection'));
        $field_collection->setHostEntity('biblio', $biblio);
        $collection_wrapper = entity_metadata_wrapper('field_collection_item', $field_collection);
        $collection_wrapper->biblio_contributor->set($biblio_contributor);

        // @todo: Add reference to correct term.
        $collection_wrapper->save();
      }
    }
  }

  public function render($options = array(), $langcode = NULL) {
    // We clone the biblio, as we might change the values.
    $biblio = clone $this->biblio;
    $wrapper = entity_metadata_wrapper('biblio', $biblio);

    $output = '';
    $journal = $series = $booktitle = $school = $organization = $institution = NULL;
    $type = $this->biblio->type;

    switch ($type) {
      case 100:
        $series = $wrapper->biblio_secondary_title->value();
        $organization = $wrapper->biblio_publisher->value();
        break;

      case 101:
      case 103:
        $booktitle = $wrapper->biblio_secondary_title->value();
        $organization = $wrapper->biblio_publisher->value();
        $series = $wrapper->biblio_tertiary_title->value();
        break;

      case 108:
        $school = $wrapper->biblio_publisher->value();
        $biblio->biblio_publisher->set(NULL);
        if (strpos($wrapper->biblio_type_of_work->value(), 'masters') === TRUE) {
          $type = 'mastersthesis';
        }
        break;

      case 109:
        $institution  = $wrapper->biblio_publisher->value();
        $biblio->biblio_publisher->set(NULL);
        break;

      case 102:
      default:
        $journal = $wrapper->biblio_secondary_title->value();
        break;
    }

    // @todo: Use the human name instead of ucfirst()?
    $output .= '@' . ucfirst($type). '{';
    $output .= $this->formatEntry('bibtexCitation', NULL, FALSE);
    $output .= $this->formatEntry('title');
    $output .= $this->formatEntry('journal', $journal);
    $output .= $this->formatEntry('booktitle', $booktitle);
    $output .= $this->formatEntry('series', $series);
    $output .= $this->formatEntry('volume');
    $output .= $this->formatEntry('number');
    $output .= $this->formatEntry('year');
    $output .= $this->formatEntry('note');
    $output .= $this->formatEntry('month');
    $output .= $this->formatEntry('pages');
    $output .= $this->formatEntry('publisher');
    $output .= $this->formatEntry('school', $school);
    $output .= $this->formatEntry('organization', $organization);
    $output .= $this->formatEntry('institution', $institution);
    $output .= $this->formatEntry('type');
    $output .= $this->formatEntry('edition');
    $output .= $this->formatEntry('chapter');
    $output .= $this->formatEntry('address');
    $output .= $this->formatEntry('abstract');
    $output .= $this->formatEntry('keywords');
    $output .= $this->formatEntry('isbn');
    $output .= $this->formatEntry('issn');
    $output .= $this->formatEntry('doi');
    $output .= $this->formatEntry('url');
    $output .= $this->formatEntry('attachments');
    $output .= $this->formatEntry('author');
    $output .= $this->formatEntry('editor');
    $output .= "\n}\n";

    // Convert any special characters to the latex equivalents.
    $converter = new PARSEENTRIES();
    $output = $converter->searchReplaceText($this->getTranstab(), $output, FALSE);

    return $output;
  }

  /**
   * Format an entry.
   *
   * @param $key
   *   The BibTeX key name.
   * @param $value
   *   Optional; The value to format. If empty, try to get it from the Biblio
   *   entity. Defaults to NULL.
   * @return string
   */
  private function formatEntry($key, $value = NULL, $use_key = TRUE) {
    if (empty($value)) {
      $map = $this->getMapping();
      $map = $map['field'];

      if (empty($map[$key])) {
        return;
      }

      // @todo: Cache the wrapper.
      $wrapper = entity_metadata_wrapper('biblio', $this->biblio);

      $property_name = $map[$key]['property'];
      if (!isset($wrapper->{$property_name})) {
        return;
      }

      $method = $map[$key]['method'];

      if (!$value = $this->{$method}($wrapper, $property_name)) {
        return;
      }
    }

    $first_entry = &drupal_static(__METHOD__, array());

    $output = isset($first_entry[$this->biblio->bid]) ? ",\n\t" : '';

    // If we reached here, it means we have a first entity, so we can turn off
    // this flag.
    $first_entry[$this->biblio->bid] = FALSE;

    if ($use_key) {
      return $output . $key . ' = {'. $value . '}';
    }

    return $output . $value;
  }

  /**
   * Generic format entry.
   *
   * @param $wrapper
   * @param $property_name
   */
  private function formatEntryGeneric($wrapper, $property_name) {
    return $wrapper->{$property_name}->value();
  }

  /**
   * Taxonomy term format entry.
   *
   * @param $wrapper
   * @param $property_name
   */
  private function formatEntryTaxonomyTerms($wrapper, $property_name) {
    if (!$terms = $wrapper->{$property_name}->value()) {
      return;
    }

    $terms = is_array($terms) ? $terms : array($terms);
    $values = array();
    foreach ($terms as $term) {
      $values[] = $term->name;
    }

    return implode(', ', $values);
  }

  /**
   * File format entry.
   *
   * @param $wrapper
   * @param $property_name
   */
  private function formatEntryFiles($wrapper, $property_name) {
    if (!user_access('view uploaded files')) {
      return;
    }

    if (!$files =  $wrapper->{$property_name}->value()) {
      return;
    }

    $url = array();
    $files = !isset($files['fid']) ? $files : array($files);
    foreach ($files as $file) {
      $url[] = file_create_url($file['uri']);
    }

    return implode(' , ', $url);
  }

  /**
   * Author contributor format entry.
   *
   * @param $wrapper
   * @param $property_name
   */
  private function formatEntryContributorAuthor($wrapper, $property_name) {
    return $this->formatEntryContributor($wrapper, $property_name, 'author');
  }

  /**
   * Editor contributor format entry.
   *
   * @param $wrapper
   * @param $property_name
   */
  private function formatEntryContributorEditor($wrapper, $property_name) {
    return $this->formatEntryContributor($wrapper, $property_name, 'editor');
  }

  /**
   * Helper function to get contributors name.
   *
   * @param $wrapper
   * @param $property_name
   * @param $role
   * @return string
   */
  private function formatEntryContributor($wrapper, $property_name, $role) {
    if (!$wrapper->{$property_name}->value()) {
      return;
    }

    $names = array();
    foreach ($wrapper->{$property_name} as $sub_wrapper) {
      if (strtolower($sub_wrapper->biblio_contributor_role->label()) != $role) {
        continue;
      }

      $names[] = $sub_wrapper->biblio_contributor->label();
    }

    return implode(' and ', $names);
  }

  /**
   * Mapping of Biblio and BibTeX.
   *
   * - type: Array with the Biblio type as key, and the BibTeX type as the
   *   value.
   * - field: Array with field mapping, keyed by BibTeX name, and the Biblio
   *   field as the value.
   */
  public function getMapping() {
    $return  = array(
      'field' => array(
        'title' => array('property' => 'title'),
        'volume' => array('property' => 'biblio_volume'),
        'number' => array('property' => 'biblio_number'),
        'year' => array('property' => 'biblio_year'),
        'note' => array('property' => 'biblio_notes'),
        'month' => array('property' => 'biblio_date'),
        'pages' => array('property' => 'biblio_pages'),
        'publisher' => array(
          'property' => 'biblio_publisher',
          'import_method' => 'getEntryValuePublisher',
        ),
        'edition' => array('property' => 'biblio_edition'),
        'chapter' => array('property' => 'biblio_section'),
        'address' => array('property' => 'biblio_place_published'),
        'abstract' => array('property' => 'biblio_abstract'),
        'isbn' => array('property' => 'biblio_isbn'),
        'issn' => array('property' => 'biblio_issn'),
        'doi' => array('property' => 'biblio_doi'),
        // @todo: Is this the Biblio URL?
        'url' => array('property' => 'biblio_url'),

        'keywords' => array('property' => 'biblio_keywords', 'method' => 'formatEntryTaxonomyTerms'),

        // @todo: Use bilbio_file instead.
        'attachments' => array('property' => 'biblio_image', 'method' => 'formatEntryFiles'),

        'author' => array(
          'property' => 'contributor_field_collection',
          'method' => 'formatEntryContributorAuthor',
        ),
        'editor' => array(
          'property' => 'contributor_field_collection',
          'method' => 'formatEntryContributorEditor',
        ),

        // @todo: Special entry types?
        'bibtexEntryType' => array('property' => 'biblio_type_of_work'),
        'bibtexCitation' => array('property' => 'biblio_citekey'),

        // @todo: Is it ok to have this "fake" keys, or add this as property
        // to the array?
        // Keys used for import.
        'secondary_title' => array(
          'property' => 'biblio_secondary_title',
          'import_method' => 'getEntryValueSecondaryTitle',
        ),

        'tertiary_title' => array(
          'property' => 'biblio_tertiary_title',
          'import_method' => 'getEntryValueTertiaryTitle',
        ),
      ),
    );

    // Assign default method to format entry.
    foreach ($return['field'] as $key => $value) {
      if (empty($value['method'])) {
        $return['field'][$key]['method'] = 'formatEntryGeneric';
      }

      if (empty($value['import_method'])) {
        $return['field'][$key]['import_method'] = 'getEntryValue';
      }
    }

    return $return;
  }


  /**
   * This is a translation table for best-effort conversion from Unicode to
   * LaTeX/BibTeX entities. It contains a comprehensive list of substitution
   * strings for Unicode characters, which can be used with the 'T1' font
   * encoding.
   *
   * Uses commands from the 'textcomp' package. Characters that can't be matched
   * are converted to ASCII equivalents.
   *
   * Adopted from 'transtab' by Markus Kuhn (transtab.utf v1.8 2000-10-12
   * 11:01:28+01 mgk25 Exp);
   *
   * see <http://www.cl.cam.ac.uk/~mgk25/unicode.html> for more info about
   * Unicode and transtab.
   */
  private function getTranstab() {

    return array(
      "(?<!\\\\)#" => '$\\#$',
      "(?<!\\\\)%" => "\\%",
      "(?<!\\\\)&" => "\\&",
      "(?<!\\\\)'" => "{\\textquoteright}",
      "(?<!\\\\)`" => "{\\textquoteleft}",
      " " => "~",
      "¡" => "{\\textexclamdown}",
      "¢" => "{\\textcent}",
      "£" => "{\\textsterling}",
      "¥" => "{\\textyen}",
      "¦" => "{\\textbrokenbar}",
      "§" => "{\\textsection}",
      "¨" => "{\\textasciidieresis}",
      "©" => "{\\textcopyright}",
      "ª" => "{\\textordfeminine}",
      "«" => "{\\guillemotleft}",
      "¬" => "{\\textlnot}",
      "­" => "-",
      "®" => "{\\textregistered}",
      "¯" => "{\\textasciimacron}",
      "°" => "{\\textdegree}",
      "±" => "{\\textpm}",
      "²" => "{\\texttwosuperior}",
      "³" => "{\\textthreesuperior}",
      "´" => "{\\textasciiacute}",
      "µ" => "{\\textmu}",
      "¶" => "{\\textparagraph}",
      "·" => "{\\textperiodcentered}",
      "¸" => "{\\c\\ }",
      "¹" => "{\\textonesuperior}",
      "º" => "{\\textordmasculine}",
      "»" => "{\\guillemotright}",
      "¼" => "{\\textonequarter}",
      "½" => "{\\textonehalf}",
      "¾" => "{\\textthreequarters}",
      "¿" => "{\\textquestiondown}",
      "À" => "{\\`A}",
      "Á" => "{\\'A}",
      "Â" => "{\\^A}",
      "Ã" => "{\\~A}",
      "Ä" => "{\\\"A}",
      "Å" => "{\\r A}",
      "Æ" => "{\\AE}",
      "Ç" => "{\\c C}",
      "È" => "{\\`E}",
      "É" => "{\\'E}",
      "Ê" => "{\\^E}",
      "Ë" => "{\\\"E}",
      "Ì" => "{\\`I}",
      "Í" => "{\\'I}",
      "Î" => "{\\^I}",
      "Ï" => "{\\\"I}",
      "Ð" => "{\\DH}",
      "Ñ" => "{\\~N}",
      "Ò" => "{\\`O}",
      "Ó" => "{\\'O}",
      "Ô" => "{\\^O}",
      "Õ" => "{\\~O}",
      "Ö" => "{\\\"O}",
      "×" => "{\\texttimes}",
      "Ø" => "{\\O}",
      "Ù" => "{\\`U}",
      "Ú" => "{\\'U}",
      "Û" => "{\\^U}",
      "Ü" => "{\\\"U}",
      "Ý" => "{\\'Y}",
      "Þ" => "{\\TH}",
      "ß" => "{\\ss}",
      "à" => "{\\`a}",
      "á" => "{\\'a}",
      "â" => "{\\^a}",
      "ã" => "{\\~a}",
      "ä" => "{\\\"a}",
      "å" => "{\\r a}",
      "æ" => "{\\ae}",
      "ç" => "{\\c c}",
      "è" => "{\\`e}",
      "é" => "{\\'e}",
      "ê" => "{\\^e}",
      "ë" => "{\\\"e}",
      "ì" => "{\\`\\i}",
      "í" => "{\\'\\i}",
      "î" => "{\\^\\i}",
      "ï" => "{\\\"\\i}",
      "ð" => "{\\dh}",
      "ñ" => "{\\~n}",
      "ò" => "{\\`o}",
      "ó" => "{\\'o}",
      "ô" => "{\\^o}",
      "õ" => "{\\~o}",
      "ö" => "{\\\"o}",
      "÷" => "{\\textdiv}",
      "ø" => "{\\o}",
      "ù" => "{\\`u}",
      "ú" => "{\\'u}",
      "û" => "{\\^u}",
      "ü" => "{\\\"u}",
      "ý" => "{\\'y}",
      "þ" => "{\\th}",
      "ÿ" => "{\\\"y}",
      "Ā" => "A",
      "ā" => "{\\={a}}",
      "Ă" => "{\\u A}",
      "ă" => "{\\u a}",
      "Ą" => "{\\k A}",
      "ą" => "{\\k a}",
      "Ć" => "{\\'C}",
      "ć" => "{\\'c}",
      "Ĉ" => "Ch",
      "ĉ" => "ch",
      "Ċ" => "C",
      "ċ" => "c",
      "Č" => "{\\v C}",
      "č" => "{\\v c}",
      "Ď" => "{\\v D}",
      "ď" => "{\\v d}",
      "Đ" => "{\\DJ}",
      "đ" => "{\\dj}",
      "Ē" => "E",
      "ē" => "e",
      "Ĕ" => "E",
      "ĕ" => "e",
      "Ė" => "E",
      "ė" => "e",
      "Ę" => "{\\k E}",
      "ę" => "{\\k e}",
      "Ě" => "{\\v E}",
      "ě" => "{\\v e}",
      "Ĝ" => "Gh",
      "ĝ" => "gh",
      "Ğ" => "{\\u G}",
      "ğ" => "{\\u g}",
      "Ġ" => "G",
      "ġ" => "g",
      "Ģ" => "G",
      "ģ" => "g",
      "Ĥ" => "Hh",
      "ĥ" => "hh",
      "Ħ" => "H",
      "ħ" => "h",
      "Ĩ" => "I",
      "ĩ" => "i",
      "Ī" => "I",
      "ī" => "i",
      "Ĭ" => "I",
      "ĭ" => "i",
      "Į" => "I",
      "į" => "i",
      "İ" => "{\\.I}",
      "ı" => "{\\i}",
      "Ĳ" => "IJ",
      "ĳ" => "ij",
      "Ĵ" => "Jh",
      "ĵ" => "jh",
      "Ķ" => "K",
      "ķ" => "k",
      "ĸ" => "k",
      "Ĺ" => "{\\'L}",
      "ĺ" => "{\\'l}",
      "Ļ" => "L",
      "ļ" => "l",
      "Ľ" => "{\\v L}",
      "ľ" => "{\\v l}",
      "Ŀ" => "L·",
      "ŀ" => "l·",
      "Ł" => "{\\L}",
      "ł" => "{\\l}",
      "Ń" => "{\\'N}",
      "ń" => "{\\'n}",
      "Ņ" => "N",
      "ņ" => "n",
      "Ň" => "{\\v N}",
      "ň" => "{\\v n}",
      "ŉ" => "'n",
      "Ŋ" => "{\\NG}",
      "ŋ" => "{\\ng}",
      "Ō" => "O",
      "ō" => "o",
      "Ŏ" => "O",
      "ŏ" => "o",
      "Ő" => "{\\H O}",
      "ő" => "{\\H o}",
      "Œ" => "{\\OE}",
      "œ" => "{\\oe}",
      "Ŕ" => "{\\'R}",
      "ŕ" => "{\\'r}",
      "Ŗ" => "R",
      "ŗ" => "r",
      "Ř" => "{\\v R}",
      "ř" => "{\\v r}",
      "Ś" => "{\\'S}",
      "ś" => "{\\'s}",
      "Ŝ" => "Sh",
      "ŝ" => "sh",
      "Ş" => "{\\c S}",
      "ş" => "{\\c s}",
      "Š" => "{\\v S}",
      "š" => "{\\v s}",
      "Ţ" => "{\\c T}",
      "ţ" => "{\\c t}",
      "Ť" => "{\\v T}",
      "ť" => "{\\v t}",
      "Ŧ" => "T",
      "ŧ" => "t",
      "Ũ" => "U",
      "ũ" => "u",
      "Ū" => "U",
      "ū" => "u",
      "Ŭ" => "U",
      "ŭ" => "u",
      "Ů" => "{\\r U}",
      "ů" => "{\\r u}",
      "Ű" => "{\\H U}",
      "ű" => "{\\H u}",
      "Ų" => "U",
      "ų" => "u",
      "Ŵ" => "W",
      "ŵ" => "w",
      "Ŷ" => "Y",
      "ŷ" => "y",
      "Ÿ" => "{\\\"Y}",
      "Ź" => "{\\'Z}",
      "ź" => "{\\'z}",
      "Ż" => "{\\.Z}",
      "ż" => "{\\.z}",
      "Ž" => "{\\v Z}",
      "ž" => "{\\v z}",
      "ſ" => "s",
      "ƒ" => "{\\textflorin}",
      "Ș" => "S",
      "ș" => "s",
      "Ț" => "T",
      "ț" => "t",
      "ʹ" => "'",
      "ʻ" => "'",
      "ʼ" => "'",
      "ʽ" => "'",
      "ˆ" => "{\\textasciicircum}",
      "ˈ" => "'",
      "ˉ" => "-",
      "ˌ" => ",",
      "ː" => ":",
      "˚" => "o",
      "˜" => "\\~{}",
      "˝" => "{\\textacutedbl}",
      "ʹ" => "'",
      "͵" => ",",
      ";" => ";",
      "Ḃ" => "B",
      "ḃ" => "b",
      "Ḋ" => "D",
      "ḋ" => "d",
      "Ḟ" => "F",
      "ḟ" => "f",
      "Ṁ" => "M",
      "ṁ" => "m",
      "Ṗ" => "P",
      "ṗ" => "p",
      "Ṡ" => "S",
      "ṡ" => "s",
      "Ṫ" => "T",
      "ṫ" => "t",
      "Ẁ" => "W",
      "ẁ" => "w",
      "Ẃ" => "W",
      "ẃ" => "w",
      "Ẅ" => "W",
      "ẅ" => "w",
      "Ỳ" => "Y",
      "ỳ" => "y",
      " " => " ",
      " " => "  ",
      " " => " ",
      " " => "  ",
      " " => " ",
      " " => " ",
      " " => " ",
      " " => " ",
      " " => " ",
      " " => " ",
      "‐" => "-",
      "‑" => "-",
      "‒" => "-",
      "–" => "{\\textendash}",
      "—" => "{\\textemdash}",
      "―" => "--",
      "‖" => "{\\textbardbl}",
      "‗" => "{\\textunderscore}",
      "‘" => "{\\textquoteleft}",
      "’" => "{\\textquoteright}",
      "‚" => "{\\quotesinglbase}",
      "‛" => "'",
      "“" => "{\\textquotedblleft}",
      "”" => "{\\textquotedblright}",
      "„" => "{\\quotedblbase}",
      "‟" => "\"",
      "†" => "{\\textdagger}",
      "‡" => "{\\textdaggerdbl}",
      "•" => "{\\textbullet}",
      "‣" => ">",
      "․" => ".",
      "‥" => "..",
      "…" => "{\\textellipsis}",
      "‧" => "-",
      " " => " ",
      "‰" => "{\\textperthousand}",
      "′" => "'",
      "″" => "\"",
      "‴" => "'''",
      "‵" => "`",
      "‶" => "``",
      "‷" => "```",
      "‹" => "{\\guilsinglleft}",
      "›" => "{\\guilsinglright}",
      "‼" => "!!",
      "‾" => "-",
      "⁃" => "-",
      "⁄" => "{\\textfractionsolidus}",
      "⁈" => "?!",
      "⁉" => "!?",
      "⁊" => "7",
      "⁰" => '$^{0}$',
      "⁴" => '$^{4}$',
      "⁵" => '$^{5}$',
      "⁶" => '$^{6}$',
      "⁷" => '$^{7}$',
      "⁸" => '$^{8}$',
      "⁹" => '$^{9}$',
      "⁺" => '$^{+}$',
      "⁻" => '$^{-}$',
      "⁼" => '$^{=}$',
      "⁽" => '$^{(}$',
      "⁾" => '$^{)}$',
      "ⁿ" => '$^{n}$',
      "₀" => '$_{0}$',
      "₁" => '$_{1}$',
      "₂" => '$_{2}$',
      "₃" => '$_{3}$',
      "₄" => '$_{4}$',
      "₅" => '$_{5}$',
      "₆" => '$_{6}$',
      "₇" => '$_{7}$',
      "₈" => '$_{8}$',
      "₉" => '$_{9}$',
      "₊" => '$_{+}$',
      "₋" => '$_{-}$',
      "₌" => '$_{=}$',
      "₍" => '$_{(}$',
      "₎" => '$_{)}$',
      "€" => "{\\texteuro}",
      "℀" => "a/c",
      "℁" => "a/s",
      "℃" => "{\\textcelsius}",
      "℅" => "c/o",
      "℆" => "c/u",
      "℉" => "F",
      "ℓ" => "l",
      "№" => "{\\textnumero}",
      "℗" => "{\\textcircledP}",
      "℠" => "{\\textservicemark}",
      "℡" => "TEL",
      "™" => "{\\texttrademark}",
      "Ω" => "{\\textohm}",
      "K" => "K",
      "Å" => "A",
      "℮" => "{\\textestimated}",
      "⅓" => " 1/3",
      "⅔" => " 2/3",
      "⅕" => " 1/5",
      "⅖" => " 2/5",
      "⅗" => " 3/5",
      "⅘" => " 4/5",
      "⅙" => " 1/6",
      "⅚" => " 5/6",
      "⅛" => " 1/8",
      "⅜" => " 3/8",
      "⅝" => " 5/8",
      "⅞" => " 7/8",
      "⅟" => " 1/",
      "Ⅰ" => "I",
      "Ⅱ" => "II",
      "Ⅲ" => "III",
      "Ⅳ" => "IV",
      "Ⅴ" => "V",
      "Ⅵ" => "VI",
      "Ⅶ" => "VII",
      "Ⅷ" => "VIII",
      "Ⅸ" => "IX",
      "Ⅹ" => "X",
      "Ⅺ" => "XI",
      "Ⅻ" => "XII",
      "Ⅼ" => "L",
      "Ⅽ" => "C",
      "Ⅾ" => "D",
      "Ⅿ" => "M",
      "ⅰ" => "i",
      "ⅱ" => "ii",
      "ⅲ" => "iii",
      "ⅳ" => "iv",
      "ⅴ" => "v",
      "ⅵ" => "vi",
      "ⅶ" => "vii",
      "ⅷ" => "viii",
      "ⅸ" => "ix",
      "ⅹ" => "x",
      "ⅺ" => "xi",
      "ⅻ" => "xii",
      "ⅼ" => "l",
      "ⅽ" => "c",
      "ⅾ" => "d",
      "ⅿ" => "m",
      "←" => "{\\textleftarrow}",
      "↑" => "{\\textuparrow}",
      "→" => "{\\textrightarrow}",
      "↓" => "{\\textdownarrow}",
      "↔" => "<->",
      "⇐" => "<=",
      "⇒" => "=>",
      "⇔" => "<=>",
      "−" => "-",
      "∕" => "/",
      "∖" => "\\",
      "∗" => "*",
      "∘" => "o",
      "∙" => ".",
      "∞" => '$\\infty$',
      "∣" => "|",
      "∥" => "||",
      "∶" => ":",
      "∼" => "\\~{}",
      "≠" => "/=",
      "≡" => "=",
      "≤" => "<=",
      "≥" => ">=",
      "≪" => "<<",
      "≫" => ">>",
      "⊕" => "(+)",
      "⊖" => "(-)",
      "⊗" => "(x)",
      "⊘" => "(/)",
      "⊢" => "|-",
      "⊣" => "-|",
      "⊦" => "|-",
      "⊧" => "|=",
      "⊨" => "|=",
      "⊩" => "||-",
      "⋅" => ".",
      "⋆" => "*",
      "⋕" => '$\\#$',
      "⋘" => "<<<",
      "⋙" => ">>>",
      "⋯" => "...",
      "〈" => "{\\textlangle}",
      "〉" => "{\\textrangle}",
      "␀" => "NUL",
      "␁" => "SOH",
      "␂" => "STX",
      "␃" => "ETX",
      "␄" => "EOT",
      "␅" => "ENQ",
      "␆" => "ACK",
      "␇" => "BEL",
      "␈" => "BS",
      "␉" => "HT",
      "␊" => "LF",
      "␋" => "VT",
      "␌" => "FF",
      "␍" => "CR",
      "␎" => "SO",
      "␏" => "SI",
      "␐" => "DLE",
      "␑" => "DC1",
      "␒" => "DC2",
      "␓" => "DC3",
      "␔" => "DC4",
      "␕" => "NAK",
      "␖" => "SYN",
      "␗" => "ETB",
      "␘" => "CAN",
      "␙" => "EM",
      "␚" => "SUB",
      "␛" => "ESC",
      "␜" => "FS",
      "␝" => "GS",
      "␞" => "RS",
      "␟" => "US",
      "␠" => "SP",
      "␡" => "DEL",
      "␣" => "{\\textvisiblespace}",
      "␤" => "NL",
      "␥" => "///",
      "␦" => "?",
      "①" => "(1)",
      "②" => "(2)",
      "③" => "(3)",
      "④" => "(4)",
      "⑤" => "(5)",
      "⑥" => "(6)",
      "⑦" => "(7)",
      "⑧" => "(8)",
      "⑨" => "(9)",
      "⑩" => "(10)",
      "⑪" => "(11)",
      "⑫" => "(12)",
      "⑬" => "(13)",
      "⑭" => "(14)",
      "⑮" => "(15)",
      "⑯" => "(16)",
      "⑰" => "(17)",
      "⑱" => "(18)",
      "⑲" => "(19)",
      "⑳" => "(20)",
      "⑴" => "(1)",
      "⑵" => "(2)",
      "⑶" => "(3)",
      "⑷" => "(4)",
      "⑸" => "(5)",
      "⑹" => "(6)",
      "⑺" => "(7)",
      "⑻" => "(8)",
      "⑼" => "(9)",
      "⑽" => "(10)",
      "⑾" => "(11)",
      "⑿" => "(12)",
      "⒀" => "(13)",
      "⒁" => "(14)",
      "⒂" => "(15)",
      "⒃" => "(16)",
      "⒄" => "(17)",
      "⒅" => "(18)",
      "⒆" => "(19)",
      "⒇" => "(20)",
      "⒈" => "1.",
      "⒉" => "2.",
      "⒊" => "3.",
      "⒋" => "4.",
      "⒌" => "5.",
      "⒍" => "6.",
      "⒎" => "7.",
      "⒏" => "8.",
      "⒐" => "9.",
      "⒑" => "10.",
      "⒒" => "11.",
      "⒓" => "12.",
      "⒔" => "13.",
      "⒕" => "14.",
      "⒖" => "15.",
      "⒗" => "16.",
      "⒘" => "17.",
      "⒙" => "18.",
      "⒚" => "19.",
      "⒛" => "20.",
      "⒜" => "(a)",
      "⒝" => "(b)",
      "⒞" => "(c)",
      "⒟" => "(d)",
      "⒠" => "(e)",
      "⒡" => "(f)",
      "⒢" => "(g)",
      "⒣" => "(h)",
      "⒤" => "(i)",
      "⒥" => "(j)",
      "⒦" => "(k)",
      "⒧" => "(l)",
      "⒨" => "(m)",
      "⒩" => "(n)",
      "⒪" => "(o)",
      "⒫" => "(p)",
      "⒬" => "(q)",
      "⒭" => "(r)",
      "⒮" => "(s)",
      "⒯" => "(t)",
      "⒰" => "(u)",
      "⒱" => "(v)",
      "⒲" => "(w)",
      "⒳" => "(x)",
      "⒴" => "(y)",
      "⒵" => "(z)",
      "Ⓐ" => "(A)",
      "Ⓑ" => "(B)",
      "Ⓒ" => "(C)",
      "Ⓓ" => "(D)",
      "Ⓔ" => "(E)",
      "Ⓕ" => "(F)",
      "Ⓖ" => "(G)",
      "Ⓗ" => "(H)",
      "Ⓘ" => "(I)",
      "Ⓙ" => "(J)",
      "Ⓚ" => "(K)",
      "Ⓛ" => "(L)",
      "Ⓜ" => "(M)",
      "Ⓝ" => "(N)",
      "Ⓞ" => "(O)",
      "Ⓟ" => "(P)",
      "Ⓠ" => "(Q)",
      "Ⓡ" => "(R)",
      "Ⓢ" => "(S)",
      "Ⓣ" => "(T)",
      "Ⓤ" => "(U)",
      "Ⓥ" => "(V)",
      "Ⓦ" => "(W)",
      "Ⓧ" => "(X)",
      "Ⓨ" => "(Y)",
      "Ⓩ" => "(Z)",
      "ⓐ" => "(a)",
      "ⓑ" => "(b)",
      "ⓒ" => "(c)",
      "ⓓ" => "(d)",
      "ⓔ" => "(e)",
      "ⓕ" => "(f)",
      "ⓖ" => "(g)",
      "ⓗ" => "(h)",
      "ⓘ" => "(i)",
      "ⓙ" => "(j)",
      "ⓚ" => "(k)",
      "ⓛ" => "(l)",
      "ⓜ" => "(m)",
      "ⓝ" => "(n)",
      "ⓞ" => "(o)",
      "ⓟ" => "(p)",
      "ⓠ" => "(q)",
      "ⓡ" => "(r)",
      "ⓢ" => "(s)",
      "ⓣ" => "(t)",
      "ⓤ" => "(u)",
      "ⓥ" => "(v)",
      "ⓦ" => "(w)",
      "ⓧ" => "(x)",
      "ⓨ" => "(y)",
      "ⓩ" => "(z)",
      "⓪" => "(0)",
      "─" => "-",
      "━" => "=",
      "│" => "|",
      "┃" => "|",
      "┄" => "-",
      "┅" => "=",
      "┆" => "|",
      "┇" => "|",
      "┈" => "-",
      "┉" => "=",
      "┊" => "|",
      "┋" => "|",
      "┌" => "+",
      "┍" => "+",
      "┎" => "+",
      "┏" => "+",
      "┐" => "+",
      "┑" => "+",
      "┒" => "+",
      "┓" => "+",
      "└" => "+",
      "┕" => "+",
      "┖" => "+",
      "┗" => "+",
      "┘" => "+",
      "┙" => "+",
      "┚" => "+",
      "┛" => "+",
      "├" => "+",
      "┝" => "+",
      "┞" => "+",
      "┟" => "+",
      "┠" => "+",
      "┡" => "+",
      "┢" => "+",
      "┣" => "+",
      "┤" => "+",
      "┥" => "+",
      "┦" => "+",
      "┧" => "+",
      "┨" => "+",
      "┩" => "+",
      "┪" => "+",
      "┫" => "+",
      "┬" => "+",
      "┭" => "+",
      "┮" => "+",
      "┯" => "+",
      "┰" => "+",
      "┱" => "+",
      "┲" => "+",
      "┳" => "+",
      "┴" => "+",
      "┵" => "+",
      "┶" => "+",
      "┷" => "+",
      "┸" => "+",
      "┹" => "+",
      "┺" => "+",
      "┻" => "+",
      "┼" => "+",
      "┽" => "+",
      "┾" => "+",
      "┿" => "+",
      "╀" => "+",
      "╁" => "+",
      "╂" => "+",
      "╃" => "+",
      "╄" => "+",
      "╅" => "+",
      "╆" => "+",
      "╇" => "+",
      "╈" => "+",
      "╉" => "+",
      "╊" => "+",
      "╋" => "+",
      "╌" => "-",
      "╍" => "=",
      "╎" => "|",
      "╏" => "|",
      "═" => "=",
      "║" => "|",
      "╒" => "+",
      "╓" => "+",
      "╔" => "+",
      "╕" => "+",
      "╖" => "+",
      "╗" => "+",
      "╘" => "+",
      "╙" => "+",
      "╚" => "+",
      "╛" => "+",
      "╜" => "+",
      "╝" => "+",
      "╞" => "+",
      "╟" => "+",
      "╠" => "+",
      "╡" => "+",
      "╢" => "+",
      "╣" => "+",
      "╤" => "+",
      "╥" => "+",
      "╦" => "+",
      "╧" => "+",
      "╨" => "+",
      "╩" => "+",
      "╪" => "+",
      "╫" => "+",
      "╬" => "+",
      "╭" => "+",
      "╮" => "+",
      "╯" => "+",
      "╰" => "+",
      "╱" => "/",
      "╲" => "\\",
      "╳" => "X",
      "╼" => "-",
      "╽" => "|",
      "╾" => "-",
      "╿" => "|",
      "○" => "o",
      "◦" => "{\\textopenbullet}",
      "★" => "*",
      "☆" => "*",
      "☒" => "X",
      "☓" => "X",
      "☹" => ":-(",
      "☺" => ":-)",
      "☻" => "(-:",
      "♭" => "b",
      "♯" => '$\\#$',
      "✁" => '$\\%<$',
      "✂" => '$\\%<$',
      "✃" => '$\\%<$',
      "✄" => '$\\%<$',
      "✌" => "V",
      "✓" => "v",
      "✔" => "V",
      "✕" => "x",
      "✖" => "x",
      "✗" => "X",
      "✘" => "X",
      "✙" => "+",
      "✚" => "+",
      "✛" => "+",
      "✜" => "+",
      "✝" => "+",
      "✞" => "+",
      "✟" => "+",
      "✠" => "+",
      "✡" => "*",
      "✢" => "+",
      "✣" => "+",
      "✤" => "+",
      "✥" => "+",
      "✦" => "+",
      "✧" => "+",
      "✩" => "*",
      "✪" => "*",
      "✫" => "*",
      "✬" => "*",
      "✭" => "*",
      "✮" => "*",
      "✯" => "*",
      "✰" => "*",
      "✱" => "*",
      "✲" => "*",
      "✳" => "*",
      "✴" => "*",
      "✵" => "*",
      "✶" => "*",
      "✷" => "*",
      "✸" => "*",
      "✹" => "*",
      "✺" => "*",
      "✻" => "*",
      "✼" => "*",
      "✽" => "*",
      "✾" => "*",
      "✿" => "*",
      "❀" => "*",
      "❁" => "*",
      "❂" => "*",
      "❃" => "*",
      "❄" => "*",
      "❅" => "*",
      "❆" => "*",
      "❇" => "*",
      "❈" => "*",
      "❉" => "*",
      "❊" => "*",
      "❋" => "*",
      "ﬀ" => "ff",
      "ﬁ" => "fi",
      "ﬂ" => "fl",
      "ﬃ" => "ffi",
      "ﬄ" => "ffl",
      "ﬅ" => "st",
      "ﬆ" => "st"
    );
  }
}