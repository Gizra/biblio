<?php

class BiblioController extends DrupalDefaultEntityController {
  public function create($pub_type = '') {
    return (object) array(
      'bid' => '',
      'pub_type' => $pub_type,
      'title' => '',
    );
  }
  
  public function save($biblio) {
    // Transactions allow multiple database queries to either all succeed or
    // all fail together.
    // Any queries between db_transaction() and when the $transaction variable
    // goes out of scope will be part of that transaction.
    $transaction = db_transaction();
    
    // Wrapping entire process in a PHP try-catch block. That's because the save
    // process could be arbitrarily complex, involving a number of queries, any
    // of which could, potentially, break.
    try {
      global $user;
      
      // Determine if we will be inserting a new biblio.
      $biblio->is_new = empty($biblio->aid);
      
      // Set the timestamp fields.
      if (empty($biblio->created)) {
        $biblio->created = REQUEST_TIME;
      }
      $biblio->changed = REQUEST_TIME;
      
      $biblio->revision_timestamp = REQUEST_TIME;
      $update_biblio = TRUE;
      
      // Give modules the opportunity to prepare field data for saving.
      field_attach_presave('biblio', $biblio);
      
      if (!$biblio->is_new && !empty($biblio->revision) && $biblio->vid) {
        $biblio->old_vid = $biblio->vid;
        unset($biblio->vid);
      }
      
      // If this is a new biblio...
      if ($biblio->is_new) {
        // Save the new biblio.
        drupal_write_record('biblio', $biblio);
        // Save the initial revision.
        $this->saveRevision($biblio, $user->uid);
        
        $op = 'insert';
      }
      else {
        // Save the updated biblio.
        drupal_write_record('biblio', $biblio, 'aid');
        
        if (!empty($biblio->revision)) {
          $this->saveRevision($biblio, $user->uid);
        }
        else {
          $this->saveRevision($biblio, $user->uid, TRUE);
          $update_biblio = FALSE;
        }
        
        $op = 'update';
      }
      
      // If the revision ID is new or updated, save it to the biblio.
      if ($update_biblio) {
        db_update('biblio')
          ->fields(array('vid' => $biblio->vid))
          ->condition('aid', $biblio->aid)
          ->execute();
      }
      
      // Save fields.
      $function = 'field_attach_' . $op;
      $function('biblio', $biblio);
      
      // Allow other modules to interact with any entity when they are
      // created or updated.
      module_invoke_all('entity_' . $op, $biblio, 'biblio');
      
      // Clear internal properties.
      unset($biblio->is_new);
      
      // Ignore slave server temporarily to give time for the saved order to
      // be propagated to the slave.
      db_ignore_slave();
      
      return $biblio;
    }
    // If any of the queries fail, the database will throw an Exception.
    // In that case, we roll back the entire transaction. 0
    catch (Exception $e) {
      $transaction->rollback();
      // Log the exception using special utility function that decodes useful
      // information from the exception for us.
      watchdog_exception('biblio', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
    }
  }
  
  function saveRevision($biblio, $uid, $update = FALSE) {
    // Hold on to the biblio's original creator_uid but swap in the revision's
    // creator_uid for the momentary write.
    $temp_uid = $biblio->uid;
    $biblio->uid = $uid;
    
    // If we're creating a new revision, we'll save a new record to the table
    // and drupal_write_record() will populate the vid property for us with 
    // the new version ID.
    if ($update) {
      drupal_write_record('biblio_new_revision', $biblio, 'vid');
    }
    else {
      drupal_write_record('biblio_new_revision', $biblio);
    }
    
    // Reset the order's creator_uid to the original value.
    $biblio->uid = $temp_uid;
  }
  
  public function delete($aids) {
    if(!empty($aids)) {
      $biblios = $this->load($aids, array());
      $transaction = db_transaction();
      
      try {
        db_delete('biblio')
          ->condition('aid', $aids, 'IN')
          ->execute();
        
        db_delete('biblio_new_revision')
          ->condition('aid', $aids, 'IN')
          ->execute();
        foreach ($biblios as $biblio_id => $biblio) {
          field_attach_delete('biblio', $biblio);
        }
        
        db_ignore_slave();
      }
      catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('biblio', $e, NULL, WATCHDOG_ERROR);
        return FALSE;
      }
      
      module_invoke_all('entity_delete', $biblio, 'biblio');
      
      // Clear the page, block and biblio caches.
      cache_clear_all();
      $this->resetCache();
    }
    
    return TRUE;
  }
  
}