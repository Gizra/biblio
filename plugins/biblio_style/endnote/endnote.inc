<?php

$plugin = array(
  'title' => t('EndNote'),
  'description' => t('Endnote (tagged or XML) style.'),
  'class' => 'BiblioStyleEndNote',
  'import' => TRUE,
  'export' => array(
    'xml' => array(
      'class' => 'BiblioStyleEndNoteXML8',
      'name' => t('XML'),
      'file extension' => 'xml',
    ),
    'tagged' => array(
      'class' => 'BiblioStyleEndNoteTagged',
      'name' => t('Tagged'),
      'file extension' => 'enl',
    ),
  ),
);
