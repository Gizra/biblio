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
  public function import($data, $options = array()) {
    $options += array(
      'type' => 'tagged',
    );

    $class_name = $options['type'] == 'tagged' ? 'BiblioStyleEndNoteTagged' : 'BiblioStyleEndNoteXML';

    $handler = new $class_name( $this->plugin, $this->biblio);
    return $handler->import($data, $options);
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
