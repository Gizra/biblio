<?php

function theme_biblio_add_css() {
  drupal_add_css(drupal_get_path('module', 'biblio').'/biblio.css');
}

function theme_biblio_coin_button($element) {
  return '<a href="/biblio/regen_coins"><input type="button"  name="'. $element['#name'] .'" value="'. $element['#value'] .'"  /></a>';
}

function theme_biblio_openurl($openURL) {
	$openurl_image = variable_get('biblio_openurlimage','');
	
	if ($openurl_image !='') 
  {
    $output .= '<div class="biblio-openurl-image"><a href="'.  $openURL . '"><img border="0" src="' . $openurl_image . '" /></a></div>';
  } else {
    $output .= '<div class="biblio-openurl-text"><a href="'.  $openURL . '">'.t('Find It Via OpenURL!').'</a></div>';
	
  }
	return $output;
}

function theme_biblio_export_links($nid) {
  $base =  variable_get('biblio_base', 'biblio');
  $links .= '<p /><ul class="tabs secondary">';
  $links .= '<li> '.l(t("Tagged"),"$base/export/tagged/$nid").'</li>';
  $links .= '<li> '.l(t("XML"),"$base/export/xml/$nid").'</li>';
  $links .= '<li> '.l(t("BibTex"),"$base/export/bib/$nid").'</li></ul>';
  
  return $links;
}

function theme_biblio_short($node, $base='biblio', $style_name='classic', $inline=false) {

  require_once( drupal_get_path('module', 'biblio') . "/biblio_style_$style_name.inc");  
  $style_function = "biblio_style_$style_name";
  $styled_node = $style_function($node, $base, $inline);
  if (empty($node->biblio_coins)) biblio_coins_generate($node);
  return($styled_node.$node->biblio_coins);

}

