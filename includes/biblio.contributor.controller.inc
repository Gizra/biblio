<?php

/**
 * Contributor class.
 */
class BiblioContributor extends Entity {

  public function __construct($values = array()) {
    parent::__construct($values, 'biblio_contributor');
  }

  /**
   * Overrides Entity::save().
   *
   * Re-build the contributor name.
   */
  public function save() {
    $this->name = $this->lastname . ' ' . $this->firstname;
    parent::save();
  }

  public function parseName() {
    if (isset($author_array['auth_category']) && $author_array['auth_category'] == 5) {
      $author_array['firstname'] = '';
      $author_array['initials'] = '';
      $author_array['lastname'] = trim($author_array['name']);
      $author_array['prefix'] = '';
      $author_array['literal'] = 1;
    }
    else {
      $value = trim($author_array['name']);
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
      $author_array['firstname'] = trim($firstname);
      $author_array['initials'] = (strlen(trim($initials)) > 10) ? drupal_substr(trim($initials), 0, 10) : trim($initials);
      $author_array['lastname'] = trim($surname);
      $author_array['prefix'] = trim($prefix);
      $author_array['suffix'] = trim($appellation);
    }
    $author_array['md5'] =  _md5sum($author_array);
    return $author_array;
  }

  /**
   * @param $creator
   * @return unknown_type
   */
  function _md5sum($creator) {
    $string = $creator['firstname'] . $creator['initials'] . $creator['prefix'] . $creator['lastname'];
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
}

