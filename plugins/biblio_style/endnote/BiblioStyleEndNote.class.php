<?php

/**
 * @file
 * EndNote tagged biblio style.
 */

class BiblioStyleEndNote extends BiblioStyleBase {

  public function settingsForm() {
    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#required' => TRUE,
      '#options' => array(
        'tagged' => t('Tagged'),
        'xml' => t('XML'),
      ),
      '#default_value' => 'tagged',

    );

    return $form;
  }

  /**
   * @inheritdoc
   */
  public function importData($data, $options = array()) {
    $options += array(
      'type' => 'tagged',
    );


    $class_name = FALSE;
    if ($options['type'] == 'tagged') {
      $class_name = 'BiblioStyleEndNoteTagged';
    }
    else {
      // This is an XML, and we need to get the format (EndNote 7 or EndNote 8).
      if (strpos($data, 'record') !== FALSE && strpos($data, 'ref-type') !== FALSE) {
        $class_name = 'BiblioStyleEndNoteXML8';
      }
      elseif (strpos($data, 'RECORD') !== FALSE && strpos($data, 'REFERENCE_TYPE') !== FALSE) {
        $class_name = 'BiblioStyleEndNoteXML7';
      }
    }

    if (!$class_name) {
      return;
    }

    $handler = new $class_name( $this->plugin, $this->biblio);
    return $handler->importData($data, $options);
  }


  /**
   * @inheritdoc
   */
  public function render($options = array(), $langcode = NULL) {
    $options += array(
      'type' => 'tagged',
    );

    $class_name = $options['type'] == 'tagged' ? 'BiblioStyleEndNoteTagged' : 'BiblioStyleEndNoteXML';

    $handler = new $class_name( $this->plugin, $this->biblio);
    return $handler->render($options, $langcode);
  }
}
