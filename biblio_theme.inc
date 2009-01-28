<?php
// $Id$
/**
 *
 *   Copyright (C) 2006-2008  Ron Jerome
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
 ****************************************************************************/
  module_load_include('inc','biblio','biblio.pages');

/**
 * @param $element
 * @return unknown_type
 */
function theme_biblio_coin_button($element) {
  return '<a href="/biblio/regen_coins"><input type="button"  name="'. $element['#name'] .'" value="'. $element['#value'] .'"  /></a>';
}

/**
 * @param $openURL
 * @return unknown_type
 */
function theme_biblio_openurl($openURL) {
  $openurl_image = variable_get('biblio_openurlimage', '');
  if ($openurl_image != '') {
    $output .= '<div class="biblio-openurl-image"><a href="'. $openURL .'"><img border="0" src="'. $openurl_image .'" /></a></div>';
  }
  else {
    $output .= '<div class="biblio-openurl-text"><a href="'. $openURL .'">'. t('Find It Via OpenURL!') .'</a></div>';
  }
  return $output;
}

/**
 * @param $node
 * @return unknown_type
 */
function biblio_openURL($node) {
  // Copyright:          Matthias Steffens <mailto:refbase@extracts.de> and the file's
  //                     original author.
  // Original Author:    Richard Karnesky <mailto:karnesky@gmail.com>  //
  // Adapted for biblio: Ron Jerome
  //  global $openURLResolver; // these variables are defined in 'ini.inc.php'
  //  global $hostInstitutionAbbrevName;
  $openURLResolver = variable_get('biblio_baseopenurl', '');
  $co = biblio_contextObject($node);
  $co["sid"] = "biblio:". variable_get('site_name', 'Drupal');
  $openURL = $openURLResolver;
  if (!ereg("\?", $openURLResolver))
    $openURL .= "?";
  else
    $openURL .= "&amp;";
  $openURL .= "ctx_ver=Z39.88-2004";
  foreach ($co as $coKey => $coValue) {
    $coKey = ereg_replace("rft.", "", $coKey);
    $coKey = ereg_replace("au[0-9]*", "au", $coKey);
    $openURL .= "&amp;". $coKey ."=". rawurlencode($coValue);
  }
  return $openURL;
}


/**
 * DEPRECIATED! this was the original output format which is not to flexable it will be removed
 * TODO: remove this function
 * @param $node
 * @param $base
 * @param $style
 * @return unknown_type
 */
