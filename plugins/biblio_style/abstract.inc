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
  function biblio_generate_biblio_md5($biblio) {
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
    if (!empty($clone->contributor_field_collection[LANGUAGE_NONE])) {
      foreach ($clone->contributor_field_collection[LANGUAGE_NONE] as &$item) {
        $wrapper = entity_metadata_wrapper('field_collection_item', $item['entity']);
        $contributors[] = array(
          $wrapper->biblio_contributor->getIdentifier(),
          $wrapper->biblio_contributor_role->getIdentifier(),
        );
      }
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
    $md5 = biblio_generate_biblio_md5($biblio);

    $query = new EntityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'biblio')
      ->propertyCondition('md5', $md5)
      ->range(0, 1)
      ->execute();

    return !empty($result['biblio']) ? key($result['biblio']) : FALSE;
  }

  /**
   * Handles getting biblio contributor object; Returns an existing contributor
   * if the given contributor was found, otherwise creates it.
   *
   * @param $biblio_contributor
   *  Biblio Contributor object.
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
      $biblio_contributor = biblio_contributor_load(key($result['biblio_contributor']));
      return;
    }

    $biblio_contributor->md5 = $md5;
    $biblio_contributor->save();
  }
}
