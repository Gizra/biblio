<?php
/**
 * @param $aid
 * @return unknown_type
 */
function biblio_get_contributor($aid) {
  $contributor = &drupal_static(__FUNCTION__);

  if (!isset($contributor[$aid])) {
    $contributor[$aid] = db_query('SELECT * FROM {biblio_contributor_data} WHERE cid = :cid', array(':cid' => $aid))->fetchObject();
  }

  return $contributor[$aid];
}

function biblio_get_contributor_category($contributors, $category) {
  $authors = array();
  foreach ($contributors as $author) {
    $wrapper = biblio_wrapper($author, 'biblio_contributor');
    if ($wrapper->biblio_contributor_category == $category) {
      $authors[] = $author;
    }
  }
  return $authors;
}

function biblio_get_contributor_by_name($name) {
  $contributors = &drupal_static(__FUNCTION__);

  if (!isset($contributors[$name])) {
    $query = new EntityFieldQuery();
    $entities = $query->entityCondition('entity_type', 'biblio_contributor')
      ->fieldCondition('biblio_contributor_name', 'value', $name)
      ->execute();
    if (!empty($entities)) {
      $contributors[$name] = $entities->fetchObject();
    }
  }

  return $contributors[$name];
}

function biblio_get_first_contributor($vid) {
  static $contributor = array();
  if (!isset($contributor[$vid])) {
    $query = db_select('biblio_contributor', 'bc');
    $query->join('biblio_contributor_data', 'bcd', 'bc.cid=bcd.cid');
    $query->fields('bcd');
    $query->condition('bc.vid', $vid);
    $query->condition('bc.rank', 0);
    $contributor[$vid] = $query->execute()->fetchObject();
  }

  return $contributor[$vid];
}

/**
 * @param $vid
 * @return unknown_type
 */
function biblio_load_contributors($vid) {
  $vids = (isset($vid) ? array($vid) : array());
  return biblio_load_contributors_multiple($vids);
}

function biblio_load_contributors_multiple($vids = array(), $auth_category = 0) {
  $contributors = array();
  if (empty($vids)) return $contributors;

  $query = db_select('biblio_contributor', 'bc');
  $query->innerJoin('biblio_contributor_data', 'bcd', 'bcd.cid = bc.cid');
  $query->fields('bc');
  $query->fields('bcd');
  $query->orderby('bc.vid');
  $query->orderby('bc.rank');
  if (count($vids) == 1) {
    $query->condition('bc.vid', $vids[0]);
  }
  else {
    $query->condition('bc.vid', $vids, 'IN');
  }
  if ($auth_category) {
    $query->condition('bc.auth_category', $auth_category);
  }
  $query->addMetaData('base_table', 'biblio_contributor');
  $query->addTag('node_access');
  $result = $query->execute();

  foreach ($result as $creator) {
    $contributors[$creator->vid][] = (array)$creator;
  }

  return $contributors;
}
/**
 * Add separate author named "et al" to the end of the author array
 *
 * @param $authors - author array to augment
 * @param $type - auth_type
 * @return TRUE if author was added, FALSE if "etal" was already there
 */
function biblio_authors_add_etal(&$authors, $type) {
  $etal = "et al"; $max_rank = 0;
  foreach ($authors as $author) { // et al author should be added only once per type
    if ($author['auth_type'] != $type) continue;
    if ($author['name'] == $etal) {
      return FALSE;
    }
    $max_rank = max($max_rank, $author['rank']);
  }
  $authors[] = biblio_parse_author(array('name' => $etal, 'auth_type' => $type, 'lastname' => $etal, 'rank' => $max_rank + 1));
  return TRUE;
}
/**
 * Parse initial contributor array and augment with additional info
 * @param $contributors initial contributor array
 * @return augmented contributor array
 */
function biblio_parse_contributors($contributors) {
  $result = array();
  if (!count($contributors)) return;
  foreach ($contributors as $cat => $authors) {
    $etal = array();
    foreach ($authors as $author) {
      // remove any form of "et al" from name field, because it confuses biblio_parse_author
      $author_cleaned = preg_replace("/et\.?\s+al\.?/", '', $author['name']);
      if ($author_cleaned != $author['name']) { // if "et al" was present:
        $author['name'] = $author_cleaned;  // store cleaned name
        $etal[$author['auth_type']] = TRUE; // mark it as "to be added" in $etal array
      }
      $author['name'] = trim($author['name']);
      if (strlen($author['name'])) {
        $result[$cat][] = biblio_parse_author($author, $cat);
      }
    }
    // add "et al" authors for all neccessary author types
    foreach ($etal as $type => $dummy) {
      if (isset($result[$cat])) { // add "et al" only if plain authors exists
        biblio_authors_add_etal($result[$cat], $type);
      }
    }
  }
  return $result;
}

