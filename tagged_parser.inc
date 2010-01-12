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
  module_load_include('inc', 'biblio', 'biblio.type.mapper');
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
          $node->title = $value;
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
    $map = biblio_get_type_map('tagged');
  }
  return (isset($map[$type]))?$map[$type]:129; //return the biblio type or 129 (Misc) if type not found
}
function tagged_parser_field_map($enfield) {
  static $fmap = array();
  if(empty($fmap)) {
    $fmap = biblio_get_field_map('tagged');
  }
  return (!empty($fmap[$enfield])) ? $fmap[$enfield] : '';

}