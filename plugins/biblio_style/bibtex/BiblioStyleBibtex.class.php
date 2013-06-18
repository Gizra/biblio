<?php

/**
 * @file
 * BibTeX style.
 */

class BiblioStyleBibtex extends BiblioStyleBase {

  public function render($options = array(), $langcode = NULL) {
  }


  /**
   * Map the fields from the Biblio entity to the ones known by CiteProc.
   */
  public function map() {
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
