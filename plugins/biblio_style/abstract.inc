<?php

/**
 * Biblio Style interface
 *
 */
interface BiblioStyleInterface {

  /**
   * Constructor for the notifier.
   *
   * @param $plugin
   *   The notifier plugin object. Note the "options" values might have
   *   been overriden in message_notify_send_message().
   * @param Biblio $biblio
   *   The Biblio entity.
   */
  public function __construct($plugin, Biblio $biblio);

  /**
   * Render the Biblio according to the style plugin.
   *
   * @return
   *   A an HTML string.
   */
  public function render($options = array());
}

/**
 * An abstract implementation of MessageNotifierInterface.
 */
abstract class BiblioStyleBase implements BiblioStyleInterface {

  /**
   * The plugin definition.
   */
  protected $plugin;

  /**
   * The Biblio entity.
   */
  protected $biblio;

  public function __construct($plugin, Biblio $biblio) {
    $this->plugin = $plugin;
    $this->biblio = $biblio;
  }


  public function render($options = array()) {
    return '';
  }
}
