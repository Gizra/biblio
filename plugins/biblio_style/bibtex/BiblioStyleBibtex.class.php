<?php

/**
 * @file
 * BibTeX style.
 */

class BiblioStyleBibtex extends BiblioStyleBase {

  public function render($options = array(), $langcode = NULL) {
    static $converter = NULL;

    $biblio = $this->biblio;

    $bibtex = '';
    $type = "article";
    $journal = $series = $booktitle = $school = $organization = $institution = NULL;
    $type = _biblio_bibtex_type_map($biblio->biblio_type);
    switch ($biblio->biblio_type) {
      case 100 :
        $series = $biblio->biblio_secondary_title;
        $organization = $biblio->biblio_publisher;
        break;
      case 101 :
      case 103 :
        $booktitle = $biblio->biblio_secondary_title;
        $organization = $biblio->biblio_publisher;
        $series = $biblio->biblio_tertiary_title;
        break;
      case 108 :
        $school = $biblio->biblio_publisher;
        $biblio->biblio_publisher = NULL;
        if (stripos($biblio->biblio_type_of_work, 'masters')) {
          $type = "mastersthesis";
        }
        break;
      case 109 :
        $institution  = $biblio->biblio_publisher;
        $biblio->biblio_publisher = NULL;
        break;
      case 102 :
      default:
        $journal = $biblio->biblio_secondary_title;
        break;
    }

    $bibtex .= '@' . $type . ' {';
    $bibtex .= ($biblio->biblio_citekey) ? $biblio->biblio_citekey  : "";
    $bibtex .= $this->formatEntry('title', $biblio->title);
    $bibtex .= $this->formatEntry('journal', $journal);
    $bibtex .= $this->formatEntry('booktitle', $booktitle);
    $bibtex .= $this->formatEntry('series', $series);
    $bibtex .= $this->formatEntry('volume', $biblio->biblio_volume);
    $bibtex .= $this->formatEntry('number', $biblio->biblio_number);
    $bibtex .= $this->formatEntry('year', $biblio->biblio_year);
    $bibtex .= $this->formatEntry('note', $biblio->biblio_notes);
    $bibtex .= $this->formatEntry('month', $biblio->biblio_date);
    $bibtex .= $this->formatEntry('pages', $biblio->biblio_pages);
    $bibtex .= $this->formatEntry('publisher', $biblio->biblio_publisher);
    $bibtex .= $this->formatEntry('school', $school);
    $bibtex .= $this->formatEntry('organization', $organization);
    $bibtex .= $this->formatEntry('institution', $institution);
    $bibtex .= $this->formatEntry('type', $biblio->biblio_type_of_work);
    $bibtex .= $this->formatEntry('edition', $biblio->biblio_edition);
    $bibtex .= $this->formatEntry('chapter', $biblio->biblio_section);
    $bibtex .= $this->formatEntry('address', $biblio->biblio_place_published);
    $bibtex .= $this->formatEntry('abstract', $biblio->biblio_abst_e);

    $kw_array = array();
    if (!empty($biblio->terms)) {
      foreach ($biblio->terms as $term) {
        $kw_array[] = $term->name;
      }
    }
    if (!empty($biblio->biblio_keywords)) {
      foreach ($biblio->biblio_keywords as $term) {
        $kw_array[] = $term;
      }
    }
    if (!empty($kw_array)) {
      $kw_array = array_unique($kw_array);
      $bibtex .= $this->formatEntry('keywords', implode(', ', $kw_array));
    }

    $bibtex .= $this->formatEntry('isbn', $biblio->biblio_isbn);
    $bibtex .= $this->formatEntry('issn', $biblio->biblio_issn);
    $bibtex .= $this->formatEntry('doi', $biblio->biblio_doi);
    $bibtex .= $this->formatEntry('url', $biblio->biblio_url);

    if (!empty ($biblio->upload) && count($biblio->upload['und']) && user_access('view uploaded files')) {
      foreach ($biblio->upload['und'] as $file) {
        $attachments[] = file_create_url($file['uri']);
      }
      $bibtex .= $this->formatEntry('attachments', implode(' , ', $attachments));
    }

    $a = $e = $authors = array();
    if ($authors = biblio_get_contributor_category($biblio->biblio_contributors, 1)) {
      foreach ($authors as $auth) $a[] = trim($auth['name']);
    }
    if ($authors = biblio_get_contributor_category($biblio->biblio_contributors, 2)) {
      foreach ($authors as $auth) $e[] = trim($auth['name']);
    }
    $a = implode(' and ', $a);
    $e = implode(' and ', $e);
    if (!empty ($a)) $bibtex .= $this->formatEntry('author', $a);
    if (!empty ($e)) $bibtex .= $this->formatEntry('editor', $e);
    $bibtex .= "\n}\n";


    // Convert any special characters to the latex equivalents.
    if (!isset($converter)) {
      include_once(drupal_get_path('module', 'biblio_bibtex') . '/transtab_unicode_bibtex.inc.php');
      $converter = new PARSEENTRIES();
    }
    $bibtex = $converter->searchReplaceText(_biblio_bibtex_get_transtab(), $bibtex, FALSE);

    return $bibtex;
  }

  public function formatEntry($key, $value) {
    return !empty($value) ? ",\n\t$key = {" . $value . "}" : '';
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
