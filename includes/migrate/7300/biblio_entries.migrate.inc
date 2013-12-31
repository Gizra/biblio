<?php

/**
 * @file
 */
class BiblioMigrateEntries extends Migration {

  public $dependencies = array('BiblioMigrateTypes');

  public function __construct($arguments = array()) {
    parent::__construct($arguments);

    $this->description = t('Migrate biblio 1.x entries.');

    $this->query = db_select('_biblio_1x', 'b');
    $this->query->innerJoin('node', 'n', 'n.nid = b.nid AND n.vid = b.vid');

    $fields = array(
      'biblio_number',
      'biblio_other_number',
      'biblio_sort_title',
      'biblio_secondary_title',
      'biblio_tertiary_title',
      'biblio_edition',
      'biblio_publisher',
      'biblio_place_published',
      'biblio_year',
      'biblio_volume',
      'biblio_pages',
      'biblio_date',
      'biblio_isbn',
      'biblio_lang',
      'biblio_issue',
      'biblio_type_of_work',
      'biblio_accession_number',
      'biblio_call_number',
      'biblio_notes',
      'biblio_research_notes',
      'biblio_number_of_volumes',
      'biblio_short_title',
      'biblio_alternate_title',
      'biblio_original_publication',
      'biblio_reprint_edition',
      'biblio_translated_title',
      'biblio_section',
      'biblio_citekey',
      'biblio_coins',
      'biblio_doi',
      'biblio_issn',
      'biblio_auth_address',
      'biblio_label',
      'biblio_refereed',
      'biblio_md5',
      'biblio_url',
      'biblio_url_title',
      'biblio_type',
      'biblio_abst_e',
    );

    $this->query
      ->fields('b', $fields)
      ->fields('n', array('nid', 'title', 'created', 'uid'))
      ->condition('n.status', NODE_PUBLISHED)
      ->condition('b.biblio_type', 0, '>');

    $fields = array_merge($fields, array('title', 'uid', 'created', 'changed'));

    $this->addSimpleMappings($fields);

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

    $this
      ->addFieldMapping('type', 'biblio_type')
      ->sourceMigration('BiblioMigrateTypes');

    $this
      ->addFieldMapping('biblio_abstract', 'biblio_abst_e')
      ->defaultValue(NULL);

    $this
      ->addFieldMapping('biblio_status', 'biblio_year')
      ->callbacks(array($this, 'prepareBiblioStatus'));

    $this
      ->addFieldMapping('biblio_language', 'biblio_lang');

    $this
      ->addFieldMapping('biblio_refereed_designation', 'biblio_refereed');
  }

  public function prepareBiblioStatus($value) {
    switch ($value) {
      case '9998':
        return 'in_press';

      case '9999':
        return 'submitted';

      default:
        return 'published';
    }
  }
}
