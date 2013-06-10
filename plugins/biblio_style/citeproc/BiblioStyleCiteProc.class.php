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
        if (!$type = $wrapper_contributor->biblio_contributor_role->label()) {
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
          'container-title' => 'container-title',
          'collection-title' => 'collection-title',
          'original-title' => 'original-title',
          'publisher' => 'publisher',
          'publisher-place' => 'publisher-place',
          'original-publisher' => 'original-publisher',
          'original-publisher-place' => 'original-publisher-place',
          'archive' => 'archive',
          'archive-place' => 'archive-place',
          'authority' => 'authority',
          'archive_location' => 'authority',
          'event' => 'event',
          'event-place' => 'event-place',
          'page' => 'biblio_pages',
          'page-first' => 'page',
          'locator' => 'locator',
          'version' => 'version',
          'volume' => 'volume',
          'number-of-volumes' => 'number-of-volumes',
          'number-of-pages' => 'number-of-pages',
          'issue' => 'issue',
          'chapter-number' => 'chapter-number',
          'medium' => 'medium',
          'status' => 'status',
          'edition' => 'edition',
          'section' => 'section',
          'genre' => 'genre',
          'note' => 'note',
          'annote' => 'annote',
          'abstract'  => 'abstract',
          'keyword' => 'keyword',
          'number' => 'number',
          'references' => 'references',
          'URL' => 'URL',
          'DOI' => 'DOI',
          'ISBN' => 'ISBN',
          'call-number' => 'call-number',
          'citation-number' => 'citation-number',
          'citation-label' => 'citation-label',
          'first-reference-note-number' => 'first-reference-note-number',
          'year-suffix' => 'year-suffix',
          'jurisdiction' => 'jurisdiction',
        ),

        'date' => array(
          // Date Variables.
          'issued' => 'biblio_issued',
          'event' => 'event',
          'accessed' => 'accessed',
          'container' => 'container',
          'original-date' => 'original-date',
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
