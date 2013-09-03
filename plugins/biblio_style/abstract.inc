<?php

/**
 * Biblio Style interface
 *
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

    // Run through field collection items and get the contributor's target IDs.
    $contributors = array();
    $wrapper = entity_metadata_wrapper('biblio', $biblio);
    foreach ($wrapper->contributor_field_collection as $sub_wrapper) {
      $contributors[] = array(
        $sub_wrapper->biblio_contributor->getIdentifier(),
        $sub_wrapper->biblio_contributor_role->getIdentifier(),
      );
    }

    // Replace field collection item in clone with the contributor's target IDs.
    $clone->contributor_field_collection = $contributors;

    $md5 = md5(serialize($clone));

    return $md5;
  }

  /**
   * Searches for a biblio object similar to the given one.
   *
   * @param $biblio
   *  Biblio object.
   *
   * @return
   *  The duplicate Biblio ID, FALSE if not found.
   */
  public function isDuplicate($biblio) {
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
  public function getBiblioContributor($biblio_contributor) {
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
      // Found similar contributor.
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
}
/**
 * Grab first name, initials and surname prefix from a given string.
 *
 * @param $value
 *   String containing first name, initials and perhaps a surname prefix.
 *
 * @return
 *   Array with three separated strings: first name, initials and prefix.
 */
function _grabFirstnameInitials($value) {
  $prefix = array();
  $firstname = array();
  $initials = array();

  // Split the given string to its parts.
  $parts = explode(' ', $value);

  foreach ($parts as $part) {
    if ((ord(substr($part, 0, 1)) >= 97) && (ord(substr($part, 0, 1)) <= 122)) {
      // The part starts with a lowercase letter, so this is a surname prefix
      // such as 'den', 'von', 'de la' etc.
      $prefix[] = $part;
      continue;
    }

    if (preg_match("/[a-zA-Z]{2,}/", trim($part))) {
      // The part is an alphabetical string containing two letters or more and
      // therefore a first name such as George, Mark etc.
      $firstname[] = $part;
      continue;
    }

    // The part contains only one letter and perhaps a dot, therefore this is
    // an initial like the W in George W. Bush.
    $initials[] = trim(str_replace(".", " ", trim($part)));
  }

  // Convert arrays to strings.
  $prefix = !empty($prefix) ? implode(' ', $prefix) : '';
  $firstname = !empty($firstname) ? implode(' ', $firstname) : '';
  $initials = !empty($initials) ? implode(' ', $initials) : '';

  return array($firstname, $initials, $prefix);
}
