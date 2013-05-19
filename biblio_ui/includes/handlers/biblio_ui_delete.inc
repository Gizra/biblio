<?php

/**
 * Delete button for a biblio entry.
 */
class biblio_ui_delete_button extends views_handler_field_entity {

  function render($values) {
    if (user_access('delete biblios')) {
      return l(t('Delete'), 'biblio/' . $values->bid . '/delete');
    }
  }
}
