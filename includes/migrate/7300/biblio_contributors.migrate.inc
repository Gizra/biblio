<?php

/**
 * @file
 */
class BiblioMigrateContributors extends Migration {
  public $dependencies = array('BiblioMigrateContributorTypes');

  public function __construct() {
    $this->description = t('Migrate biblio 1.x contributors.');

    $this->query = db_select('_biblio_1x', 'b');
    $this->query->fields('b', array('biblio_type', 'biblio_year'));
    $this->query->fields('n', array('nid', 'title', 'created', 'uid'));
    $this->query->innerJoin('node', 'n', 'n.nid = b.nid');
    // Ignore unpublished biblio entries.
    $this->query->condition('n.status', NODE_PUBLISHED);

    parent::__construct();

    $source_key = array(
      'nid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'alias' => 'n',
      ),
    );

    $this->map = new MigrateSQLMap($this->machineName, $source_key, MigrateDestinationBiblioContributor::getKeySchema());
    $this->source = new MigrateSourceSQL($this->query);
    $this->destination = new MigrateDestinationBiblioContributor();

    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('type', 'biblio_type')->sourceMigration('BiblioMigrateContributorTypes');
    $this->addFieldMapping('uid', 'uid');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', NULL)->defaultValue(REQUEST_TIME);
  }
}
