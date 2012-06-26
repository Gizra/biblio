<?php

/**
 * A handler to convert the UID to username.
 */
class biblio_handler_uid extends views_handler_field {
  function render($values) {
    $uid = $this->get_value($values);
    if ($uid) {
      $account = user_load($uid);
      return $account->name;
    }
  }
}
