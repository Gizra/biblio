<?php
function _biblio_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  if ($entity_type == 'node' && $bundle_name == 'biblio') {
    $schema = drupal_get_schema('biblio');
    foreach ($schema['fields'] as $field => $spec) {
      if (strstr($field, 'biblio_')) {
        $type = $spec['type'];
        $length = isset($spec['length']) ? ' ['. $spec['length'] .']': '';
        $targets[$field] = array(
          'name' => $field . '  ('. $type . $length . ')',
          'description' =>'',
          'callback' => '_biblio_feeds_set__simple_target',
        );
      }
    }
    $targets['biblio_type']['callback'] = '_biblio_feeds_set__type_target';
    $targets['biblio_contributor'] = array(
      'name' => t('biblio_contributor'),
      'description' => t('This is a contributor (author) contained in a biblio entry.'),
      'callback' => '_biblio_feeds_set__contrib_target',
 //     'real_target' => 'my_node_field_two', // Specify real target field on node.
    );
    $targets['biblio_keyword'] = array(
      'name' => t('biblio_keyword'),
      'description' => t('This is a keyword contained in a biblio entry.'),
      'callback' => '_biblio_feeds_set__keyword_target',
 //     'real_target' => 'my_node_field_two', // Specify real target field on node.
    );
  }
}

function _biblio_feeds_set__type_target($source, $entity, $target, $value) {
  static $types = array();

  if (isset($value[0]) && !empty($value[0])) {
    if(empty($types)) {
      $result = db_query('SELECT t.* FROM {biblio_types} as t WHERE t.tid > 0');
      foreach ($result as $row) {
        $types[$row->tid] = str_replace(" ","_",strtolower($row->name));
      }
    }
    $type = array_search($value[0], $types);
    $entity->biblio_type = (!empty($type)) ? $type : 129;
  }
}

function _biblio_feeds_set__simple_target($source, $entity, $target, $value) {
  if (isset($value[0]) && !empty($value[0])) {
    $entity->$target = $value[0];
    if($target == 'biblio_abst_e') {
      $entity->biblio_formats[$target] = 'full_html';
    }
  }
}

function _biblio_feeds_set__contrib_target($source, $entity, $target, $value) {
  foreach ($value as $author) {
    $entity->biblio_contributors[] =  array(
            'name' => $author,
            'auth_category' => 1,
          );
  }
}

function _biblio_feeds_set__keyword_target($source, $entity, $target, $value) {
  if (!empty($value)) {
    $entity->biblio_keywords = $value;
  }
}