function theme_biblio_long($node, $base = 'biblio', $style = 'classic') {
  if (module_exists('popups')){
     popups_add_popups();
  }
  if (empty ($node->biblio_coins))
    biblio_coins_generate($node);
  $output .= $node->biblio_coins;
  $layout = variable_get('biblio_node_layout', 'orig');
  if (variable_get('biblio_node_layout', 'orig') == 'ft' && strlen(trim($node->body)) && user_access('view full text')) {
    $output .= '<div class="biblio-head">'. theme('biblio_style', $node, $base, $style) .'</div>';
    $annotation_field = variable_get('biblio_annotations', 'none');
    if ($annotation_field != 'none' && $node-> $annotation_field) {
      $output .= '<div class="biblio-annotation">';
      $output .= check_markup($node-> $annotation_field, $node->format, FALSE);
      $output .= '</div>';
    }
    $output .= check_markup($node->body, $node->format, FALSE);
    if (biblio_access('export', $node)) {
      $output .= theme('biblio_export_links', $node->nid);
    }
    return $output;
  }
  foreach ((array)$node->biblio_contributors[1] as $auth) {
    $authors[] = _biblio_author_link($auth['name'], $auth['cid']);
  }
  $authors = implode('; ', (array)$authors);
  $openurl_base = variable_get('biblio_baseopenurl', '');
  if ($openurl_base)
    $output .= theme('biblio_openurl', biblio_openurl($node, $openurl_base));
  $output .= '<div class="biblio_type"><h3>'. t("Publication Type") .":</h3> $node->biblio_type_name</div>\n";
  $output .= '<div class="biblio_authors"><h3>'. t("Authors") .':</h3> '. $authors ."</div>\n";
  $output .= '<div class="biblio_source"><h3>'. t("Source") .': </h3> ';
  $source = null;
  if ($node->biblio_secondary_title)
    $source .= check_plain($node->biblio_secondary_title);
  if ($node->biblio_publisher) {
    $source .= $source ? ", " : "";
    $source .= check_plain($node->biblio_publisher);
  }
  if ($node->biblio_volume) {
    $source .= $source ? ", " : "";
    $source .= t('Volume ') . check_plain($node->biblio_volume);
  }
  if ($node->biblio_issue) {
    $source .= $source ? ", " : "";
    $source .= t('Issue ') . check_plain($node->biblio_issue);
  }
  if ($node->biblio_number) {
    $source .= $source ? ", " : "";
    $source .= t('Number ') . check_plain($node->biblio_number);
  }
  if ($node->biblio_place_published) {
    $source .= $source ? ", " : "";
    $source .= check_plain($node->biblio_place_published);
  }
  if ($node->biblio_pages) {
    $source .= $source ? ", " : "";
    $source .= 'p.'. check_plain($node->biblio_pages);
  }
  if (isset ($node->biblio_year)) {
    if ($node->biblio_year == 9998) {
      $node->biblio_year = t("In Press");
    }
    if ($node->biblio_year == 9999) {
      $node->biblio_year = t("Submitted");
    }
    $source .= ' ('. check_plain($node->biblio_year) .')';
  }
  $output .= "$source</div>\n";
  if ($node->biblio_isbn)
    $output .= '<h3>'. t("ISBN") .':</h3> '. check_plain($node->biblio_isbn) ."\n";
  if ($node->biblio_call_number)
    $output .= '<h3>'. t("Call Number") .':</h3> '. check_plain($node->biblio_call_number) ."\n";
  if ($node->biblio_accession_number)
    $output .= '<h3>'. t("Accession Number") .':</h3> '. check_plain($node->biblio_accession_number) ."\n";
  if ($node->biblio_other_number)
    $output .= '<h3>'. t("Other Number") .':</h3> '. check_plain($node->biblio_other_number) ."\n";
  if ($node->biblio_url) {
    $attrib = (variable_get('biblio_links_target_new_window', null)) ? array(
      'target' => '_blank'
    ) : array();
    $output .= '<h3>'. t("URL") .':</h3>'. l($node->biblio_url, $node->biblio_url, $attrib) ."\n";
  }
  if ($node->biblio_keywords)
    $output .= '<h3>'. t("Keywords") .':</h3> '. _biblio_keyword_links($node->biblio_keywords, $base) ."\n";
  if ($node->biblio_abst_e)
    $output .= '<h3>'. t("Abstract") .':</h3> '. check_markup($node->biblio_abst_e, $node->format, FALSE) ."\n";
  if ($node->biblio_abst_f)
    $output .= '<p>'. check_markup($node->biblio_abst_f, $node->format, FALSE) ."\n";
  if ($node->biblio_notes)
    $output .= '<h3>'. t("Notes") .': </h3>'. check_markup($node->biblio_notes, $node->format, FALSE) ."\n";
  /*  if ( strlen(trim($node->body)) && user_access('view full text') ) {
       $output .= '<h3>'.t('Full Text').':</h3>'.check_markup($node->body, $node->format, FALSE);
    }
  */
  if (biblio_access('export', $node)) {
    $output .= theme('biblio_export_links', $node->nid);
  }
  return $output;
}
/**
 * @param $node
 * @param $base
 * @param $teaser
 * @return unknown_type
 */