function biblio_delete_contributors($bid) {
  $count = db_delete('biblio_contributor')
            ->condition('bid', $bid)
            ->execute();
  return $count;
}

function biblio_delete_contributors_revision($vid) {
  $count = db_delete('biblio_contributor')
            ->condition('vid', $vid)
            ->execute();
  return $count;
}

function biblio_delete_contributor($cid) {
    db_delete('biblio_contributor')
      ->condition('cid', $cid)
      ->execute();

  return db_delete('biblio_contributor_data')
            ->condition('cid', $cid)
            ->execute();

}
function biblio_delete_contributor_revision($cid, $vid) {
  return db_delete('biblio_contributor')
          ->condition('cid', $cid)
          ->condition('vid', $vid)
          ->execute();

}

function biblio_count_orphan_authors() {
  return db_query('SELECT COUNT(*) FROM {biblio_contributor_data} bcd WHERE bcd.cid NOT IN (SELECT DISTINCT(bc.cid) FROM {biblio_contributor} bc )')->fetchField();
}

function biblio_get_orphan_authors() {
  $authors = array();
  $result =  db_query('SELECT bcd.* FROM {biblio_contributor_data} bcd WHERE bcd.cid NOT IN (SELECT DISTINCT(bc.cid) FROM {biblio_contributor} bc )');
  foreach ($result as $author) {
    $authors[] = $author;
  }
  return $authors;
}

function biblio_delete_orphan_authors($force = FALSE) {
  if (variable_get('biblio_auto_orphaned_author_delete', 0) || $force) {
    $query = db_select('biblio_contributor', 'bc');
    $active_cids = $query
      ->fields('bc', array('cid'))
      ->groupBy('cid')
      ->execute()
      ->fetchCol();

    $query = db_select('biblio_contributor_data', 'bcd');
    $all_cids = $query
      ->fields('bcd', array('cid'))
      ->groupBy('cid')
      ->execute()
      ->fetchCol();

    $orphans = array_diff($all_cids, $active_cids);

    if (!empty($orphans)) {
      db_delete('biblio_contributor_data')
        ->condition('cid', $orphans, 'IN')
        ->execute();
    }
  }
}

// @todo we may not need this
function biblio_insert_contributors($contributor) {
  if (!empty($contributor)) {
    return _save_contributors($contributor, $biblio->bid, $biblio->vid);
  }
}

function biblio_update_contributors($node) {
  if (!empty($node->biblio_contributors)) {
    _save_contributors($node->biblio_contributors, $node->nid, $node->vid, TRUE);
  }
  return;

}

function biblio_save_contributor(&$author) {
  return drupal_write_record('biblio_contributor_data', $author);
}

function biblio_update_contributor(&$author) {
  if (!isset($author['cid'])) return FALSE;
  return drupal_write_record('biblio_contributor_data', $author, 'cid');
}
function _biblio_contributor_sort(&$authors) {
  foreach($authors as $key => $author) {
    if(!isset($author['rank']) || empty($author['rank'])) {
      $authors[$key]['rank'] = $key;
    }
  }
  usort($authors, '_biblio_contributor_usort');
}
function _biblio_contributor_usort($a, $b) {
  if(empty($a['name'])) return 1;
  if(empty($b['name'])) return -1;
  return ($a['rank'] < $b['rank']) ? -1 : 1;
}
/**
 * Perform actions to a contributor object before saving
 * @param object $contributor
 * @return success of operations
 */
function biblio_contributor_pre_save($contributor) {
  $rank = 0;
  $wrapper = biblio_wrapper($contributor, 'biblio_contributor');
  $name = $wrapper->biblio_contributor_name->value();
  $cid = $wrapper->cid->value();

  if (!empty($name)) {
    // if we don't have a cid, lets see if we can find and exact match to the name and use that cid
    if (!isset($cid) || empty($cid)) {
      // @todo implement author duplicate resolution
      // $auth = biblio_get_contributor_by_name($name);
      if (!empty($auth) && isset($auth->cid)) {
        dpm('duplicate author found');
        // $author['cid'] = $auth->cid;
      }
    }

    if (empty($cid)) {
      biblio_parse_author($contributor);
    }
  }
  $contributor->title = $name;
  //TODO  check if it is necessary to reset aka here...
  //  db_query("UPDATE {biblio_contributor_data} SET aka = cid WHERE aka = 0 OR aka IS NULL");
  //  db_update('biblio_contributor_data')
  //    ->fields(array('aka', )
  return TRUE; // successfully saved all contributors
}
/*
 Released through http://bibliophile.sourceforge.net under the GPL licence.
 Do whatever you like with this -- some credit to the author(s) would be appreciated.

 A collection of PHP classes to manipulate bibtex files.

 If you make improvements, please consider contacting the administrators at bibliophile.sourceforge.net so that your improvements can be added to the release package.

 Mark Grimshaw 2004/2005
 http://bibliophile.sourceforge.net

 28/04/2005 - Mark Grimshaw.
 Efficiency improvements.

 11/02/2006 - Daniel Reidsma.
 Changes to preg_matching to account for Latex characters in names such as {\"{o}}
 */
