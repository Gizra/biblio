<?php

/**
 * @file
 * EndNote tagged biblio style.
 */

class BiblioStyleEndNoteTagged extends BiblioStyleBase {

  public function import($data, $options = array()) {
    $data = str_replace("\n\r", "\n", $data);
    $data = explode("\n", $data);

    $biblio = $this->biblio;
    $wrapper = entity_metadata_wrapper('biblio', $biblio);

    foreach ($data as $row) {
      $tag = substr($row, 0, 2);
      $value = substr($row, 2);

      switch ($tag) {
        case '%0' :
          $biblio->biblio_type = strtolower($value);
          break;

        /*
        case '%A' :
          $node->biblio_contributors[] = array(
            'name' => $value,
            'auth_category' => 1,
            'auth_type' => _biblio_get_auth_type(1, $node->biblio_type));
          break;

        case '%E' :
          $node->biblio_contributors[] = array(
            'name' => $value,
            'auth_category' => 2,
            'auth_type' => _biblio_get_auth_type(2, $node->biblio_type));
          break;

        case '%T' :
          $biblio->title = $value;
          break;
        case '%Y' :
          $node->biblio_contributors[] = array(
            'name' => $value,
            'auth_category' => 3,
            'auth_type' => _biblio_get_auth_type(3, $node->biblio_type));
          break;
        case '%?' :
          $node->biblio_contributors[] = array(
            'name' => $value,
            'auth_category' => 4,
            'auth_type' => _biblio_get_auth_type(4, $node->biblio_type));
          break;

        */
        case '%X' :
          $wrapper->biblio_abstract->set($value);

          break;
        case '%Z' :
          $wrapper->biblio_notes->set($value);
          break;

        default :
          $map = getMapping();
          if (!empty($map[$tag])) {
            $wrapper->{$map[$tag]}->set($value);
          }
      }

      // $wrapper->save();
    }
  }

  public function getMapping() {
    return array(
      '%B' => 'biblio_secondary_title',
      '%C' => 'biblio_place_published',
      '%D' => 'biblio_year',
      '%F' => 'biblio_label',
      '%G' => 'biblio_lang',
      '%I' => 'biblio_publisher',
      '%J' => 'biblio_secondary_title',
      '%K' => 'biblio_keywords',
      '%L' => 'biblio_call_number',
      '%M' => 'biblio_accession_number',
      '%N' => 'biblio_issue',
      '%P' => 'biblio_pages',
      '%R' => 'biblio_doi',
      '%S' => 'biblio_tertiary_title',
      '%U' => 'biblio_url',
      '%V' => 'biblio_volume',
      '%1' => 'biblio_custom1',
      '%2' => 'biblio_custom2',
      '%3' => 'biblio_custom3',
      '%4' => 'biblio_custom4',
      '%#' => 'biblio_custom5',
      '%$' => 'biblio_custom6',
      '%]' => 'biblio_custom7',
      '%6' => 'biblio_number_of_volumes',
      '%7' => 'biblio_edition',
      '%8' => 'biblio_date',
      '%9' => 'biblio_type_of_work',
      '%?' => '',
      '%@' => 'biblio_isbn',
      '%<' => 'biblio_research_notes',
      '%!' => 'biblio_short_title',
      '%&' => 'biblio_section',
      '%(' => 'biblio_original_publication',
      '%)' => 'biblio_reprint_edition',
      '%*' => '',
      '%+' => '',
    );
  }
}
