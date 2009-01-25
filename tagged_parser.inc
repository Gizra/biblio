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
  if (!($fp = fopen($file->filepath, "r"))) {
    drupal_set_message("could not open EndNote Tagged input",'error');
    return;
  }
  $nids = array();
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
          switch ($value) {
            case "Journal Article" :
              $node['biblio_type'] = 102;
              break; // journal
            case "Conference Paper" :
              $node['biblio_type'] = 103;
              break;
            case "Conference Proceedings" :
              $node['biblio_type'] = 104;
              break; // conference proceedings
            case "Report" :
              $node['biblio_type'] = 109;
              break; // report
            case "Book" :
            case "Edited Book" :
              $node['biblio_type'] = 100;
              break; // book section
            case "Book Section" :
              $node['biblio_type'] = 101;
              break; // book section
            case "Thesis" :
              $node['biblio_type'] = 108;
              break; // thesis
            case "Patent" :
              $node['biblio_type'] = 119;
              break; // patent
            case "Generic" :
            default :
              $node['biblio_type'] = 129;
              break; // generic
          }
          break;
            case '%A' :
              $node['biblio_contributors'][1][] = array(
            'name' => $value,
            'auth_type' => _biblio_get_auth_type(1, $node['biblio_type']));
              break;
            case '%B' :
              $node['biblio_secondary_title'] = $value;
              break;
            case '%C' :
              $node['biblio_place_published'] = $value;
              break;
            case '%D' :
              $node['biblio_year'] = $value;
              break;
            case '%E' :
              $node['biblio_contributors'][2][] = array(
            'name' => $value,
            'auth_type' => _biblio_get_auth_type(2, $node['biblio_type']));
              break;
            case '%F' :
              $node['biblio_label'] = $value;
              break;
            case '%I' :
              $node['biblio_publisher'] = $value;
              break;
            case '%J' : // Journal
              $node['biblio_secondary_title'] = $value;
              break;
            case '%K' :
              if ($vid = variable_get('biblio_keyword_vocabulary', 0)) {
                $node['taxonomy']['tags'][$vid] .= (empty($node['taxonomy']['tags'][$vid])) ? "\"$value\"" : ",\"$value\"";
                unset($node['biblio_keywords']);
              }else{
                $node['biblio_keywords'] .= (empty($node['biblio_keywords'])) ? "\"$value\"" : ",\"$value\"";
              }
              break;
            case '%L' :
              $node['biblio_call_number'] = $value;
              break;
            case '%M' : // accession_number
              $node['biblio_accession_number'] = $value;
              break;
            case '%N' :
              $node['biblio_issue'] = $value;
              break;
            case '%P' :
              $node['biblio_pages'] = $value;
              break;
            case '%R' :
              $node['biblio_doi'] = $value;
              break;
            case '%S' :
              $node['biblio_tertiary_title'] = $value;
              break;
            case '%T' :
              $node['title'] = $value;
              break;
            case '%U' :
              $node['biblio_url'] = $value;
              break;
            case '%V' :
              $node['biblio_volume'] = $value;
              break;
            case '%X' :
              $node['biblio_abst_e'] .= $value;
              break;
            case '%Y' :
              $node['biblio_contributors'][3][] = array(
            'name' => $value,
            'auth_type' => _biblio_get_auth_type(3, $node['biblio_type']));
              break;
            case '%Z' :
              $node['biblio_notes'] .= $value;
              break;
            case '%1' : // CUSTOM 1
              $node['biblio_custom1'] = $value;
              break;
            case '%2' : // CUSTOM 2
              $node['biblio_custom2'] = $value;
              break;
            case '%3' : // CUSTOM 3
              $node['biblio_custom3'] = $value;
              break;
            case '%4' : // CUSTOM 4
              $node['biblio_custom4'] = $value;
              break;
            case '%#' : // CUSTOM 5
              $node['biblio_custom5'] = $value;
              break;
            case '%$' : // CUSTOM 6
              $node['biblio_custom6'] = $value;
              break;
            case '%6' :
              break;
            case '%7' :
              $node['biblio_edition'] = $value;
              break;
            case '%8' :
              $node['biblio_date'] = $value;
              break;
            case '%9' :
              $node['biblio_type_of_work'] = $value;
              break;
            case '%?' :
              break;
            case '%@' :
              $node['biblio_isbn'] = $value;
              break;
            case '%!' :
              break;
            case '%&' :
              break;
            case '%(' :
              break;
            case '%)' :
              break;
            case '%*' :
              break;
            case '%+' :
              break;
            default :
              break;
      } //end switch
    }
    else {
      $incite = false;
      if (!empty($node)) {
        if (!empty($terms)) $node['taxonomy'] = array_merge($terms,$node['taxonomy']);
        $nids[] = biblio_save_node($node, $batch, $session_id);
      }

      $node = array();

    } // end if ($start !== false)
  } // end while
  fclose($fp);
  return (!empty($nids)) ? $nids : array();
}