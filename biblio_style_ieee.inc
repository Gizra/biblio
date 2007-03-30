<?php
// $Id$
// by Jeffrey Dwoskin
// A style closer to IEEE format
// see http://standards.ieee.org/guides/style/section7.html#992

function biblio_style_ieee_info(){
  return array('ieee'=>'Institute of Electrical and Electronics Engineers (IEEE)');                 
}

function biblio_style_ieee($pub){

  $attrib = (variable_get('biblio_links_target_new_window',null)) ? array('target'=>'_blank') : null;

  if (variable_get('biblio_normalize', 0)) {
    $authors = _biblio_parse_authors(check_plain($pub->biblio_authors));
  } else {
    $authors = check_plain($pub->biblio_authors);
  } 
  $output .= '<span class="biblio-authors">' . str_replace(";", ",", _biblio_author_links($authors)) . ", </span> \n";

      switch ($pub->biblio_type) {
        default:

          if ($pub->biblio_secondary_title) {
            $output .=  '<span class="biblio-title">&quot;' .l("$pub->title", "node/$pub->nid",$attrib) . "&quot;, </span> \n";
            $output .= '<i>'.check_plain($pub->biblio_secondary_title).'</i>';

          } else {
            $output .=  '<span class="biblio-title"><i>' .l("$pub->title", "node/$pub->nid", $attrib) . ", </i></span> \n";
          }

          if ($pub->biblio_edition) $output .= ', ' . check_plain($pub->biblio_edition);

          if ($pub->biblio_volume) $output .= ', vol. ' . check_plain($pub->biblio_volume);
          if ($pub->biblio_issue) $output .= ', issue ' . check_plain($pub->biblio_issue);
          if ($pub->biblio_number) $output .= ', no. ' . check_plain($pub->biblio_number);

          if ($pub->biblio_place_published) $output .= ', ' . check_plain($pub->biblio_place_published);
          if ($pub->biblio_publisher) {
            $output .= (check_plain($pub->biblio_place_published)) ? ', ' : ': ';
            $output .= check_plain($pub->biblio_publisher);
          }

          // if a single page instead of a range, should use 'p.' instead of 'pp.'  -- ignoring
          if ($pub->biblio_pages) $output .= ', pp. ' . check_plain($pub->biblio_pages);

          // if it is a book, year should go before pages instead -- ignoring
          // for non-books, should also include month of publication (e.g. "Mar. 2006") -- use date instead of year if available
          if ($pub->biblio_date) {
            $output .= ', ' . check_plain($pub->biblio_date);
          } elseif (isset($pub->biblio_year)) {
            if ($pub->biblio_year == 0) { $pub->biblio_year = t("(In Press)"); }
            $output .= ', ' . check_plain($pub->biblio_year) . '&nbsp;&nbsp;' ;
          }



          $output .= ".\n";
          break; // generic
      } 

/*  if ($pub->biblio_date) $output .= ', ' . check_plain($pub->biblio_date);

*/
  return $output;

}
