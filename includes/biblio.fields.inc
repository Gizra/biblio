<?php
function _biblio_field_extra_fields() {
  $extras['node']['biblio']['form'] = array();
  $fields = array();

    $result = db_query("SELECT * FROM {biblio_fields} b
              INNER JOIN {biblio_field_type} bt ON b.fid = bt.fid
              INNER JOIN {biblio_field_type_data} btd ON btd.ftdid=bt.ftdid
              WHERE bt.tid=:tid ORDER BY bt.weight ASC", array(':tid' => 0), array('fetch' => PDO::FETCH_ASSOC));

    foreach ($result as $row) {
        $fields[$row['name']] = $row;
    }
    _biblio_localize_fields($fields);

    $extras['node']['biblio']['form']['biblio_type'] = array(
      'label'       => t('Publication Type'),
      'description' => t('Biblio module form.'),
      'weight'      => -4
    );

    foreach ($fields as $key => $fld) {
      $label = check_plain($fld['title']);
      if ($fld['type'] == 'textarea' ||  $fld['type'] == 'contrib_widget') {
        $key = $key .'_field';
        $label = $label . ' (' . t('Fieldset') .')';
      }
      $extras['node']['biblio']['form'][$key] = array(
        'label'       => $label,
        'description' => t('Biblio module form.'),
        'weight'      => $fld['weight'] / 10
      );
    }
    $extras['node']['biblio']['form']['other_fields'] = array(
      'label'       => t('Other Biblio Fields') . ' (' . t('Fieldset') .')',
      'description' => t('Biblio module form.'),
      'weight'      => 0
      );


  return $extras;
}
/**
 * Adds fields to Drupal's Field API 
 * @todo Add biblio.field.type.data.csv data (field instance labels, etc)
 */
function biblio_add_fields() { 
  $file_data = biblio_parse_field_link_data_csv();
  foreach ($file_data as $key => $row) {
    $field = array(
      'field_name' => $row['field name'], 
      'type' => $row['type'],
    );
    
    // Only act on CSV rows that have a name (not blank rows)
    if (isset($row['field name'])) {
      // Add any field-specific settings or other values here...
      switch ($row['field name']) {
        case 'biblio_contributor_category':
          $field['settings']['allowed_values'] = biblio_get_contributor_options();
          break;
        case 'biblio_biblio_ref':
          $field['settings']['target_type'] = 'biblio';
          break;
        case 'biblio_contributor_ref':
          $field['settings']['target_type'] = 'biblio_contributor';
          break;
      }
      
      if (!field_info_field($field['field_name'])) {
        field_create_field($field);
      }
    }
  }
}

/**
 * Add all field instances for a given entity type and bundle.
 * Adding field instances takes a long amount of time for the number of fields
 * and bundles we have. Fields for a biblio publication type are added the first
 * time a user visits the Biblio Add page for that publication type.
 * Contributor and Contribution fields are added upon install, becasue there are
 * so few fields.
 * 
 * @param type $entity biblio, biblio_contributor, or biblio_contribution
 * @param type $bundle The bundle for which field instances should be created.
 * for example, a biblio publication type
 */
function biblio_add_field_instances($entity, $bundle) {
  $file_data = biblio_parse_field_link_data_csv();
  
  // @todo: remove all timers after development
  timer_start('field_instances_creation');
  foreach ($file_data as $key => $row) {
    // convert 1 to true and 0 to false
    $row['required'] = ($row['required'] == '1' ? TRUE : FALSE);
    $instance = array(
      'field_name' => $row['field name'],
      'entity_type' => $row['entity'],
      'bundle' => $bundle, 
      'label' => $row['title'],
      'required' => $row['required'],
      'widget' => array('weight' =>$row['weight']),
      'settings' => array('vtab' => $row['vtab']),
    );
    
    if (!empty($row['field name'])) {
      // Perform any post-parsing modifications to instances
      switch ($row['field name']) {
        case 'biblio_contributor_category':
          $instance['default_value'] = array(0 => array('value' => 1));
          break;
        case 'biblio_keywords':
          $instance['widget']['type'] = 'taxonomy_autocomplete';
          break;
      }
    }
    
    // If row has a field name specified (this excludes blank lines in the CSV)
    // && If the field instance doesn't already exist
    // && Only add field instances for the specified entity type
    if (isset($row['field name']) && 
        !field_info_instance($row['entity'], $row['field name'], $bundle) &&
        $row['entity'] == $entity) {
      field_create_instance($instance);
    }
  }
  timer_stop('field_instances_creation');
  drupal_set_message('Biblio field instances have been added for the ' 
      . $bundle . ' bundle. Time elapsed: ' 
      . timer_read('field_instances_creation') . 'ms');
}

/**
 * Gets data from the biblio.field.link.data.csv file and creates a structured
 * array for use with field creation
 *
 * @return array
 * Array of rows from the biblio.field.link.data.csv file, each row containing
 * an array of values from the csv. Array is structured like so:
 *  0 => array(
 *    'title' => 'Human-Readable Field Title',
 *    'hint' => 'This is a hint to give to the user next to the field',
 *    ... other attrubutes
 *  1 => array(
 *    'title' => 'Another Title',
 *    and so on...
 */
function biblio_parse_field_link_data_csv() {
  $csv_file = drupal_get_path('module', 'biblio') . '/misc/biblio.field.link.data.csv';
  $file_handle = fopen($csv_file, 'r');
  // array containing an array for each row in the csv
  $file_data = array();
  // the first line containing column names
  $header_line = fgetcsv($file_handle, 10000, ","); 
  // count for array keys in the while loop. Starts at 1 to match the fid in the csv
  $count = 1;
  while (!feof($file_handle)) {
    // the rest of the lines in the csv containing actual field data
    $data_line = fgetcsv($file_handle, 10000, ',');
    $file_data[$count] = array();
    foreach ($header_line as $key => $header_value) {
      $file_data[$count][$header_value] = $data_line[$key];
    }
    $count++;
  }
  fclose($file_handle);
  
  return $file_data;
}

/**
 * Determines if the field instances for a publication type have been created.
 * Used to decide whether or not to run biblio_add_field_instances when
 * admin/content/biblio/add is accessed for the first time for a specific publication type.
 *
 * @param string $publication_type
 * @param string $entity_type biblio, biblio_contributor, etc...
 * @return boolean Whether or not biblio's required field instances are indeed missing.
 */
function biblio_field_instances_missing($publication_type, $entity_type = 'biblio') {
  // Array of fields defined in /misc/biblio.field.link.data.csv
  $biblio_instances = biblio_parse_field_link_data_csv();
  // Get array of field instances that exist (if any) for the current pub type
  $params = array('entity_type' => 'biblio', 'bundle' => $publication_type);
  $existing_instances = field_read_instances($params);
  $existing_instance_names = array();
  foreach ($existing_instances as $key => $instance_data) {
    // change the array values to the field name, so we can search for the 
    // field instance in the array using in_array()
    $existing_instance_names[] = $instance_data['field_name'];
  }
  foreach ($biblio_instances as $key => $field_data) {
    // If CSV line isn't blank &&
    // The field name isn't already in the list of existing field instances &&
    // Only for instances of the entity type given
    if (isset($biblio_instances[$key]['field name']) && 
        !in_array($field_data['field name'], $existing_instance_names) &&
        $biblio_instances[$key]['entity'] == $entity_type) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Get a list of all biblio fields that have been created, and prepended
 * with "biblio_"
 *
 * @return array 
 */
function biblio_field_list() {
  // Get an array of all fields
  $fields = field_info_fields();
  $biblio_fields = array();
  foreach ($fields as $field => $info) {
    // Check if name of field has the 'biblio_' prefix
    if (strstr($field, 'biblio_') != FALSE
      || ($info['type'] == 'biblio_text')) {
      $biblio_fields[] = $field;
    }
  }
  
  return $biblio_fields;
}

/**
 * Deletes all Biblio fields
 * 
 * @return array Array of all fields that were deleted.
 */
function biblio_delete_fields() {
  $fields = biblio_field_list();
  foreach ($fields as $field) {
    // Each iteration takes ~90ms to run
    field_delete_field($field);
  }
  
  return $fields;
}

/**
 * Checks all existing fields instances for a given instance, and returns wheter
 * or not it actually exists
 *
 * @param string $instance_name The machine name to check whether or not it is
 *  a valid field instance
 * @param string $entity_type biblio, contributor, etc.
 * @param string $bundle book, journal_article, etc.
 * @return boolean whether or not the given string exists as a field instance 
 */
function biblio_is_field_instance($instance_name, $entity_type, $bundle) {
  $instances = field_info_instances($entity_type, $bundle);
  if (isset($instances[$instance_name])) {
    return TRUE;
  }
  return FALSE;
}

//  module_load_include('inc', 'biblio', 'includes/biblio.fields');
//  modify_biblio_field_link_data_csv();
/**
 * This function only exists because I'm lazy and don't want to manipulate
 * CSV data by hand. Copy the above lines to call this function quickly
 * @todo remove after development
 */
function modify_biblio_field_link_data_csv() {
  $csv_file = drupal_get_path('module', 'biblio') . '/misc/biblio.field.link.data.csv';
  $handle = fopen($csv_file, 'r+');
  $data = biblio_parse_field_link_data_csv();
  $data_to_write[] = array_keys($data[1]);
  foreach($data as $value) {
    $data_to_write[] = $value;
  }
  
  foreach($data_to_write as $key => $row) {
    if (isset($row[0])) {
      // Modify headers here...
    }
    if (isset($row['fid'])) {
      // Modify data here...

      // get rid of unsupported field types listed in the CSV
      // @todo clean up the CSV and replace with applicable field types
      if ($row['type'] == 'contrib_widget' ||
          $row['type'] == 'textfield' ||
          $row['type'] == 'select'
          ) {
        $data_to_write[$key]['type'] = 'text';
      }
      if ($row['type'] == 'text_format') {
        $data_to_write[$key]['type'] = 'text_long';
      }
      if($row['type'] == 'text') {
        $data_to_write[$key]['type'] = 'biblio_text';
      }
      // change all field ids to the order in which they are placed in the file.
      $data_to_write[$key]['fid'] = $key;
      
    }
  }
  // Add a new row
//  $data_to_write[] = array(
//    'fid' => $key+1,
//    'title' => 'Author Role',
//    'hint' => '',
//    'common' => '',
//    'autocomplete' => '',
//    'required' => '1',
//    'weight' => '2',
//    'visible' => '',
//    'field name' => 'contributor_role',
//    'type' => 'biblio_text',
//    'width' => '60',
//    'maxlength' => '255',
//    'vtab' => '0',
//    'entity' => 'biblio_contribution',
//  );
  foreach ($data_to_write as $key => $value) {
    if (isset($value['fid']) || isset($value[0])) {
      fputcsv($handle, $value);
    }
  }
}