function theme_biblio_long($node,$base='biblio',$style='classic') {
  
  if (empty($node->biblio_coins)) biblio_coins_generate($node);
  $output .= $node->biblio_coins;
  $layout = variable_get('biblio_node_layout', 'orig');
  if (variable_get('biblio_node_layout', 'orig') == 'ft' && strlen(trim($node->body)) && user_access('view full text') ) {
    $output .= '<div class="biblio-head">'.theme('biblio_short', $node,$base,$style).'</div>';
    $annotation_field = variable_get('biblio_annotations','none');
    if ($annotation_field != 'none' && $node->$annotation_field) {
      $output .= '<div class="biblio_annotation">';
      $output .= check_markup($node->$annotation_field,FILTER_HTML_ESCAPE,FALSE);
      $output .= '</div>';
    }
    $output .= check_markup($node->body,FILTER_HTML_ESCAPE,FALSE) ;
    if ( biblio_access('export',$node)) {
      $output .= theme('biblio_export_links',$node->nid);
    } 
    return $output;
  }

  if (variable_get('biblio_normalize', 0)) {
    $authors = _biblio_parse_authors($node->biblio_authors);
  } else {
    $authors = $node->biblio_authors;
  } 

  $openurl_base = variable_get('biblio_baseopenurl','');
  
  if ($openurl_base) $output .= theme('biblio_openurl',biblio_openurl($node, $openurl_base));
  

  $output .= '<div class="biblio_authors"><h3>'.t("Authors").':</h3> ' . _biblio_author_links($authors,$base) . "</div>\n";
  $output .= '<div class="biblio_source"><h3>'.t("Source").': </h3> ';
  $source = null;
  if ($node->biblio_secondary_title) $source .= check_plain($node->biblio_secondary_title);
  if ($node->biblio_publisher) {
    $source .= $source?", ":"";
    $source .= check_plain($node->biblio_publisher);
  } 
  if ($node->biblio_volume) {
    $source .= $source?", ":"";
    $source .= t('Volume ') . check_plain($node->biblio_volume);
  } 
  if ($node->biblio_issue) {
    $source .= $source?", ":"";
    $source .= t('Issue ') . check_plain($node->biblio_issue);
  } 
  if ($node->biblio_number) {
    $source .= $source?", ":"";
    $source .= t('Number ') . check_plain($node->biblio_number);
  } 

  if ($node->biblio_place_published) {
    $source .= $source?", ":"";
    $source .= check_plain($node->biblio_place_published);
  } 
  if ($node->biblio_pages) {
    $source .= $source?", ":"";
    $source .= 'p.' . check_plain($node->biblio_pages);
  } 
  if (isset($node->biblio_year)) {
    if ($node->biblio_year == 9998) { $node->biblio_year = t("In Press"); }
    if ($node->biblio_year == 9999) { $node->biblio_year = t("Submitted"); }
    $source .= ' (' . check_plain($node->biblio_year) . ')';
  } 
  $output .= "$source</div>\n";

  if ($node->biblio_isbn) $output .= '<h3>'.t("ISBN").':</h3> ' . check_plain($node->biblio_isbn) . "\n";
  if ($node->biblio_call_number) $output .= '<h3>'.t("Call Number").':</h3> ' . check_plain($node->biblio_call_number) . "\n";
  if ($node->biblio_accession_number) $output .= '<h3>'.t("Accession Number").':</h3> ' . check_plain($node->biblio_accession_number) . "\n";
  if ($node->biblio_other_number) $output .= '<h3>'.t("Other Number").':</h3> ' . check_plain($node->biblio_other_number) . "\n";
  if ($node->biblio_url) {
      $attrib = (variable_get('biblio_links_target_new_window',null)) ? array('target'=>'_blank') : array();
      $output .= '<h3>'.t("URL").':</h3>'.l($node->biblio_url,$node->biblio_url,$attrib) . "\n";
  }
  if ($node->biblio_keywords) $output .= '<h3>'.t("Keywords").':</h3> ' . _biblio_keyword_links($node->biblio_keywords,$base) . "\n";
  if ($node->biblio_abst_e) $output .= '<h3>'.t("Abstract").':</h3> ' . check_markup($node->biblio_abst_e,FILTER_HTML_ESCAPE,FALSE) . "\n";
  if ($node->biblio_abst_f) $output .= '<p>' . check_markup($node->biblio_abst_f,FILTER_HTML_ESCAPE,FALSE) . "\n";
  if ($node->biblio_notes) $output .= '<h3>'.t("Notes").': </h3>' . check_markup($node->biblio_notes,FILTER_HTML_ESCAPE,FALSE) . "\n";
  if ( strlen(trim($node->body)) && user_access('view full text') ) {
     $output .= '<h3>'.t('Full Text').':</h3>'.check_markup($node->body,FILTER_HTML_ESCAPE,FALSE);
  }    
  if ( biblio_access('export',$node)) {
    $output .= theme('biblio_export_links',$node->nid);
  } 

  return $output;
} 

function theme_biblio_list($node, $base='biblio', $style='classic', $inline=false) {
  
  $output .= theme('biblio_short', $node, $base, $style, $inline);
  if ($node->biblio_abst_e) {
    $output .= '<span class="biblio_abstract_link">';
    $output .= l(" Abstract", "node/$node->nid") . "\n";
    $output .= '</span>';
  }

  $annotation_field = variable_get('biblio_annotations','none');
  if ($annotation_field != 'none' && $node->$annotation_field) {
    $output .= '<div class="biblio_annotation">';
    $output .= check_markup($node->$annotation_field,FILTER_HTML_ESCAPE,FALSE);
    $output .= '</div>';
  }
  
  $openurl_base = variable_get('biblio_baseopenurl','');
  if ($openurl_base) $output .= theme('biblio_openurl',biblio_openurl($node));
  
  if ( biblio_access('export',$node)) {
    $base =  variable_get('biblio_base', 'biblio');
    $output .= '<span class="biblio_export_links">';
    $output .= l("Tagged","$base/export/tagged/$node->nid") . '&nbsp;';
    $output .= l("XML","$base/export/xml/$node->nid") . '&nbsp;';
    $output .= l("BibTex","$base/export/bib/$node->nid") ;
    $output .= '</span>';
  } 
  return $output;
} 