function theme_biblio_tabular($node, $base = 'biblio', $teaser = false) {
  
  if (module_exists('popups')){
     popups_add_popups();
  }
  $tid = $node->biblio_type;
  $style_name = variable_get('biblio_style', 'cse');
  module_load_include('inc','biblio',"biblio_style_$style_name");
  $fields = _biblio_get_field_information($node->biblio_type);
  $rows[] = array(
    array('data' => t('Title'), 'class' => 'biblio-row-title'),
    array('data' => $node->title)
  );
  $rows[] = array(
    array('data' => t('Publication Type'), 'class' => 'biblio-row-title'),
    array('data' => $node->biblio_type_name)
  );
  
  if ($node->biblio_keywords &&  !variable_get('biblio_keyword_vocabulary', 0)) {
    $node->biblio_keywords = _biblio_keyword_links($node->biblio_keywords, $base);
  }
  if ($node->biblio_url) {
    $attrib = (variable_get('biblio_links_target_new_window', null)) ? array(
      'target' => '_blank'
    ) : array();
    $node->biblio_url = l($node->biblio_url, $node->biblio_url, $attrib);
  }

  foreach ($fields as $key => $row) {
    // handling the contributor categories like any other field orders them correctly by weight
    if ($row['type'] == 'contrib_widget' && !empty($node->biblio_contributors[$row['fid']])) {
      $data =  theme('biblio_authors', $node->biblio_contributors[$row['fid']], $style_name, $row['fid']) ;
      if (!$data) {
  //      if ($row['fid'] == 1) $data = t('No author information available');
  //      else continue;
      }
    }
    else if (empty ($node->$row['name']) || $row['name'] == 'biblio_coins') continue;
    else {
      switch ($row['name']) {
        case 'biblio_keywords' :
        case 'biblio_url' :
          // check_plain is not need on these since they have gone through
          // the l() function which does a check_plain
          $data = $node-> $row['name'];
          break;
        default :
          if ($row['type'] == 'textarea') {
            $data = check_markup($node-> $row['name'], $node->format, FALSE);
          }
          else {
            $data = check_plain($node-> $row['name']);
          }
      }
    }
    $rows[] = array(
      array(
        'data' => $row['title'],
        'class' => 'biblio-row-title'
      ),
      array(
        'data' => $data
      )
    );
  }

  if (biblio_access('export', $node) && $teaser == false) {
    $rows[] = array(
      array(
        'data' => t('Export'),
      'class' => 'biblio-row-title'
    ), array(
      'data' => theme('biblio_export_links', $node->nid) )); 
  }
  $output = '<div id="biblio-node">';
  $output .= $node->biblio_coins;
  $header = array();
  $output .= theme('table', $header, $rows);
  $output .= '</div>';
  return $output;
}

function theme_biblio_authors($contributors, $style = 'classic', $cat = 1, $inline = false, $glue = ', ') {
  if (empty($contributors)) return; // t('No author information available');
  $author_style_function = '_'.$style.'_format_author';
  $author_links = variable_get('biblio_author_links', 1);
  $base  = variable_get('biblio_base', 'biblio');
  foreach ($contributors as $key => $author) {
    $author = biblio_parse_author($author, $cat); // this is needed for form preview to fill in all fields
    if (strlen($author['name'])) {
      $format = $author_style_function($author);
      if ($author_links) $format = _biblio_author_link($format, $author['cid'], $base, $inline);
      // add the auth_type as css id to allow later formatting
      $author_array[] = '<span id="'.$author['auth_type'].'">'.$format.'</span>';
    }
  }
  if (empty($author_array)) return '';

  $output  = '<span class="biblio_authors">';
  $output .= implode($glue, $author_array);
  $output .= '</span>';
  return $output;
}
/**
 * Applies a "style" function to a single node.
 * 
 * @param $node A node
 * @param $base The base url for biblio (defaults to /biblio)
 * @param $style_name The name of the style to apply
 * @param $inline "inline" mode returns the raw HTML rather than letting drupal render the whole page.
 * @return A string containing the styled (HTML) node 
 */
function theme_biblio_style(& $node, $base = 'biblio', $style_name = 'classic', $inline = false) {
  module_load_include('inc','biblio',"biblio_style_$style_name");

  $style_function = "biblio_style_$style_name";
  $styled_node = $style_function ($node, $base, $inline);
  if (empty ($node->biblio_coins))
    biblio_coins_generate($node);
  return ($styled_node . $node->biblio_coins);
}

/**
 * @param $node
 * @param $base
 * @param $style
 * @param $inline
 * @return unknown_type
 */
function theme_biblio_entry($node, $base = 'biblio', $style = 'classic', $inline = false) {
  $output  = '<div class="biblio-entry">' . "\n";
  
// first add the styled entry...
  $output .= theme('biblio_style', $node, $base, $style, $inline);

// now add the various links   
  if ($node->biblio_abst_e) {
    $output .= '<span class="biblio-abstract-link">';
    $output .= l(" Abstract", "node/$node->nid") ."\n";
    $output .= '</span>';
  }
  $annotation_field = variable_get('biblio-annotations', 'none');
  if ($annotation_field != 'none' && $node-> $annotation_field) {
    $output .= '<div class="biblio-annotation">';
    $output .= check_markup($node-> $annotation_field, FILTER_HTML_ESCAPE, FALSE);
    $output .= '</div>';
  }
  $openurl_base = variable_get('biblio_baseopenurl', '');

  if ($openurl_base){
    $output .= theme('biblio_openurl', biblio_openurl($node));
  }    

  if (biblio_access('export', $node)) {
    $base = variable_get('biblio_base', 'biblio');
    $output .= theme('biblio_export_links',$node->nid);
  }

  if (biblio_access('download', $node)) {
    // add links to attached files (if any)
    $output .= theme('biblio_download_links',$node);
  }

  $output .= "</div>\n";
  
  return $output;
}

