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

function _biblio_add_fields() {
  $file_data = biblio_parse_field_link_data_csv();
}

/**
 *
 * @return array
 * Array of rows from the biblio.field.link.data.csv file, each row containing
 * an array of values from the csv.
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