<?php

/**
 * @file
 *
 * Migrate biblio contributors.
 */

class BiblioMigrateContributors extends Migration {
  public $dependencies = array('BiblioMigrateContributorTypes');

  public function __construct() {
    $this->description = t('Migrate biblio 1.x contributors.');

    $this->query = db_select('_biblio_contributor_data_1x', 'cd');
    $this->query->fields('cd', array('cid', 'name'));
    $this->query->addField('c', 'auth_type');
    $this->query->innerJoin('_biblio_contributor_1x', 'c', 'cd.cid = c.cid');

    parent::__construct();

    $source_key = array(
      'cid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'alias' => 'cd',
      ),
    );

    $this->map = new MigrateSQLMap($this->machineName, $source_key, MigrateDestinationBiblioContributor::getKeySchema());
    $this->source = new MigrateSourceSQL($this->query);
    $this->destination = new MigrateDestinationBiblioContributor();

    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('type', 'auth_type')->sourceMigration('BiblioMigrateContributorTypes');
    $this->addFieldMapping('created', NULL)->defaultValue(REQUEST_TIME);
    $this->addFieldMapping('changed', NULL)->defaultValue(REQUEST_TIME);
  }
}
