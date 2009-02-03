<?php
// $Id$
/**
 *   biblio.module for Drupal
 *
 *   Copyright (C) 2006-2009  Ron Jerome
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

/**
 * @param $name
 * @return array of keywords
 */
function biblio_get_keyword_by_name($name) {
  $db_result = db_query("SELECT * FROM {biblio_keyword_data} k WHERE LOWER(k.word) = LOWER('%s')", trim($name));
  $result = array();
  while ($term = db_fetch_object($db_result)) {
    $result[] = $term;
  }

  return $result;
  
}

/**
 * @param $kid
 * @return unknown_type
 */
function biblio_get_keyword_by_id($kid) {
  static $keywords = array();

  if (!isset($keywords[$kid])) {
    $keywords[$kid] = db_fetch_object(db_query('SELECT * FROM {biblio_keyword_data} WHERE kid = %d', $kid));
  }

  return $keywords[$kid];
  
}

/**
 * @param $node
 * @return unknown_type
 */
function biblio_get_node_keywords($node) {
  static $keywords;

  if (!isset($keywords[$node->vid])) {
    $result = db_query('SELECT t.* FROM {biblio_keywords} bk INNER JOIN {biblio_keyword_data} bkd ON bk.kid = bkd.kid  WHERE bk.vid = %d', $node->vid);
    $keywords[$node->vid] = array();
    while ($keyword = db_fetch_object($result)) {
      $keywords[$node->vid][$keyword->kid] = $keyword;
    }
  }
  return $keywords[$node->vid];
  
}

/**
 * @param $node
 * @return 
 */
function biblio_save_node_keywords($node) {
  
}

/**
 * @param $word
 * @return 
 */
function biblio_save_keyword(&$word) {
  
}

/**
 * @param $node
 * @return none
 */
function biblio_delete_node_keywords($node) {
  db_query('DELETE FROM {biblio_keywords} WHERE nid = %d', $node->nid);
}

/**
 * @param $node
 * @return none
 */
function biblio_delete_revision_keywords($node) {
  db_query('DELETE FROM {biblio_keywords} WHERE vid = %d', $node->vid);
}
