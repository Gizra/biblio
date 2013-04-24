<?php

/**
 * @file
 * Chicago biblio style.
 */

class BiblioStyleCiteProc extends BiblioStyleBase {

  public function render($options = array(), $langcode = NULL) {
    global $language;
    $langcode = $langcode ? $langcode : $language->language;

    // Make sure the CSL file exists.
    $style_name = $options['style_name'];
    // @todo: Allow adding more styles in the Library.
    $file_path = !empty($options['style_path']) ? $options['style_path'] : './styles/' . $style_name . '.csl';
    if (!file_exists($file_path)) {
      throw new Exception(format_string('@style file does not exist.', array('@style' => $style_name)));
    }

    $citeproc = new citeproc($csl_file_contents, $langcode);

    return 'CiteProc';
  }

}
