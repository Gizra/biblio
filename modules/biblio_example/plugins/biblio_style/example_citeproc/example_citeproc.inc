<?php

/**
 * This is an example of a Plugin extending CiteProc.
 */
if (module_exists('libraries') && file_exists(libraries_get_path('citeproc-php') . '/CiteProc.php')) {
  $plugin = array(
    'title' => t('Example CiteProc'),
    'description' => t('Example of extending CiteProc styles.'),
    'class' => 'BiblioStyleExampleCiteProc',
    'options' => array(
      // Default values.
      'style_name' => 'chicago-author-date',
      // Allow settings the path to custom CSL styles (should include the .csl
      // extension).
      'style_path' => libraries_get_path('citeproc-php') . '/style',
    ),
    'assets' => array(
      'js' => array(
        drupal_get_path('module', 'biblio_example') . '/js/biblio_example.js',
      ),
      'css' => array(
        drupal_get_path('module', 'biblio_example') . '/css/biblio_example.css',
      ),
    ),
  );
}