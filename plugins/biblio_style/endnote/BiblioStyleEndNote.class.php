<?php

/**
 * @file
 * EndNote tagged biblio style.
 */

class BiblioStyleEndNote extends BiblioStyleBase {

  public function settingsForm() {
    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#required' => TRUE,
      '#options' => array(
        'tagged' => t('Tagged'),
        'xml' => t('XML'),
      ),
      '#default_value' => 'tagged',
    );

    return $form;
  }

  /**
   * @todo: Add import options defaults.
   */
  public function import($data, $options = array()) {
    $options += array(
      'type' => 'tagged',
    );
    if ($options['type'] == 'tagged') {
      return $this->importTagged($data, $options);
    }
    elseif ($options['type'] == 'xml') {
      return $this->importXML($data, $options);
    }
  }

  public function importTagged($data, $options = array()) {
    $biblios = array();

    $data = str_replace("\r\n", "\n", $data);
    $data = explode("\n", $data);

    foreach ($data as $row) {
      if (empty($row)) {
        // Empty line.
        continue;
      }
      $tag = substr($row, 0, 2);
      $value = substr($row, 3);

      if ($tag == '%0') {
        $type = strtolower(str_replace(array(' ', '-'), '_', $value));

        $biblio = biblio_create($type);
        $wrapper = entity_metadata_wrapper('biblio', $biblio);

        continue;
      }

      $map = $this->getMapping();
      if (empty($map[$tag])) {
        continue;
      }

      $method = $map[$tag]['import_method'];
      $this->{$method}($wrapper, $tag, $value);
    }

    // @todo: Check md5.
    $wrapper->save();
    $biblios[] = $biblio;
    return $biblios;
  }

  public function importXML($data, $options = array()) {
    require_once drupal_get_path('module', 'biblio') . '/plugins/biblio_style/endnote/BiblioStyleEndNote.class.php';

    $parser = new EndNoteXMLParser;
  }

  private function importEntryGeneric($wrapper, $tag, $value) {
    $map = $this->getMapping();
    $key = $map[$tag]['property'];
    $wrapper->{$key}->set($value);
  }

  /**
   * Create Biblio Contributor entities.
   */
  private function importEntryContributors($wrapper, $tag, $value) {
    switch ($tag) {
      case '%A':
        $type = 'author';
        break;

      case '%E':
        $type = 'editor';
        break;

      case '%Y':
        // @todo: Find the role.
        break;

      case '%?':
        // @todo: Find the role.
        break;
    }

    $biblio = $wrapper->value();

    // @todo: Add $this->getBiblioContributorsFromNames() to get
    // new $biblio_contributors or existing.

    // split names.
    $names = preg_split("/(and|&)/i", trim($value));
    foreach ($names as $name) {
      // Try to extract the given and family name.
      // @todo: Fix this preg_split.
      $sub_name = preg_split("/{|}/i", $name);
      $values = array('given' =>$sub_name[0]);
      if (!empty($sub_name[1])) {
        $values['family'] = $sub_name[1];
      }

      $biblio_contributor = biblio_contributor_create($values);
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

  public function getMapping() {
    $return = array(
      '%A' => array('import_method' => 'importEntryContributors'),
      '%B' => array('property' => 'biblio_secondary_title'),
      '%C' => array('property' => 'biblio_place_published'),
      '%D' => array('property' => 'biblio_year'),
      '%E' => array('import_method' => 'importEntryContributors'),
      '%F' => array('property' => 'biblio_label'),
      '%G' => array('property' => 'language'),
      '%I' => array('property' => 'biblio_publisher'),
      '%J' => array('property' => 'biblio_secondary_title'),
      '%K' => array('property' => 'biblio_keywords'),
      '%L' => array('property' => 'biblio_call_number'),
      '%M' => array('property' => 'biblio_accession_number'),
      '%N' => array('property' => 'biblio_issue'),
      '%P' => array('property' => 'biblio_pages'),
      '%R' => array('property' => 'biblio_doi'),
      '%S' => array('property' => 'biblio_tertiary_title'),
      '%T' => array('property' => 'title'),
      '%U' => array('property' => 'biblio_url'),
      '%V' => array('property' => 'biblio_volume'),
      '%X' => array('property' => 'biblio_abstract'),
      '%Y' => array('import_method' => 'importEntryContributors'),
      '%X' => array('property' => 'biblio_notes'),
      '%1' => array('property' => 'biblio_custom1'),
      '%2' => array('property' => 'biblio_custom2'),
      '%3' => array('property' => 'biblio_custom3'),
      '%4' => array('property' => 'biblio_custom4'),
      '%#' => array('property' => 'biblio_custom5'),
      '%$' => array('property' => 'biblio_custom6'),
      '%]' => array('property' => 'biblio_custom7'),
      '%6' => array('property' => 'biblio_number_of_volumes'),
      '%7' => array('property' => 'biblio_edition'),
      '%8' => array('property' => 'biblio_date'),
      '%9' => array('property' => 'biblio_type_of_work'),
      '%?' => array('import_method' => 'importEntryContributors'),
      '%@' => array('property' => 'biblio_isbn'),
      '%<' => array('property' => 'biblio_research_notes'),
      '%!' => array('property' => 'biblio_short_title'),
      '%&' => array('property' => 'biblio_section'),
      '%(' => array('property' => 'biblio_original_publication'),
      '%)' => array('property' => 'biblio_reprint_edition'),
      '%*' => array('property' => ''),
      '%+' => array('property' => ''),
    );

    // Assign default import method.
    foreach ($return as $key => $value) {
      if (empty($value['import_method'])) {
        $return[$key]['import_method'] = 'importEntryGeneric';
      }
    }

    return $return;

  }
}
