<?php

$plugin = array(
  'title' => t('EndNote'),
  'export extension' => 'enl',
  'description' => t('Endnote (tagged or XML) style.'),
  'class' => 'BiblioStyleEndNote',
  'import' => TRUE,
  'export' => array(
    'xml' => array(
      'class' => 'BiblioStyleEndNoteXML',
      'options' => array(
        'style_name' => 'xml',
      ),
    ),
    'tagged' => array(
      'class' => 'BiblioStyleEndNoteTagged',
      'options' => array(
        'style_name' => 'xml',
      ),
    ),
  ),
);
