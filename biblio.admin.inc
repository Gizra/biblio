<?php
// $Id$
/** 
 *   biblio.admin.inc
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
 */  
function _biblio_admin_build_user_select($my_uid) {
  $sql = 'SELECT DISTINCT u.uid, u.name, u.status, u.mail FROM {users} u  WHERE u.uid != 0 ';
  $result = db_query($sql);
  while ($user = db_fetch_object($result)) {
      $users[$user->uid] = $user->name ." ($user->mail)";   
  }
  asort($users);
  $select = array(
    '#type' => 'select',
    '#title' => t("Set user ID of entries in this file to"),
    '#options' => $users,
    '#value' => $my_uid
  );
  return $select;
}

function biblio_admin_import_form() {
global $user;
  if (biblio_access('import', $node)) { // && !user_access('administer nodes')) {

      $form['#attributes']['enctype'] = 'multipart/form-data';
      $form["file_upload"] = array(
        '#type' => 'file',
        '#title' => t('Import file'),
        '#default_value' => $edit['file_upload'],
        '#size' => 60,
      );
      $form['filetype'] = array(
        '#type' => 'radios',
        '#title' => t('File Type'),
        '#default_value' => $edit['filetype'],
        '#options' => array('tagged' => t('EndNote Tagged'),'ris' => t('RIS') , 'xml'=>t('EndNote 7 XML'), 'xml8'=>t('EndNote 8+ XML'), 'bib'=>t('BibTex')),
      );
      if ($user->uid) {
        $form{'userid'} =  _biblio_admin_build_user_select($user->uid);
      }  

   // Get the vocabularies  attached to the biblio node type ...
      $vocabularies = module_invoke('taxonomy', 'get_vocabularies', 'biblio');
      // ... and print a form to select the terms in each of them
      $form['import_taxonomy'] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#title' => t('Taxonomy Settings'),
        '#description' => t('Here you may select terms to be assigned to imported records (if any vocabularies are assigned to the biblio node type)'),
      );

      if (count($vocabularies)) {
        $i=0;
        foreach ($vocabularies as $vocabulary) {
          $form['import_taxonomy']['vocabulary'.$i] = module_invoke('taxonomy', 'form', $vocabulary->vid, 0);     
          $form['import_taxonomy']['vocabulary'.$i++]['#description'] = t("Select taxonomy term to assigned to imported entries");
        }
      }else{
        $vocab_msg = t('There are currently no vocabularies assigned to the biblio node type, please go the the !url page to fix this', array('!url'=> l(t('admin/taxonomy'),'admin/taxonomy')));
        $form['import_taxonomy']['vocabulary_message'] = array('#value' => '<p><div>'.$vocab_msg.'</div></p>');
      }      
        
      if (variable_get('biblio_keyword_freetagging', 0)) {
        $freetag_vocab = module_invoke('taxonomy', 'get_vocabulary', variable_get('biblio_freetagging_vocab', 0));
        $msg = t('Keywords will be added to the <b>@name</b> vocabulary as specified in the Taxonomy section of the !url page.', array('@name'=> $freetag_vocab->name, '!url' => l(t('admin/settings/biblio'),'admin/settings/biblio')));
      }else{
        $msg = t('Keywords will <b>NOT</b> be added to the vocabulary as specified in the Taxonomy section of the !url page.', array('!url' => l(t('admin/settings/biblio'),'admin/settings/biblio')));
      } 
      $form['import_taxonomy']['freetagging_information'] = array('#value' => '<p><div>'.$msg.'</div></p>');
        
      
      $form['button'] = array(
        '#type' => 'submit',
        '#value' => t('Import')
      );

      return $form;
      //print theme('page', form($form, 'post', null, $attributes));
     
  } else {
    drupal_set_message("You are not authorized to access the biblio import page", 'error');

    print theme('page', '');
  } 
} 

function biblio_admin_import_form_validate($form, &$form_state) {

    $op = $form_state['values']['op'];
    $filetype = $form_state['values']['filetype'];
    if (!file_save_upload('file_upload')) {
        form_set_error('',t("You must provide a file name")); 
    }
    if ($op == t('Import') && $filetype == '') {
       form_set_error('',t("You must select a file type"));
    }

}

