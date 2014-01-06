<?php

if (module_exists('libraries') && file_exists(libraries_get_path('citeproc-php') . '/CiteProc.php')) {
  $plugin = array(
    'title' => t('CiteProc'),
    'description' => t('CiteProc styles.'),
    'class' => 'BiblioStyleCiteProc',
    'options' => array(
      // Default values.
      'style_name' => 'chicago-author-date',
      // Allow settings the path to custom CSL styles (should include the .csl
      // extension).
      'style_path' => libraries_get_path('citeproc-php') . '/style',
      // Link to the biblio.
      'label_as_link' => TRUE,
    ),
    'export' => array(
      'csl' => array(
        'name' => t('Citeproc'),
        'file extension' => 'csl',
      ),
    ),
  );
}
