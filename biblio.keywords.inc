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
  static $keywords = array();
  
  if (!$kid = array_search($name, $keywords)) {
    $term = db_fetch_object(db_query("SELECT * FROM {biblio_keyword_data} k WHERE LOWER(k.word) = LOWER('%s')", trim($name)));
    $keywords[$term->kid] = $term;
    return $term;
  }
  
  return $keywords[$kid];
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
function biblio_load_keywords($vid) {
  static $keywords;

  if (!isset($keywords[$vid])) {
    $result = db_query('SELECT bkd.* FROM {biblio_keyword} bk INNER JOIN {biblio_keyword_data} bkd ON bk.kid = bkd.kid  WHERE bk.vid = %d', $vid);
    $keywords[$vid] = array();
    while ($keyword = db_fetch_object($result)) {
      $keywords[$vid][$keyword->kid] = $keyword;
    }
  }
  return $keywords[$vid];
  
}

/**
 * @param $node
 * @return 
 */
function biblio_save_keywords($node, $update = FALSE) {
  if (!is_array($node->biblio_keywords)) {
    $sep = variable_get('biblio_keyword_sep', ',');
    $node->biblio_keywords = explode($sep,$node->biblio_keywords );
  }
  db_query('DELETE FROM {biblio_keyword} WHERE nid = %d AND vid = %d', array($node->nid, $node->vid));

  foreach ($node->biblio_keywords as $keyword) {
    $word = (is_object($keyword)) ? $keyword->word : $keyword;
    if (!strlen(trim($word))) continue; //skip if we have a blank
    $kid = NULL;
    // See if the term exists 
    if ($term = biblio_get_keyword_by_name($word)) $kid = $term->kid;
    if (!$kid) {
      $term = array('word' => trim($word));
      $status = biblio_save_keyword($term);
      $kid = $term['kid'];
    }
    // Defend against duplicate, differently cased tags
    if (!isset($inserted[$kid])) {    
      db_query('INSERT INTO {biblio_keyword} (kid, nid, vid) VALUES (%d, %d, %d)', $kid, $node->nid, $node->vid);
      $inserted[$kid] = TRUE;
    }
  }
  return count($inserted);
}

/**
 * @param $word
 * @return 
 */
function biblio_save_keyword(&$form_values) {
  if (!empty($form_values['kid']) && $form_values['word']) {
    drupal_write_record('biblio_keyword_data', $form_values, 'kid');
    $status = SAVED_UPDATED;
  } else {
    drupal_write_record('biblio_keyword_data', $form_values);
    $status = SAVED_NEW;
  }
  
  return $status;  
}

/**
 * @param $node
 * @return none
 */
function biblio_delete_node_keywords($node) {
  db_query('DELETE FROM {biblio_keyword} WHERE nid = %d', $node->nid);
}

/**
 * @param $node
 * @return none
 */
function biblio_delete_revision_keywords($node) {
  db_query('DELETE FROM {biblio_keywords} WHERE vid = %d', $node->vid);
}

function biblio_delete_keyword($keyword_id) {
  db_query('DELETE FROM {biblio_keyword} WHERE kid = %d', $keyword_id);
  db_query('DELETE FROM {biblio_keyword_data} WHERE kid = %d', $keyword_id);
}