function biblio_admin_import_form_submit($form, &$form_state) {

    if ($form_state['values']['op'] == t('Import') && isset($form_state['values']['filetype'])) {
      
      if ($import = file_save_upload('file_upload')) {
        drupal_set_message(t("@file was uploaded", array('@file' => $import->filename)), 'status');

        // Concatenate all the terms of the different vocabularies
        // in a single array to be sent to _biblio_import
        $terms = array();
        foreach (array_keys($form_state['values']) as $key) {
          if (preg_match('/(vocabulary[0-9]+)/', $key)) {
            if ($form_state['values'][$key] > 0) $terms[] = $form_state['values'][$key];
          }
        }
        // Added the $terms argument
        // the array of terms to be attached to the node(s)
        $userid = (isset($form_state['values']['userid'])? $form_state['values']['userid'] : 1;
        $filetype = $form_state['values']['filetype'];
        $content = _biblio_import($userid, $import->filepath,$filetype, $terms);
        file_delete($import->filepath);

      } else {
        drupal_set_message(t("File was NOT successfully uploaded"), 'error');
      } 
  }
}

/**
* Implementation of hook_settings().
*/
function biblio_admin_settings() {
  $version = '$Revision$ $Date$';
  $version = str_replace('$', '', $version);
  $form['biblio_rev'] = array(
     '#value' => $version,
  );
  $form['biblio_base'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL'),
    '#size' => 20,
    '#default_value' => variable_get('biblio_base', 'biblio'),
    '#description' =>  t('This sets the base URL used to access the biblio module (e.g. /biblio ).'),
  );
  $form['biblio_rowsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of results per page'),
    '#default_value' => variable_get('biblio_rowsperpage', 25),
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t('This sets the number of results that will be displayed per page.'),
  );
  $form['biblio_view_only_own'] = array(
    '#type' => 'checkbox',
    '#title' => t('Restrict users such that they can only view their own biblio entries'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_view_only_own', 0),
    '#description' => t('This option restricts the users capability to view biblio entries.  They will only be able to see the entries which they have created and own.') );
  $form['block'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Block settings'),
    '#description' => '',
  );
  $form['block']['biblio_block_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title for the "New Publications" block'),
    '#default_value' => variable_get('biblio_block_title', 'New Publications'),
    '#size' => 40,
    '#maxlength' => 40,
    '#description' => '',
  );
  $form['block']['biblio_rowsperblock'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of results in the "New Publications" block'),
    '#default_value' => variable_get('biblio_rowsperblock', 4),
    '#size' => 2,
    '#maxlength' => 2,
    '#description' => t('This sets the number of results that will be displayed in the "New Publications" block.'),
  );
  $form['block']['biblio_block_order'] = array(
    '#type' => 'radios',
    '#title' => t('Order by'),
    '#default_value' => variable_get('biblio_block_order', 'n.created'),
    '#options' => array('n.created' => t('Date Created'), 'b.biblio_year' => t('Year Published')),
  );
  
  $form['footnotes'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Footnotes'),
    '#description' => t('You can integrate with the !url module here.', array('!url' => l('footnotes', url("http://www.drupal.org/project/footnotes", array('query' => NULL, 'fragment' => NULL, 'absolute' => TRUE))))),
  );
  if (!module_exists('footnotes')) {
    $additional_text = '<div class="admin-dependencies">'. t('Depends on') .': '. t('Footnotes') .' (<span class="admin-disabled">'. t('disabled') .'</span>)</div>';
    $disabled = TRUE;
    variable_set('biblio_footnotes_integration', 0);
  }
  else{
    $additional_text = '<div class="admin-dependencies">'. t('Depends on') .': '. t('Footnotes') .' (<span class="admin-enabled">'. t('enabled') .'</span>)</div>';
    $disabled = FALSE;

  }
  $form['footnotes']['biblio_footnotes_integration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Integration with the footnotes module') . $additional_text,
    '#disabled' => $disabled,
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_footnotes_integration', 0),
    '#description' => t('This will convert &lt;bib&gt; tags into &lt;fn&gt; tags.  This will cause intermingled &lt;bib&gt; and &lt;fn&gt; tags to be sequentially numbered.  For this to work, you must put the &lt;bib&gt; filter ahead of the &lt;fn&gt; filter in the filter chain.  If this option is not set, &lt;bib&gt; and &lt;fn&gt; tags will be handled separately.'), 
  );
  $form['keywords'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Keywords'),
    '#description' => '',
  );
  $form['keywords']['biblio_keyword_sep'] = array(
    '#type' => 'textfield',
    '#title' => t('Keyword separator'),
    '#size' => 2,
    '#default_value' => variable_get('biblio_keyword_sep', ','),
    '#description' =>  t('Enter the character which will be used to separate multiple keywords in the keyword field'),
  );
  $form['links'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Links'),

  );
  $form['links']['biblio_links_target_new_window'] = array(
    '#type' => 'checkbox',
    '#title' => t('Links open in new browser'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_links_target_new_window', 0),
    '#description' => t('This causes related URLs to open in a new browser window') );
/*  $form['links']['biblio_inlinemode_in_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Carry "inline" mode through to all links'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_inlinemode_in_links', 0),
    '#description' => t('This causes the "inline" mode to be applied to all links such as titles, authors and keywords') );
*/
  $form['links']['biblio_link_title_url'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hyperlink titles using supplied URL if available'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_link_title_url', 0),
    '#description' => t('Selecting this links the titles to the supplied URL (if available) rather than the "node" view.') );
  $form['openurl'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('OpenURL'),
    '#description' => t('You can set an <a href="http://en.wikipedia.org/wiki/OpenURL">openurl</a> link here'),
  );
  $form['openurl']['biblio_baseopenurl'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenURL Base URL'),
    '#size' => 95,
    '#default_value' => variable_get('biblio_baseopenurl', ''),
    '#description' =>  t('This sets your institution\'s base <a href="http://en.wikipedia.org/wiki/OpenURL">OpenURL</a> gateway, which is used to generate OpenURL links. To implement a "Universal" OpenURL system, try using OCLC\'s <a href="http://www.oclc.org/productworks/urlresolver.htm">OpenURL Resolver Registry</a> gateway: <a href="http://worldcatlibraries.org/registry/gateway">http://worldcatlibraries.org/registry/gateway</a>'),
  );
  $form['openurl']['biblio_openurlimage'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenURL Image'),
    '#size' => 95,
    '#default_value' => variable_get('biblio_openurlimage', ''),
    '#description' =>  t('Enter a path to your image here, this image will be used as button which when clicked will find the entry via the OpenURL link'),
  );
  
  _biblio_get_user_profile_form($form);

  $form['sort'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Sorting'),
    '#description' => t('You can set the default sorting and ordering for the /biblio page here.'),
  );
  $form['sort']['biblio_sort'] = array(
    '#type' => 'radios',
    '#title' => t('Sort by'),
    '#default_value' => variable_get('biblio_sort', 'year'),
    '#options' => array('author' => t('Author'), 'keyword' => t('Keyword'), 'title' => t('Title'), 'type' => t('Type'), 'year' => t('Year')),
  );
  $form['sort']['biblio_sort_tabs'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show sort links'),
    '#default_value' => variable_get('biblio_sort_tabs', array('author', 'title', 'type', 'year')),
    '#options' => array('author' => t('Author'), 'keyword' => t('Keyword'), 'title' => t('Title'), 'type' => t('Type'), 'year' => t('Year')),
    '#description' => t('You turn the sorting links at the top of the /biblio page here.'),
  );
  $form['sort']['biblio_order'] = array(
    '#type' => 'radios',
    '#title' => t('Order'),
    '#default_value' => variable_get('biblio_order', 'DESC'),
    '#options' => array('DESC' => t('Descending'), 'ASC' => t('Ascending')),
  );
  $form['style'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Styling'),
    '#description' => t('You can set the default style for the /biblio page here.'),
  );
  $form['style']['biblio_normalize'] = array(
    '#type' => 'checkbox',
    '#title' => t('Normalize author names when displaying biblio records'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_normalize', 0),
    '#description' => t('Tries (doesn\'t always work) to reformat author names so that they are displayed in the format "Lastname, Initials" e.g. Smith, J.S. (Note: This setting does not modify the entry in the database, it only reformats it\'s presentation. This option can be turned off at any time to diplay the oringal format.)'), );
  $form['style']['biblio_node_layout'] = array(
    '#type' => 'radios',
    '#title' => t('Node Layout'),
    '#default_value' => variable_get('biblio_node_layout', 'orig'),
    '#options' => array('orig' => t('Original'), 'ft' => t('Only Fulltext if available'), 'tabular' => t('Tabular')),
    '#description' => t('This alters the layout of the "node" (full) view.'),
  );
  $form['style']['biblio_annotations'] = array(
    '#type' => 'select',
    '#title' => t('Annotations'),
    '#default_value' => variable_get('biblio_annotations', 'none'),
    '#options' => array('none' => t('none'), 'biblio_notes' => t('notes'), 'biblio_custom1' => t('custom1'), 'biblio_custom2' => t('custom2'), 'biblio_custom3' => t('custom3'), 'biblio_custom4' => t('custom4'), 'biblio_custom5' => t('custom5'), 'biblio_custom6' => t('custom6'), 'biblio_custom7' => t('custom7')),
    '#description' => t('Select a field from which an annotation will be displayed below biblo entry in "short" listings'),
    '#multiple' => FALSE,
    '#size' =>  0
  );
  $form['style']['biblio_style'] = array(
    '#type' => 'radios',
    '#title' => t('Style'),
    '#default_value' => variable_get('biblio_style', 'classic'),
    '#options' => _biblio_admin_get_styles(),
    '#description' => t('This alters the layout of the "list" (short) view.'),
  );
  $form['syndication'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Syndication'),
    '#description' => t('You can set the RSS defaults here.'),
  );
  $form['syndication']['biblio_rss'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow RSS feeds of new biblio entries'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_rss', 0),
    '#description' => t('This will create an rss feed of the 10 most recent biblio entries. It will be available at /biblio/rss.xml'), 
  );
  $form['syndication']['biblio_rss_number_of_entries'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of items in the RSS feed.'),
    '#default_value' => variable_get('biblio_rss_number_of_entries', 10),
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t('Limits the number of items in the /biblio/rss.xml feed to this number.'),
  );

  $form['taxo'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Taxonomy'),
    '#description' => t('You can set the Taxonomy defaults here.'),
  );
  $form['taxo']['biblio_keyword_freetagging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use keywords from biblio entries as taxonomy "free tags"'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_keyword_freetagging', 0),
    '#description' => t('This option allows user to add keywords (free tags) to describe their documents. These keywords will be registered as taxonomy.'),
   );
  $vocabularies = module_invoke('taxonomy', 'get_vocabularies');
   // ... and print a form to select the terms in each of them
  $choice = new stdClass();
  $choice->option = array(0 =>  '<'. t('none') .'>');
  $options[0] = $choice;
  if (count($vocabularies)) {
    foreach ($vocabularies as $voc) {
        $choice = new stdClass();
        $choice->option = array($voc->vid =>  $voc->name);
        $options[] = $choice;
    }
  
    $form['taxo']['biblio_freetagging_vocab'] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary'),
    '#default_value' => variable_get('biblio_freetagging_vocab', 0),
    '#options' => $options,
    '#description' => t('Select vocabulary (category) to use for free tags.'),
    '#multiple' => FALSE,
    '#disabled' => (!variable_get('biblio_keyword_freetagging', 0)),
    '#size' => $multiple ? min(9, count($options)) : 0,
    '#weight' => 15,
    );

  }

  return system_settings_form($form);
}

