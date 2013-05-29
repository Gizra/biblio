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
        'description' => 'The entry ID',
        'type' => 'int',
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
   *  Object object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields of the object that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $entity, stdClass $row) {

    $this->prepare($entity, $row);

    if (empty($entity->type)) {
      throw new MigrateException(t('Missing biblio type.'));
    }
    if (empty($entity->description)) {
      throw new MigrateException(t('Missing description.'));
    }

    $biblio_type = clone($entity);
    unset($biblio_type->migrate);
    $result = biblio_type_save($biblio_type);
    dpm($result, 'result');

    $this->complete($entity, $row);

    return array();
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
    $entity->migrate = array(
      'machineName' => $migration->getMachineName(),
    );

    // Call any general handlers.
    migrate_handler_invoke_all('og_membership', 'prepare', $entity, $row);
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
    $entity->migrate = array(
      'machineName' => $migration->getMachineName(),
    );

    // Call any general handlers.
    migrate_handler_invoke_all('og_membership', 'complete', $entity, $row);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'complete')) {
      $migration->complete($entity, $row);
    }
  }

  public function fields() {
    return array(
      'group_type' => 'Group entity type',
      'gid' => 'Group entity id',
      'entity_type' => 'Entity type',
      'etid' => 'Entity ID (e.g. user ID)',
      'state' => 'User\'s group membership can be OG_STATE_ACTIVE or OG_STATE_PENDING. Defaults to OG_STATE_ACTIVE.',
      'created' => 'Create date for this membership. Defaults to REQUEST_TIME',
      'is_admin' => 'Legacy: Is member an administrator in this group. Defaults to No.',
      'group_roles' => 'Array of role names to assign to the user in this group.',
    );
  }
}
