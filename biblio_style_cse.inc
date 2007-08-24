<?php
// $Id$
/**
 * Get the style information
 *
 * @return
 *   The name of the style
 */
function biblio_style_cse_info(){

    return array('cse'=>'Council of Science Editors (CSE)');
                 
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
function biblio_style_cse($node, $base='biblio', $inline=false){

  if (variable_get('biblio_normalize', 0)) {
    $authors = _biblio_parse_authors($node->biblio_authors);
  } else {
    $authors = $node->biblio_authors;
  } 
  $output .= '<span class="biblio-authors">' . str_replace(";", ",", str_replace(array(".",","),"",_biblio_author_links($authors,$base,$inline))) . "</span>.&nbsp; \n";
//  $output .= (strrpos($authors, '.' ) == strlen($authors))? ".&nbsp;&nbsp;":" ";

      switch ($node->biblio_type) {
        case 1: // Journal Article
        case 2: //Conference Paper
        case 3: // are all
        case 4: 
        case 5: 
        case 6:
        case 7:
        case 8: 
        case 9:
        default:
          if (isset($node->biblio_year)) {
            if ($node->biblio_year == 0) { $node->biblio_year = t("In Press"); }
            $output .= check_plain($node->biblio_year).".&nbsp;&nbsp;" ;
            
          }
          $output .= '<span class="biblio-title">' ;
          $output .= $inline ? l("$node->title", "$base/viewinline/$node->nid") : l("$node->title", "node/$node->nid"); 
          $output .= ". </span> \n";
          $output .= ($node->biblio_secondary_title) ? check_plain($node->biblio_secondary_title).'. ' : '';
          $output .= ($node->biblio_volume)?  check_plain($node->biblio_volume) : '';
          $output .= ($node->biblio_issue) ? '(' . check_plain($node->biblio_issue).')' :'';
          $output .= ($node->biblio_pages) ? ':' . str_replace(" ","",check_plain($node->biblio_pages)).'.' :'';
          break; // generic
      } 

/*  if ($node->biblio_date) $output .= ', ' . check_plain($node->biblio_date);
  if ($node->biblio_number) $output .= ', Number ' . check_plain($node->biblio_number);

  if ($node->biblio_place_published) $output .= ', ' . check_plain($node->biblio_place_published);
*/
  return $output;

}
?>
