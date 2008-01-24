<?PHP
// $Id$
/**
 * @file
 * Functions that are used to import and export biblio data.
 *
 */
 
 /*   biblio.import.export.inc
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

/**
 * Return a form select box populated with all the users of the site.
 *
 * @param $my_uid
 *   The user id of the person accessing the form so the select box defaults 
 *   to their userid 
 * @return
 *   An array which will be used by the form builder to add a select box to a form
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

/**
 * Return a form used to import files into biblio.
 *
 * @return
 *   An array which will be used by the form builder to build the import form
 */
function biblio_import_form() {
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
        $vocab_msg = t('There are currently no vocabularies assigned to the biblio node type, please go the the !url page to fix this', array('!url'=> l(t('admin/content/taxonomy'),'admin/content/taxonomy')));
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
/**
 * Implementation of hook_validate() for the biblio_import_form.
 */
function biblio_import_form_validate($form, &$form_state) {

    $op = $form_state['values']['op'];
    $filetype = $form_state['values']['filetype'];
    if (!file_save_upload('file_upload')) {
        form_set_error('',t("You must provide a file name")); 
    }
    if ($op == t('Import') && $filetype == '') {
       form_set_error('',t("You must select a file type"));
    }

}
/**
 * Implementation of hook_submit() for the biblio_import_form.
 */
function biblio_import_form_submit($form, &$form_state) {

    if ($form_state['values']['op'] == t('Import') && isset($form_state['values']['filetype'])) {
      
      if ($import = file_save_upload('file_upload')) {
        drupal_set_message(t("@file was uploaded", array('@file' => $import->filename)), 'status');

        // Concatenate all the terms of the different vocabularies
        // in a single array to be sent to biblio_import
        $terms = array();
        foreach (array_keys($form_state['values']) as $key) {
          if (preg_match('/(vocabulary[0-9]+)/', $key)) {
            if ($form_state['values'][$key] > 0) $terms[] = $form_state['values'][$key];
          }
        }
        // Added the $terms argument
        // the array of terms to be attached to the node(s)
        $userid = (isset($form_state['values']['userid']))? $form_state['values']['userid'] : 1;
        $filetype = $form_state['values']['filetype'];
        $content = biblio_import($userid, $import->filepath,$filetype, $terms);
        file_delete($import->filepath);

      } else {
        drupal_set_message(t("File was NOT successfully uploaded"), 'error');
      } 
  }
}
/**
 * Import data from a file and return the node ids created.
 *
 * @param $userid
 *   The user id of that will be assigned to each node imported
 * @param $filename
 *   The name of the file containing the data to import 
 * @param $type
 *   The format of the file to be imported (tagged, XML, RIS, bibTEX) 
 * @param $terms
 *   the vocabulary that the imported nodes will be associated with
 * @return
 *   An array the node id's of the items imported
 */
function biblio_import($userid = 1, $filename = null, $type='tagged', $terms = null) {
  global $user;
  $node = array();
  if (file_exists($filename)) {
    if (($file_content = @file_get_contents($filename)) === false) {
        return false;
    } else {

      $options = variable_get('node_options_biblio', array('status'));
      if (module_exists('i18n') && variable_get('i18n_node_biblio', 0)){
        $node['language'] = module_invoke('i18n', 'default_language');
      }
      $node['type']     = "biblio";
      $node['created']  = time();
      $node['changed']  = time();
      $node['comment']  = variable_get('comment_biblio', 0); 
      $node['promote']  = in_array('promote', $options);
      $node['moderate'] = in_array('moderate', $options);
      $node['sticky']   = in_array('sticky', $options);
      $node['format']   = 0;
      $node['status']   = in_array('status', $options);
      $node['uid']      = $userid;

      switch($type) {
      case 'tagged': // EndNote Tagged 
        require_once(drupal_get_path('module', 'biblio') . '/tagged_parser.inc');
        $node_ids  = _endnote_tagged_import($file_content, $node);
        break;
      case 'xml': // EndNote 7 XML
       $node_ids  = biblio_endnote_XML_import($file_content ,$node, 7 ); 
      break;
      case 'xml8': // EndNote 8+ XML
       $node_ids  = biblio_endnote_XML_import($file_content ,$node, 8 ); 
        break;
      case 'bib': // BibTex
       $node_ids  = biblio_bibtex_import($file_content ,$node ); 
        break;
      case 'ris': // RIS
        require_once(drupal_get_path('module', 'biblio') . '/ris_parser.inc');
        $node_ids  = _ris_tagged_import($file_content, $node);
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
/**
 * Export nodes in a given file format.
 *
 * @param $format
 *   The file format to export the nodes in (tagged, XML, bibTEX)
 * @param $nid
 *   If not NULL, then export only the given nodeid, else we will 
 *   use the session variable which holds the most recent query. If neither
 *   $nid or the session variable are set, then nothing is exported   
 * @param $version
 *   The version of EndNote XML to use.  There is one format for ver. 1-7 and
 *   a different format for versions 8 and greater.  
 * @return
 *   none
 */
function biblio_export($format="tagged", $nid = null, $version = 7) {
$params=array();
  if ($nid === null && isset($_SESSION['last_biblio_query']) && !empty($_SESSION['last_biblio_query']) ) {
    $query = $_SESSION['last_biblio_query'];
    $params = $_SESSION['last_biblio_query_terms'];    
  } elseif (!empty($nid)) {
    $query = db_rewrite_sql("SELECT * FROM {node} n left join {biblio} b  on n.vid=b.vid 
                WHERE n.nid=%d ");
    $params[] = $nid;
  }else{
    return;
  } 

  $result = db_query($query, $params);

  if ($result) {
     switch($format) {
     case "tagged":
       biblio_endnote_tagged_export($result);
       break;
     case "xml":
       biblio_endnote_XML_export($result, $version );
       break;
     case "bib":
       biblio_bibtex_export($result);
       break;
     }
  }
}
/**
 * Import bibtex data.
 *
 * @param $data
 *   the contents of a bibtex file passed as one big string 
 * @param $node
 *   an array (populated in biblio_import() ), containing the boiler plate 
 *   information common to all nodes 
 * @return
 *   an array of node ids
 */
function biblio_bibtex_import(&$data, $node) {
require_once(drupal_get_path('module', 'biblio') . '/bibtex.php');

  $node_array = array();
  
  $bibtex = new Structures_BibTex();
  $bibtex->loadString($data);
  
  
  $bibtex->parse();
  if ($bibtex->amount()) {
    $bibtex->bib2node($node_array, $node);
    $node_ids = biblio_save_imported_nodes($node_array);
  }
  return $node_ids;
} 

/**
 * Export data in bibtex format.
 *
 * @param $result
 *   a database result set pointer
 * @return
 *   none
 */
function biblio_bibtex_export($result) {
$bibtex = '';
    while ($pub = db_fetch_object($result)) {
    $type = "article";
    $journal = $series = $booktitle = $school = $organization = null;
    switch ($pub->biblio_type) {
      case 102:
        $type = "article";
        $journal = $pub->biblio_secondary_title;
       break; 
      case 100:
        $type = "book";
        $series = $pub->biblio_secondary_title;
         break; 
      case 101:
        $type = "inbook";
        $booktitle = $pub->biblio_secondary_title;
         break; 
      case 103:
        $type = "conference";
         break; 
      case 100:
        $type = "incollection";
         break; 
      case 103:
        $type = "inproceedings";
         break; 
      case 129:
        $type = "misc";
         break; 
      case 108:
        $type = "phdthesis";
        $school = $pub->biblio_publisher;
        $pub->biblio_publisher = null;
         break; 
      case 104:
        $type = "proceedings";
         break; 
      case 109:
        $type = "techreport";
        $organization = $pub->biblio_publisher;
        $pub->biblio_publisher = null;
         break; 
      case 124:
        $type = "unpublished";
         break; 
    }
    
    $bibtex .= '@'.$type.' { ';
    $bibtex .= ($pub->biblio_citekey)? $pub->biblio_citekey.",\n":",\n";
    if ($pub->title) $bibtex .= "\ttitle = {".$pub->title."},\n";
    if ($journal) $bibtex .= "\tjournal = {".$journal."},\n";
    if ($booktitle) $bibtex .= "\tbooktitle = {".$booktitle."},\n";
    if ($series) $bibtex .= "\tseries = {".$series."},\n";
    if ($pub->biblio_secondary_authors) $bibtex .= "\teditor = {".$pub->biblio_secondary_authors."},\n";
    if ($pub->biblio_volume) $bibtex .= "\tvolume = {".$pub->biblio_volume."},\n";
    if ($pub->biblio_number) $bibtex .= "\tnumber = {".$pub->biblio_number."},\n";
    if ($pub->biblio_year) $bibtex .= "\tyear = {".$pub->biblio_year."},\n";
    if ($pub->biblio_notes) $bibtex .= "\tnote = {".$pub->biblio_notes."},\n";
    if ($pub->biblio_date) $bibtex .= "\tmonth = {".$pub->biblio_date."},\n";
    if ($pub->biblio_pages) $bibtex .= "\tpages = {".$pub->biblio_pages."},\n";
    if ($pub->biblio_publisher) $bibtex .= "\tpublisher = {".$pub->biblio_publisher."},\n";
    if ($school) $bibtex .= "\tschool = {".$school."},\n";
    if ($organization) $bibtex .= "\torganization = {".$organization."},\n";
    if ($institution) $bibtex .= "\tinstitution = {".$institution."},\n";
    if ($pub->biblio_type_of_work) $bibtex .= "\ttype = {".$pub->biblio_type_of_work."},\n";
    if ($pub->biblio_edition) $bibtex .= "\tedition = {".$pub->biblio_edition."},\n";
    if ($pub->biblio_section) $bibtex .= "\tchapter = {".$pub->biblio_section."},\n";
    if ($pub->biblio_place_published) $bibtex .= "\taddress = {".$pub->biblio_place_published."},\n";
    if ($pub->biblio_abst_e) $bibtex .= "\tabstract = {".$pub->biblio_abst_e."},\n";
    if ($pub->biblio_keywords) $bibtex .= "\tkeywords = {".$pub->biblio_keywords."},\n";
    if ($pub->biblio_isbn) $bibtex .= "\tISBN = {".$pub->biblio_isbn."},\n";
    if ($pub->biblio_url) $bibtex .= "\tURL = {".$pub->biblio_url."},\n";
    $author =  str_replace("; ", " and ", $pub->biblio_authors);
    $bibtex .= "\tauthor = {".$author."}\n";
    
    $bibtex.="}\n\n";

  }

  header('Content-type: 	application/text');
  header('Content-Disposition:  filename="bibtex.bib"');
  print utf8_decode($bibtex);


}

/**
 * Save node imported from a file.
 *
 * @param $node_array
 *   a 2 dimensional array containing all the node information
 * @return
 *   The node ids of the saved nodes
 */
function biblio_save_imported_nodes(&$node_array) {

  if (function_exists('node_save')) {
    foreach ($node_array as $node) {
      $node = (object)$node;
      if (variable_get('biblio_keyword_freetagging', 0) && variable_get('biblio_freetagging_vocab', 0)) {
        $vid = variable_get('biblio_freetagging_vocab', 0);
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

/**
 * Import EndNote XML data.
 *
 * @param $data
 *   the contents of an EndNote XML file passed as one big string 
 * @param $node
 *   boiler plate information common to all nodes
 * @param $version
 *   the EndNote version of the XML file.  EndNote uses one format up to version
 *   7 then change to another format in version 8 and greater. 
 * @return
 *   The node ids of the saved nodes
 */
function biblio_endnote_XML_import(&$data, $node, $version = 8) {
global $user,$records, $rec_count, $node_array, $node, $node_id;
  $node_array = array();
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

    en7_parse($values, $node_array, $node) ;

  }

  xml_parser_free($xml_parser);

  $node_ids = biblio_save_imported_nodes($node_array);

  return ($node_ids);
}

/**
 * Export data in EndNote XML format.
 *
 * @param $result
 *   a database pointer to a result set
 * @param $version
 *   the EndNote version of the XML file.  EndNote uses one format up to version
 *   7 then change to another format in version 8 and greater. 
 * @return
 *   none
 */
function biblio_endnote_XML_export($result, $version = 7) {

   if ($version == 8) {
      require_once(drupal_get_path('module', 'biblio') . '/endnote8_export.inc');
     $xml = _endnote8_XML_export($result);
    }elseif ($version == 7) {
      require_once(drupal_get_path('module', 'biblio') . '/endnote7_export.inc');
     $xml = _endnote7_XML_export($result);
    }
    header('Content-type: application/xml; charset=utf-8');
    header('Content-Disposition: attachment; filename="endnote'. $version. '.xml"');
    print $xml;
}


/**
 * Export data in EndNote tagged format.
 *
 * @param $result
 *   a database pointer to a result set
* @return
 *   none
 */
function biblio_endnote_tagged_export($result) {

    $doc = "";
    while ($pub = db_fetch_object($result)) {
      switch ($pub->biblio_type) {
        case 1:
        case 102: $doc .= "%0 Journal Article\r\n";
          if ($pub->biblio_secondary_title) $doc .= "%J " . trim($pub->biblio_secondary_title) . "\r\n";
          break; // journal
        case 2:  
        case 103: $doc .= "%0 Conference Paper\r\n";
          if ($pub->biblio_secondary_title) $doc .= "%B " . trim($pub->biblio_secondary_title) . "\r\n";
          break;
        case 3: // are all
        case 4: $doc .= "%0 Conference Proceedings\r\n";
          if ($pub->biblio_secondary_title) $doc .= "%B " . trim($pub->biblio_secondary_title) . "\r\n";
          break; // conference proceedings 
        case 5: 
        case 109:
          $doc .= "%0 Report\r\n";
          break; // report
        case 6: 
        case 101:
          $doc .= "%0 Book Section\r\n";
          if ($pub->biblio_secondary_title) $doc .= "%B " . trim($pub->biblio_secondary_title) . "\r\n";
          break; // book section
        case 7: 
        case 108:
          $doc .= "%0 Thesis\r\n";
          if ($pub->biblio_secondary_title) $doc .= "%B " . trim($pub->biblio_secondary_title) . "\r\n";
          break; // thesis
        case 8: 
        case 119:
          $doc .= "%0 Patent\r\n";
          if ($pub->biblio_secondary_title) $doc .= "%B " . trim($pub->biblio_secondary_title) . "\r\n";
          break; // patent
        case 9:
        default:
          $doc .= "%0 Generic\r\n";
          break; // generic
      } 
      $doc .= "%D " . trim($pub->biblio_year) . "\r\n";
      $doc .= "%T " . trim($pub->title) . "\r\n";
      
      $author_array = _parse_author_array(explode(';', $pub->biblio_authors));
      foreach($author_array as $auth) {
        $doc .= "%A " . $auth['last_name'] . ($auth['first_name'] ? (', ' . $auth['first_name']) : ($auth['initials'] ? (', ' . $auth['initials']) : '')) . "\r\n";
      } 

      if ($pub->biblio_place_published) $doc .= "%C " . trim($pub->biblio_place_published) . "\r\n";
      if ($pub->biblio_secondary_authors) {
        $author_array = explode(";", $pub->biblio_secondary_authors);
        foreach($author_array as $auth) {
          $doc .= "%E " . trim($auth) . "\r\n";
        } 
      } 
      if ($pub->biblio_publisher) $doc .= "%I " . trim($pub->biblio_publisher) . "\r\n";
      if (trim($pub->biblio_keywords)) $doc .= "%K " . trim($pub->biblio_keywords) . "\r\n";
      if ($pub->biblio_call_number) $doc .= "%L " . trim($pub->biblio_call_number) . "\r\n";
      if ($pub->biblio_accession_number) $doc .= "%M " . trim($pub->biblio_accession_number) . "\r\n";
      if ($pub->biblio_issue) $doc .= "%N " . trim($pub->biblio_issue) . "\r\n";
      if ($pub->biblio_pages) $doc .= "%P " . trim($pub->biblio_pages) . "\r\n";
      if ($pub->biblio_tertiary_title) $doc .= "%S " . trim($pub->biblio_tertiary_title) . "\r\n";
      if ($pub->biblio_url) $doc .= "%U " . trim($pub->biblio_url) . "\r\n";
      if ($pub->biblio_volume) $doc .= "%V " . trim($pub->biblio_volume) . "\r\n";
      $abst = "";
      if ($pub->biblio_abst_e) $abst .= trim($pub->biblio_abst_e);
      if ($pub->biblio_abst_f) $abst .= trim($pub->biblio_abst_f);
      if ($abst) {
        $search = array("/\r/", "/\n/");
        $replace = " ";
        $abst = preg_replace($search, $replace, $abst);
        $doc .= "%X " . $abst . "\r\n";
      } 
      if ($pub->biblio_tertiary_authors) {
        $author_array = explode(";", $pub->biblio_tertiary_authors);
        foreach($author_array as $auth) {
          $doc .= "%Y " . trim($auth) . "\r\n";
        } 
      } 
      if ($pub->biblio_notes) $doc .= "%Z " . trim($pub->biblio_notes) . "\r\n";
      if ($pub->biblio_edition) $doc .= "%7 " . trim($pub->biblio_edition) . "\r\n";
      if ($pub->biblio_date) $doc .= "%8 " . trim($pub->biblio_date) . "\r\n";
      if ($pub->biblio_type_of_work) $doc .= "%9 " . trim($pub->biblio_type_of_work) . "\r\n";
      if ($pub->biblio_isbn) $doc .= "%@ " . trim($pub->biblio_isbn) . "\r\n";

      $doc .= "\r\n";
    } 
   
  header('Content-type: 	application/x-endnote-refer');
  header('Content-Disposition:  filename="endnote.enw"');
  print utf8_decode($doc);
} 

function biblio_csv_export($result,$btypes,$bfields){
  while ($rec = db_fetch_object($result)){

    $node_id = $rec->nid;
    $node_array[$node_id]['Type'] = $btypes[$rec->biblio_type];
    $col_array['Type'] += 1;

	foreach( array_keys($bfields) as $fieldname ) {
		if (!empty($rec->$fieldname) && !in_array($fieldname,array('biblio_citekey','biblio_coins'))) {
			$col_array[$bfields[$fieldname]] +=1;	// mark field as in use
			$node_array[$node_id][$bfields[$fieldname]] = trim(strtr(strtr($rec->$fieldname,'"','""'),';',"\n"));
		}
	}
  } //end while
   

  // Enclosing text in "<text>" is neccessary to enshure 
  // multi line fields (like author) are handled correctly.
  // Therefore existing " must be excaped before.
  $csv = '"' . join("\"\t\"", array_keys($col_array)) . "\"\n";
  foreach($node_array as $line_array) {
    $csv_line = '';
    foreach(array_keys($col_array) as $col) {
      $csv_line .= "\t\"" . $line_array[$col] . '"';
    }
    $csv .= substr($csv_line,1) . "\n";
    // $csv .= '"' . join("\"\t\"", $line_array) . '"' . "\n";
  }

  header('Content-type: 	application/text');
  header('Content-Disposition:  filename="biblio_export.csv"');

  // return utf8_decode($csv);
  return $csv;
}
