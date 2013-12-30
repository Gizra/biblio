<?php

/**
 * @file
 */
class BiblioMigrateContributorCollections extends Migration {
  public $dependencies = array(
    'BiblioMigrateEntries',
  );

  public function __construct($arguments = array()) {
    $this->description = t('Migrate biblio 1.x contributors into field collections.');

    // Fetch the used contributor types.
    $this->query = db_select('_biblio_contributor_1x', 'c');
    $this->query->innerJoin('_biblio_contributor_type_data_1x', 'ctd', 'c.auth_type = ctd.auth_type');
    $this->query->fields('c', array('nid', 'cid', 'auth_type'));
    $this->query->addField('ctd', 'title', 'role');

    parent::__construct($arguments);

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
        'alias' => 'ctd',
      ),
    );

    $this->map = new MigrateSQLMap($this->machineName, $source_key, BiblioMigrateDestinationFieldCollection::getKeySchema());
    $this->source = new MigrateSourceSQL($this->query);
    $this->destination = new BiblioMigrateDestinationFieldCollection('contributor_field_collection', array('host_entity_type' => 'biblio'));

    $this->addFieldMapping('host_entity_id', 'nid')->sourceMigration('BiblioMigrateEntries');
    $this->addFieldMapping('biblio_contributor', 'cid');
    $this->addFieldMapping('biblio_contributor_role', 'role');

    $this->roles_vocabulary = taxonomy_vocabulary_machine_name_load('biblio_roles');
  }

  public function prepare($entity, $row) {
    // Replace the role name with its term ID.
    $name = $entity->biblio_contributor_role[LANGUAGE_NONE][0]['target_id'];
    if ($term = taxonomy_get_term_by_name($name, 'biblio_roles')) {
      $term = reset($term);
    }
    // Create the term if it doesn't exist.
    else {
      $term = (object) array(
        'name' => $name,
        'vid' => $this->roles_vocabulary->vid,
      );
      taxonomy_term_save($term);
    }
    $wrapper = entity_metadata_wrapper('field_collection_item', $entity);
    $wrapper->biblio_contributor_role->set($term->tid);
  }
}
