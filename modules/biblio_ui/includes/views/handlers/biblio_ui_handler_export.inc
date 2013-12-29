<?php

/**
 * Export button for a biblio entry.
 */
class biblio_ui_handler_export extends views_handler_field_entity {

  function render($values) {
    global $user;

    if (user_access('export biblio entries') || ($values->uid == $user->uid && user_access('export own biblio entries'))) {
      return l(t('Export'), 'biblio/' . $values->bid . '/export');
    }
  }
}
