<?php

/**
 * @file
 * EndNote tagged biblio style.
 */

class BiblioStyleEndNoteTagged extends BiblioStyleEndNote implements BiblioStyleImportInterface {

  /**
   * @inheritdoc
   */
  public function importData($data, $options = array()) {
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
      if (empty($map['field'][$tag])) {
        continue;
      }

      $method = $map['field'][$tag]['import_method'];
      $this->{$method}($wrapper, $tag, $value);
    }

    // @todo: Check md5.
    $wrapper->save();
    $biblios['new'][] = $biblio;
    return $biblios;
  }

  private function importEntryGeneric($wrapper, $tag, $value) {
    $map = $this->getMapping();
    $key = $map['field'][$tag]['property'];
    $wrapper->{$key}->set($value);
  }

  /**
   * Create Biblio Contributor entities.
   */
  private function importEntryContributors($wrapper, $tag, $value) {
    // The role is in the map.
    $map = $this->getMapping();
    $role = $map['field'][$tag]['role'];

    $biblio = $wrapper->value();
    $this->addBiblioContributorsToCollection($biblio, $value, $role);
  }

  /**
   * @inheritdoc
   */
  public function render($options = array(), $langcode = NULL) {
    $output = array();

    // We clone the biblio, as we might change the values.
    $biblio = clone $this->biblio;
    $wrapper = entity_metadata_wrapper('biblio', $biblio);

    $type = biblio_types($biblio->type);

    $output[] = "%0 " . $type->name;

    $execute_once = array();

    $map = $this->getMapping();
    foreach ($map['field'] as $tag => $tag_info) {
      $method = $tag_info['render_method'];
      if ($tag_info['execute_once']) {
        $execute_once[$method] = $method;
        // Skip rendering contributors as we will do it in one step, to prevent
        // iterating over the same values over and over again.
        continue;
      }
      $this->{$method}($output, $wrapper, $tag);
    }

    foreach ($execute_once as $method) {
      // Render the contributors.
      $this->{$method}($output, $wrapper);
    }

    return implode("\n", $output);
  }

  /**
   * Generic entry render.
   *
   * @param array $output
   * @param EntityMetadataWrapper $wrapper
   * @param $tag
   */
  public function renderEntryGeneric(&$output = array(), EntityMetadataWrapper $wrapper, $tag) {
    $map = $this->getMapping();
    if (!$property = $map['field'][$tag]['property']) {
      return;
    }

    if (!isset($wrapper->{$property}) || !$value = $wrapper->{$property}->value()) {
      return;
    }

    $output[] = "{$tag} " . $value;
  }

  public function renderEntrySecondaryTitle(&$output = array(), EntityMetadataWrapper $wrapper) {
    if (!$value = $wrapper->biblio_secondary_title->value()) {
      return;
    }

    $biblio = $wrapper->value();
    $tag = $biblio->type == 'journal' ? '%J' : '%B';

    $output[] = $tag . ' ' . $value;
  }


  public function renderEntryKeywords(&$output = array(), EntityMetadataWrapper $wrapper, $tag) {
    foreach ($wrapper->biblio_keywords as $sub_wrapper) {
      $output[] = "%K " . $sub_wrapper->label();
    }
  }

  public function renderEntryFile(&$output = array(), EntityMetadataWrapper $wrapper, $tag) {
    if (!$file = $wrapper->biblio_pdf->value()) {
      return;
    }

    $output[] = "%> " . file_create_url($file['uri']);
  }

  public function renderEntryContributors(&$output = array(), EntityMetadataWrapper $wrapper) {
    if (!$values = $wrapper->contributor_field_collection->value()) {
      return;
    }
    $contrib_map = array();

    // Normalize map, to get array keyed by Biblio role and the EndNote tag as
    // the value.
    $map = $this->getMapping();
    foreach ($map['field'] as $tag => $tag_info) {
      if ($tag_info['render_method'] != 'renderEntryContributors') {
        continue;
      }

      $role = $tag_info['role'];
      $contrib_map[$role] = $tag;
    }

    foreach ($wrapper->contributor_field_collection as $sub_wrapper) {
      $role = $sub_wrapper->biblio_contributor_role->label();
      $contributor = $sub_wrapper->biblio_contributor->value();

      // If we can't map the type, assume it is an author.
      // @todo: Is this right?
      $tag = !empty($contrib_map[$role]) ? $contrib_map[$role] : '%A';
      $output[] = $tag . ' ' . $contributor->name;
    }
  }


  /**
   * @inheritdoc
   */
  public function getMapping() {
    $return = parent::getMapping();
    $return['type'] = array(
      'Artwork' => 'artwork',
      'Audiovisual Material' => 'audiovisual',
      'Bill' => 'bill',
      'Book' => 'book',
      'Book Section' => 'book_chapter',
      'Case' => 'case',
      'Chart or Table' => 'chart',
      'Classical Work' => 'classical',
      'Conference Paper' => 'conference_paper',
      'Conference Proceedings' => 'conference_proceedings',
      'Edited Book' => 'book',
      'Film or Broadcast' => 'film',
      'Generic' => 'miscellaneous',
      'Government Document' => 'government_report',
      'Hearing' => 'hearing',
      'Journal Article' => 'journal_article',
      'Legal Rule or Regulation' => 'legal_ruling',
      'Magazine Article' => 'magazine_article',
      'Manuscript' => 'manuscript',
      'Map' => 'map',
      'Newspaper Article' => 'newspaper_article',
      'Online Database' => 'database',
      'Patent' => 'patent',
      'Personal Communication' => 'personal',
      'Report' => 'report',
      'Statute' => 'statute',
      'Thesis' => 'thesis',
      'Unpublished Work' => 'unpublished',
      'Web Page' => 'web_article',
    );

    $return['field'] = array(
      '%A' => array(
        'import_method' => 'importEntryContributors',
        'render_method' => 'renderEntryContributors',
        'role' => 'Author',
        'execute_once' => TRUE,
      ),
      '%B' => array(
        'property' => 'biblio_secondary_title',
        'render_method' => 'renderEntrySecondaryTitle',
        'execute_once' => TRUE,
      ),
      '%C' => array('property' => 'biblio_place_published'),
      '%D' => array('property' => 'biblio_year'),
      '%E' => array(
        'import_method' => 'importEntryContributors',
        'render_method' => 'renderEntryContributors',
        'role' => 'Secondary Author',
        'execute_once' => TRUE,
      ),
      '%F' => array('property' => 'biblio_label'),
      '%G' => array('property' => 'biblio_language'),
      '%I' => array('property' => 'biblio_publisher'),
      '%J' => array(
        'property' => 'biblio_secondary_title',
        'render_method' => 'renderEntrySecondaryTitle',
        'execute_once' => TRUE,
      ),
      '%K' => array(
        'property' => 'biblio_keywords',
        'render_method' => 'renderEntryKeywords',
      ),
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
      '%Y' => array(
        'import_method' => 'importEntryContributors',
        'render_method' => 'renderEntryContributors',
        'role' => 'Tertiary Author',
        'execute_once' => TRUE,
      ),
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
      '%?' => array(
        'import_method' => 'importEntryContributors',
        'render_method' => 'renderEntryContributors',
        'role' => 'Subsidiary Author',
        'execute_once' => TRUE,
      ),
      '%@' => array('property' => 'biblio_isbn'),
      '%<' => array('property' => 'biblio_research_notes'),
      '%>' => array(
        'property' => 'biblio_pdf',
        // @todo: We can try and download the file.
        'import_method' => FALSE,
        'render_method' => 'renderEntryFile'
      ),
      '%!' => array('property' => 'biblio_short_title'),
      '%&' => array('property' => 'biblio_section'),
      '%(' => array('property' => 'biblio_original_publication'),
      '%)' => array('property' => 'biblio_reprint_edition'),
    );

    // Assign default import method.
    foreach ($return['field'] as $key => $value) {
      $return['field'][$key] += array(
        'import_method' => 'importEntryGeneric',
        'render_method' => 'renderEntryGeneric',
        'execute_once' => FALSE,
      );
    }

    return $return;
  }
}