function biblio_admin_types_edit_form() {
  if ($arg_list = func_get_args()) {// show an existing type  
    $tid = $arg_list[1];
  }  

  // first get all of the field info
  $result = db_query('SELECT * FROM {biblio_fields} as b  ORDER BY b.weight ASC');
  while ($row = db_fetch_array($result)) {
    if ($tid) {
      if ($row['common']) {
        $fields[$row['fid']] = $row;
      }else{
         $other_fields[$row['fid']] = $row;     
      }
    }else{
      $fields[$row['fid']] = $row;
    }
  }
  $form['configured_flds']= array('#tree'=>1);
  $form['cust'] = array('#tree'=>1);
  if ($tid) {// show an existing type  

    $result = db_query('SELECT t.* FROM {biblio_types} as t WHERE t.tid = %d',$tid);
    $row = db_fetch_array($result);
    $form['type_name'] =array('#type'=>'value','#title'=>'tid','#value'=>$row['name']);
  
   // now get any type specific customizations 
    $result = db_query('SELECT d.*, f.name FROM {biblio_type_details} as d INNER JOIN {biblio_fields} as f on d.fid=f.fid where d.tid=%d  ORDER BY d.weight ASC', $tid);
    while ($row = db_fetch_array($result)) {
      $type_fields[$row['fid']] = $row;
      $form['cust'][$row['fid']] =array('#type'=>'value','#value'=>$row['fid']);
      unset($other_fields[$row['fid']]);
    }
    if (count($type_fields)) {
    // now merge the customizations with the main field array
    foreach($type_fields as $key=>$value) {
        $fields[$key] = (isset($fields[$key])) ? array_merge($fields[$key],$value) : $value;
    } 
    }

    $form['tid'] =array('#type'=>'value','#value'=>$tid);
  } 

  uasort($fields, "_biblio_form_sort"); // resort the fields since the weight may have changed

  $vis_comm = $tid?'visible':'common';
  if (!$tid) $options["$vis_comm"] = '';
  $options['required'] = '';
  if (!$tid) $options['autocomplete'] = '';

  foreach($fields as $key=>$fld) {
    if ($fld['common']) $def_values[$fld['name']][] = 'common';
    if ($fld['required']) $def_values[$fld['name']][] = 'required';
    if ($fld['autocomplete']) $def_values[$fld['name']][] = 'autocomplete';

    $form['configured_flds'][$key]['name'] = array('#type'=>'markup', '#value'=>$fld['name'],'#weight'=>$fld['weight']);
    $form['configured_flds'][$key]['title'] = array('#type'=>'textfield','#default_value' => $fld['title'],'#size'=>15,'#weight'=>$fld['weight']);
    $form['configured_flds'][$key]['weight'] = array('#type'=>'textfield','#default_value' => $fld['weight'],'#size'=>2,'#weight'=>$fld['weight']);
    $form['configured_flds'][$key]['hint'] = array('#type'=>'textfield','#default_value' => $fld['hint'],'#size'=>10,'#weight'=>$fld['weight']);
    //if (!$tid) {
    //  $form['configured_flds'][$key]['hint']['#type']= 'markup';
    //  $form['configured_flds'][$key]['title']['#type']= 'markup';
    //}
    $form['configured_flds'][$key]['checkboxes'] = array('#type'=>'checkboxes','#options'=>$options,'#default_value' =>$def_values[$fld['name']],'#weight'=>$fld['weight']);      
  }

  if ($tid) {
    foreach($other_fields as $key=>$fld) {
      $form['avail_flds'][$key]['name'] = array('#type'=>'markup', '#value'=>$fld['name'],'#weight'=>$fld['weight']);
      $form['avail_flds'][$key]['title'] = array('#type'=>'markup','#value' => $fld['title'],'#weight'=>$fld['weight']);
      $form['avail_flds'][$key]['size'] = array('#type'=>'markup','#value' => $fld['maxsize'],'#weight'=>$fld['weight']);
    }
  }
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  
  return $form;
}

