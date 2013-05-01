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
    foreach ($mapping['biblio']['text'] as $key => $value) {
      if (!isset($wrapper->{$value})) {
        continue;
      }

      $this->mappedBiblio->{$key} = $wrapper->{$value}->value();
    }

    // Add contribuots.
    if (isset($wrapper->contributor_collection) && $contributors = $wrapper->contributor_collection->value()) {
      foreach ($contributors as $contributor) {
        $type = $contributor->type;

        $mapped_contributor = new stdClass();
        $contributor_wrapper = entity_metadata_wrapper('biblio_contributor', $contributor);

        // Map the contributor data.
        foreach ($mapping['contributor']['text'] as $contributor_key => $contributor_value) {
          if (!isset($wrapper->{$value})) {
            continue;
          }

          $mapped_contributor = $contributor_wrapper->{$value}->value();
        }

        if ($mapped_contributor) {
          // @todo: Is that correct?
          // If the contributor doesn't have given or family name, use the
          // "name"
          if (empty($mapped_contributor->given) && empty($mapped_contributor->family)) {
            $mapped_contributor->family = $contributor->name;
          }
          $this->mappedBiblio->{$type}[] = $mapped_contributor;
        }
      }
    }

    $name = 'publisher-place';

    $this->mappedBiblio->{$name} = 'Haifa';

    $name = 'issued';
    $sub_name = 'date-parts';
    $this->mappedBiblio->{$name}->{$sub_name} = array(array('2013', '6'));
    $this->mappedBiblio->author[] = (object)array('literal' => 'foooo');


    dpm($this->mappedBiblio);

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
          'issued' => 'issued',
          'event' => 'event',
          'accessed' => 'accessed',
          'container' => 'container',
          'original-date' => 'original-date',
        ),

        'name' => array(
          //Name Variables.
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
