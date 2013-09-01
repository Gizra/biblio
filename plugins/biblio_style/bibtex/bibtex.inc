<?php

$plugin = array(
  'title' => t('BibTeX'),
  'description' => t('BibTeX style.'),
  'class' => 'BiblioStyleBibtex',
  // Indicates this style plugin can import entries.
  'import' => array(
    array('id' => t('PuBMed ID List'), 'method' => 'ImportIdList'),
  ),
  'options' => array(
    'opening_tag' => '{',
    'closing_tag' => '}',
  ),
);
