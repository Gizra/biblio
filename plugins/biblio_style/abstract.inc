<?php

/**
 * Biblio Style interface
 */
interface BiblioStyleInterface {

  /**
   * Constructor for the notifier.
   *
   * @param $plugin
   *   The notifier plugin object. Note the "options" values might have
   *   been overriden in message_notify_send_message().
   * @param Biblio $biblio
   *   The Biblio entity.
   */
  public function __construct($plugin, Biblio $biblio = NULL);


  /**
   * Render the Biblio according to the style plugin.
   *
   * @return
   *   A an HTML string.
   */
  public function render($options = array(), $langcode = NULL);
}

/**
 * Biblio Style interface for a style that supports import.
 */
interface BiblioStyleImportInterface extends BiblioStyleInterface {

  /**
   * Import data to Biblio.
   *
   * @param $data
   *   String of data.
   * @param array $options
   *   Array of options to pass to the Biblio style.
   *
   * @return
   *   Array keyed with:
   *   - 'succuess': Array of unsaved Biblio objects.
   *   - 'error': Array of data that could not be imported.
   *     @code
   *     $return = array(
   *       'error' => array(
   *         array(
   *           'data' => 'data sent to import',
   *           'error' => 'error message',
   *         ),
   *       ),
   *     ),
   *     @endcode
   */
  public function importData($data, $options = array());

}

/**
 * An abstract implementation of MessageNotifierInterface.
 */
abstract class BiblioStyleBase implements BiblioStyleInterface {

  /**
   * The plugin definition.
   */
  protected $plugin;

  /**
   * The Biblio entity.
   */
  protected $biblio;

  public function __construct($plugin, Biblio $biblio = NULL) {
    $this->plugin = $plugin;
    $this->biblio = $biblio;
  }

  public function render($options = array(), $langcode = NULL) {
    return '';
  }

  public function settingsForm() {
    return array();
  }

  public function settingsFormValidate() {

  }

  /**
   * Generates an md5 string based on a biblio object.
   * The md5 is later used to determine whether or not two Biblio objects are the
   * same and prevent duplications.
   *
   * @param $biblio
   *  Biblio object.
   *
   * @return
   *  MD5 string that represents the given biblio.
   */
  public static function generateBiblioMd5(Biblio $biblio) {
    $clone = clone $biblio;

    // Remove unique or temporary data.
    unset($clone->bid);
    unset($clone->cache);
    unset($clone->cache_id);
    unset($clone->changed);
    unset($clone->created);
    unset($clone->is_new);
    unset($clone->md5);
    unset($clone->_skip_cache);
    unset($clone->biblio_status);


    if (!empty($clone->contributor_field_collection)) {
      // Run through field collection items and get the contributor's target
      // IDs. We can't use the wrapper, as the entities might not be saved yet
      // so we access the fields directly.
      $contributors = array();
      foreach ($clone->contributor_field_collection[LANGUAGE_NONE] as $value) {
        $entity = !empty($value['entity']) ? $value['entity'] : entity_load_single('field_collection_item', $value['value']);

        $contributors[] = array(
          $entity->biblio_contributor[LANGUAGE_NONE][0]['target_id'],
          $entity->biblio_contributor_role[LANGUAGE_NONE][0]['target_id'],
        );
      }

      // Replace field collection item in clone with the contributor's target IDs.
      $clone->contributor_field_collection = $contributors;
    }

    $md5 = md5(serialize($clone));

    return $md5;
  }

  /**
   * Searches for a biblio object identical to the given one.
   *
   * @param $biblio
   *  Biblio object.
   *
   * @return
   *  The duplicate Biblio ID, FALSE if not found.
   */
  public function isDuplicate(Biblio $biblio) {
    $md5 = $this->generateBiblioMd5($biblio);

    $query = new EntityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'biblio')
      ->propertyCondition('md5', $md5)
      ->range(0, 1)
      ->execute();

