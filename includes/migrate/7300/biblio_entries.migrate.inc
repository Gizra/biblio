<?php

/**
 * @file
 */
class BiblioMigrateEntries extends Migration {

  public $dependencies = array('BiblioMigrateTypes');

  public function __construct() {
    $this->description = t('Migrate biblio 1.x entries.');

    $this->query = db_select('_biblio_1x', 'b');
    $this->query->innerJoin('node', 'n', 'n.nid = b.nid AND n.vid = b.vid');

    $this->query
      ->fields('b')
      ->fields('n', array('nid', 'title', 'created', 'uid'))
      // @todo: Ignore unpublished biblio entries.
      ->condition('n.status', NODE_PUBLISHED);

    parent::__construct();

    $source_key = array(
      'nid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'alias' => 'n',
      ),
    );

    $this->map = new MigrateSQLMap($this->machineName, $source_key, MigrateDestinationEntityAPI::getKeySchema('biblio'));
    $this->source = new MigrateSourceSQL($this->query);

    // @todo: All bundles have all the field, so we hardcode the bundle. Is this
    // ok?
    $this->destination = new MigrateDestinationEntityAPI('biblio', 'book');

    $fields = array(
      'title',
      'uid',
      'created',
      'biblio_year',
      'biblio_other_numbers',
      'biblio_publisher',
      'biblio_isbn',
      'biblio_type_of_work',
      'biblio_call_number',
      'biblio_alternate_title',
      'biblio_url',
      'biblio_accession_number',
      'biblio_doi',
      'biblio_short_title',
    );

    // @todo: Add biblio_in_press

    $this->addSimpleMappings($fields);

    $this
      ->addFieldMapping('type', 'biblio_type')
      ->sourceMigration('BiblioMigrateTypes');

    $this
      ->addFieldMapping('changed', NULL)
      ->defaultValue(REQUEST_TIME);

    // TODO: The migration map the field as 'biblio_abstract' and not as
    // 'biblio_abst_e' which cause to the abstract from the original biblio
    // to not be imported.
    $this
      ->addFieldMapping('biblio_abstract', 'biblio_abst_e')
      ->defaultValue(NULL);
  }
}