function theme_biblio_filters(&$form) {

  if (sizeof($form['current'])) {
    $output .= '<ul>';
    foreach (element_children($form['current']) as $key) {
      $output .= '<li>' . drupal_render($form['current'][$key]) . '</li>';
    }
    $output .= '</ul>';
  }

  $output .= '<dl class="multiselect">' . (sizeof($form['current']) ? '<dt><em>'. t('and') .'</em> '. t('where') .'</dt>' : '') . '<dd class="a">';
  foreach (element_children($form['filter']) as $key) {
    $output .= drupal_render($form['filter'][$key]);
  }
  $output .= '</dd>';

  $output .= '<dt>'. t('is') .'</dt>' . '<dd class="b">';

  foreach (element_children($form['status']) as $key) {
    $output .= drupal_render($form['status'][$key]);
  }
  $output .= '</dd>';

  $output .= '</dl>';
  $output .= '<div class="container-inline" id="node-buttons">'. drupal_render($form['buttons']) .'</div>';
  $output .= '<br class="clear" />';

  return $output;
}

function theme_biblio_form_filter(&$form) {

  $output .= '<div id="biblio-admin-filter">';
  $output .= drupal_render($form['filters']);
  $output .= '</div>';
  $output .= drupal_render($form);
  return $output;
}

function theme_biblio_form_types_edit($form) {
  $base =  variable_get('biblio_base', 'biblio');
print_r($form);die;
  $tid = (!empty($form['#parameters'][2])) ? $form['#parameters'][2] : FALSE;

  drupal_set_title($form['type_name']?$form['type_name']['#value']:"Default");

  // build the table with all the fields if no $tid is given, or only the common 
  // and customized fields if $tid is given
  $conf_table = array();
  foreach(element_children($form['configured_flds']) as $fld) {
    $conf_row = array();
    $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['name']));
    $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['title']),'align' => 'center');
    $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['hint']),'align' => 'center');
    foreach (element_children($form['configured_flds'][$fld]['checkboxes']) as $oid) {
      if (is_array($form['configured_flds'][$fld]['checkboxes'])) {
            $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['checkboxes'][$oid]), 'align' => 'center', 'title' => $oid);
      }
    }
    $conf_row[] = array('data' => drupal_render($form['configured_flds'][$fld]['weight']),'align' => 'center');
    if ($tid) {
      if ($form['cust'][$fld]) {
        $conf_row[] = array('data' => l('remove',"$base/type/remove/$tid/$fld"),'align' => 'left');
      }else{
        $conf_row[] = array('data' => "common",'align' => 'left');
      }
    }
   $conf_table[] = $conf_row;
  }
  if ($tid) {
    $header =array(t('Field Name'),t('Default Title'),t('Hint'),t('Required'),t('Weight'),t('Action'));
  }else{
    $header =array(t('Field Name'),t('Default Title'),t('Hint'),t('Common'),t('Required'),t('Autocomplete'),t('Weight'));

  }
  
   
  $output .= '<p>';
  drupal_add_js('misc/collapse.js');
  $output .= '<p><fieldset class=" collapsible"><legend>Currently configured fields</legend>';
  $output .= theme('table',$header,$conf_table);
  $output .= '<p><center>'.drupal_render($form['submit']).'</center></p>';
  $output .= '</fieldset>';
 
  if ($tid) {
    $avail_table = array();
  
    foreach(element_children($form['avail_flds']) as $fld) {
      $avail_row = array();
  
      $avail_row[] = array('data' => '<div>'.drupal_render($form['avail_flds'][$fld]['name']).'</div>');
      $avail_row[] = array('data' => '<div>'.drupal_render($form['avail_flds'][$fld]['size']).'</div>','align' => 'left');
      $avail_row[] = array('data' => '<b>'.drupal_render($form['avail_flds'][$fld]['title']).'</b>','align' => 'left');
      $avail_row[] = array('data' => l('add',"$base/type/add/$tid/$fld"),'align' => 'left');
      $avail_table[] = $avail_row;
    }

    $header2 =array(array('data'=>'Field Name','align'=>'right'),'Size','Default Name','Action');

    $output .= '<p><fieldset class=" collapsible"><legend>' . t('Other available fields') .'</legend>';
    $output .= theme('table',$header2,$avail_table);
    $output .= '<p><center>'.drupal_render($form['add']).'</center></p>';
    $output .= '</fieldset>';
  }
  $output .= drupal_render($form);
  return $output;
  
}
