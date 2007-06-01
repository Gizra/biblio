<?PHP
// $Id$
function biblio_style_classic_info(){

    return array('classic'=>'Classic - This is the original biblio style');
                 
}

function biblio_style_classic($node,$base='biblio'){
  if (variable_get('biblio_normalize', 0)) {
    $authors = check_plain(_biblio_parse_authors($node->biblio_authors));
  } else {
    $authors = check_plain($node->biblio_authors);
  } 
  $output .=  l($node->title, "node/$node->nid") . "</span> \n";
  $output .=  _biblio_author_links($authors,$base) . "</span> \n";
  if ($node->biblio_secondary_title) {
    $output .= ', ' . check_plain($node->biblio_secondary_title);
  } 
  if ($node->biblio_date) $output .= ', ' . check_plain($node->biblio_date);
  if ($node->biblio_volume) $output .= ', Volume ' . check_plain($node->biblio_volume);
  if ($node->biblio_issue) $output .= ', Issue ' . check_plain($node->biblio_issue);
  if ($node->biblio_number) $output .= ', Number ' . check_plain($node->biblio_number);

  if ($node->biblio_place_published) $output .= ', ' . check_plain($node->biblio_place_published);
  if ($node->biblio_pages) $output .= ', p.' . check_plain($node->biblio_pages);
  if (isset($node->biblio_year)) {
    if ($node->biblio_year == 0) { $node->biblio_year = t("In Press"); }
    $output .= ', (' . check_plain($node->biblio_year) . ")\n";
  }

  return $output;
} 
?>
