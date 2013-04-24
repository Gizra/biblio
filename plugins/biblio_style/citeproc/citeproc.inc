<?php

// if (module_exists('libraries') && file_exists(libraries_get_path('citeproc-php') . '/CiteProc.php')) {
  $plugin = array(
    'title' => t('CiteProc'),
    'description' => t('CiteProc styles.'),
    'class' => 'BiblioStyleCiteProc',
    'options' => array(
      // Default values.
      'style_name' => 'chicago-author-date',
      // Allow settings the path to custom CSL styles (should include the .csl
      // extension).
      'style_path' => drupal_get_path('module', 'biblio') . '/plugins/biblio_style/citeproc/styles',
    ),
  );
// }