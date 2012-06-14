<?php

/**
 * A handler to provide proper displays for dates.
 */
class biblio_handler_date extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $options['date_format'] = array('default' => 'small');

    return $options;
  }

  function options_form(&$form, &$form_state) {

    $date_formats = array();
    $date_types = system_get_date_types();
    foreach ($date_types as $key => $value) {
      $date_formats[$value['type']] = check_plain(t($value['title'] . ' format')) . ': ' . format_date(REQUEST_TIME, $value['type']);
    }

    $form['date_format'] = array(
      '#type' => 'select',
      '#title' => t('Date format'),
      '#options' => $date_formats,
      '#default_value' => isset($this->options['date_format']) ? $this->options['date_format'] : 'small',
    );
    parent::options_form($form, $form_state);
  }

  function render($values) {
    $value = $this->get_value($values);
    $format = $this->options['date_format'];
    if ($value) {
      $date = format_date($value, $format);
      return $date;
    }
  }
}
