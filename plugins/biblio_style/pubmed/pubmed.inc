<?php

$plugin = array(
  'title' => t('PubMed'),
  'extension' => 'pub', // @TODO: what is the right extension ?
  'description' => t('PubMed style.'),
  'class' => 'BiblioStylePubmed',
  // Indicates this style plugin can import entries.
  'import' => TRUE,
);