function biblio_admin_types_edit_form_submit($form, &$form_state) {
  $tid = $edit['tid']?$edit['tid']:false;
  if ($tid) {
    $type_query = "UPDATE {biblio_type_details} SET 
                      title = '%s' , weight = %d ,
                      hint = '%s' , required = %d
                      WHERE fid = %d AND tid = %d";
  }else{
    $field_query = "UPDATE {biblio_fields} SET title = '%s', weight = %d, 
                    common = %d, required = %d, autocomplete = %d, hint = '%s' WHERE fid = %d";  
  }

  foreach ($form_state['values']['configured_flds'] as $key=>$v) {
    $common = ($v['checkboxes']['common'])?TRUE:FALSE;
    $required = ($v['checkboxes']['required'])?TRUE:FALSE;
    $autocomplete = ($v['checkboxes']['autocomplete'])?TRUE:FALSE;
    if (is_numeric($key) ) {
      if ($tid && $edit['cust'][$key]) { // is type and not a common field update biblio_type_details
        db_query($type_query, $v['title'],$v['weight'],$v['hint'],$required,$key,$tid);      
      }elseif (!$tid) { // is a common field, update the biblio_field table
        db_query($field_query, $v['title'],$v['weight'],$common,$required,$autocomplete,$v['hint'],$key);
      }
    }
 }    

  drupal_set_message(t("The changes have been saved."));

  // Clear the cached pages and menus:
  menu_rebuild();
// drupal_goto('admin/settings/biblio/types');  

}
 
