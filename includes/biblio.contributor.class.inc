<?php
class BiblioContributors {
  public $data;
  public $sort_name;
  public $sort_letter;
  private $categories;
  private $nid;
  private $vid;

  function __construct() {
      $this->categories = array(1 => '', 2 => '', 3 => '', 4 => '', 5 => '');
      $this->sort_name = '';
      $this->sort_letter = '';
      $a = func_get_args();
      $num_args = func_num_args();
      switch($num_args) { // poor mans constructor overloading...
        case 1:
          if(is_array($a[0])) {
            $this->_contstruct_from_authors($a[0]);
          }
          break;
        case 2:
          if(is_numeric($a[0]) && is_numeric($a[1])) {
            $this->_contstruct_from_node($a[0], $a[1]);
          }
        case 0:
        default:
          $this->_contstruct_empty();
          break;
      }
  }

  private function _contstruct_empty() {
      $this->nid = NULL;
      $this->vid = NULL;
  }

  private function _contstruct_from_authors(array $authors) {
      $this->nid = NULL;
      $this->vid = NULL;
      $this->add_authors($authors);
  }

  private function _contstruct_from_node($nid, $vid) {
      $this->nid = $nid;
      $this->vid = $vid;
      $this->_load_contributors();
  }

  private function _load_contributors() {
    if (empty($this->vid)) return;
    $contributors = array();
    $query = db_select('biblio_contributor', 'bc', array('fetch' => PDO::FETCH_ASSOC));
    $query->join('biblio_contributor_data', 'bcd', 'bc.cid = bcd.cid');
    $query->fields('bcd');
    $query->fields('bc', array('auth_type', 'auth_category', 'rank'));
    $query->condition('bc.vid', $this->vid)->orderBy('bc.rank', 'ASC');
    $result = $query->execute();

    foreach  ($result as $creator ) {
      $this->data[] = $creator;
      $this->categories[$creator['auth_category']]++;
      if (empty($this->sort_name)) {
        $this->sort_name = $creator['lastname'];
        $this->sort_letter = drupal_substr(drupal_ucfirst(ltrim($this->sort_name)), 0, 1);
      }
    }
  }

  function add_authors(array $authors) {
    foreach ($authors as $author) {
      $this->add_author($author);
    }
  }

  function add_author(array $author) {
    if (!empty($author) && is_array($author) && !empty($author['name'])) {
      $this->data[] = $author;
      end($this->data);
      $this->_parse_author(key($this->data));
    }
  }

  function load($vid) {
    $this->vid = $vid;
    $this->_load_contributors();
  }

  function insert($nid, $vid) {
    $this->nid = $nid;
    $this->vid = $vid;

    if (!empty($this->data)) {
      return $this->_save_contributors();
    }
  }

  function update($nid, $vid) {
    $this->nid = $nid;
    $this->vid = $vid;
    if (!empty($this->data)) {
      $this->_save_contributors(TRUE);
    }
    $this->delete_orphan_authors();
    return;
  }

  function delete() {
    if (!$this->nid) return;
    $count = db_delete('biblio_contributor')
      ->condition('nid', $this->nid)
      ->execute();
    $this->data = array();
    $this->delete_orphan_authors();
    return $count;
  }

  function has_category($category) {
    return isset($this->categories[$category]) ? $this->categories[$category] : NULL;
  }

  function get_all() {
    return $this->data;
  }