/**
 * @param $form
 * @return unknown_type
 */
function theme_biblio_filters(& $form) {
  if (sizeof($form['current'])) {
    $output .= '<ul>';
    foreach (element_children($form['current']) as $key) {
      $output .= '<li>'. drupal_render($form['current'][$key]) .'</li>';
    }
    $output .= '</ul>';
  }
  $output .= '<dl class="multiselect">'. (sizeof($form['current']) ? '<dt><em>'. t('and') .'</em> '. t('where') .'</dt>' : '') .'<dd class="a">';
  foreach (element_children($form['filter']) as $key) {
    $output .= drupal_render($form['filter'][$key]);
  }
  $output .= '</dd>';
  $output .= '<dt>'. t('is') .'</dt><dd class="b">';
  foreach (element_children($form['status']) as $key) {
    $output .= drupal_render($form['status'][$key]);
  }
  $output .= '</dd>';
  $output .= '</dl>';
  $output .= '<div class="container-inline" id="node-buttons">'. drupal_render($form['buttons']) .'</div>';
  $output .= '<br class="clear" />';
  return $output;
}

/**
 * @param $form
 * @return unknown_type
 */
function theme_biblio_form_filter(& $form) {
  $output .= '<div id="biblio-admin-filter">';
  $output .= drupal_render($form['filters']);
  $output .= '</div>';
  $output .= drupal_render($form);
  return $output;
}

/**
 * @param $form
 * @return unknown_type
 */
function theme_biblio_admin_types_edit_form($form) {
  drupal_add_tabledrag('field-table', 'order', 'sibling', 'weight', array('hidden' => false));

  $base = variable_get('biblio_base', 'biblio');
  $tid = (!empty ($form['#parameters'][2])) ? $form['#parameters'][2] : FALSE;
  drupal_set_title($form['type_name'] ? $form['type_name']['#value'] : "Default");

  // build the table with all the fields if no $tid is given, or only the common
  // and customized fields if $tid is given
  $conf_table = array();
  foreach (element_children($form['configured_flds']) as $fld) {
    $form['configured_flds'][$fld]['weight']['#attributes']['class'] = 'weight';
    
    $conf_row = array();
    $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['name']));
    $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['title']));
    if (isset($form['configured_flds'][$fld]['auth_type'])) {
      $form['configured_flds'][$fld]['hint']['#size'] = 15;
      $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['hint']));
      $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['auth_type']));
    } else {
      $form['configured_flds'][$fld]['hint']['#size'] = ($tid ? 38: 36);
      $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['hint']), 'colspan' => 2);
    }
    foreach (element_children($form['configured_flds'][$fld]['checkboxes']) as $oid) {
      if (is_array($form['configured_flds'][$fld]['checkboxes'])) {
        $conf_row[] = array(
          'data' => drupal_render($form['configured_flds'][$fld]['checkboxes'][$oid]),
          'title' => $oid);
      }
    }
    $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['weight']));
    $conf_table[] = array('data' => $conf_row, 'class' => 'draggable');
  }
  if ($tid) {
    $header = array(t('Field Name'), t('Title'), t('Hint'), '', t('Visible'), t('Required'), t('Weight'));
  }
  else {
    $header = array(t('Field Name'), t('Default Title'), t('Hint'), '', t('Common'), t('Required'), t('Autocomplete'), t('Weight'));
  }
  $output .= '<p>';
  $output .= drupal_render($form['help']);
  $output .= theme('table', $header, $conf_table, array('id' => 'field-table'));
  $output .= '<p><center>'. drupal_render($form['submit']) .'</center></p>';
  $output .= drupal_render($form);
  return $output;
}

