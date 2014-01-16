<?php

/**
 * Delete button for a biblio entry.
 */
class biblio_ui_handler_delete extends views_handler_field_entity {

  function render($values) {
    global $user;

    if (user_access('delete biblio content') || ($values->uid == $user->uid && user_access('delete own biblio content'))) {
      return l(t('Delete'), 'biblio/' . $values->bid . '/delete');
    }
  }
}
