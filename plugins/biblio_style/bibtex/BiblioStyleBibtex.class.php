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
    $journal = $series = $booktitle = $school = $organization = $institution = NULL;
    $type = $this->typeMap();

    switch ($type) {
      case 100:
        $series = $biblio->biblio_secondary_title;
        $organization = $biblio->biblio_publisher;
        break;

      case 101:
      case 103:
        $booktitle = $biblio->biblio_secondary_title;
        $organization = $biblio->biblio_publisher;
        $series = $biblio->biblio_tertiary_title;
        break;

      case 108:
        $school = $biblio->biblio_publisher;
        $biblio->biblio_publisher = NULL;
        if (stripos($biblio->biblio_type_of_work, 'masters')) {
          $type = "mastersthesis";
        }
        break;

      case 109:
        $institution  = $biblio->biblio_publisher;
        $biblio->biblio_publisher = NULL;
        break;

      case 102:
      default:
        $journal = $biblio->biblio_secondary_title;
        break;
    }

    $bibtex .= '@' . $type . ' {';
    $bibtex .= ($biblio->biblio_citekey) ? $biblio->biblio_citekey  : "";
    $bibtex .= $this->formatEntry('title');
    $bibtex .= $this->formatEntry('journal', $journal);
    $bibtex .= $this->formatEntry('booktitle', $booktitle);
    $bibtex .= $this->formatEntry('series', $series);
    $bibtex .= $this->formatEntry('volume');
    $bibtex .= $this->formatEntry('number');
    $bibtex .= $this->formatEntry('year');
    $bibtex .= $this->formatEntry('note');
    $bibtex .= $this->formatEntry('month');
    $bibtex .= $this->formatEntry('pages');
    $bibtex .= $this->formatEntry('publisher');
    $bibtex .= $this->formatEntry('school', $school);
    $bibtex .= $this->formatEntry('organization', $organization);
    $bibtex .= $this->formatEntry('institution', $institution);
    $bibtex .= $this->formatEntry('type');
    $bibtex .= $this->formatEntry('edition');
    $bibtex .= $this->formatEntry('chapter');
    $bibtex .= $this->formatEntry('address');
    $bibtex .= $this->formatEntry('abstract');

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

    $bibtex .= $this->formatEntry('isbn');
    $bibtex .= $this->formatEntry('issn');
    $bibtex .= $this->formatEntry('doi');
    $bibtex .= $this->formatEntry('url');

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
    if (!empty ($a)) {
      $bibtex .= $this->formatEntry('author', $a);
    }
    if (!empty ($e)) {
      $bibtex .= $this->formatEntry('editor', $e);
    }

    $bibtex .= "\n}\n";


    // Convert any special characters to the latex equivalents.
    if (!isset($converter)) {
      include_once(drupal_get_path('module', 'biblio_bibtex') . '/transtab_unicode_bibtex.inc.php');
      $converter = new PARSEENTRIES();
    }
    $bibtex = $converter->searchReplaceText(_biblio_bibtex_get_transtab(), $bibtex, FALSE);

    return $bibtex;
  }

  /**
   * Format an entry.
   *
   * @param $key
   *   The BibTeX key name.
   * @param $value
   *   Optional; The value to format. If empty, try to get it from the Biblio
   *   entity. Defaults to NULL.
   * @return string
   */
  private function formatEntry($key, $value = NULL) {
    if (empty($value)) {
      $map = $this->getMapping();
      $map = $map['field'];

      if (empty($map[$key])) {
        return;
      }

      // @todo: Cache the wrapper.
      $wrapper = entity_metadata_wrapper('biblio', $this->biblio);

      $property_name = $map[$key];
      if (!isset($wrapper->{$property_name})) {
        return;
      }

      if (!$value = $wrapper->{$property_name}->value()) {
        return;
      }
    }

    return ",\n\t$key = {" . $value . "}";
  }

  /**
   * Get the BibTeX type from the Biblio entity.
   *
   * @return
   */
  private function typeMap() {
    $type = $this->biblio->type;
    $map = $this->getMapping();
    return !empty($map['type'][$type]) ? $map['type'][$type] : 'article';
  }


  /**
   * Map the fields from the Biblio entity to the ones known by BibTeX.
   */
  public function map() {
  }

  public function getMapping() {
    return array(
      'type' => array(
      ),
      // Array with field mapping, keyed by BibTeX name, and the Biblio field
      // as the value.
      'field' => array(
        'title' => 'title',
        'volume' => 'biblio_volume',
        'number' => 'biblio_number',
        'year' => 'biblio_year',
        'note' => 'biblio_notes',
        'month' => 'biblio_date',
        'pages' => 'biblio_pages',
        'publisher' => 'biblio_publisher',
        'type' => 'biblio_type_of_work',
        'edition' => 'biblio_edition',
        'chapter' => 'biblio_section',
        'address' => 'biblio_place_published',
        'abstract' => 'biblio_abstract',
        'isbn' => 'biblio_isbn',
        'issn' => 'biblio_issn',
        'doi' => 'biblio_doi',
        // @todo: Is this the Biblio URL?
        'url' => 'biblio_url',
      ),
    );
  }
}