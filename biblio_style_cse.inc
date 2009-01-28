<?php

// $Id$
/**
 * Get the style information
 *
 * @return
 *   The name of the style
 */
function biblio_style_cse_info() {
  return array (
    'cse' => 'Council of Science Editors (CSE)'
  );
}
/**
 * Apply a bibliographic style to the node
 *
 *
 * @param $node
 *   An object containing the node data to render
 * @param $base
 *   The base URL of the biblio module (defaults to /biblio)
 * @param $inline
 *   A logical value indicating if this is being rendered within the
 *   Drupal framwork (false) or we are just passing back the html (true)
 * @return
 *   The styled biblio entry
 */
function biblio_style_cse($node, $base = 'biblio', $inline = false) {
  $authors = theme('biblio_authors', $node->biblio_contributors[1], 'cse', 1, $inline);
  if (empty($authors)) $authors = theme('biblio_authors', $node->biblio_contributors[5], 'cse', 5, $inline);  // if no authors substitute corp author if available
  if (empty($authors)) $authors = '[' . t('Anonymous') . ']';  // use anonymous if we still have nothing.
  $output .= '<span class="biblio-authors">' . $authors . "</span>.&nbsp; \n";
  switch ($node->biblio_type) {

    default :
      if (isset ($node->biblio_year)) {
        $output .= check_plain($node->biblio_year) . ".&nbsp;&nbsp;";
      }
      $output .= '<span class="biblio-title">';
      $output .= $inline ? l("$node->title", "$base/viewinline/$node->nid") : l("$node->title", "node/$node->nid");
      $output .= ". </span> \n";
      $output .= (!empty ($node->biblio_secondary_title)) ? check_plain($node->biblio_secondary_title) . '. ' : '';
      $output .= (!empty ($node->biblio_volume))          ? check_plain($node->biblio_volume) : '';
      $output .= (!empty ($node->biblio_issue))           ? '(' . check_plain($node->biblio_issue) . ')' : '';
      $output .= (!empty ($node->biblio_pages))           ? ':' . str_replace(" ", "", check_plain($node->biblio_pages)) . '.' : '';
      break; // generic
  }
  /*  if ($node->biblio_date) $output .= ', '. check_plain($node->biblio_date);
    if ($node->biblio_number) $output .= ', Number '. check_plain($node->biblio_number);
  
    if ($node->biblio_place_published) $output .= ', '. check_plain($node->biblio_place_published);
  */
  return $output;
}
function _cse_format_author($author) {
  $format = $author['prefix'] . ' ' . $author['lastname'] . ' ';
  $format .= !empty ($author['firstname']) ? ' ' . drupal_substr($author['firstname'], 0, 1) : '';
  $format .= !empty ($author['initials']) ? str_replace(' ', '', $author['initials']) : '';
  return $format;
}