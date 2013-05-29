<?php

/**
 * @file
 */

/**
 *
 */
class MigrateDestinationBiblioRole extends MigrateDestination {
  public function __toString() {
    return 'biblio-role';
  }

  static public function getKeySchema() {
    return array(
      'rid' => array(
        'type' => 'int',
        'not null' => TRUE,
      ),
    );
  }

  /**
   * Delete a biblio role.
   *
   * @param $rid
   *  Role ID to remove.
   */
  public function rollback($rid) {
    user_role_delete($rid);
  }

  /**
   * Import a biblio role.
   *
   * @param $entity
   *  Object filled with any fields mapped in the migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   *
   * @return
   *   The new role ID.
   */
  public function import(stdClass $entity, stdClass $row) {
    $this->prepare($entity, $row);
    $status = user_role_save($entity);
    $this->complete($entity, $row);

    if ($status) {
      $role = user_role_load_by_name($entity->name);
      return array($role->rid);
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
      'rid' => 'The role ID.',
      'name' => 'The role name.',
      'weight' => 'Ordering weight.',
    );
  }
}