  function get_category($category = NULL) {
    if (empty($category)) return;
    $authors = array();
    foreach ($this->data as $author) {
      if ($author['auth_category'] == $category) {
        $authors[] = $author;
      }
    }
    return $authors;
  }

function get_first_contributor() {
  return $this->data[0];
}



function biblio_delete_contributors_revision($node) {
  $count = db_delete('biblio_contributor')
            ->condition('vid', $node->vid)
            ->execute();
  biblio_delete_orphan_authors();
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

function delete_orphan_authors($force = FALSE) {
  if (variable_get('biblio_auto_orphaned_author_delete', 0) || $force) {
//    db_query('DELETE FROM {biblio_contributor_data} WHERE cid NOT IN (SELECT DISTINCT(cid) FROM {biblio_contributor})');
    $sub_select = db_select('biblio_contributor', 'bc')->fields('bc', array('cid'))->distinct();
    db_delete('biblio_contributor_data')
      ->condition('cid', $sub_select, 'NOT IN')
      ->execute();
  }
}


private function _save_contributor($key) {
  return drupal_write_record('biblio_contributor_data', $this->data[$key]);
}

function update_contributor(&$author) {
  if (!isset($author['cid'])) return FALSE;
  return drupal_write_record('biblio_contributor_data', $author, 'cid');
}

function update_contributors(array $contributors) {
  $this->data = array();
  foreach ($contributors as $author) {
    $this->add_author($author);
  }
}

/**
 * Save contributors to the database
 * @param $authors
 * @param $nid
 * @param $vid
 * @param $update
 * @return success of database operations
 */
private function _save_contributors($update = FALSE) {
  $md5 = _loadMD5();
  $rank = 0;
  //  db_query('DELETE FROM {biblio_contributor} WHERE nid = :nid AND vid = :vid', array(':nid' => $nid, ':vid' => $vid));
  db_delete('biblio_contributor')
    ->condition(db_and()->condition('nid', $this->nid)->condition('vid', $this->vid))
    ->execute();
  foreach ($this->data as  $key => &$author) {
 //   foreach ($authors as $key => $author) {
      if (!empty($author['name'])) {
        if (empty($author['lastname'])) {
          $this->_parse_author($key);
        }
        if ($update && !empty($author['cid']))  $author['cid'] = NULL; // null out the cid so we don't do a global change
        if (empty ($author['cid']) && isset($author['md5']) && !empty ($md5)) {
          $author['cid'] = array_search($author['md5'], $md5);
        }
        if (empty ($author['cid'])) {
          $this->_save_contributor($key);
          if (empty ($author['cid'])) return FALSE;
        }

        $link_array = array(
          'nid' => $this->nid,
          'vid' => $this->vid,
          'cid' => $author['cid'],
          'rank' => $rank++, //((isset($author['rank']) && is_numeric($author['rank'])) ? $author['rank'] : $key),
          'auth_type' => !empty($author['auth_type']) ? $author['auth_type'] :  $author['auth_category'],
          'auth_category' =>  $author['auth_category'],
        );

        if (!drupal_write_record('biblio_contributor', $link_array)) return FALSE;
      }
   // }
  }
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
 * @param $author_array
 * @return unknown_type
 */
private function _parse_author($key) {

  if ($this->data[$key]['auth_category'] == 5) {
    $this->data[$key]['firstname'] = '';
    $this->data[$key]['initials'] = '';
    $this->data[$key]['lastname'] = trim($this->data[$key]['name']);
    $this->data[$key]['prefix'] = '';
  }
  else {
    $this->data[$key]['name'] = trim($this->data[$key]['name']);
    $appellation = $prefix = $surname = $firstname = $initials = '';
    $prefix = "";
    $this->data[$key]['name'] = preg_replace("/\s{2,}/", ' ', $this->data[$key]['name']); // replace multiple white space by single space
    $author = explode(",", $this->data[$key]['name']);
    $size = count($author);
    // No commas therefore something like Mark Grimshaw, Mark Nicholas Grimshaw, M N Grimshaw, Mark N. Grimshaw
    if ($size == 1) {
      // Is complete surname enclosed in {...}, unless the string starts with a backslash (\) because then it is
      // probably a special latex-sign..
      // 2006.02.11 DR: in the last case, any NESTED curly braces should also be taken into account! so second
      // clause rules out things such as author="a{\"{o}}"
      //
      if (preg_match("/(.*) {([^\\\].*)}/", $this->data[$key]['name'], $matches) && !(preg_match("/(.*) {\\\.{.*}.*}/", $value, $matches2))) {
        $author = explode(" ", $matches[1]);
        $this->data[$key]['lastname'] = trim($matches[2]);
      }
      else {
        $author = explode(" ", $this->data[$key]['name']);
        // last of array is surname (no prefix if entered correctly)
        $this->data[$key]['lastname'] = array_pop($author);
      }
    }
    // Something like Grimshaw, Mark or Grimshaw, Mark Nicholas  or Grimshaw, M N or Grimshaw, Mark N.
    else
    if ($size == 2) {
      // first of array is surname (perhaps with prefix)
      $this->_grabSurname(array_shift($author), $key);
    }
    // If $size is 3, we're looking at something like Bush, Jr. III, George W
    else {
      // middle of array is 'Jr.', 'IV' etc.
      $appellation = implode(' ', array_splice($author, 1, 1));
      // first of array is surname (perhaps with prefix)
      $this->_grabSurname(array_shift($author), $key);
    }
    $remainder = implode(" ", $author);
    list ($firstname, $initials, $prefix2) = $this->_grabFirstnameInitials($remainder, $key);
    if (!empty ($prefix2))
    $prefix .= $prefix2;
     $this->data[$key]['suffix'] = trim($appellation);
  }
  $this->_md5sum($key);
}
/**
 * @param $creator
 * @return unknown_type
 */
private function _md5sum($key) {
  $string = $this->data[$key]['firstname'] . $this->data[$key]['initials'] . $this->data[$key]['prefix'] . $this->data[$key]['lastname'];
  $string = str_replace(' ', '', drupal_strtolower($string));
  $this->data[$key]['md5'] = md5($string);
}
// grab firstname and initials which may be of form "A.B.C." or "A. B. C. " or " A B C " etc.
/**
 * @param $remainder
 * @return unknown_type
 */
private function _grabFirstnameInitials($remainder, $key) {
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
//  if (!empty ($prefix)) {
    $prefix = implode(" ", $prefix);
//  }

  $this->data[$key]['firstname'] = trim($firstname);
  $this->data[$key]['initials'] = trim($initials);
  $this->data[$key]['prefix'] = !empty($this->data[$key]['prefix']) ? $this->data[$key]['prefix'] . trim($prefix) : trim($prefix);

}
// surname may have title such as 'den', 'von', 'de la' etc. - characterised by first character lowercased.  Any
// uppercased part means lowercased parts following are part of the surname (e.g. Van den Bussche)
/**
 * @param $input
 * @return unknown_type
 */
private function _grabSurname($input, $key) {
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
  $this->data[$key]['lastname'] = $surname;
  $this->data[$key]['prefix'] = $prefix;
}
/**
 * @return unknown_type
 */
private function _loadMD5() {
  static $md5   = array();
  static $count = 0;
  $db_count = db_query("SELECT COUNT(*) FROM {biblio_contributor_data}")->fetchField();;
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
}