    return !empty($result['biblio']) ? key($result['biblio']) : FALSE;
  }

  /**
   * Returns saved biblio contributor object; Returns an existing contributor
   * if the given contributor was found, otherwise creates it first.
   *
   * @param $biblio_contributor
   *  Biblio Contributor object.
   *
   * @return
   *  Saved Biblio Contributor object.
   */
  public function getBiblioContributor(BiblioContributor $biblio_contributor) {
    $clone = clone $biblio_contributor;

    unset($clone->cid);
    unset($clone->revision_id);
    unset($clone->changed);
    unset($clone->created);
    unset($clone->is_new);
    unset($clone->md5);

    $md5 = md5(serialize($clone));

    $query = new EntityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'biblio_contributor')
      ->propertyCondition('md5', $md5)
      ->range(0, 1)
      ->execute();

    if (!empty($result['biblio_contributor'])) {
      // Found existing contributor.
      return biblio_contributor_load(key($result['biblio_contributor']));
    }

    $biblio_contributor->md5 = $md5;
    $biblio_contributor->save();

    return $biblio_contributor;
  }

  public function renderEntryFiles(EntityMetadataWrapper $wrapper, $property_name = 'biblio_pdf') {
    if (!user_access('view uploaded files')) {
      return;
    }

    if (!$files =  $wrapper->{$property_name}->value()) {
      return;
    }

    $url = array();
    $files = !isset($files['fid']) ? $files : array($files);
    foreach ($files as $file) {
      $url[] = file_create_url($file['uri']);
    }

    return $url;
  }


  /**
   * Add contributors to the field collection of a Biblio.
   *
   * @param $names
   *   String with name(s) of the contributors.
   * @param $role
   *   The role of the contributor (e.g "Author", "Editor).
   */
  public function addBiblioContributorsToCollection(Biblio $biblio, $names, $role) {
    $contributors = $this->getBiblioContributorsFromNames($names);
    foreach ($contributors as $contributor) {
      // Create contributors field collections.
      $field_collection = entity_create('field_collection_item', array('field_name' => 'contributor_field_collection'));
      $field_collection->setHostEntity('biblio', $biblio);
      $collection_wrapper = entity_metadata_wrapper('field_collection_item', $field_collection);
      $collection_wrapper->biblio_contributor->set($contributor);

      // @todo: Add reference to correct term.
      if ($term = taxonomy_get_term_by_name(ucfirst($role), 'biblio_roles')) {
        $term = reset($term);
      }
      else {
        // Create a new term.
        $vocabulary = taxonomy_vocabulary_machine_name_load('biblio_roles');
        $values = array(
          'name' => ucfirst($role),
          'vid' => $vocabulary->vid,
        );
        $term = entity_create('taxonomy_term', $values);
        taxonomy_term_save($term);
      }

      $collection_wrapper->biblio_contributor_role->set($term);
    }
  }

  /**
   * Get saved contributor objects by their names.
   *
   * @param $names
   *  String of contributors' names separated by 'and'.
   *
   * @return
   *  Array of saved contributor objects.
   */
  public function getBiblioContributorsFromNames($names) {
    // Split names.
    $names = preg_split("/(and|&)/i", trim($names));

    $contributors = array();
    foreach ($names as $name) {
      // Parse contributor's name to get each part separately.
      $values = $this->parseContributorName($name);

      // Get existing Biblio Contributor object, save it if it doesn't exist.
      $biblio_contributor = biblio_contributor_create($values);
      $biblio_contributor = $this->getBiblioContributor($biblio_contributor);

      $contributors[] = $biblio_contributor;
    }

    return $contributors;
  }

  /**
   * Get each part of a contributor's name separately.
   *
   * @param $full_name
   *  Full name of contributor.
   *
   * @return
   *  Array of parsed name, ready for creating a contributor.
   */
  public function parseContributorName($full_name) {

    // @todo
    /*if (isset($contributor_array['auth_category']) && $contributor_array['auth_category'] == 5) {
      $contributor_array['firstname'] = '';
      $contributor_array['initials'] = '';
      $contributor_array['lastname'] = trim($contributor_array['name']);
      $contributor_array['prefix'] = '';
      $contributor_array['literal'] = 1;

      return $contributor_array;
    }*/

    $appellation = $prefix = $lastname = $firstname = $initials = '';

    // Remove unneeded spaces from full name.
    $full_name = trim($full_name);
    $full_name = preg_replace("/\s{2,}/", ' ', $full_name);

    // Split full name by commas.
    $name_parts = explode(',', $full_name);
    // Count commas.
    $commas = count($name_parts) - 1;

    if (!$commas) {
      // No commas in full name.

      if (preg_match("/(.*) {([^\\\].*)}/", $full_name, $matches) && !(preg_match("/(.*) {\\\.{.*}.*}/", $full_name, $matches2))) {
        // Complete last name enclosed in {...}, unless the string starts with a
        // backslash (\) because then it is probably a special latex-sign.
        // In the last case, any NESTED curly braces should also be taken into
        // account! so second clause rules out things such as 'a{\"{o}}'.

        $name_parts = explode(' ', $matches[1]);
        $lastname = $matches[2];
      }
      else {
        // The pattern is firstname-initials-lastname, such as 'George Bush',
        // 'George W. Bush', 'G W Bush' etc.

        // Split full name by spaces.
        $name_parts = explode(' ', $full_name);

        // Last element in the array should be the last name.
        // There shouldn't be a prefix if the name was entered correctly.
        $lastname = array_pop($name_parts);
      }
    }
    elseif ($commas == 1) {
      // There is 1 comma in full name, such as in 'Bush, George', 'Bush, G W',
      // 'Bush, George W', 'de la Bush, George W' etc.

      // Get last name and prefix separately.
      list ($lastname, $prefix) = $this->getLastname(array_shift($name_parts));
    }
    else {
      // There are 2 commas in full name, such as in 'Bush, Jr. III, George W'.

      // Middle element in array is 'Jr.', 'IV' etc.
      $appellation = implode(' ', array_splice($name_parts, 1, 1));

      // Get last name and prefix separately.
      list($lastname, $prefix) = $this->getLastname(array_shift($name_parts));
    }

    // After removing the last name, prefix and appellation from the full name,
    // we are left with first name and initials, and perhaps last name prefix.
    $remainder = implode(' ', $name_parts);
    list($firstname, $initials, $prefix2) = $this->getFirstnameInitials($remainder);

    if (!empty($prefix2)) {
      // Found a prefix in remainder, add it to the last name prefix.
      $prefix .= $prefix2;
    }

    // Fill results in the array.
    $contributor_array['firstname'] = trim($firstname);
    $contributor_array['initials'] = substr(trim($initials), 0, 10);
    $contributor_array['lastname'] = trim($lastname);
    $contributor_array['prefix'] = trim($prefix);
    $contributor_array['suffix'] = trim($appellation);

    return $contributor_array;
  }

  /**
   * Get first name, initials and last name prefix from a given string.
   *
   * @param $value
   *   String containing first name, initials and perhaps a last name prefix.
   *
   * @return
   *   Array with three separated strings: first name, initials and prefix.
   */
  public function getFirstnameInitials($value) {
    $prefix = array();
    $firstname = array();
    $initials = array();

    // Split the given string to its parts.
    $parts = explode(' ', $value);

    foreach ($parts as $part) {
      if ((ord(substr($part, 0, 1)) >= 97) && (ord(substr($part, 0, 1)) <= 122)) {
        // The part starts with a lowercase letter, so this is a last name prefix
        // such as 'den', 'von', 'de la' etc.
        $prefix[] = $part;
        continue;
      }

      if (preg_match("/[a-zA-Z]{2,}/", $part)) {
        // The part starts with an uppercase letter and contains two letters or
        // more and therefore is a first name such as 'George'.
        $firstname[] = $part;
        continue;
      }

      // The part contains only one uppercase letter and perhaps a dot and
      // therefore is an initial such as the W in 'George W. Bush'.
      $initials[] = trim(str_replace('.', ' ', $part));
    }

    // Convert arrays to strings.
    $prefix = !empty($prefix) ? implode(' ', $prefix) : '';
    $firstname = !empty($firstname) ? implode(' ', $firstname) : '';
    $initials = !empty($initials) ? implode(' ', $initials) : '';

    return array($firstname, $initials, $prefix);
  }

  /**
   * Get last name and last name prefix from a given string.
   *
   * @param $value
   *   String containing last name and last name prefix.
   *
   * @return
   *   Array with two separated strings: last name and last name prefix.
   */
  public function getLastname($value) {
    $prefix  = array();
    $lastname = array();

    // Split the given string to its parts.
    $parts = explode(' ', $value);

    foreach ($parts as $part) {
      if (empty($lastname) && (ord(substr($part, 0, 1)) >= 97) && (ord(substr($part, 0, 1)) <= 122)) {
        // The part starts with a lowercase letter and comes before any last names
        // were found so this is a prefix such as 'den', 'von', 'de la' etc.
        $prefix[] = $part;
        continue;
      }

      // The part starts with an uppercase letter and/or comes after a last name
      // was found in the given string so this is a last name such as 'Bush'.
      $lastname[] = $part;
    }

    // Convert arrays to strings.
    $prefix = !empty($prefix) ? implode(' ', $prefix) : '';
    $lastname = !empty($lastname) ? implode(' ', $lastname) : '';

    return array($lastname, $prefix);
  }

  /*
   * Returns Biblio Publication Type based on Publication Type.
   *
   * @param $type
   *  Bibtex Publication Type.
   *
   * @return
   *  Biblio Publication Type if the Publication Type was found in the Mapping,
   *  otherwise returns the given type in lowercase.
   */
  function getBiblioType($type) {
    $map = $this->getMapping();
    $type = strtolower($type);

    if (!empty($map['type'][$type])) {
      return $map['type'][$type];
    }

    return $type;
  }

  /**
   * @todo: Make abstract function?
   */
  public function getMapping() {
    return array(
      'field' => array(),
      'type' => array(),
    );
  }
}
