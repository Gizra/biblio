<?php

$plugin = array(
  'title' => t('BibTeX'),
  'description' => t('BibTeX style.'),
  'class' => 'BiblioStyleBibtex',
  // Indicates this style plugin can import entries.
  'import' => TRUE,
  'options' => array(
    'opening_tag' => '{',
    'closing_tag' => '}',
  ),
  'export' => array(
    'bibtex' => array(
      'name' => t('BibTeX'),
      'file extension' => 'bib',
    ),
  ),
);
