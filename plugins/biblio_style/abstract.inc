<?php

/**
 * Biblio Style interface
 *
 */
interface BiblioStyleInterface {

  /**
   * Constructor for the notifier.
   *
   * @param $plugin
   *   The notifier plugin object. Note the "options" values might have
   *   been overriden in message_notify_send_message().
   * @param Biblio $biblio
   *   The Biblio entity.
   */
  public function __construct($plugin, Biblio $biblio);

  public function map();

  public function getMapping();

  /**
   * Render the Biblio according to the style plugin.
   *
   * @return
   *   A an HTML string.
   */
  public function render($options = array(), $langcode = NULL);
}

/**
 * An abstract implementation of MessageNotifierInterface.
 */
abstract class BiblioStyleBase implements BiblioStyleInterface {

  /**
   * The plugin definition.
   */
  protected $plugin;

  /**
   * The Biblio entity.
   */
  protected $biblio;

  public function __construct($plugin, Biblio $biblio) {
    $this->plugin = $plugin;
    $this->biblio = $biblio;
  }

  public function map() {
    $mapping = $this->getMapping();
    $wrapper = entity_metadata_wrapper('biblio', $this->biblio);
    foreach ($mapping['field'] as $key => $value) {
      if (!isset($wrapper->{$value})) {
        continue;
      }

      $this->mappedBiblio->{$key} = $wrapper->{$value}->value();
    }
  }

  public function getMapping() {
    return array(
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
        'page' => 'page',
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

  public function render($options = array(), $langcode = NULL) {
    return '';
  }
}
