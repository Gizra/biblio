<?PHP

// $Id$
/**
 * Get the style information
 *
 * @return
 *   The name of the style
 */
function biblio_style_classic_info() {
  return array (
    'classic' => 'Classic - This is the original biblio style'
  );
}
function biblio_style_classic_author_options() {
  $author_options = array(
    'BetweenAuthorsDelimStandard'     =>  ', ',      //4
    'BetweenAuthorsDelimLastAuthor'   =>  ', and ',    //5
    'AuthorsInitialsDelimFirstAuthor' =>  ', ',      //7
    'AuthorsInitialsDelimStandard'    =>  ' ',       //8
    'betweenInitialsDelim'            =>  '. ',      //9
    'initialsBeforeAuthorFirstAuthor' =>  FALSE,     //10
    'initialsBeforeAuthorStandard'    =>  FALSE,      //11
    'shortenGivenNames'               =>  FALSE,      //12
    'numberOfAuthorsTriggeringEtAl'   =>  10,        //13
    'includeNumberOfAuthors'          =>  10,        //14
    'customStringAfterFirstAuthors'   =>  ', et al.',//15
    'encodeHTML'                      =>  true
  );
  return $author_options;
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
function biblio_style_classic($node, $base = 'biblio', $inline = false) {
  $author_options = biblio_style_classic_author_options();
  $authors = theme('biblio_format_authors', $node->biblio_contributors[1], $author_options, $inline);
  $output = '<span class="biblio-title">';
  $output .= $inline ? l("$node->title", "$base/viewinline/$node->nid") : l("$node->title", "node/$node->nid");
  $output .= "</span>, \n";
  $output .= '<span class="biblio-authors">' . $authors . "</span> \n";
  if ($node->biblio_secondary_title) {
    $output .= ', ' . check_plain($node->biblio_secondary_title);
  }
  if ($node->biblio_date)
    $output .= ', ' . check_plain($node->biblio_date);
  if ($node->biblio_volume)
    $output .= ', Volume ' . check_plain($node->biblio_volume);
  if ($node->biblio_issue)
    $output .= ', Issue ' . check_plain($node->biblio_issue);
  if ($node->biblio_number)
    $output .= ', Number ' . check_plain($node->biblio_number);
  if ($node->biblio_place_published)
    $output .= ', ' . check_plain($node->biblio_place_published);
  if ($node->biblio_pages)
    $output .= ', p.' . check_plain($node->biblio_pages);
  if (isset ($node->biblio_year)) {
    $output .= ', (' . check_plain($node->biblio_year) . ")\n";
  }
  return $output;
}

function _classic_format_author($author) {
  $format = $author['prefix'] . ' ' . $author['lastname'] . ' ';
  $format .= !empty ($author['firstname']) ? ' ' . drupal_substr($author['firstname'], 0, 1) : '';
  $format .= !empty ($author['initials']) ? str_replace(' ', '', $author['initials']) : '';
  return $format;
}