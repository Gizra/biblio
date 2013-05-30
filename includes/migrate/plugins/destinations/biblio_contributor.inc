<?php

/**
 * @file
 *
 * Destination handler for biblio contributors.
 */

class MigrateDestinationBiblioContributor extends MigrateDestination {
  public function __toString() {
    return 'biblio-contributor';
  }

  static public function getKeySchema() {
    return array(
      'cid' => array(
        'type' => 'int',
        'not null' => TRUE,
      ),
    );
  }

  /**
   * Delete a biblio type.
   *
   * @param $cid
   *  Contributor ID to remove.
   */
  public function rollback($cid) {
    entity_delete('biblio_contributor', $cid['destid1']);
  }

  /**
   * Import a biblio contributor.
   *
   * @param $entity
   *  Object filled with any fields mapped in the migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   *
   * @return
   *   The new contributor ID.
   */
  public function import(stdClass $entity, stdClass $row) {
    $this->prepare($entity, $row);
    $status = entity_save('biblio_contributor', $entity);
    if (!$status) {
      throw new Exception(t('Biblio contributor could not be created.'));
    }
    $this->complete($entity, $row);

    return array($entity->cid);
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

  public function fields() {
    return array(
      'cid' => 'Biblio Contributor ID. Unique ID for the contributor entity type.',
      'name' => 'The name of the contributor.',
      'type' => 'The bundle name of the contributor entity',
      'created' => '',
      'changed' => '',
    );
  }
}