function biblio_admin_types_form() {

  $result = db_query('SELECT t.* FROM {biblio_types as t WHERE t.tid > 0}');
  //$rows[] = array('',t('Fields Common To All Types'),l('edit', 'admin/settings/biblio/types/edit'),'');

  while ($row = db_fetch_object($result)) {
    if ($row->tid < 999) {
      $rows[] = array($row->tid, $row->name, ($row->visible)?l(t('edit'), 'admin/settings/biblio/types/edit/'. $row->tid):'', ($row->visible)?l(t('hide'), 'admin/settings/biblio/types/hide/'. $row->tid):l(t('show'), 'admin/settings/biblio/types/show/'. $row->tid));
    }
    else{
      $rows[] = array($row->tid, $row->name, l(t('edit'), 'admin/settings/biblio/types/edit/'. $row->tid), l(t('delete'), 'admin/settings/biblio/types/delete/'. $row->tid));    
    }
  } 
  $header =array(t('Type Id'), t('Type Name'), array('data' => t('Operations'), 'colspan' => '2'));
  $output  = theme('table', $header, $rows);
  $output .= '<p>[ '. l(t('Add New Type'), 'admin/settings/biblio/types/add') .' ]';
  $output .= ' [ '. l(t('Reset all types to defaults'), 'admin/settings/biblio/types/reset') .' ]';
  return $output;
  
}
function biblio_admin_types_add_form() {
    $form['type_name'] = array('#type' => 'textfield',
                          '#title' => t('Type Name'),
                          '#size' => 20,
                          '#weight' => 1,
                          '#required' => true,
                          '#maxlength' => 64);
    $form['type_description'] = array('#type' => 'textfield',
                                 '#title' => t('Description'),
                                 '#size' => 60,
                                 '#weight' => 2,
                                 '#maxlength' => 255);
    $form['type_button'] = array('#type' => 'submit', 
                    '#value' => t('Create New Type'),
                    '#weight' => 3);
    return $form;
}
function biblio_admin_types_add_form_submit($form, &$form_state) {

   $tid =  variable_get('biblio_max_user_tid', '999') + 1;
   $query = "INSERT INTO {biblio_types} (`name`,`description`,`tid`) VALUES ('%s', '%s','%d')";
   db_query($query, $form_state['values']['type_name'], $form_state['values']['type_description'], $tid);
   variable_set('biblio_max_user_tid', $tid);
   drupal_goto('admin/settings/biblio/types');

}

