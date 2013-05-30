<?php

/**
 * @file
 */
class BiblioMigrateContributorCollections extends Migration {
  public $dependencies = array(
    'BiblioMigrateEntries',
    'BiblioMigrateContributors',
    'BiblioMigrateContributorTypes',
  );

  public function __construct() {
    $this->description = t('Migrate biblio 1.x contributors into field collections.');

    // Fetch the used contributor types.
    $this->query = db_select('_biblio_contributor_1x', 'c');
    $this->query->fields('c', array('nid', 'cid', 'auth_type'));

    parent::__construct();

    $source_key = array(
      'nid' => array(
        'type' => 'int',
        'not null' => TRUE,
      ),
      'cid' => array(
        'type' => 'int',
        'not null' => TRUE,
      ),
      'auth_type' => array(
        'type' => 'int',
        'not null' => TRUE,
      ),
    );

    $this->map = new MigrateSQLMap($this->machineName, $source_key, BiblioMigrateDestinationFieldCollection::getKeySchema());
    $this->source = new MigrateSourceSQL($this->query);
    $this->destination = new BiblioMigrateDestinationFieldCollection('contributor_field_collection', array('host_entity_type' => 'biblio'));

    $this->addFieldMapping('bid', 'nid')->sourceMigration('BiblioMigrateEntries');
    $this->addFieldMapping('cid', 'cid')->sourceMigration('BiblioMigrateContributors');
    $this->addFieldMapping('rid', 'auth_type')->sourceMigration('BiblioMigrateContributorTypes');
  }
}
