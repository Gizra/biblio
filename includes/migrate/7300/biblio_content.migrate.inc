<?php

/**
 * @file
 */
class BiblioMigrateContent extends Migration {
  public $dependencies = array('BiblioMigrateTypes');

  public function __construct() {
    $this->description = t('Migrate old biblio entries.');

    $this->query = db_select('_biblio_1x', 'b')->fields('b');

    parent::__construct();

    $source_key = array(
      'nid' => array(
        'type' => 'int',
        'not null' => TRUE,
      ),
    );

    $this->map = new MigrateSQLMap($this->machineName, $source_key, MigrateDestinationBiblioEntry::getKeySchema());
    $this->source = new MigrateSourceSQL($this->query);
    $this->destination = new MigrateDestinationBiblioEntry();

    $this->addFieldMapping('title', 'biblio_sort_title');
    $this->addFieldMapping('biblio_year', 'biblio_year');
    $this->addFieldMapping('type', 'biblio_type')->sourceMigration('BiblioMigrateTypes');
    $this->addFieldMapping('created', NULL)->defaultValue(REQUEST_TIME);
    $this->addFieldMapping('changed', NULL)->defaultValue(REQUEST_TIME);
  }
}
