<?php

$plugin = array(
  'title' => t('EndNote'),
  'description' => t('Endnote (tagged or XML) style.'),
  'class' => 'BiblioStyleEndNote',
  'import' => TRUE,
  'export' => array(
    'xml' => array(
      'name' => t('EndNote - XML'),
      'file extension' => 'xml',
    ),
    'tagged' => array(
      'name' => t('EndNote - Tagged'),
      'file extension' => 'enl',
    ),
  ),
);
