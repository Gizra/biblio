<?php
/**
 * Edit button for a biblio contributor entry.
 */
class biblio_contributor_ui_handler_edit extends views_handler_field_entity {

  function render($values) {
    if (user_access('edit biblio contributor')) {
      return l(t('Edit'), 'biblio-contributor/' . $values->cid . '/edit');
    }
  }
}
