<?php

/**
 * Contributor class.
 */
class BiblioContributor extends Entity {
  protected function defaultLabel() {
    $wrapper = entity_metadata_wrapper('bilio_contributor', $this);
    return $wrapper->biblio_contributor_name->value();
  }
  protected function defaultUri() {
    return array('path' => 'biblio/contributor/' . $this->identifier());
  }
}


/**
 * Contributor controller class 
 */
class BiblioContributorController extends EntityAPIController {
  /**
   * Creates default property values for our entity object
   * 
   * @global type $user
   * @param array $values
   * @return array 
   */
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'cid' => '',
      'title' => '',
      // Default value, because we only have one bundle for this entity
      'type' => 'contributor',
      'description' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
    );
    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('biblio_contributor', $entity);
    $content['author'] = array('#markup' => t('Created by: !author', array('!author' => $wrapper->uid->name->value(array('sanitize' => TRUE)))));

    // Make Description and Status themed like default fields.
    $content['description'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' =>t('Description'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'biblio_contributor',
      '#bundle' => $entity->type,
      '#items' => array(array('value' => $entity->description)),
      '#formatter' => 'text_default',
      0 => array('#markup' => check_plain($entity->description))
    );

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}