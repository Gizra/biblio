<?php

/**
 * @file
 * PubMed style.
 */

class BiblioStylePubmed extends BiblioStyleBase {

  /**
   * Import PubMed entries.
   *
   * @todo: Deal with duplication.
   *
   * @param $data
   * @param string $type
   * @return array
   */
  public function import($data, $type = 'text') {
    $xml = $data;

    $pubmed = new BiblioEntrezPubmedArticle();

    // Array of Biblios.
    $biblios = array();

    foreach ($xml->xpath('//PubmedArticle') as $article) {
      if (!$biblio = $pubmed->setArticle($article)->getBiblioAsObject()) {
        continue;
      }

      $biblios[] = $biblio;
    }

    return $biblios;


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
        $term = taxonomy_get_term_by_name(ucfirst($type), 'biblio_roles');
        $term = reset($term);

        $collection_wrapper->biblio_contributor_role->set($term);

        $collection_wrapper->save();
      }
    }
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
}