function biblio_admin_types_hide() {
 $args = func_get_args();
 if ($args[0] > 0 && is_numeric($args[0]) ) {
    $result = db_query('UPDATE {biblio_types} SET visible = 0 WHERE tid = %d ', $args[0]);
 }
 drupal_goto('admin/settings/biblio/types');
}
function biblio_admin_types_show() {
 $args = func_get_args();
 if ($args[0] > 0 && is_numeric($args[0]) ) {
    $result = db_query('UPDATE {biblio_types} SET visible = 1 WHERE tid = %d ', $args[0]);
 }
 drupal_goto('admin/settings/biblio/types');
}

function biblio_admin_types_delete_form() {
 $args = func_get_args();
 if ($args[0] > 0 && is_numeric($args[0]) ) {
    $result = db_query('SELECT t.* FROM {biblio_types} as t WHERE t.tid = %d ', $args[0]);
    $row = db_fetch_array($result);
    $num_rows = db_result(db_query('SELECT COUNT(*) FROM {biblio} as b WHERE b.biblio_type = %d', $row['tid']));
    if ($num_rows ) {
      $existing_msg = t('There are @count biblio entries of this type, you should change the type of these entries before proceeding otherwise they will be deleted', array('@count' => $num_rows));
    }
    
    $form['tid'] = array('#type' => 'value', '#value' => $row['tid']);
    $output = confirm_form( $form,
           t('Are you sure you want to delete the custom biblio type:  %title ? ', array('%title' => $row['name'])) . $existing_msg,
           $_GET['destination'] ? $_GET['destination'] : 'admin/settings/biblio/types', t('This action cannot be undone.'),
           t('Delete'), t('Cancel')  );
  
    return $output;                
  }
  else{
    drupal_goto('admin/settings/biblio/types');  
  }
}

function biblio_form_types_delete_submit($form, &$form_state) {

    db_query('DELETE FROM {biblio_types} WHERE tid = %d', $form_state['values']['tid']);
    drupal_goto('admin/settings/biblio/types');  
}

function biblio_admin_types_reset_form() {

    $form['reset'] = array('#type' => 'value', '#value' => 'reset');

    $output = confirm_form( $form,
           t('Are you sure you want to reset ALL the field definitions to the defaults? '),
           $_GET['destination'] ? $_GET['destination'] : 'admin/settings/biblio/types', 
           t('This action will reset any and all field customizations to the defaults, and cannot be undone!'),
           t('Reset'), t('Cancel')  );
  
    return $output;                

}