function theme_biblio_download_links($node = NULL) {
    if (!empty ($node->files) && count($node->files) > 0 && user_access('view uploaded files')) {
      $files .= '<span class="biblio_file_links">';
      $files .= '&nbsp;'. t('Download') .':&nbsp;';
      $file_count = 0;
      foreach ($node->files as $file) {
        if ($file->list) {
          $file_count++;
          $href = file_create_url($file->filepath);
          $text = $file->description ? $file->description : $file->filename;
          $files .= l($text, $href) .'&nbsp;('. format_size($file->filesize).')';
        }
      }
      $files .= '</span>';
      if ($file_count) return $files;
    }
    return '';
}

/**
 * Creates a group of links for the various export functions
 * @param $nid the node id to export (if omitted, all nodes in the current view will be exported
 * @return an un-ordered list of class "biblio-export-buttons"
 */
function theme_biblio_export_links($nid = NULL) {
  $base = variable_get('biblio_base', 'biblio');
  $links = '';
  $links .= '<ul class="biblio-export-buttons">';
  $links .= '<li> '. theme('biblio_tagged_link', $base, $nid) .'</li>';
  $links .= '<li> '. theme('biblio_xml_link', $base, $nid) .'</li>';
  $links .= '<li> '. theme('biblio_bibtex_link', $base, $nid) .'</li></ul>';
  return $links;
}

/** 
 * Creates a link to export a node (or view) in EndNote Tagged format
 * 
 * @param $base this is the base url (defaults to /biblio)
 * @param $nid  the node id, if NULL then the current view is exported
 * @return  a link (<a href=...>Tagged</a>)
 */
function theme_biblio_tagged_link($base, $nid = NULL) {
  if (module_exists('popups') && !empty($nid)) {
    $output = l(t("Tagged"), "$base/export/tagged/$nid/popup", array('attributes'=>array('class' => 'popups', 'title' => t("Click to get the EndNote Tagged output "))));
  } else {
    $output = l(t("Tagged"), "$base/export/tagged/$nid", array('attributes'=>array('title' => t("Click to download the EndNote Tagged formated file"))));
  }
  return $output;
}

/**
 * Creates a link to export a node (or view) in BibTEX format
 * 
 * @param $base this is the base url (defaults to /biblio)
 * @param $nid  the node id, if NULL then the current view is exported
 * @return  a link (<a href=...>BibTEX</a>)
 */
function theme_biblio_bibtex_link($base, $nid = NULL) {
  if (module_exists('popups') && !empty($nid)) {
    $output = l(t("BibTex"), "$base/export/bibtex/$nid/popup",array('attributes'=>array('class' => 'popups', 'title' => t("Click to get the BibTEX output "))));
  } else {
    $output = l(t("BibTex"), "$base/export/bibtex/$nid", array('attributes'=>array('title' => t("Click to download the BibTEX formated file"))));
  }
  return $output;
}

/**
 * Creates a link to export a node (or view) in XML format
 * 
 * @param $base this is the base url (defaults to /biblio)
 * @param $nid  the node id, if NULL then the current view is exported
 * @return  a link (<a href=...>XML</a>)
 */
function theme_biblio_xml_link($base, $nid = NULL) {
  if (module_exists('popups') && !empty($nid)) {
    $output = l(t("XML"), "$base/export/xml/$nid/popup",array('attributes'=>array('class' => 'popups', 'title' => t("Click to get the XML output "))));
  } else {
    $output = l(t("XML"), "$base/export/xml/$nid", array('attributes'=>array('title' => t("Click to download the XML formated file"))));
  }
  return $output;
}

/**
 * @param $form
 * @return unknown_type
 */
function theme_biblio_contributors($form) {
  $rows = array();
  if ($form['#hideRole']) $headers = array('', t('Name'), t('Weight'));
  else $headers = array('', t('Name'), t('Role'), t('Weight'));
  drupal_add_tabledrag($form['#id'], 'order', 'sibling', 'rank');
  
  foreach (element_children($form) as $key) {
    // No need to print the field title every time.
    unset ($form[$key]['name']['#title'], $form[$key]['auth_type']['#title']);
    // Add class to group weight fields for drag and drop.
    $form[$key]['rank']['#attributes']['class'] = 'rank';
    
    // Build the table row.
    $row = array('');
    $row[] = array('data' => drupal_render($form[$key]['name']),
              		 'class' => 'biblio-contributor');
    if (!$form['#hideRole']) {
      $row[] = array('data' => drupal_render($form[$key]['auth_type']),
      							 'class' => 'biblio-contributor-type');
    }
    $row[] = drupal_render($form[$key]['rank']);
    $rows[] = array('data' => $row, 'class' => 'draggable');
  }
  $output = theme('table', $headers, $rows, array('id' => $form['#id']));
  $output .= drupal_render($form);
  return $output;
}

