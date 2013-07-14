<?php

/**
 * @file
 * Chicago biblio style.
 */

class BiblioStyleCiteProc extends BiblioStyleBase {

  public function render($options = array(), $langcode = NULL) {
    global $language;
    $langcode = $langcode ? $langcode : $language->language;

    // Make sure the CSL file exists.
    $style_name = $this->plugin['options']['style_name'] . '.csl';
    // @todo: Allow adding more styles in the Library.
    $file_path = $this->plugin['options']['style_path'] . '/' . $style_name;
    if (!file_exists($file_path)) {
      throw new Exception(format_string('@style file does not exist in @path.', array('@style' => $style_name, '@path' => $file_path)));
    }

    $csl_file_contents = file_get_contents($file_path);

    // @todo: Define CiteProc as library.
    include_once libraries_get_path('citeproc-php') . '/CiteProc.php';
    $citeproc = new citeproc($csl_file_contents, $langcode);

    // Pass CiteProc the mapped biblio.
    $mapped_data = $this->map();
    return $citeproc->render($mapped_data);
  }


  /**
   * Map the fields from the Biblio entity to the ones known by CiteProc.
   */
  public function map() {
    $this->mappedBiblio = new stdClass();
    $mapping = $this->getMapping();
    $wrapper = entity_metadata_wrapper('biblio', $this->biblio);

    // Text variables.
    foreach ($mapping['biblio']['text'] as $key => $field_name) {
      if (!isset($wrapper->{$field_name})) {
        continue;
      }

      $this->mappedBiblio->{$key} = $wrapper->{$field_name}->value();
    }


    // Date variables.
    foreach ($mapping['biblio']['date'] as $key => $field_name) {
      if (!isset($wrapper->{$field_name})) {
        continue;
      }

      $date = array();

      // @todo: Add "In press".
      if (isset($wrapper->biblio_in_press) && $wrapper->biblio_in_press->value()) {
        // CiteProc currently doesn't support the literal key. So this is
        // actually ignored, however, this is the "right" way.
        $this->mappedBiblio->{$key}->literal = 'In press';
        // This hack is just to make sure the In Press is added.
        // @todo: Check localization.
        $this->mappedBiblio->{$key}->{'date-parts'}[] = array('In press');
        continue;
      }

      // Check if the field is date field, or text.
      $field = field_info_field($field_name);
      if ($field['type'] == 'datestamp') {
        // Get the date granularity from the field settings.
        $timestamp = $wrapper->{$field_name}->value();

        $date_info = array(
          'year' => 'Y',
          'month' => 'm',
          'day' => 'd',
        );

        foreach ($date_info as $granularity => $format) {
          if (empty($field['settings']['granularity'][$granularity])) {
            continue;
          }

          $date[] = date($format, $timestamp);
        }
      }
      else {
        // Textfield, so grab the value as literal.
        $date = array($wrapper->{$field_name}->value());
      }

      $this->mappedBiblio->{$key}->{'date-parts'}[] = $date;
    }

    // Add contributors.
    if (isset($wrapper->contributor_field_collection) && $wrapper_contributors = $wrapper->contributor_field_collection) {
      foreach ($wrapper_contributors as $wrapper_contributor) {
        if (!$type = strtolower($wrapper_contributor->biblio_contributor_role->label())) {
          $type = 'author';
        }

        $mapped_contributor = new stdClass();
        $contributor_wrapper = $wrapper_contributor->biblio_contributor;

        // Map the contributor data.
        foreach ($mapping['contributor']['text'] as $key => $field_name) {
          if (!isset($contributor_wrapper->{$field_name})) {
            continue;
          }

          $mapped_contributor->{$key} = $contributor_wrapper->{$field_name}->value();
        }

        if ($mapped_contributor) {
          $this->mappedBiblio->{$type}[] = $mapped_contributor;
        }
      }
    }

    return $this->mappedBiblio;
  }

  public function getMapping() {
    return array(
      'biblio' => array(
        'text' => array(
          // Text variables.
          'title' => 'title',
          'container-title' => 'biblio_secondary_title',
          'collection-title' => 'biblio_secondary_title',
          'original-title' => 'biblio_alternate_title',
          'publisher' => 'biblio_publisher',
          'publisher-place' => 'biblio_publisher_place',
          'original-publisher' => 'original-publisher',
          'original-publisher-place' => 'original-publisher-place',
          'archive' => 'archive',
          'archive-place' => 'archive-place',
          'authority' => 'authority',
          'archive_location' => 'authority',
          'event' => 'biblio_date',
          'event-place' => 'biblio_place_published',
          'page' => 'biblio_pages',
          'page-first' => 'page',
          'locator' => 'locator',
          'version' => 'biblio_edition',
          'volume' => 'biblio_volume',
          'number-of-volumes' => 'biblio_number_of_volumes',
          'number-of-pages' => 'number-of-pages',
          'issue' => 'biblio_issue',
          'chapter-number' => 'biblio_section',
          'medium' => 'medium',
          'status' => 'status',
          'edition' => 'biblio_edition',
          'section' => 'biblio_section',
          'genre' => 'genre',
          'note' => 'biblio_note',
          'annote' => 'annote',
          'abstract'  => 'biblio_abstract',
          'keyword' => 'keyword',
          'number' => 'biblio_number',
          'references' => 'references',
          'URL' => 'URL',
          'DOI' => 'biblio_doi',
          'ISBN' => 'biblio_isbn',
          'call-number' => 'biblio_call_number',
          'citation-number' => 'citation-number',
          'citation-label' => 'biblio_citekey',
          'first-reference-note-number' => 'first-reference-note-number',
          'year-suffix' => 'year-suffix',
          'jurisdiction' => 'jurisdiction',
        ),

        'date' => array(
          // Date Variables.
          // @todo: We use the biblio_year instead of biblio_issued, as timestamp
          // is starting in 1970, and dates can be before that.
          'issued' => 'biblio_year',
          'event' => 'event',
          'accessed' => 'biblio_access_date',
          'container' => 'biblio_date',
          'original-date' => 'biblio_date',
        ),

        'contributor' => array(
          // Contributor Variables.
          'author' => 'author',
          'editor' => 'editor',
          'translator' => 'translator',
          'recipient' => 'recipient',
          'interviewer' => 'interviewer',
          'publisher' => 'publisher',
          'composer' => 'composer',
          'original-publisher' => 'original-publisher',
          'original-author' => 'original-author',
          'container-author' => 'container-author',
          'collection-editor' => 'collection-editor',
        ),
      ),
      'contributor' => array(
        'text' => array(
          'initials' => 'contributor_initials',
          'given' => 'contributor_given',
          'family' => 'contributor_family',
        ),
      ),
    );
  }
}