// For a quick command-line test (php -f PARSECREATORS.php) after installation, uncomment these lines:
/***********************
 $authors = "Mark \~N. Grimshaw and Bush III, G.W. & M. C. H{\\'a}mmer Jr. and von Frankenstein, Ferdinand Cecil, P.H. & Charles Louis Xavier Joseph de la Vallee P{\\\"{o}}ussin";
 $creator = new PARSECREATORS();
 $creatorArray = $creator->parse($authors);
 print_r($creatorArray);
 ***********************/
/* Create writer arrays from bibtex input.
 'author field can be (delimiters between authors are 'and' or '&'):
 1. <first-tokens> <von-tokens> <last-tokens>
 2. <von-tokens> <last-tokens>, <first-tokens>
 3. <von-tokens> <last-tokens>, <jr-tokens>, <first-tokens>
 */
/**
 * @param $contribution Contributor entity object
 * @return unknown_type
 */
function biblio_parse_author($contributor) {
  $wrapper = biblio_wrapper($contributor, 'biblio_contributor');
  $name = $wrapper->biblio_contributor_name->value();
  
  if (isset($category) && $category == 5) {
    $author_object->firstname = '';
    $author_object->initials = '';
    $author_object->lastname = trim($author_object->name);
    $author_object->prefix = '';
    $author_object->literal = 1;
  }
  else {
    $value = trim($name);
    $appellation = $prefix = $surname = $firstname = $initials = '';
    $prefix = "";
    $value = preg_replace("/\s{2,}/", ' ', $value); // replace multiple white space by single space
    $author = explode(",", $value);
    $size = sizeof($author);
    // No commas therefore something like Mark Grimshaw, Mark Nicholas Grimshaw, M N Grimshaw, Mark N. Grimshaw
    if ($size == 1) {
      // Is complete surname enclosed in {...}, unless the string starts with a backslash (\) because then it is
      // probably a special latex-sign..
      // 2006.02.11 DR: in the last case, any NESTED curly braces should also be taken into account! so second
      // clause rules out things such as author="a{\"{o}}"
      //
      if (preg_match("/(.*) {([^\\\].*)}/", $value, $matches) && !(preg_match("/(.*) {\\\.{.*}.*}/", $value, $matches2))) {
        $author = explode(" ", $matches[1]);
        $surname = $matches[2];
      }
      else {
        $author = explode(" ", $value);
        // last of array is surname (no prefix if entered correctly)
        $surname = array_pop($author);
      }
    }
    // Something like Grimshaw, Mark or Grimshaw, Mark Nicholas  or Grimshaw, M N or Grimshaw, Mark N.
    else
    if ($size == 2) {
      // first of array is surname (perhaps with prefix)
      list ($surname, $prefix) = _grabSurname(array_shift($author));
    }
    // If $size is 3, we're looking at something like Bush, Jr. III, George W
    else {
      // middle of array is 'Jr.', 'IV' etc.
      $appellation = implode(' ', array_splice($author, 1, 1));
      // first of array is surname (perhaps with prefix)
      list ($surname, $prefix) = _grabSurname(array_shift($author));
    }
    $remainder = implode(" ", $author);
    list ($firstname, $initials, $prefix2) = _grabFirstnameInitials($remainder);
    if (!empty ($prefix2))
    $prefix .= $prefix2;
    //var_dump($prefix);
    //$surname = $surname . ' ' . $appellation;
    $initials = (strlen(trim($initials)) > 10) ? drupal_substr(trim($initials), 0, 10) : trim($initials);
    $wrapper->biblio_contributor_first_name = (empty($firstname))? NULL : trim($firstname);
    $wrapper->biblio_contributor_initials = (empty($initials))? NULL : $initials;
    $wrapper->biblio_contributor_last_name = (empty($surname))? NULL : trim($surname);
    $wrapper->biblio_contributor_prefix = (empty($prefix))? NULL : trim($prefix);
    $wrapper->biblio_contributor_suffix = (empty($appellation))? NULL : trim($appellation);
  }
  // @todo: get hashing working for contributors
  // $author_object->md5 =  _md5sum($author_object);
  return $contributor;
}
/**
 * @param $creator
 * @return unknown_type
 */
