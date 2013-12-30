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

    if (empty($result['biblio'])) {
      return;
    }

    $row = reset($result['biblio']);
    $biblio = biblio_load($row->bid);

    return $biblio;
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
   *   String with name(s) of the contributors, or an array of Biblio
   *   contributor objects.
   * @param $role
   *   The role of the contributor (e.g "Author", "Editor).
   */
  public function addBiblioContributorsToCollection(Biblio $biblio, $names, $role) {
    $contributors = is_string($names) ? $this->getBiblioContributorsFromNames($names) : $names;

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
   * Returns Biblio Publication Type based on Publication Type.
   *
   * @param $type
   *  Bibtex Publication Type.
   *
   * @return
   *  Biblio Publication Type if the Publication Type was found in the Mapping,
   *  otherwise returns the given type in lowercase.
   */
  public function getBiblioType($type) {
    $map = $this->getMapping();
    $type = strtolower($type);

    if (!empty($map['type'][$type])) {
      return $map['type'][$type];
    }

    return $type;
  }

  /**
   * Entry point for data import.
   *
   * @param $data
   *   String of data.
   * @param array $options
   *   Array of options to pass to the Biblio style.
   *
   * @return array
   *   Array keyed with 'new', 'duplicate', and 'error.
   */
  public function import($data, $options = array()) {
    $return = array('new' => array(), 'duplicate' => array(), 'error' => array());

    if ($result = $this->importData($data, $options)) {
      $return = $this->saveOrFindDuplicateFromImportData($result);
    }
    return $return;
  }

  /**
   * Helper function to import existing or new keywords.
   *
   * @param EntityMetadataWrapper $wrapper
   *   The wrapped Biblio.
   * @param $property_name
   *   The propery name. Defaults to "biblio_keywords".
   * @param array $keywords
   *   Array of term names.
   * @param string $vocabulary_name
   *   The vocabulary of the terms. Defaults to "biblio_keywords".
   */
  public function importKeywordsList(EntityMetadataWrapper $wrapper, $keywords = array(), $property_name = 'biblio_keywords', $vocabulary_name = 'biblio_keywords') {
    $terms = array();
    foreach ($keywords as $term_name) {
      if (!$term = taxonomy_get_term_by_name($term_name, $vocabulary_name)) {
        $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_name);
        $values = array(
          'name' => $term_name,
          'vid' => $vocabulary->vid,
        );
        $term = entity_create('taxonomy_term', $values);
        taxonomy_term_save($term);
      }

      // taxonomy_get_term_by_name() returns an array, so normalize it.
      $terms[] = is_array($term) ? reset($term) : $term;
    }

    $wrapper->{$property_name}->set($terms);
  }

  /**
   * Given a of successful import, find duplicates or save the new Biblios.
   *
   * @param $result
   *   Array keyed with 'success' and 'error'.
   *
   * @return array
   *   Array keyed with 'new', 'duplicate', and 'error.
   *
   * @see BiblioStyleBase::saveOrFindDuplicateFromImportData()
   */
  public function saveOrFindDuplicateFromImportData($result) {
    $result += array('success' => array());
    foreach ($result['success'] as $biblio) {
      if ($duplicate_biblio = $this->isDuplicate($biblio)) {
        $result['duplicate'][] = $duplicate_biblio;
      }
      else {
        $biblio->save();
        $result['new'][] = $biblio;
      }
    }
    unset($result['success']);
    return $result;
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
