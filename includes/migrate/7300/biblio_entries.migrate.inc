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

    $this->map = new MigrateSQLMap($this->machineName, $source_key, MigrateDestinationBiblioEntry::getKeySchema());
    $this->source = new MigrateSourceSQL($this->query);
    $this->destination = new MigrateDestinationBiblioEntry();

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

    $this->addSimpleMappings($fields);

    $this
      ->addFieldMapping('type', 'biblio_type');

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
