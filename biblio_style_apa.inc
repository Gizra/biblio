<?php
// $Id$
function biblio_style_apa_info(){

    return array('apa'=>'American Psychological Association (APA)');
                 
}
function biblio_style_apa($node,$base='biblio'){

  if (variable_get('biblio_normalize', 0)) {
    $authors = _biblio_parse_authors(check_plain($node->biblio_authors));
  } else {
    $authors = check_plain($node->biblio_authors);
  } 
  $output .= '<span class="biblio-authors">' . str_replace(";", ",", _biblio_author_links($authors,$base)) . "</span> \n";
  $output .= (strrpos($authors, '.' ) == strlen($authors))? ".&nbsp;&nbsp;":" ";

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
            $output .= "(" . check_plain($node->biblio_year) . ").&nbsp;&nbsp;" ;
          }
          $output .=  '<span class="biblio-title-apa">' .l("$node->title", "node/$node->nid") . ". </span> \n";
          $output .= ($node->biblio_secondary_title) ? '<u>'.check_plain($node->biblio_secondary_title).'. ' : '<u>';
          $output .= ($node->biblio_volume)?  check_plain($node->biblio_volume).($node->biblio_issue?'</u>('.check_plain($node->biblio_issue).'),&nbsp;' :',</u> ') : '</u> ';
        //  $output .= ($node->biblio_issue) ? '(' . check_plain($node->biblio_issue).')' :'';
          $output .= ($node->biblio_pages) ?  check_plain($node->biblio_pages).'.' :'';
          break; // generic
      } 

/*  if ($node->biblio_date) $output .= ', ' . check_plain($node->biblio_date);
  if ($node->biblio_number) $output .= ', Number ' . check_plain($node->biblio_number);

  if ($node->biblio_place_published) $output .= ', ' . check_plain($node->biblio_place_published);
*/
  return $output;

}
?>