/**
 * This function creates a string of letters (A - Z), which 
 * depending on the sorting are either linked to author or title 
 * filters i.e. clicking on the A when in the listing is sorted by 
 * authors will bring up a list of all the entries where the first 
 * character of the primary authors last name is "A"
 * 
 * @param $type either "author or title"
 * @return a chunk of HTML code as described above
 */
function theme_biblio_alpha_line($type = 'author',$current = NULL) {
  $options = array();
  $base = variable_get('biblio_base', 'biblio');
  $all = '';
  switch ($type){
    case 'authors':
      $group = 'authors/';
      $all = '['.l(t('Show ALL'), "$base/authors").']' ;
      break;
    case 'author':
        $options['query'] .= 'sort=author';
        $group = 'ag/';
        $all_query['query'] = 'sort=author'. ($_GET['order']?'&order='.$_GET['order']:''); 
        $all = '['.l(t('Show ALL'), $base,$all_query).']' ;
        break;
    case 'title':
        $options['query'] .= 'sort=title';
        $group = 'tg/';
      break;      
    default:
      if (!isset ($_GET['sort']) || $_GET['sort'] == 'year' || $_GET['sort'] == 'type')
      return;
      $inline = $inline ? "/inline" : "";
      if (isset ($_GET['sort'])) {
        $options['query'] .= "sort=". $_GET['sort'];
        if ($_GET['sort'] == 'author')
        $group = 'ag/';
        if ($_GET['sort'] == 'title')
        $group = 'tg/';
      }
      
  }
  if (isset ($_GET['order'])) {
    $options['query'] .= $options['query'] ? "&" : "";
    $options['query'] .= "order=". $_GET['order'];
  }
  $output = '<div class="biblio-alpha-line">';
  for ($i = 65; $i <= 90; $i++) {
    if ($i == ord($current)){
      $output .= '<b>['.chr($i).']</b>&nbsp;';
    }
    else{
      $output .= l(chr($i), "$base/$group". chr($i), $options) .'&nbsp;';
    }
  }
  if($current) $output .= '&nbsp;&nbsp;'.$all;
  $output .= '</div>';
  return $output;
}

/**
 * Themes the author editing form
 * 
 * @param $form
 * @return rendered form
 */
function theme_biblio_admin_author_edit_form($form){ 
  $rows = array();
  $header = array();
  $rows[] = array(
            array('data' => drupal_render($form['prefix'])),
            array('data' => drupal_render($form['firstname'])),
            array('data' => drupal_render($form['initials'])),
            array('data' => drupal_render($form['lastname'])),
            array('data' => drupal_render($form['suffix'])));
  $rows[] = array(array('data' => drupal_render($form['name']), 'colspan' => 5));
  $rows[] = array(array('data' => drupal_render($form['affiliation']), 'colspan' => 5));
  $rows[] = array(array('data' => drupal_render($form['drupal_uid']), 'colspan' => 5));
  
  $output = theme('table', $header, $rows);
  $output .= drupal_render($form['merge']);
  $output .= drupal_render($form);
  
  return $output;
  
}

function theme_biblio_admin_orphans_form($form) {
  // If there are any orphans, then $form['name'] contains a list of the author names
  $has_items = isset($form['name']) && is_array($form['name']);
  $select_header = $has_items ? theme('table_select_header_cell') : '';
  $header = array($select_header, t('Author Name'), t('Affiliation'));

  $output = '';
  $output .= drupal_render($form['submit']);
  if ($has_items) {
    foreach (element_children($form['name']) as $key) {
      $row = array();
      $row[] = drupal_render($form['authors'][$key]);
      $row[] = drupal_render($form['name'][$key]);
      $row[] = drupal_render($form['affiliation'][$key]);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('No orphaned authors available.'), 'colspan' => '3'));
  }
  $output .= theme('table', $header, $rows);

  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);

  return $output;
}