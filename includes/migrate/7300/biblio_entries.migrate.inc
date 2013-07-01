<?php

/**
 * @file
 */
class BiblioMigrateEntries extends Migration {
  public $dependencies = array('BiblioMigrateTypes');

  private $optionalFields = array(
    'biblio_year',
    'biblio_other_numbers',
    'biblio_publisher',
    'biblio_abst_e',
    'biblio_isbn',
    'biblio_type_of_work',
    'biblio_call_number',
    'biblio_alternate_title',
    'biblio_url',
    'biblio_accession_number',
    'biblio_doi',
    'biblio_short_title',
  );

  public function __construct() {
    $this->description = t('Migrate biblio 1.x entries.');

    $this->query = db_select('_biblio_1x', 'b');
    $this->query->fields('b');
    $this->query->fields('n', array('nid', 'title', 'created', 'uid'));
    $this->query->innerJoin('node', 'n', 'n.nid = b.nid');
    // @todo: Ignore unpublished biblio entries.
    $this->query->condition('n.status', NODE_PUBLISHED);

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

    $this->addFieldMapping('title', 'title');

    $this
      ->addFieldMapping('type', 'biblio_type')
      ->sourceMigration('BiblioMigrateTypes');

    $this->addFieldMapping('uid', 'uid');

    $this->addFieldMapping('created', 'created');

    $this
      ->addFieldMapping('changed', NULL)
      ->defaultValue(REQUEST_TIME);

    $this->addFieldMapping('biblio_year', 'biblio_year');

    $this
      ->addFieldMapping('biblio_other_numbers', 'biblio_other_number')
      ->defaultValue(NULL);

    $this
      ->addFieldMapping('biblio_publisher', 'biblio_publisher')
      ->defaultValue(NULL);

    // TODO: The migration map the field as 'biblio_abstract' and not as
    // 'biblio_abst_e' which cause to the abstract from the original biblio
    // to not be imported.
    $this->addFieldMapping('biblio_abstract', 'biblio_abst_e')->defaultValue(NULL);

    $this
      ->addFieldMapping('biblio_isbn', 'biblio_isbn')
      ->defaultValue(NULL);

    $this
      ->addFieldMapping('biblio_type_of_work', 'biblio_type_of_work')
      ->defaultValue(NULL);

    $this
      ->addFieldMapping('biblio_call_number', 'biblio_call_number')
      ->defaultValue(NULL);

    $this
      ->addFieldMapping('biblio_alternate_title', 'biblio_alternate_title')
      ->defaultValue(NULL);

    $this
      ->addFieldMapping('biblio_url', 'biblio_url')
      ->defaultValue(NULL);

    $this
      ->addFieldMapping('biblio_accession_number', 'biblio_accession_number')
      ->defaultValue(NULL);

    $this
      ->addFieldMapping('biblio_doi', 'biblio_doi')
      ->defaultValue(NULL);

    $this
      ->addFieldMapping('biblio_short_title', 'biblio_short_title')
      ->defaultValue(NULL);
  }

  public function prepare($entity, $row) {
    // Make sure optional fields are attached if they have an assigned value.
    foreach ($this->optionalFields as $field_name) {
      if (!$entity->$field_name) {
        continue;
      }

      biblio_create_field($field_name, 'biblio', $entity->type);

      $entity->$field_name = array(LANGUAGE_NONE => array(array('value' => $entity->$field_name)));
    }
  }
}
