<?php

$plugin = array(
  'title' => t('EndNote'),
  'description' => t('Endnote (tagged or XML) style.'),
  'class' => 'BiblioStyleEndNote',
  'import' => TRUE,
  'export' => array(
    'xml7' => array(
      'class' => 'BiblioStyleEndNoteXML7',
      'name' => t('XML7'),
      'file extension' => 'xml',
    ),
    'xml8' => array(
      'class' => 'BiblioStyleEndNoteXML8',
      'name' => t('XML8'),
      'file extension' => 'xml',
    ),
    'tagged' => array(
      'class' => 'BiblioStyleEndNoteTagged',
      'name' => t('Tagged'),
      'file extension' => 'enl',
    ),
  ),
);