function biblio_admin_types_reset_form_submit($form, &$form_state) {
  require_once( drupal_get_path('module', 'biblio') ."/biblio.install");
  biblio_reset_types();
  drupal_goto("admin/settings/biblio/types");

}

function _biblio_form_sort($a, $b) {
  $a_weight = (is_array($a) && isset($a['weight'])) ? $a['weight'] : 0;
  $b_weight = (is_array($b) && isset($b['weight'])) ? $b['weight'] : 0;
  if ($a_weight == $b_weight) {
    return 0;
  }
  return ($a_weight < $b_weight) ? -1 : 1;
}
function _biblio_import($userid = 1, $filename = null,$type='tagged',$terms = null) {
  global $user;
  $node = array();
  if (file_exists($filename)) {
    if (($file_content = @file_get_contents($filename)) === false) {
        return false;
    } else {

      $options = variable_get('node_options_biblio',array('status'));
      if (module_exists('i18n') && variable_get('i18n_node_biblio',0)){
        $node['language'] = module_invoke('i18n','default_language');
      }
      $node['type']     = "biblio";
      $node['created']  = time();
      $node['changed']  = time();
      $node['comment']  = variable_get('comment_biblio',0); 
      $node['promote']  = in_array('promote',$options);
      $node['moderate'] = in_array('moderate',$options);
      $node['sticky']   = in_array('sticky',$options);
      $node['format']   = 0;
      $node['status']   = in_array('status',$options);
      $node['uid']      = $userid;

      switch($type) {
      case 'tagged': // EndNote Tagged 
        require_once(drupal_get_path('module', 'biblio') . '/tagged_parser.inc');
        $node_ids  = _endnote_tagged_import($file_content,$node);
        break;
      case 'xml': // EndNote 7 XML
       $node_ids  = _endnote_XML_import($file_content ,$node, 7 ); 
      break;
      case 'xml8': // EndNote 8+ XML
       $node_ids  = _endnote_XML_import($file_content ,$node, 8 ); 
        break;
      case 'bib': // BibTex
       $node_ids  = _bibtex_import($file_content ,$node ); 
        break;
      case 'ris': // RIS
        require_once(drupal_get_path('module', 'biblio') . '/ris_parser.inc');
        $node_ids  = _ris_tagged_import($file_content,$node);
        break;
     }  
     if ($terms) {
    	  // set the vocabulary for the returned nodes 
    	  foreach ($node_ids as $node_id) {
    		  module_invoke('taxonomy', 'node_save', $node_id,  $terms);
    	  }
     }
      return $node_ids;
    }
  } else {
    return false;
  }
}

function _endnote_XML_import(&$data ,$nodedata, $version = 8) {
global $user,$records, $rec_count,$node_array,$node,$node_id;
  $node_array = array();
  $node = $nodedata;
  $xml_parser = drupal_xml_parser_create($data);

// use case-folding so we are sure to find the tag in 
  xml_parser_set_option($xml_parser, XML_OPTION_CASE_FOLDING, false);
  xml_parser_set_option($xml_parser, XML_OPTION_SKIP_WHITE, true);
  if ($version == 8) {
    require_once(drupal_get_path('module', 'biblio') . '/endnote8_parser.inc');
    xml_set_element_handler($xml_parser, "en8_startElement", "en8_endElement");
    xml_set_character_data_handler($xml_parser, "en8_characterData");
    xml_parse($xml_parser, $data,1);
  }elseif ($version == 7) {
    require_once(drupal_get_path('module', 'biblio') . '/endnote7_parser.inc');

    xml_parse_into_struct($xml_parser, $data, $values);

    en7_parse($values,$node_array,$node) ;

  }

  xml_parser_free($xml_parser);

  $node_ids = _biblio_save_imported_nodes($node_array);

  return ($node_ids);
}
 
function _bibtex_import(&$data, $node) {
require_once(drupal_get_path('module', 'biblio') . '/bibtex.php');

  $node_array = array();
  
  $bibtex = new Structures_BibTex();
  $bibtex->loadString($data);
  
  
  $bibtex->parse();
  if ($bibtex->amount()) {
    $bibtex->bib2node($node_array, $node);
    $node_ids = _biblio_save_imported_nodes($node_array);
  }
  return $node_ids;
} 

