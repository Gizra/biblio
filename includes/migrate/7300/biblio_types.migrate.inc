<?php

/**
 * @file
 */

class BiblioMigrateTypes extends Migration {

  public function __construct($arguments = array()) {
    $this->description = t('Migrate biblio 1.x entry types.');

    // Select the only the types that were in use in the 1.x biblio.
    $this->query = db_select('_biblio_types_1x', 'bt')->fields('bt');
    $this->query->innerJoin('_biblio_1x', 'b', 'biblio_type = tid');
    $this->query->distinct();

    parent::__construct($arguments);

    $source_key = array(
      'tid' => array(
        'type' => 'int',
        'not null' => TRUE,
      ),
    );

    $this->map = new MigrateSQLMap($this->machineName, $source_key, MigrateDestinationBiblioType::getKeySchema());
    $this->source = new MigrateSourceSQL($this->query);
    $this->destination = new MigrateDestinationBiblioType();

    $this->addFieldMapping('type', 'name');
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('description', 'description');
  }

  public function prepare($entity, $row) {
    // Create the type machine name.
    $entity->type = drupal_strtolower($entity->type);
    $entity->type = str_replace(array('-', ' '), '_', $entity->type);

    // Make sure there's a description.
    if (!$entity->description) {
      $entity->description = $entity->name;
    }
  }
}
