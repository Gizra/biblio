<?php

/**
 * Contribution class.
 */
class BiblioContribution extends Entity {
  protected function defaultLabel() {
    // @todo: return something useful as the title
//    return $this->title;
    return;
  }
  protected function defaultUri() {
    return array('path' => 'biblio/contribution/' . $this->identifier());
  }
}


/**
 * Contribution controller class 
 */
class BiblioContributionController extends EntityAPIController {
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
      'cnid' => '',
      'category' => 'Primary',
      'rank' => 0,
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
    );
    return parent::create($values);
  }
}