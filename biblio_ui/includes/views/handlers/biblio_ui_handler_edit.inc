<?php
/**
 * Edit button for a biblio entry.
 */
class biblio_ui_handler_edit extends views_handler_field_entity {

  function render($values) {
    if (user_access('edit by all biblio authors')) {
      return l(t('Edit'), 'biblio/' . $values->bid . '/edit');
    }
  }
}