function _md5sum($creator) {
  $string = $creator->firstname . $creator->initials . $creator->prefix . $creator->lastname;
  $string = str_replace(' ', '', drupal_strtolower($string));
  return md5($string);
}
// grab firstname and initials which may be of form "A.B.C." or "A. B. C. " or " A B C " etc.
/**
 * @param $remainder
 * @return unknown_type
 */
function _grabFirstnameInitials($remainder) {
  $prefix = array();
  $firstname = $initials = '';
  $array = explode(" ", $remainder);
  foreach ($array as $value) {
    $first_char = drupal_substr($value, 0, 1);
    if ((ord($first_char) >= 97) && (ord($first_char) <= 122)) {
    $prefix[] = $value;
    }
    elseif (preg_match("/[a-zA-Z]{2,}/", trim($value))) {
      $firstname_array[] = trim($value);
    }
    else {
      $initials_array[] = trim(str_replace(".", " ", trim($value)));
    }
  }
  if (isset ($initials_array)) {
    $initials = implode(" ", $initials_array);
  }
  if (isset ($firstname_array)) {
    $firstname = implode(" ", $firstname_array);
  }
  if (!empty ($prefix)) {
    $prefix = implode(" ", $prefix);
  }
  return array($firstname, $initials, $prefix);
}
// surname may have title such as 'den', 'von', 'de la' etc. - characterised by first character lowercased.  Any
// uppercased part means lowercased parts following are part of the surname (e.g. Van den Bussche)
/**
 * @param $input
 * @return unknown_type
 */
function _grabSurname($input) {
  $no_prefix = FALSE;
  $surname = FALSE;
  $prefix  = FALSE;

  $surname_array = explode(" ", $input);

  foreach ($surname_array as $value) {
    $first_char = substr($value, 0, 1);
    if (!$no_prefix && (ord($first_char) >= 97) && (ord($first_char) <= 122)) {
      $prefix[] = $value;
    }
    else {
      $surname[] = $value;
      $no_prefix = TRUE;
    }
  }
  if (!empty($surname)) {
    $surname = implode(" ", $surname);
  }
  if (!empty ($prefix)) {
    $prefix = implode(" ", $prefix);
  }
  return array($surname, $prefix);
}
/**
 * @return unknown_type
 */
function _loadMD5() {
  static $md5   = array();
  static $count = 0;
  $db_count = db_query("SELECT COUNT(*) FROM {biblio_contributor_data}")->fetchField();
  if ($db_count != $count) { //refresh the cached data as some new authors may have been added or removed
    $count = $db_count;
    $md5 = array();
    $result = db_query('SELECT md5,cid  FROM {biblio_contributor_data} ');
    foreach ($result as $row ) {
      $md5[$row->cid] = $row->md5;
    }
  }
  return (count($md5)) ? $md5 : NULL;
}

function biblio_contributor_categories($publication_type = NULL) {
  $categories =  array(
    'primary' => array(
      'label' => t('Primary Contributors'),
      'field' => 'biblio_primary_contributors',
    ),
    'secondary' => array(
      'label' => t('Secondary Contributors'),
      'field' => 'biblio_secondary_contributors',
    ),
    'tertiary' => array(
      'label' => t('Tertiary Contributors'),
      'field' => 'biblio_tertiary_contributors',
    ),
    'subsidiary' => array(
      'label' => t('Subsidiary Contributors'),
      'field' => 'biblio_subsidiary_contributors',
    ),
    'corporate' => array(
      'label' => t('Corporate/Institutional Contributors'),
      'field' => 'biblio_corporate_contributors',
    ),
  );

  if ($publication_type) {
    $instances = field_info_instances('biblio', $publication_type);
    foreach($categories as $category => $info) {
      if (isset($instances[$info['field']])) {
        $categories[$category]['label'] = $instances[$info['field']]['label'];
      }
    }
  }

  return $categories;
}

/**
 * Extracts Contributor data from a Biblio entity and lists contributors
 * together in an array.
 *
 * @param  [type] $biblio [description]
 * @return [type]         [description]
 */
function biblio_get_contributors($biblio) {
  $wrapper = biblio_wrapper($biblio, 'biblio');
  $categories = biblio_contributor_categories();
  $contributors = array();

  foreach ($categories as $category => $info) {
    // dpm('Catgegory: ' . $category);
    // biblio_primary_contributors, biblio_secondary_contributors, etc...
    $field = $info['field'];
    foreach ($wrapper->{$field}->value() as $delta => $contributor) {
      $cid = $wrapper->{$field}[$delta]->cid->value();
      if (isset($cid) && !empty($cid)) {
        $contributors[$cid]['name'] = 
          $wrapper->{$field}[$delta]->biblio_contributor_name->value();
        $contributors[$cid]['category'] = $category;
      }
    }
  }

  return $contributors;
}
