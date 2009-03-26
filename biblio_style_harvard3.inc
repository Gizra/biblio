<?php
	// Original File:       ./cite/styles/cite_Harvard_3.php
	// Original Author(s):  Matthias Steffens <mailto:refbase@extracts.de>
	//

	// This is a variant of the Harvard author/date style, modeled after these resources:
	// <http://library.curtin.edu.au/referencing/harvard.html> (not available anymore?)
	// <http://www.usq.edu.au/library/help/ehelp/ref_guides/harvard.htm>
	// <http://www.library.uwa.edu.au/education_training_and_support/guides/harvard_citation_style>
	// <http://www.lib.unimelb.edu.au/cite/harvard_dis/>


	// TODO: - patents & reports?
	//       - should we shorten ending page numbers if necessary (e.g. "pp. 10-8" or "pp. 51-5", but "pp. 19-26"), or only if numbers are >=3 digits?
	//       - where to put (and how to format) series info & editors of whole books that also have an author?

	// --------------------------------------------------------------------

	// --- BEGIN CITATION STYLE ---
function biblio_style_harvard3_info() {
  return array (
    'harvard3' => 'Harvard - 3'
    );
}

function biblio_style_harvard3_author_options() {
  $author_options = array(
    'oldAuthorsInitialsDelim'           => ',',
    'BetweenAuthorsDelimStandard' =>', ',          //4 for all authors except the last author: new delimiter that separates different authors
    'BetweenAuthorsDelimLastAuthor' => ' &amp; ',  //5 for the last author: new delimiter that separates the last author from all other authors
    'AuthorsInitialsDelimFirstAuthor' => ', ',     //7 for the first author: new delimiter that separates author name & initials (within one author)
    'AuthorsInitialsDelimStandard'=> ', ',         //8 for all authors except the first author: new delimiter that separates author name & initials (within one author)
    'betweenInitialsDelim' => '',                  //9 new delimiter that separates multiple initials (within one author)
    'initialsBeforeAuthorFirstAuthor' => false,    //10 for the first author: boolean value that specifies if initials go *before* the author's name ['true'], or *after* the author's name ['false'] (which is the default in the db)
    'initialsBeforeAuthorStandard' => false,       //11 for all authors except the first author: boolean value that specifies if initials go *before* the author's name ['true'], or *after* the author's name ['false'] (which is the default in the db)
    'shortenGivenNames' => TRUE,                   //12 boolean value that specifies whether an author's full given name(s) shall be shortened to initial(s)
    'numberOfAuthorsTriggeringEtAl' => 10,         //13 if the total number of authors is greater than the given number (integer >= 1), only the number of authors given in (14) will be included in the citation along with the string given in (15); keep empty if all authors shall be returned
    'includeNumberOfAuthors' => 10,                //14 number of authors (integer >= 1) that is included in the citation if the total number of authors is greater than the number given in (13); keep empty if not applicable
    'customStringAfterFirstAuthors' => ' et al',   //15 string that's appended to the number of authors given in (14) if the total number of authors is greater than the number given in (13); the actual number of authors can be printed by including '__NUMBER_OF_AUTHORS__' (without quotes) within the string
    'encodeHTML' => true
  );
  
  return $author_options;
}

