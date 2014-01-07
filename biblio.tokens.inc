<?php
/**
 * @file
 * Builds placeholder replacement tokens for biblio entity.
 */

/**
 * Implements hook_token_info().
 */
function biblio_token_info() {
  $type = array(
    'name' => t('Biblio'),
    'description' => t('Tokens related to the module biblio'),
    'needs-data' => 'biblio',
  );

  // Core tokens for biblios.
  $biblio['bid'] = array(
    'name' => t("Biblio ID"),
    'description' => t('The unique ID of the Biblio.'),
  );
  $biblio['type'] = array(
    'name' => t("Biblio type"),
    'description' => t("The type of the biblio."),
  );
  $biblio['type-name'] = array(
    'name' => t("Biblio name"),
    'description' => t("The human-readable name of the biblio type."),
  );
  $biblio['title'] = array(
    'name' => t("Title"),
    'description' => t("The title of the biblio."),
  );
  $biblio['url'] = array(
    'name' => t("URL"),
    'description' => t("The URL of the biblio."),
  );
  $biblio['edit-url'] = array(
    'name' => t("Edit URL"),
    'description' => t("The URL of the biblio's edit page."),
  );
  $biblio['created'] = array(
    'name' => t("Date created"),
    'description' => t("The date the biblio was posted."),
    'type' => 'date',
  );
  $biblio['changed'] = array(
    'name' => t("Date changed"),
    'description' => t("The date the biblio was most recently updated."),
    'type' => 'date',
  );
  $biblio['author'] = array(
    'name' => t("Author"),
    'description' => t("The author of the biblio."),
    'type' => 'user',
  );

  return array(
    'types' => array('biblio' => $type),
    'tokens' => array('biblio' => $biblio),
  );
}

/**
 * Implements hook_tokens().
 */
function biblio_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if (empty($data['entity']) || empty($data['entity_type']) || $data['entity_type'] != 'biblio') {
    return;
  }

  $biblio = $data['entity'];

  foreach ($tokens as $name => $original) {
    switch ($name) {
      case 'bid':
        $replacements[$original] = $biblio->bid;
        break;

      case 'type':
        $replacements[$original] = $sanitize ? check_plain($biblio->type) : $biblio->type;
        break;

      case 'type-name':
        $type = biblio_types($biblio->type);
        $replacements[$original] = $sanitize ? check_plain($type->name) : $type->name;
        break;

      case 'title':
        $replacements[$original] = $sanitize ? check_plain($biblio->title) : $biblio->title;
        break;

      case 'url':
        $replacements[$original] = url('biblio/' . $biblio->bid, $url_options);
        break;

      case 'edit-url':
        $replacements[$original] = url('biblio/' . $biblio->bid . '/edit', $url_options);
        break;

      case 'author':
        $account = user_load($biblio->uid);
        $name = format_username($account);
        $replacements[$original] = $sanitize ? check_plain($name) : $name;
        break;

      case 'created':
        $replacements[$original] = format_date($biblio->created, 'medium', '', NULL, $language_code);
        break;

      case 'changed':
        $replacements[$original] = format_date($biblio->changed, 'medium', '', NULL, $language_code);
        break;
    }
  }

  if ($author_tokens = token_find_with_prefix($tokens, 'author')) {
    $author = user_load($biblio->uid);
    $replacements += token_generate('user', $author_tokens, array('user' => $author), $options);
  }

  if ($created_tokens = token_find_with_prefix($tokens, 'created')) {
    $replacements += token_generate('date', $created_tokens, array('date' => $biblio->created), $options);
  }

  if ($changed_tokens = token_find_with_prefix($tokens, 'changed')) {
    $replacements += token_generate('date', $changed_tokens, array('date' => $biblio->changed), $options);
  }

  return $replacements;
}
