<?php
// $Id$
// by Jeffrey Dwoskin
// A style closer to IEEE format
// see http://standards.ieee.org/guides/style/section7.html#992

function biblio_style_ieee_info(){
  return array('ieee'=>'Institute of Electrical and Electronics Engineers (IEEE)');                 
}

function biblio_style_ieee($node,$base='biblio'){

  if (variable_get('biblio_normalize', 0)) {
    $authors = _biblio_parse_authors(check_plain($node->biblio_authors));
  } else {
    $authors = check_plain($node->biblio_authors);
  } 
  $output .= '<span class="biblio-authors">' . str_replace(";", ",", _biblio_author_links($authors,$base)) . ", </span> \n";

      switch ($node->biblio_type) {
        default:

          if ($node->biblio_secondary_title) {
            $output .=  '<span class="biblio-title">&quot;' .l("$node->title", "node/$node->nid") . "&quot;, </span> \n";
            $output .= '<i>'.check_plain($node->biblio_secondary_title).'</i>';

          } else {
            $output .=  '<span class="biblio-title"><i>' .l("$node->title", "node/$node->nid") . ", </i></span> \n";
          }

          if ($node->biblio_edition) $output .= ', ' . check_plain($node->biblio_edition);

          if ($node->biblio_volume) $output .= ', vol. ' . check_plain($node->biblio_volume);
          if ($node->biblio_issue) $output .= ', issue ' . check_plain($node->biblio_issue);
          if ($node->biblio_number) $output .= ', no. ' . check_plain($node->biblio_number);

          if ($node->biblio_place_published) $output .= ', ' . check_plain($node->biblio_place_published);
          if ($node->biblio_publisher) {
            $output .= (check_plain($node->biblio_place_published)) ? ', ' : ': ';
            $output .= check_plain($node->biblio_publisher);
          }

          // if a single page instead of a range, should use 'p.' instead of 'pp.'  -- ignoring
          if ($node->biblio_pages) $output .= ', pp. ' . check_plain($node->biblio_pages);

          // if it is a book, year should go before pages instead -- ignoring
          // for non-books, should also include month of publication (e.g. "Mar. 2006") -- use date instead of year if available
          if ($node->biblio_date) {
            $output .= ', ' . check_plain($node->biblio_date);
          } elseif (isset($node->biblio_year)) {
            if ($node->biblio_year == 0) { $node->biblio_year = t("(In Press)"); }
            $output .= ', ' . check_plain($node->biblio_year) . '&nbsp;&nbsp;' ;
          }



          $output .= ".\n";
          break; // generic
      } 

/*  if ($node->biblio_date) $output .= ', ' . check_plain($node->biblio_date);

*/
  return $output;

}
