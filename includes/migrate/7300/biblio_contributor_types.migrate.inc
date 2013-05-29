<?php

/**
 * @file
 */
class BiblioMigrateContributorTypes extends Migration {
  public function __construct() {
    $this->description = t('Migrate biblio 1.x contributor types.');

    // Fetch the used contributor types.
    $this->query = db_select('_biblio_contributor_type_data_1x', 'ctd')->distinct();
    $this->query->addField('ctd', 'title');
    $this->query->innerJoin('_biblio_contributor_1x', 'c', 'ctd.auth_type = c.auth_type');

    parent::__construct();

    $source_key = array(
      'title' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
    );

    $this->map = new MigrateSQLMap($this->machineName, $source_key, MigrateDestinationBiblioRole::getKeySchema());
    $this->source = new MigrateSourceSQL($this->query);
    $this->destination = new MigrateDestinationBiblioRole();

    $this->addFieldMapping('name', 'title');
    $this->addFieldMapping('weight', NULL)->defaultValue(0);
  }

  public function prepare($entity, $row) {
    $entity->name = drupal_strtolower($entity->name);
  }
}
