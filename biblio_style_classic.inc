<?PHP
// $Id$
function biblio_style_classic_info(){

    return array('classic'=>'Classic - This is the original biblio style');
                 
}

function biblio_style_classic($pub){
  //$attrib = (variable_get('biblio_links_target_new_window',null)) ? array('target'=>'_blank') : null;
  $attrib = null;

  if (variable_get('biblio_normalize', 0)) {
    $authors = check_plain(_biblio_parse_authors($pub->biblio_authors));
  } else {
    $authors = check_plain($pub->biblio_authors);
  } 
  $output .=  l($pub->title, "node/$pub->nid",$attrib) . "</span> \n";
  $output .=  _biblio_author_links($authors) . "</span> \n";
  if ($pub->biblio_secondary_title) {
    $output .= ', ' . check_plain($pub->biblio_secondary_title);
  } 
  if ($pub->biblio_date) $output .= ', ' . check_plain($pub->biblio_date);
  if ($pub->biblio_volume) $output .= ', Volume ' . check_plain($pub->biblio_volume);
  if ($pub->biblio_issue) $output .= ', Issue ' . check_plain($pub->biblio_issue);
  if ($pub->biblio_number) $output .= ', Number ' . check_plain($pub->biblio_number);

  if ($pub->biblio_place_published) $output .= ', ' . check_plain($pub->biblio_place_published);
  if ($pub->biblio_pages) $output .= ', p.' . check_plain($pub->biblio_pages);
  if (isset($pub->biblio_year)) {
    if ($pub->biblio_year == 0) { $pub->biblio_year = t("In Press"); }
    $output .= ', (' . check_plain($pub->biblio_year) . ")\n";
  }

  return $output;
} 
?>
