<?php

/**
 * Get the style information
 *
 * @return
 *   The name of the style
 */
function biblio_style_cse_info() {
  return array(
    'cse' => 'Council of Science Editors (CSE)'
  );
}
/**
 * Apply a bibliographic style to the biblio
 *
 *
 * @param $biblio
 *   An object containing the biblio data to render
 * @return
 *   The styled biblio entry
 */
function biblio_style_cse_author_options() {
  $author_options = array(
    'BetweenAuthorsDelimStandard'     =>', ',      //4
    'BetweenAuthorsDelimLastAuthor'   => ', ', //5
    'AuthorsInitialsDelimFirstAuthor' => ' ',     //7
    'AuthorsInitialsDelimStandard'    => ' ',      //8
    'betweenInitialsDelim'            => '',     //9
    'initialsBeforeAuthorFirstAuthor' => FALSE,    //10
    'initialsBeforeAuthorStandard'    => FALSE,     //11
    'shortenGivenNames'               => TRUE,    //12
    'numberOfAuthorsTriggeringEtAl'   => 10,       //13
    'includeNumberOfAuthors'          => 10,       //14
    'customStringAfterFirstAuthors'   => ' et al.',//15
    'encodeHTML'                      => TRUE
  );
  return $author_options;
}

function biblio_style_cse($biblio) {
  module_load_include('inc', 'biblio', '/includes/biblio.contributors');
  $output = $authors = '';
  
  if (!empty($biblio->biblio_authors_primary)) {
    $primary_authors = $biblio->biblio_authors_primary[$biblio->language];
    $author_options = biblio_style_cse_author_options();
  }
  if (!empty($biblio->biblio_authors_secondary)) {
    $editors = $biblio->biblio_authors_secondary[$biblio->language];
  }
  if (!empty($biblio->biblio_authors_corporate)) {
    $corp_authors = $biblio->biblio_authors_corporate[$biblio->language];
  }
  
  if (!empty($primary_authors)) {
    $authors = theme('biblio_format_authors', array('contributors' => $primary_authors, 'options' => $author_options));
  }
  if (empty($authors) && !empty($corp_authors)) {// if no authors substitute corp author if available
    foreach ($corp_authors as $rank => $author) {
      $authors .= (empty($authors)) ? '' : ', ';
      $authors .= (variable_get('biblio_author_links', 1)) ?  theme('biblio_author_link', array('author' => $author)) : $author['name'];

    }
  }
  if (empty($authors)) $authors = '[' . t('Anonymous') . ']';  // use anonymous if we still have nothing.
  if (!empty ($biblio->biblio_citekey[$biblio->language][0]['value'])&&(variable_get('biblio_display_citation_key',0))) {
    $output .= '[' . check_plain($biblio->biblio_citekey[$biblio->language][0]['value']) . '] ';
  }
  $output .= '<span class="biblio-authors">' . $authors . "</span>.&nbsp; \n";

  switch ($biblio->publication_type) {

    default :
      if (isset ($biblio->biblio_year[$biblio->language][0]['value'])) {
        $output .= check_plain($biblio->biblio_year[$biblio->language][0]['value']) . ".&nbsp;&nbsp;";
      }
      $output .= '<span class="biblio-title">';
      $url = biblio_get_title_url_info($biblio);
      $output .= l($biblio->biblio_title[$biblio->language][0]['value'], $url['link'], $url['options']);
      $output .= (strpos($biblio->biblio_title[$biblio->language][0]['value'], '?'))? " </span>" : ". </span>";  // if the title ends in a question mark, don't put a period after it.
      $output .= (!empty ($biblio->biblio_title_secondary[$biblio->language][0]['value'])) ? check_plain($biblio->biblio_title_secondary[$biblio->language][0]['value']) . '. ' : '';
      $output .= (!empty ($biblio->biblio_volume[$biblio->language][0]['value']))          ? check_plain($biblio->biblio_volume[$biblio->language][0]['value']) : '';
      $output .= (!empty ($biblio->biblio_issue[$biblio->language][0]['value']))           ? '(' . check_plain($biblio->biblio_issue[$biblio->language][0]['value']) . ')' : '';
      $output .= (!empty ($biblio->biblio_pages[$biblio->language][0]['value']))           ? ':' . str_replace(" ", "", check_plain($biblio->biblio_pages[$biblio->language][0]['value'])) . '.' : '';
      break; // generic
  }
  /*  if ($node->biblio_date) $output .= ', '. check_plain($node->biblio_date);
    if ($node->biblio_number) $output .= ', Number '. check_plain($node->biblio_number);

    if ($node->biblio_place_published) $output .= ', '. check_plain($node->biblio_place_published);
  */

  return filter_xss($output, biblio_get_allowed_tags());

}
