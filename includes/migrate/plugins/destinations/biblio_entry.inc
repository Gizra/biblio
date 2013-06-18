<?php

/**
 * @file
 */

/**
 *
 */
class MigrateDestinationBiblioEntry extends MigrateDestination {
  public function __toString() {
    return 'biblio-entry';
  }

  static public function getKeySchema() {
    return array(
      'bid' => array(
        'description' => 'The entry ID',
        'type' => 'int',
        'not null' => TRUE,
      ),
    );
  }

  /**
   * Delete a biblio entry.
   *
   * @param $id
   *  IDs to be deleted.
   */
  public function rollback(array $id) {
    entity_delete('biblio', $id['destid1']);
  }

  /**
   * Import a single biblio entry.
   *
   * @param $entity
   *  Object object to build. Filled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   *
   * @return array
   *  Array of key fields of the object that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $entity, stdClass $row) {
    $this->prepare($entity, $row);
    $bid = entity_save('biblio', $entity);
    $this->complete($entity, $row);

    return array($bid);
  }

  /**
   * Give handlers a shot at modifying the object before saving it.
   *
   * @param $entity
   *   OGMembership object to build. Prefilled with any fields mapped in
   *   the Migration.
   * @param $source_row
   *   Raw source data object - passed through to prepare handlers.
   */
  public function prepare($entity, stdClass $row) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();

    // Then call any prepare handler for this specific Migration.
    if (method_exists($migration, 'prepare')) {
      $migration->prepare($entity, $row);
    }
  }

  /**
   * Give handlers a shot at modifying the object (or taking additional action)
   * after saving it.
   *
   * @param $entity
   *   OGMembership object to build. This is the complete object after
   *   saving.
   * @param $source_row
   *   Raw source data object - passed through to complete handlers.
   */
  public function complete($entity, stdClass $row) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();

    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'complete')) {
      $migration->complete($entity, $row);
    }
  }

  public function fields() {
    return array(
      'bid' => '',
      'title' => 'The title of this Biblio, always treated as non-markup plain text.',
      'type' => 'The publication type of the biblio',
      'uid' => 'The Drupal user ID who created this biblio.',
      'created' => '',
      'changed' => '',
    );
  }
}
