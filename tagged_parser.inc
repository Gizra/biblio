<?php
// $Id$
/**
 *   Copyright (C) 2006  Ron Jerome
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
function _endnote_tagged_import($file, $terms = array(), $batch = FALSE, $session_id = NULL) {
  if (!($fp = fopen($file->uri, "r"))) {
    drupal_set_message("could not open EndNote Tagged input",'error');
    return;
  }
  $nids = array();
  $node = new stdClass();
  $incite = false;
  $node_id = null;
  $contributors = null;
  while (!feof($fp)) {
    $line = trim(fgets($fp));
    $line_len = strlen($line);
    if ($line_len) {
      $start = strpos($line, "%"); // There could be some unprintables at the beginning of the line so fine the location of the %
      if ($start !== false) {
        $tag = drupal_substr($line, $start, 2);
        $value = trim(drupal_substr($line, $start +3));
      }
      else {
        $value = $line;
      }
    }
    if ($line_len) { // if this is not a blank line
      if (!$incite) {
        $incite = true;
      }
      switch ($tag) {
        case '%0' :
          $node->biblio_type = tagged_type_map($value);
          break;
        case '%A' :
          $node->biblio_contributors[1][] = array(
            'name' => $value,
            'auth_type' => _biblio_get_auth_type(1, $node->biblio_type));
          break;
        case '%E' :
          $node->biblio_contributors[2][] = array(
            'name' => $value,
            'auth_type' => _biblio_get_auth_type(2, $node->biblio_type));
          break;
        case '%T' :
        	$langcode = FIELD_LANGUAGE_NONE;
          $node->title[$langcode][0]['value'] = $value;
          break;
        case '%Y' :
          $node->biblio_contributors[3][] = array(
            'name' => $value,
            'auth_type' => _biblio_get_auth_type(3, $node->biblio_type));
          break;
        case '%X' :
          $node->biblio_abst_e .= $value;
          break;
        case '%Z' :
          $node->biblio_notes .= $value;
          break;

        default :
          $field = tagged_parser_field_map($tag);
          $node->$field = $value;
          break;
      } //end switch
    }
    else {
      $incite = false;
      if (!empty($node)) {
        if (!empty($terms)) {
          if (!isset($node->taxonomy)) $node->taxonomy = array();
          $node->taxonomy = array_merge($terms,$node->taxonomy);
        }
        biblio_save_node($node, $batch, $session_id);
        if (!empty($node->nid)) $nids[] = $node->nid;
      }

      $node = new stdClass();
      $node->biblio_contributors = array();
    } // end if ($start !== false)
  } // end while
  fclose($fp);
  return (!empty($nids)) ? $nids : array();
}
function tagged_type_map($type) {
  static $map = array();
  if (empty($map)) {
    module_load_include('inc', 'biblio', 'biblio.type.mapper');
    $map = biblio_get_type_map('tagged');
  }
  return (isset($map[$type]))?$map[$type]:129; //return the biblio type or 129 (Misc) if type not found
}
function tagged_parser_field_map($enfield) {
  static $fmap = array();
  if(empty($fmap)) {
    $fmap = array(
      '%B' => 'biblio_secondary_title',
      '%C' => 'biblio_place_published',
      '%D' => 'biblio_year',
      '%F' => 'biblio_label',
      '%G' => 'biblio_lang',
      '%I' => 'biblio_publisher',
      '%J' => 'biblio_secondary_title',
      '%K' => 'biblio_keywords[]',
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
      '%!' => '',
      '%&' => '',
      '%(' => '',
      '%)' => '',
      '%*' => '',
      '%+' => '');
  }
  return (!empty($fmap[$enfield])) ? $fmap[$enfield] : '';

}