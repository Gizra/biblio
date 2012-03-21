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
 * Adds fields and field instances to Drupal's Field API 
 * @todo Add biblio.field.type.data.csv data (field instance labels, etc)
 */
function biblio_add_fields() { 
  $file_data = biblio_parse_field_link_data_csv();
  foreach ($file_data as $key => $row) {
    $field = array(
      'field_name' => $row['field name'], 
      'type' => $row['type'],
    );
    // Allow user to add multiple author fields
    switch ($field['field_name']) {
      case 'biblio_authors_primary':
      case 'biblio_authors_secondary':
      case 'biblio_authors_tertiary':
      case 'biblio_authors_subsidiary':
      case 'biblio_authors_corporate':
      case 'biblio_keywords':
        $field['cardinality'] = FIELD_CARDINALITY_UNLIMITED;
    }
    if (isset($row['field name'])) {
      field_create_field($field);
    }
  }
}

function biblio_add_field_instances($publication_type) {
  $file_data = biblio_parse_field_link_data_csv();
  $publication_types = biblio_types();
  
  // @todo: remove all timers after development
  timer_start('field_instances_creation');
  foreach ($file_data as $key => $row) {
    // convert 1 to true and 0 to false
    $row['required'] = ($row['required'] == '1' ? TRUE : FALSE);
    $instance = array(
      'field_name' => $row['field name'],
      'entity_type' => 'biblio',
      'bundle' => $publication_type, 
      'label' => $row['title'],
      'required' => $row['required'],
      'widget' => array('weight' =>$row['weight']),
    );
    if (isset($row['field name']) && 
        !field_info_instance('biblio', $row['field name'], $publication_type)) {
      field_create_instance($instance);
    }
  }
  timer_stop('field_instances_creation');
  drupal_set_message('Biblio field instances have been added for the ' 
      . $publication_type . ' publication type. Time elapsed: ' 
      . timer_read('field_instances_creation') . 'ms');
}

/**
 * Gets data from the biblio.field.link.data.csv file and creates a structured
 * array for use with field creation
 *
 * @return array
 * Array of rows from the biblio.field.link.data.csv file, each row containing
 * an array of values from the csv.
 */
function biblio_parse_field_link_data_csv() {
  // lines have been removed from the csv file to remove some fields for
  // development purposes. This is only to reduce install time. 
  // @todo: put back full csv file. 
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
 * biblio/add is accessed for the first time for a specific publication type.
 *
 * @param string $publication_type
 * @return boolean Whether or not biblio's required field instances are indeed missing.
 */
function biblio_field_instances_missing($publication_type) {
  // Array of fields defined in /misc/biblio.field.link.data.csv
  $biblio_instances = biblio_parse_field_link_data_csv();
  // Get array of field instances that exist (if any) for the current pub type
  $params = array('entity_type' => 'biblio', 'bundle' => $publication_type);
  $existing_instances = field_read_instances($params);
  foreach ($existing_instances as $key => $instance_data) {
    // change the array values to the field name, so we can search for the 
    // field instance in the array using in_array()
    $existing_instances[$key] = $instance_data['field_name'];
  }
  foreach ($biblio_instances as $key => $field_data) {
    // If CSV line isn't blank and field name isn't already in the list of 
    // existing field instances
    if (isset($biblio_instances[$key]['field name']) && 
        !in_array($field_data['field name'], $existing_instances)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Get a list of all biblio fields
 *
 * @return array 
 */
function biblio_field_list() {
  // Get an array of all fields
  $list_of_fields = array_keys(field_info_fields());
  $biblio_fields = array();
  foreach ($list_of_fields as $field) {
    // Check if name of field has the 'biblio_' prefix
    if (strstr($field, 'biblio_') != FALSE) {
      $biblio_fields[] = $field;
    }
  }
  
  return $biblio_fields;
}

/**
 * Deletes all Biblio fields
 * 
 * @return array Array of all fields that were deleted.
 * @todo make this function not run so slow. field_d
 */
function biblio_delete_fields() {
  $fields = biblio_field_list();
  foreach ($fields as $field) {
    // Each iteration takes ~90ms to run :(
    field_delete_field($field);
  }
  
  return $fields;
}

//  module_load_include('inc', 'biblio', 'includes/biblio.fields');
//  modify_biblio_field_link_data_csv();
/**
 * This function only  exists because I'm lazy and don't want to manipulate
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
    }
  }
  foreach ($data_to_write as $key => $value) {
    if (isset($value['fid']) || isset($value[0])) {
      fputcsv($handle, $value);
    }
  }
}