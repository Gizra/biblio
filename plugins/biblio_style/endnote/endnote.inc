<?php

$plugin = array(
  'title' => t('EndNote'),
  'description' => t('Endnote (tagged or XML) style.'),
  'class' => 'BiblioStyleEndNote',
  'import' => TRUE,
  'export' => array(
    'tagged' => array(
      'name' => t('EndNote - Tagged'),
      'file extension' => 'enl',
    ),
  ),
);
