<?php

/**
 * @file
 * Upgrade nodes that are group-content, by settings the correct field
 * value.
 */

class BiblioMigrateTypes extends Migration {

  public function __construct() {
    $this->description = t('Migrate old biblio entries.');

    $this->query = db_select('_biblio_types_1x', 'bt')->fields('bt');
    $this->query->innerJoin('_biblio_1x', 'b', 'biblio_type = tid');

    parent::__construct();

    $source_key = array(
      'type_id' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Biblio type ID.',
      ),
    );

    $this->map = new MigrateSQLMap($this->machineName, $source_key, MigrateDestinationBiblioType::getKeySchema());
    $this->source = new MigrateSourceSQL($this->query);
    $this->destination = new MigrateDestinationBiblioType();

    $this->addFieldMapping('group_type', NULL)->defaultValue('node');
    $this->addFieldMapping('gid', 'group_nid');

    $this->addFieldMapping('entity_type', NULL)->defaultValue('node');
    $this->addFieldMapping('etid', 'nid');
  }
}

class BiblioMigrateContent extends Migration {

  public function __construct() {
    $this->description = t('Migrate old biblio entries.');

    $this->query = db_select('_biblio_1x', 'b')->fields('b');

    parent::__construct();

    $source_key = array(
      'group_nid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Biblio entry ID.',
      ),
    );

    $this->map = new MigrateSQLMap($this->machineName, $source_key, MigrateDestinationBiblioEntry::getKeySchema());
    $this->source = new MigrateSourceSQL($this->query);
    $this->destination = new MigrateDestinationBiblioEntry();

    $this->addFieldMapping('group_type', NULL)->defaultValue('node');
    $this->addFieldMapping('gid', 'group_nid');

    $this->addFieldMapping('entity_type', NULL)->defaultValue('node');
    $this->addFieldMapping('etid', 'nid');
  }
}
