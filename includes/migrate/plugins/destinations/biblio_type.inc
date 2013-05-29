<?php

/**
 * @file
 * Import Organic group memberships.
 */

/**
 * Destination class implementing migration into an OG Audience field.
 */
class MigrateDestinationBiblioType extends MigrateDestination {

  public function __toString() {
    return t('biblio-entry');
  }

  static public function getKeySchema() {
    return array(
      'id' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
    );
  }

  /**
   * Delete a biblio type.
   *
   * @param $type
   *  The type to be deleted.
   */
  public function rollback($type) {
    biblio_type_delete($type);
  }

  /**
   * Import a biblio type.
   *
   * @param $entity
   *  Object object to build. Filled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   *
   * @return
   */
  public function import(stdClass $entity, stdClass $row) {
    $this->prepare($entity, $row);

    if (empty($entity->type)) {
      throw new MigrateException(t('Missing biblio type.'));
    }
    if (empty($entity->description)) {
      throw new MigrateException(t('Missing description.'));
    }

    // Save the type.
    biblio_type_save($entity);

    $this->complete($entity, $row);

    return array($entity->type);
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
      'type' => 'The machine-readable name of this type.',
      'name' => 'The human-readable name of this type.',
      'description' => 'A brief description of this type.',
    );
  }
}
