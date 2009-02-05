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
function _ris_tagged_import($file, $terms = array(), $batch = FALSE, $session_id = NULL) {
  if (!($fp = fopen($file->filepath, "r"))) {
    drupal_set_message("could not open RIS input file",'error');
    return;
  }
  $incite = false;
  $nids = array();
  $contributors = null;
  $node=array();
  while (!feof($fp)) {
    $line = fgets($fp);
    $line_len = strlen($line);
     if ($line_len > 3) {
       $start = strpos($line, '  - '); // There could be some unprintables at the beginning of the line so fine the location of the %
       if ($start !== false) {
        $tag = drupal_substr($line, $start -2, 2);
        $value = trim(drupal_substr($line, $start +4));
     }
      else {
        $value = $line;
      }
    }
    if ($line_len > 3) { // if this is not a blank line
      switch ($tag) {
        case 'ER' :         
          if (!empty($node)) {
            if (empty ($node['title'])) $node['title'] = t("Untitled");
            if (!empty($terms)) $node['taxonomy'] = array_merge($terms,$node['taxonomy']);
            $nids[] = biblio_save_node($node, $batch, $session_id);
          }
          $node = array();
          $incite = false;
          break;
        case 'TY' :
          $node['biblio_contributors'] = array();
          $incite = true;
          switch ($value) {
            case "ART" : // art work
              $node['biblio_type'] = 112;
              break;
            case "COMP" : // computer program
              $node['biblio_type'] = 113;
              break;
            case "ADVS" : //Audiovisual material
              $node['biblio_type'] = 114;
              break;
            case "HEAR" : // Hearing
              $node['biblio_type'] = 115;
              break;
            case "CASE" : // Case
              $node['biblio_type'] = 116;
              break;
            case "BILL" : // Bill / resolution
              $node['biblio_type'] = 117;
              break;
            case "JOUR" : // journal
              $node['biblio_type'] = 102;
              break;
            case "CONF" : // conference proceedings
              $node['biblio_type'] = 103;
              break;
            case "NEWS" : // newspaper
              $node['biblio_type'] = 105;
              break;
            case "MGZN" : // magazine article
              $node['biblio_type'] = 106;
              break;
            case "ICOMM" : // internet communication
              $node['biblio_type'] = 107;
              break;
            case "RPRT" : // report
              $node['biblio_type'] = 109;
              break;
            case "MPCT" : //motion picture
              $node['biblio_type'] = 110;
              break;
            case "BOOK" : // book
              $node['biblio_type'] = 100;
              break;
            case "CHAP" : // book chapter
              $node['biblio_type'] = 101;
              break;
            case "THES" : // thesis
              $node['biblio_type'] = 108;
              break;
            case "PAT" : // patent
              $node['biblio_type'] = 119;
              break;
            case "PCOMM" : // Personal communication
              $node['biblio_type'] = 120;
              break;
            case "MAP" : // map
              $node['biblio_type'] = 122;
              break;
            case "DATA" : // database
              $node['biblio_type'] = 125;
              break;
            case "STAT" : // statute
              $node['biblio_type'] = 125;
              break;
            case "GEN" :
            default :
              $node['biblio_type'] = 129;
              break; // generic
          }
          break;
            case 'A1' :
            case 'AU' :
              $node['biblio_contributors'][1][] = array(
                'name' => $value,
                'auth_type' => _biblio_get_auth_type(1, $node['biblio_type']));
              break;
            case 'JF' :
              $node['biblio_secondary_title'] = $value;
              break;
            case 'CY' :
              $node['biblio_place_published'] = $value;
              break;
            case 'Y1' :
            case 'PY' :
              $node['biblio_year'] = ($end = strpos($value, "/")) ? substr($value, 0, $end) : $value;
              $node['biblio_date'] = $value;
              break;
            case 'A2' :
            case 'ED' :
              $node['biblio_contributors'][2][] = array(
                'name' => $value,
                'auth_type' => _biblio_get_auth_type(2, $node['biblio_type']));
              break;
            case 'PB' :
              $node['biblio_publisher'] = $value;
              break;
            case 'T2' :
              $node['biblio_secondary_title'] = $value;
              break;
            case 'KW' :
              if ($vid = variable_get('biblio_keyword_vocabulary', 0) && variable_get('biblio_keyword_freetagging', 1)) {
                $node['taxonomy']['tags'][$vid] .= (empty($node['taxonomy']['tags'][$vid])) ? "\"$value\"" : ",\"$value\"";
              }
              $node['biblio_keywords'][] = $value;
              break;
            case 'IS' :
            case 'CP' :
              $node['biblio_issue'] = $value;
              break;
            case 'SP' :
            case 'EP' :
              $node['biblio_pages'] .= ($tag == "SP") ? $value : " - " . $value;
              break;
            case 'T3' :
              $node['biblio_tertiary_title'] = $value;
              break;
            case 'TI' :
            case 'T1' :
            case 'CT' :
            case 'BT' :
              $node['title'] = $value;
              break;
            case 'UR' :
              $node['biblio_url'] = $value;
              break;
            case 'VL' :
              $node['biblio_volume'] = $value;
              break;
            case 'AB' :
            case 'N2' :
              $node['biblio_abst_e'] .= $value;
              break;
            case 'A3' :
              $node['biblio_contributors'][5][] = array(
                'name' => $value,
                'auth_type' => _biblio_get_auth_type(5, $node['biblio_type']));
              break;
            case 'N1' :
              $node['biblio_notes'] .= $value;
              break;
            case 'U1' : // CUSTOM 1
              $node['biblio_custom1'] = $value;
              break;
            case 'U2' : // CUSTOM 2
              $node['biblio_custom2'] = $value;
              break;
            case 'U3' : // CUSTOM 3
              $node['biblio_custom3'] = $value;
              break;
            case 'U4' : // CUSTOM 4
              $node['biblio_custom4'] = $value;
              break;
            case 'U5' : // CUSTOM 5
              $node['biblio_custom5'] = $value;
              break;
            case 'SN' :
              $node['biblio_isbn'] = $value;
              break;
            default :
              break;
      } //end switch
    }
    else {
      //$incite=false;
    } // end if ($start !== false)
  } // end while
  fclose($fp);
  return (!empty($nids)) ? $nids : array();
}