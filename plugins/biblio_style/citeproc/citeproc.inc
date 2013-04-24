<?php

if (module_exists('libraries') && file_exists(libraries_get_path('citeproc') . '/CiteProc.php')) {
  $plugin = array(
    'title' => t('CiteProc'),
    'description' => t('CiteProc styles.'),
    'class' => 'BiblioStyleCiteProc',
    'options' => array(
      // Default values.
      'style' => 'chicago-author-date',
      // Allow settings the path fow custom CSL styles.
      'style_path' => FALSE,
    ),
  );
}