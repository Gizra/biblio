<?php

/**
 * Delete button for a biblio contributor entry.
 */
class biblio_contributor_ui_handler_delete extends views_handler_field_entity {

  function render($values) {
    if (user_access('delete biblio contributor entry')) {
      return l(t('Delete'), 'biblio-contributor/' . $values->cid . '/delete');
    }
  }
}
