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
    foreach ($mapping['biblio']['field'] as $key => $value) {
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
        foreach ($mapping['contributor']['field'] as $contributor_key => $contributor_value) {
          if (!isset($wrapper->{$value})) {
            continue;
          }

          $mapped_contributor = $contributor_wrapper->{$value}->value();
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
        'field' => array(
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

          //Date Variables'
          'issued' => 'issued',
          'event' => 'event',
          'accessed' => 'accessed',
          'container' => 'container',
          'original-date' => 'original-date',

          //Name Variables'
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
        'field' => array(
          'initials' => 'contributor_initials',
          'given' => 'contributor_given',
          'family' => 'contributor_family',
        ),
      ),
      'type' => array(
        'article' => 'article',
        'article-magazine'  => 'article-magazine',
        'article-newspaper' => 'article-newspaper',
        'article-journal' => 'article-journal',
        'bill' => 'bill',
        'book'  => 'book',
        'broadcast' => 'broadcast',
        'chapter' => 'chapter',
        'entry' => 'entry',
        'entry-dictionary'  => 'entry-dictionary',
        'entry-encyclopedia'  => 'entry-encyclopedia',
        'figure'  => 'figure',
        'graphic'  => 'graphic',
        'interview'  => 'interview',
        'legislation' => 'legislation',
        'legal_case' => 'legal_case',
        'manuscript' => 'manuscript',
        'map' => 'map',
        'motion_picture' => 'motion_picture',
        'musical_score'  => 'musical_score',
        'pamphlet'  => 'pamphlet',
        'paper-conference' => 'paper-conference',
        'patent' => 'patent',
        'post'  => 'post',
        'post-weblog'  => 'post-weblog',
        'personal_communication' => 'personal_communication',
        'report' => 'report',
        'review'  => 'review',
        'review-book'  => 'review-book',
        'song'  => 'song',
        'speech'  => 'speech',
        'thesis' => 'thesis',
        'treaty'  => 'treaty',
        'webpage' => 'webpage',
      ),
    );
  }
}
