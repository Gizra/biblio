<?php

$plugin = array(
  'title' => t('BibTeX'),
  'extension' => 'bib',
  'description' => t('BibTeX style.'),
  'class' => 'BiblioStyleBibtex',
  // Indicates this style plugin can import entries.
  'import' => TRUE,
  'options' => array(
    'opening_tag' => '{',
    'closing_tag' => '}',
  ),
);