function biblio_style_harvard3($node, $base = 'biblio', $inline = false) {

  $output = ""; // make sure that our buffer variable is empty
  
  $author_options = biblio_style_harvard3_author_options();
  $authors = theme('biblio_format_authors', $node->biblio_authors, $author_options, $inline);
  
  if (!empty ($node->biblio_citekey)&&(variable_get('biblio_display_citation_key',0))) {
    $output .= '[' . check_plain($node->biblio_citekey) . '] ';
  }
  // --- BEGIN TYPE = JOURNAL ARTICLE / MAGAZINE ARTICLE / NEWSPAPER ARTICLE --------------------------------------------------------------
  switch ($node->biblio_type) {
    case 102: //Journal Article
    case 105: //Newspaper Article
    case 106: //Magazine Article
  
				if (!empty($authors)) $output .= $authors;

				if (!empty($node->biblio_year))      // year
				{
					if (!empty($authors))
						$output .= " ";

					if (!empty($node->biblio_year))
						$output .= $node->biblio_year;
				}

				if (!empty($node->title))      // title
				{
					if (!empty($authors) || !empty($node->biblio_year))
						$output .= ", ";

					$output .= $markupPatternsArray["single-quote-left"] . $node->title . $markupPatternsArray["single-quote-right"];
					$output .= ",";
				}

				// From here on we'll assume that at least one of the fields 'author', 'year' or 'title' did contain some contents

				if (!empty($node->biblio_secondary_title))      // publication (= journal) name
					$output .= ' <i>' . $node->biblio_secondary_title . '</i>';

				// if there's no full journal name, we'll use the abbreviated journal name instead:
				elseif (!empty($node->biblio_alternate_title))      // abbreviated journal name
					$output .= ' <i>' . $node->biblio_alternate_title . '</i>';

				if ((!empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)) && (!empty($node->biblio_volume) || !empty($node->biblio_issue)))
					$output .= ",";

				if ($node->biblio_type == 102) // for journal articles, volume (or issue) information is printed with a "vol." (or "no.") prefix
				{
					if (!empty($node->biblio_volume))      // volume
						$output .= " vol. " . $node->biblio_volume;

					if (!empty($node->biblio_issue))      // issue
					{
						if (!empty($node->biblio_volume))
							$output .= ",";

						$output .= " no. " . $node->biblio_issue;
					}
				}

				elseif ($node->biblio_type == 105 || $node->biblio_type == 106) // for newspaper and magazine articles, volume (=month) and issue (=day) information is printed without prefix
				{
					if (!empty($node->biblio_issue))      // issue (=day)
						$output .= " " . $node->biblio_issue;

					if (!empty($node->biblio_volume))      // volume (=month)
						$output .= " " . $node->biblio_volume;
				}

				if (!empty($node->biblio_pages))      // pages
				{
					if (!empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)) // only add ", " if either volume, issue, abbrev_journal or publication isn't empty
						$output .= ", ";

					$output .= theme_biblio_page_number($node->biblio_pages, '-', "p. ", "pp. "); // function 'formatPageInfo()' is defined in 'cite.inc.php'
				}

				if ($row['online_publication'] == "yes") // this record refers to an online article
				{
					// append an optional string (given in 'online_citation') plus the current date and the DOI (or URL):

					if (!empty($row['online_citation']))      // online_citation
					{
						if (!empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)) // only add "," if either volume, issue, abbrev_journal or publication isn't empty
							$output .= ",";

						$output .= " " . $row['online_citation'];
					}

					if (!empty($node->biblio_doi) || !empty($node->biblio_url))      // doi OR url
					{
						if (!empty($row['online_citation']) OR (empty($row['online_citation']) AND (!empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)))) // only add "." if online_citation isn't empty, or else if either volume, issue, abbrev_journal or publication isn't empty
							$output .= ".";

						$today = date("j F Y");

						$output .= " Available from: "; // NOTE: some of the above mentioned resources use ", accessed 26 February 2004, from <http://...>." (or ", viewed on ...") instead

						if (!empty($node->biblio_doi))      // doi
							$uri = "http://dx.doi.org/" . $node->biblio_doi;
						else      // url
							$uri = $node->biblio_url;

						if ($encodeHTML)
							$output .= encodeHTML('<' . $uri . '>');
						else
							$output .= '<' . $uri . '>';

						$output .= " [" . $today . "]";
					}
				}

				if (!ereg("\. *$", $output)) // if the string doesn't end with a period
					$output .= ".";
     break;
     
		// --- BEGIN TYPE = ABSTRACT / BOOK CHAPTER / CONFERENCE ARTICLE ------------------------------------------------------------------------
    case 101: //Book Chapter
    case 103: //Conference Paper
     
				if (!empty($authors)) $output .= $authors;

				if (!empty($node->biblio_year))      // year
				{
					if (!empty($authors))	$output .= " ";

					$output .= $node->biblio_year;
				}

				if (!empty($node->title))      // title
				{
					if (!empty($authors) || !empty($node->biblio_year))
						$output .= ", ";

					$output .= $markupPatternsArray["single-quote-left"] . $node->title . $markupPatternsArray["single-quote-right"];
					$output .= ",";
				}

				// From here on we'll assume that at least one of the fields 'author', 'year' or 'title' did contain some contents
				// if this is not the case, the output string will begin with a space. However, any preceding/trailing whitespace will be removed at the cleanup stage (see below)

				if (!empty($node->biblio_secondary_authors))      // editor
					{
						// Call the 'reArrangeAuthorContents()' function (defined in 'include.inc.php') in order to re-order contents of the author field. Required Parameters:
						//   1. input:  contents of the author field
						//   2. input:  boolean value that specifies whether the author's family name comes first (within one author) in the source string
						//              ('true' means that the family name is followed by the given name (or initials), 'false' if it's the other way around)
						//
						//   3. input:  pattern describing old delimiter that separates different authors
						//   4. output: for all authors except the last author: new delimiter that separates different authors
						//   5. output: for the last author: new delimiter that separates the last author from all other authors
						//
						//   6. input:  pattern describing old delimiter that separates author name & initials (within one author)
						//   7. output: for the first author: new delimiter that separates author name & initials (within one author)
						//   8. output: for all authors except the first author: new delimiter that separates author name & initials (within one author)
						//   9. output: new delimiter that separates multiple initials (within one author)
						//  10. output: for the first author: boolean value that specifies if initials go *before* the author's name ['true'], or *after* the author's name ['false'] (which is the default in the db)
						//  11. output: for all authors except the first author: boolean value that specifies if initials go *before* the author's name ['true'], or *after* the author's name ['false'] (which is the default in the db)
						//  12. output: boolean value that specifies whether an author's full given name(s) shall be shortened to initial(s)
						//
						//  13. output: if the total number of authors is greater than the given number (integer >= 1), only the number of authors given in (14) will be included in the citation along with the string given in (15); keep empty if all authors shall be returned
						//  14. output: number of authors (integer >= 1) that is included in the citation if the total number of authors is greater than the number given in (13); keep empty if not applicable
						//  15. output: string that's appended to the number of authors given in (14) if the total number of authors is greater than the number given in (13); the actual number of authors can be printed by including '__NUMBER_OF_AUTHORS__' (without quotes) within the string
						//
						//  16. output: boolean value that specifies whether the re-ordered string shall be returned with higher ASCII chars HTML encoded
						$editor = reArrangeAuthorContents($node->biblio_secondary_authors, // 1.
						                                  true, // 2.
						                                  " *; *", // 3.
						                                  ", ", // 4.
						                                  " " . $markupPatternsArray["ampersand"] . " ", // 5.
						                                  " *, *", // 6.
						                                  " ", // 7.
						                                  " ", // 8.
						                                  "", // 9.
						                                  true, // 10.
						                                  true, // 11.
						                                  true, // 12.
						                                  "", // 13.
						                                  "", // 14.
						                                  " et al", // 15.
						                                  $encodeHTML); // 16.

						$output .= " in " . $editor . " (";
						if (ereg("^[^;\r\n]+(;[^;\r\n]+)+$", $node->biblio_secondary_authors)) // there are at least two editors (separated by ';')
							$output .= "eds.";
						else // there's only one editor (or the editor field is malformed with multiple editors but missing ';' separator[s])
							$output .= "ed.";
						$output .= "),";
					}

				$publication = ereg_replace("[ \r\n]*\(Eds?:[^\)\r\n]*\)", "", $node->biblio_secondary_title);
				if (!empty($publication))      // publication
				{
					if (empty($node->biblio_secondary_authors))
						$output .= " in";

					$output .= " " . '<i>' . $publication . '</i>';
				}

				if (!empty($node->biblio_edition) && !preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition) || !empty($node->biblio_volume))
				{
					$output .= ", ";

					if (!empty($node->biblio_edition) && !preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition))      // edition
					{
						if (preg_match("/^\d{1,3}$/", $node->biblio_edition)) // if the edition field contains a number of up to three digits, we assume it's an edition number (such as "2nd edn")
						{
							if ($node->biblio_edition == "2")
								$editionSuffix = "nd";
							elseif ($node->biblio_edition == "3")
								$editionSuffix = "rd";
							else
								$editionSuffix = "th";
						}
						else
							$editionSuffix = "";

						if (!empty($node->biblio_edition) && !preg_match("/( ed\.?| edition)$/i", $node->biblio_edition))
							$editionSuffix .= " edn";

						$output .= $node->biblio_edition . $editionSuffix;
					}

					if (!empty($node->biblio_volume))      // volume
					{
						if (!empty($node->biblio_edition) && !preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition))
							$output .= ", ";

						$output .= "vol. " . $node->biblio_volume;
					}
				}

				if (!empty($node->biblio_alternate_title) OR !empty($node->biblio_tertiary_title)) // if there's either a full or an abbreviated series title
				{
					$output .= ", ";

					if (!empty($node->biblio_tertiary_title))
						$output .= $node->biblio_tertiary_title;      // full series title

					// if there's no full series title, we'll use the abbreviated series title instead:
					elseif (!empty($node->biblio_alternate_title))
						$output .= $node->biblio_alternate_title;      // abbreviated series title

					if (!empty($node->biblio_volume)||!empty($node->biblio_issue))
						$output .= ", ";

					if (!empty($node->biblio_volume))      // series volume (I'm not really sure if -- for this cite style -- the series volume & issue should be rather omitted here)
						$output .= "vol. " . $node->biblio_volume;

					if (!empty($node->biblio_issue))      // series issue (see note for series volume)
					{
						if (!empty($node->biblio_volume))
							$output .= ", ";

						$output .= "no. " . $node->biblio_issue;
					}
				}

				$output .= ",";

				if (!empty($node->biblio_publisher))      // publisher
					$output .= " " . $node->biblio_publisher;

				if (!empty($node->biblio_place_published))      // place
				{
					if (!empty($node->biblio_publisher))
						$output .= ",";

					$output .= " " . $node->biblio_place_published;
				}

				if (!empty($node->biblio_pages))      // pages
				{
					if (!empty($node->biblio_publisher) || !empty($node->biblio_place_published))
						$output .= ", ";

					$output .= formatPageInfo($node->biblio_pages, $markupPatternsArray["endash"], "p. ", "pp. "); // function 'formatPageInfo()' is defined in 'cite.inc.php'
				}

				if (!ereg("\. *$", $output))
					$output .= ".";
			
      break;

    
			
		// --- BEGIN TYPE = BOOK WHOLE / CONFERENCE VOLUME / JOURNAL / MANUAL / MANUSCRIPT / MAP / MISCELLANEOUS / PATENT / REPORT / SOFTWARE ---
    default : // all other types
				if (!empty($authors))      // author
					{
						$author = ereg_replace("[ \r\n]*\(eds?\)", "", $authors);

						// Call the 'reArrangeAuthorContents()' function (defined in 'include.inc.php') in order to re-order contents of the author field. Required Parameters:
						//   1. input:  contents of the author field
						//   2. input:  boolean value that specifies whether the author's family name comes first (within one author) in the source string
						//              ('true' means that the family name is followed by the given name (or initials), 'false' if it's the other way around)
						//
						//   3. input:  pattern describing old delimiter that separates different authors
						//   4. output: for all authors except the last author: new delimiter that separates different authors
						//   5. output: for the last author: new delimiter that separates the last author from all other authors
						//
						//   6. input:  pattern describing old delimiter that separates author name & initials (within one author)
						//   7. output: for the first author: new delimiter that separates author name & initials (within one author)
						//   8. output: for all authors except the first author: new delimiter that separates author name & initials (within one author)
						//   9. output: new delimiter that separates multiple initials (within one author)
						//  10. output: for the first author: boolean value that specifies if initials go *before* the author's name ['true'], or *after* the author's name ['false'] (which is the default in the db)
						//  11. output: for all authors except the first author: boolean value that specifies if initials go *before* the author's name ['true'], or *after* the author's name ['false'] (which is the default in the db)
						//  12. output: boolean value that specifies whether an author's full given name(s) shall be shortened to initial(s)
						//
						//  13. output: if the total number of authors is greater than the given number (integer >= 1), only the number of authors given in (14) will be included in the citation along with the string given in (15); keep empty if all authors shall be returned
						//  14. output: number of authors (integer >= 1) that is included in the citation if the total number of authors is greater than the number given in (13); keep empty if not applicable
						//  15. output: string that's appended to the number of authors given in (14) if the total number of authors is greater than the number given in (13); the actual number of authors can be printed by including '__NUMBER_OF_AUTHORS__' (without quotes) within the string
						//
						//  16. output: boolean value that specifies whether the re-ordered string shall be returned with higher ASCII chars HTML encoded
						$author = reArrangeAuthorContents($author, // 1.
						                                  true, // 2.
						                                  " *; *", // 3.
						                                  ", ", // 4.
						                                  " " . $markupPatternsArray["ampersand"] . " ", // 5.
						                                  " *, *", // 6.
						                                  ", ", // 7.
						                                  ", ", // 8.
						                                  "", // 9.
						                                  false, // 10.
						                                  false, // 11.
						                                  true, // 12.
						                                  "", // 13.
						                                  "", // 14.
						                                  " et al", // 15.
						                                  $encodeHTML); // 16.

						// if the author is actually the editor of the resource we'll append ', ed' (or ', eds') to the author string:
						// [to distinguish editors from authors in the 'author' field, the 'modify.php' script does append ' (ed)' or ' (eds)' if appropriate,
						//  so we're just checking for these identifier strings here. Alternatively, we could check whether the editor field matches the author field]
						if (ereg("[ \r\n]*\(ed\)", $authors)) // single editor
							$author = $author . " (ed.)";
						elseif (ereg("[ \r\n]*\(eds\)", $authors)) // multiple editors
							$author = $author . " (eds.)";

						$output .= $author;
					}

				if (!empty($node->biblio_year))      // year
				{
					if (!empty($authors))
						$output .= " ";

					$output .= $node->biblio_year;
				}

				if (!empty($node->title))      // title
				{
					if (!empty($authors) || !empty($node->biblio_year))
						$output .= ", ";

					if (!empty($row['thesis'])) // for theses, the title is printed in roman type and in quotation marks
						$output .= $markupPatternsArray["single-quote-left"] . $node->title . $markupPatternsArray["single-quote-right"];
					else // otherwise, the title is printed in italics
						$output .= '<i>' . $node->title . '</i>';
				}

				if (!empty($node->biblio_secondary_authors) && !ereg("[ \r\n]*\(eds?\)", $authors))      // editor (if different from author, see note above regarding the check for ' (ed)' or ' (eds)')
					{
						// Call the 'reArrangeAuthorContents()' function (defined in 'include.inc.php') in order to re-order contents of the author field. Required Parameters:
						//   1. input:  contents of the author field
						//   2. input:  boolean value that specifies whether the author's family name comes first (within one author) in the source string
						//              ('true' means that the family name is followed by the given name (or initials), 'false' if it's the other way around)
						//
						//   3. input:  pattern describing old delimiter that separates different authors
						//   4. output: for all authors except the last author: new delimiter that separates different authors
						//   5. output: for the last author: new delimiter that separates the last author from all other authors
						//
						//   6. input:  pattern describing old delimiter that separates author name & initials (within one author)
						//   7. output: for the first author: new delimiter that separates author name & initials (within one author)
						//   8. output: for all authors except the first author: new delimiter that separates author name & initials (within one author)
						//   9. output: new delimiter that separates multiple initials (within one author)
						//  10. output: for the first author: boolean value that specifies if initials go *before* the author's name ['true'], or *after* the author's name ['false'] (which is the default in the db)
						//  11. output: for all authors except the first author: boolean value that specifies if initials go *before* the author's name ['true'], or *after* the author's name ['false'] (which is the default in the db)
						//  12. output: boolean value that specifies whether an author's full given name(s) shall be shortened to initial(s)
						//
						//  13. output: if the total number of authors is greater than the given number (integer >= 1), only the number of authors given in (14) will be included in the citation along with the string given in (15); keep empty if all authors shall be returned
						//  14. output: number of authors (integer >= 1) that is included in the citation if the total number of authors is greater than the number given in (13); keep empty if not applicable
						//  15. output: string that's appended to the number of authors given in (14) if the total number of authors is greater than the number given in (13); the actual number of authors can be printed by including '__NUMBER_OF_AUTHORS__' (without quotes) within the string
						//
						//  16. output: boolean value that specifies whether the re-ordered string shall be returned with higher ASCII chars HTML encoded
						$editor = reArrangeAuthorContents($node->biblio_secondary_authors, // 1.
						                                  true, // 2.
						                                  " *; *", // 3.
						                                  ", ", // 4.
						                                  " " . $markupPatternsArray["ampersand"] . " ", // 5.
						                                  " *, *", // 6.
						                                  " ", // 7.
						                                  " ", // 8.
						                                  "", // 9.
						                                  true, // 10.
						                                  true, // 11.
						                                  true, // 12.
						                                  "", // 13.
						                                  "", // 14.
						                                  " et al", // 15.
						                                  $encodeHTML); // 16.

						if (!empty($authors) || !empty($node->biblio_year) || !empty($node->title))
							$output .= " ";

						$output .= " (" . $editor . ", ";
						if (ereg("^[^;\r\n]+(;[^;\r\n]+)+$", $node->biblio_secondary_authors)) // there are at least two editors (separated by ';')
							$output .= "eds.";
						else // there's only one editor (or the editor field is malformed with multiple editors but missing ';' separator[s])
							$output .= "ed.";
						$output .= ")";
					}

				if (!empty($node->biblio_edition) || !empty($node->biblio_volume))
				{
					if (!empty($authors) || !empty($node->biblio_year) || !empty($node->title) || (!empty($node->biblio_secondary_authors) && !ereg("[ \r\n]*\(eds?\)", $authors)))
						$output .= ", ";

					if ($row['type'] == "Software")      // software edition (=version)
					{
						if (!empty($node->biblio_edition))
						{
							$output .= "version " . $node->biblio_edition;

							if (!empty($node->biblio_volume) || !empty($node->biblio_issue))
								$output .= ", ";
						}

						if (!empty($node->biblio_issue))      // issue (=day)
							$output .= " " . $node->biblio_issue;

						if (!empty($node->biblio_volume))      // volume (=month)
							$output .= " " . $node->biblio_volume;
					}
					elseif (!preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition))      // regular edition (other than the first)
					{
						if (preg_match("/^\d{1,3}$/", $node->biblio_edition)) // if the edition field contains a number of up to three digits, we assume it's an edition number (such as "2nd edn")
						{
							if ($node->biblio_edition == "2")
								$editionSuffix = "nd";
							elseif ($node->biblio_edition == "3")
								$editionSuffix = "rd";
							else
								$editionSuffix = "th";
						}
						else
							$editionSuffix = "";

						if (!empty($node->biblio_edition) && !preg_match("/( ed\.?| edition)$/i", $node->biblio_edition))
							$editionSuffix .= " edn";

						$output .= $node->biblio_edition . $editionSuffix;

						if (!empty($node->biblio_volume))      // volume
						{
							if (!empty($node->biblio_edition) && !preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition))
								$output .= ", ";

							$output .= "vol. " . $node->biblio_volume;
						}
					}
				}

				if ($row['type'] == "Software") // for software, add software label
				{
					$output .= ", computer software";
				}
				else // add series info
				{
					if (!empty($node->biblio_alternate_title) OR !empty($node->biblio_tertiary_title)) // if there's either a full or an abbreviated series title
					{
						if ((!empty($node->title) && !ereg("[?!.]$", $node->title)) || (!empty($node->biblio_secondary_authors) && !ereg("[ \r\n]*\(eds?\)", $authors)) || !empty($node->biblio_edition) || !empty($node->biblio_volume))
							$output .= ",";

						$output .= " ";

						if (!empty($node->biblio_tertiary_title))
							$output .= $node->biblio_tertiary_title;      // full series title

						// if there's no full series title, we'll use the abbreviated series title instead:
						elseif (!empty($node->biblio_alternate_title))
							$output .= $node->biblio_alternate_title;      // abbreviated series title

						if (!empty($node->biblio_volume)||!empty($node->biblio_issue))
							$output .= ", ";

						if (!empty($node->biblio_volume))      // series volume (I'm not really sure if -- for this cite style -- the series volume & issue should be rather omitted here)
							$output .= "vol. " . $node->biblio_volume;

						if (!empty($node->biblio_issue))      // series issue (see note for series volume)
						{
							if (!empty($node->biblio_volume))
								$output .= ", ";
	
							$output .= "no. " . $node->biblio_issue;
						}
					}
				}

				if (!empty($row['thesis']))      // thesis
				{
					$output .= ", " . $row['thesis'];
					$output .= ", " . $node->biblio_publisher;
					$output .= ", " . $node->biblio_place_published; // NOTE: should we omit the place of publication for theses?
				}
				else // not a thesis
				{
					if (!empty($node->biblio_publisher))      // publisher
						$output .= ", " . $node->biblio_publisher;

					if (!empty($node->biblio_place_published))      // place
					{
						if (!empty($node->biblio_publisher))
							$output .= ",";

						$output .= " " . $node->biblio_place_published;
					}
				}

				if ($row['online_publication'] == "yes" || $row['type'] == "Software") // this record refers to an online article, or a computer program/software
				{
					if (!empty($row['online_citation']))      // online_citation
					{
						if (!ereg("\. *$", $output))
							$output .= ".";

						$output .= " " . $row['online_citation'];
					}

					if (!empty($node->biblio_doi) || !empty($node->biblio_url))      // doi OR url
					{
						if (!ereg("\. *$", $output))
							$output .= ".";

						$today = date("j F Y");

						$output .= " Available from: "; // NOTE: some of the above mentioned resources use ", accessed 26 February 2004, from <http://...>." (or ", viewed on ...") instead

						if (!empty($node->biblio_doi))      // doi
							$uri = "http://dx.doi.org/" . $node->biblio_doi;
						else      // url
							$uri = $node->biblio_url;

						if ($encodeHTML)
							$output .= encodeHTML($markupPatternsArray["less-than"] . $uri . $markupPatternsArray["greater-than"]);
						else
							$output .= $markupPatternsArray["less-than"] . $uri . $markupPatternsArray["greater-than"];

						$output .= " [" . $today . "]";
					}
				}

			}

		// --- BEGIN POST-PROCESSING -----------------------------------------------------------------------------------------------------------
        if (!ereg("\. *$", $output)) // if the string doesn't end with a period
          $output .= ".";
			
		// do some further cleanup:
		$output = trim($output); // remove any preceding or trailing whitespace


		return $output;
	}

	// --- END CITATION STYLE ---
?>
