<?php

/**
 * Delete button for a biblio entry.
 */
class biblio_ui_delete_button extends views_handler_field_entity {

  function render($values) {
    global $user;

    if (user_access('delete biblio entries') || ($values->uid == $user->uid && user_access('delete own biblio entries'))) {
      return l(t('Delete'), 'biblio/' . $values->bid . '/delete');
    }
  }
}
