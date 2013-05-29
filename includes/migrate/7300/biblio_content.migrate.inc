<?php

/**
 * @file
 * Upgrade nodes that are group-content, by settings the correct field
 * value.
 */

class BiblioMigrateContent extends Migration {
  public $dependencies = array('BiblioMigrateTypes');

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