function _biblio_save_imported_nodes(&$node_array) {

  if (function_exists('node_save')) {
    foreach ($node_array as $node) {
      $node = (object)$node;
      if (variable_get('biblio_keyword_freetagging',0) && variable_get('biblio_freetagging_vocab',0)) {
        $vid = variable_get('biblio_freetagging_vocab',0);
        $node->taxonomy['tags'][$vid] = $node->biblio_keywords;
      }
      node_save($node);
      if ($node->nid) { 
         $node_ids[] = $node->nid;
      }
    } 
  }
  drupal_set_message(t("Succesfully imported @count entries.", array('@count' => count($node_ids))), 'status');

  return $node_ids;
}

function biblio_admin_type_add($tid=0,$fid=0) {
 
   $result = db_query("SELECT fid,title,weight,required,hint FROM {biblio_fields} WHERE fid=%d",$fid);
   if ($fld = db_fetch_object($result)) {
     db_query("INSERT INTO {biblio_type_details} (tid, fid, title, weight, required, hint)
                   VALUES (%d,%d,'%s',%d,%d,'%s')",$tid,$fid,$fld->title,$fld->weight,$fld->required,$fld->hint);
   }   
  drupal_goto("admin/settings/biblio/types/edit/$tid");
}
function biblio_admin_type_remove($tid=0,$fid=0) {
 
  db_query('DELETE FROM {biblio_type_details} WHERE tid = %d AND fid = %d', $tid,$fid);
  
  drupal_goto("admin/settings/biblio/types/edit/$tid");
}

/* 
 * This functin is used by both the admin/settings/biblio page and user profile page
 *   - if $user is set, then it is being called from the user profile page
 */   
function _biblio_get_user_profile_form(&$form, $user=null){
    $form['profile'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => ($user) ? FALSE : TRUE,
      '#title' => ($user) ? t('My publications') : t('Profile pages'),
  
    );
    $form['profile']['biblio_show_profile'] = array(
      '#type' => 'checkbox',
      '#title' => ($user) ? t('Show my publications on my profile page') : t('Show publications on users profile pages'),
      '#return_value' => 1,
      '#default_value' => ( isset($user->biblio_show_profile) ) ? $user->biblio_show_profile : variable_get('biblio_show_profile', '0') ,
      '#description' => ($user) ? t('Selecting this will create a listing of your publications on your profile page') : t('This sets the site wide default, users may change this in their profile')
    );
    $form['profile']['biblio_my_pubs_menu'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show "My publications" item in the navigation menu'),
      '#return_value' => 1,
      '#default_value' => ( isset($user->biblio_my_pubs_menu) ) ? $user->biblio_my_pubs_menu : variable_get('biblio_my_pubs_menu', '0'),
      '#description' => ''
    );
    if ($user) {
      $form['profile']['biblio_lastname'] = array(
        '#type' => 'textfield',
        '#title' => t('My last name'),
        '#default_value' => $user->biblio_lastname,
        '#description' => t('This may be ueed to determine which publications are mine') 
      );
    }
    $form['profile']['biblio_profile_uid'] = array(
      '#type' => 'radios',
      '#title' => t('Select publications by'),
      '#default_value' => (isset($user->biblio_profile_uid) ) ? $user->biblio_profile_uid : variable_get('biblio_profile_uid', 'uid'),
      '#options' => array('uid' => t('Drupal User ID'), 'lastname' => ($user)?t('My last name'):t('Last name'), 'uid_lastname' => t('Both')),
      '#description' => t('This defines the criteria used to select which publications will be listed on the users profile page'),    
    );
}
/*  This function parses the module directory for 'style' files, loads them and
 *  calls the info fuction to get some basic information like the short and long
 *  names of the style
*/   
function _biblio_admin_get_styles() {
  $styles = array();
  $dir    = drupal_get_path('module', 'biblio');
  $files  = array();
  if ($dh = opendir($dir)) {
    while (($files[] = readdir($dh)) !== false) {
    }
    closedir($dh);
    foreach ($files as $file) {
      if (strstr ( $file, 'biblio_style_')) { //find the style files
        include_once $dir ."/". $file;          
        if (function_exists(basename($file, ".inc") .'_info')) {
          $styles = array_merge($styles, call_user_func(basename($file, ".inc") .'_info')); //build and array of the short and long names
        }
      }
    }
  }
  return ($styles);
}
