<?php
  // Original File:       ./cite/styles/cite_MLA.php
  // Original Repository: https://refbase.svn.sourceforge.net/svnroot/refbase/trunk/cite/styles/cite_MLA.php $
  // Original Author(s):  Richard Karnesky <mailto:karnesky@gmail.com> and
  //                      Matthias Steffens <mailto:refbase@extracts.de>
  //
  // Modified for use in biblio by Ron Jerome
  //
// $Id$
/**
 * Get the style information
 *
 * @return
 *   The name of the style
 */
function biblio_style_mla_info() {
  return array (
    'mla' => 'Modern Language Association (MLA)'
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
function biblio_style_mla($node, $base = 'biblio', $inline = false) {
  $author_options = array(
    'BetweenAuthorsDelimStandard' =>', ',         //4
    'BetweenAuthorsDelimLastAuthor' => ', and ',  //5
    'AuthorsInitialsDelimFirstAuthor' => ', ',     //7
    'AuthorsInitialsDelimStandard'=> ' ',         //8
    'betweenInitialsDelim' => '. ',               //9
    'initialsBeforeAuthorFirstAuthor' => false,   //10
    'initialsBeforeAuthorStandard' => true,       //11
    'shortenGivenNames' => false,                 //12
    'numberOfAuthorsTriggeringEtAl' => 3,         //13
    'includeNumberOfAuthors' => 1,                //14
    'customStringAfterFirstAuthors' => ', et al.',//15
    'encodeHTML' => true
  );
  $authors = theme('biblio_authors', $node->biblio_contributors[1], 'mla', 1, $inline);
  if (empty($authors)) $authors = theme('biblio_authors', $node->biblio_contributors[5], 'mla', 5, $inline);  // if no authors substitute corp author if available
  if (empty($authors)) $authors = '[' . t('Anonymous') . ']';  // use anonymous if we still have nothing.
  $output .= '<span class="biblio-authors">' . $authors . "</span>.&nbsp; \n";

  switch ($node->biblio_type) {
    case 102:
    case 105:
    case 106:
      if(!empty($authors)) {
        if (!ereg("\. *$", $author))
        $output .= $author . ".";
        else
        $output .= $node->title;
      }

      if (!empty($node->title))      // title
      {
        if (!empty($author)) $output .= " ";
        $output .= '"' . $node->title;
        if (!ereg("[?!.]$", $node->title)) $output .= ".";
        $output .= '"';
      }

      // From here on we'll assume that at least either the 'author' or the 'title' field did contain some contents
      // if this is not the case, the output string will begin with a space. However, any preceding/trailing whitespace will be removed at the cleanup stage (see below)

      if (!empty($node->biblio_alternate_title)) {      // abbreviated journal name
        $output .= " " . $markupPatternsArray["italic-prefix"] . $node->biblio_alternate_title . $markupPatternsArray["italic-suffix"];

        // if there's no abbreviated journal name, we'll use the full journal name
      }
      elseif (!empty($node->biblio_secondary_title)) {      // publication (= journal) name
        $output .= " " . $markupPatternsArray["italic-prefix"] . $node->biblio_secondary_title . $markupPatternsArray["italic-suffix"];
      }
      if (!empty($node->biblio_volume)) {     // volume
        if (!empty($node->biblio_alternate_title) || !empty($node->biblio_secondary_title)) {
          $output .= ".";
        }
        $output .= " " . $node->biblio_volume;
      }
      if (!empty($node->biblio_issue)) {     // issue
        $output .=  "." . $node->biblio_issue;
      }
      if (!empty($node->biblio_year))      // year
      {
        $output .= " (".$node->biblio_year . ")";
      }
      // FIXME do something with the online pubs section
      if ($node->online_publication == "yes") // this record refers to an online article
      {
        // instead of any pages info (which normally doesn't exist for online publications) we append
        // an optional string (given in 'online_citation') plus the current date and the DOI (or URL):

        $today = date("j M. Y");

        if (!empty($node->online_citation))      // online_citation
        {
          if (!empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->abbrev_journal) || !empty($node->publication)) // only add ":" if either volume, issue, abbrev_journal or publication isn't empty
          $output .= ":";

          $output .= " " . $node->online_citation;
        }

        if (!empty($node->doi))      // doi
        {
          if (!empty($node->online_citation) OR (empty($node->online_citation) AND (!empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->abbrev_journal) || !empty($node->publication)))) // only add "." if online_citation isn't empty, or else if either volume, issue, abbrev_journal or publication isn't empty
          $output .= ".";

          if ($encodeHTML)
          $output .= " " . $today . encodeHTML(" <http://dx.doi.org/" . $node->doi . ">");
          else
          $output .= " " . $today . " <http://dx.doi.org/" . $node->doi . ">";
        }
        elseif (!empty($node->url))      // url
        {
          if (!empty($node->online_citation) OR (empty($node->online_citation) AND (!empty($node->biblio_volume) || !empty($node->biblio_issue) || !empty($node->abbrev_journal) || !empty($node->publication)))) // only add "." if online_citation isn't empty, or else if either volume, issue, abbrev_journal or publication isn't empty
          $output .= ".";

          if ($encodeHTML)
          $output .= " " . $today . encodeHTML(" <" . $node->url . ">");
          else
          $output .= " " . $today . " <" . $node->url . ">";
        }

      }
      else // $node->online_publication == "no" -> this record refers to a printed article, so we append any pages info instead:
      {
        if (!empty($node->pages))      // pages
        {
          if (!empty($node->biblio_year) ||
          !empty($node->biblio_volume) ||
          !empty($node->biblio_issue) ||
          !empty($$node->biblio_alternate_title) ||
          !empty($node->biblio_secondary_title)) {  // only add ": " if either volume, issue, abbrev_journal or publication isn't empty
            $output .= ": ";
          }
          // TODO: FIXME $output .= formatPageInfo($node->pages, $markupPatternsArray["endash"]); // function 'formatPageInfo()' is defined in 'cite.inc.php'
        }
      }

      if (!ereg("\. *$", $output))
      $output .= ".";

      break;
    case 101:
    case 103:
      $editor_options = array(
        'BetweenAuthorsDelimStandard' =>', ',         //4
        'BetweenAuthorsDelimLastAuthor' => ', and ',  //5
        'AuthorsInitialsDelimFirstAuthor' => ' ',     //7
        'AuthorsInitialsDelimStandard'=> ' ',         //8
        'betweenInitialsDelim' => '. ',               //9
        'initialsBeforeAuthorFirstAuthor' => true,   //10
        'initialsBeforeAuthorStandard' => true,       //11
        'shortenGivenNames' => false,                 //12
        'numberOfAuthorsTriggeringEtAl' => 3,         //13
        'includeNumberOfAuthors' => 1,                //14
        'customStringAfterFirstAuthors' => ', et al.',//15
        'encodeHTML' => true
      );
      if(!empty($authors)) {
        if (!ereg("\. *$", $authors)) {
          $record .= $authors . ".";
        }
        else {
          $record .= $authors;
        }

        if (!empty($node->title))      // title
        {
          if (!empty($node->author))
          $record .= " ";

          $record .= '"' . $node->title;
          if (!ereg("[?!.]$", $node->title))
          $record .= ".";
          $record .= '"';
        }

        $publication = ereg_replace("[ \r\n]*\(Eds?:[^\)\r\n]*\)", "", $node->publication);
        if (!empty($publication))      // publication
        $record .= " " . $markupPatternsArray["italic-prefix"] . $publication . $markupPatternsArray["italic-suffix"];


        // From here on we'll assume that at least either the 'author' or the 'title' field did contain some contents
        // if this is not the case, the output string will begin with a space. However, any preceding/trailing whitespace will be removed at the cleanup stage (see below)

        if (!empty($node->contributors[2]))      // editor
        {
          $editor = reArrangeAuthorContents($node->editor); // 1.

          if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $record))
          $record .= ".";

          if (count($node->contributors[2]) > 1) { // there are at least two editors (separated by ';')
            $record .= " Eds. " . $editor;
          }
          else { // there's only one editor (or the editor field is malformed with multiple editors but missing ';' separator[s])
            $record .= " Ed. " . $editor;
          }
        }

        if (!empty($node->biblio_edition) && !preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $node->biblio_edition))      // edition
        {
          if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $record))
          $record .= ".";

          if (preg_match("/^\d{1,3}$/", $node->biblio_edition)) // if the edition field contains a number of up to three digits, we assume it's an edition number (such as "2nd ed.")
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

          if (preg_match("/^(Rev\.?|Revised)( ed\.?| edition)?$/i", $node->biblio_edition))
          $node->biblio_edition = "Rev.";

          elseif (preg_match("/^(Abr\.?|Abridged)( ed\.?| edition)?$/i", $node->biblio_edition))
          $node->biblio_edition = "Abr.";

          if (!preg_match("/( ed\.?| edition)$/i", $node->biblio_edition))
          $editionSuffix .= " ed.";

          $record .= " " . $node->biblio_edition . $editionSuffix;
        }

        if (!empty($node->biblio_volume))      // volume
        {
          if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $record))
          $record .= ".";

          $record .= " Vol. " . $node->biblio_volume;
        }

        if (!empty($node->abbrev_series_title) OR !empty($node->series_title)) // if there's either a full or an abbreviated series title
        {
          if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $record))
          $record .= ".";

          $record .= " ";

          if (!empty($node->abbrev_series_title))
          $record .= $node->abbrev_series_title;      // abbreviated series title

          // if there's no abbreviated series title, we'll use the full series title instead:
          elseif (!empty($node->series_title))
          $record .= $node->series_title;      // full series title

          if (!empty($node->series_volume)||!empty($node->series_issue))
          $record .= ", ";

          if (!empty($node->series_volume))      // series volume
          $record .= $node->series_volume;

          if (!empty($node->series_issue))      // series issue (I'm not really sure if -- for this cite style -- the series issue should be rather omitted here)
          $record .= "." . $node->series_issue; // is it correct to format series issues similar to journal article issues?
        }

        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $record))
        $record .= ".";

        if (!empty($node->place))      // place
        $record .= " " . $node->place;

        if (!empty($node->publisher))      // publisher
        {
          if (!empty($node->place))
          $record .= ":";

          $record .= " " . $node->publisher;
        }



        if (!empty($node->biblio_year))      // year
        {
          $record .= ", " . $node->biblio_year;
        }

        if (!empty($node->pages))      // pages
        $record .= ". " . formatPageInfo($node->pages, $markupPatternsArray["endash"]); // function 'formatPageInfo()' is defined in 'cite.inc.php'

        if (!ereg("\. *$", $record))
        $record .= ".";
      }

      break;

    default :
      if (!empty($row['author']))      // author
      {
        $author = ereg_replace("[ \r\n]*\(eds?\)", "", $row['author']);

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
        $author = reArrangeAuthorContents($author, // 1.$authorContents
        true, // 2.$familyNameFirst
                                              " *; *", // 3.$oldBetweenAuthorsDelim
                                              ", ", // 4.$newBetweenAuthorsDelimStandard
                                              ", and ", // 5.$newBetweenAuthorsDelimLastAuthor
                                              " *, *", // 6.$oldAuthorsInitialsDelim
                                              ", ", // 7.$newAuthorsInitialsDelimFirstAuthor
                                              " ", // 8.$newAuthorsInitialsDelimStandard
                                              ". ", // 9.$betweenInitialsDelim
        false, // 10.$initialsBeforeAuthorFirstAuthor
        true, // 11.$initialsBeforeAuthorStandard
        false, // 12.$shortenGivenNames
                                              "3", // 13.$numberOfAuthorsTriggeringEtAl
                                              "1", // 14.$includeNumberOfAuthors
                                              ", et al.", // 15.$customStringAfterFirstAuthors
        $encodeHTML); // 16.

        // if the author is actually the editor of the resource we'll append ', ed' (or ', eds') to the author string:
        // [to distinguish editors from authors in the 'author' field, the 'modify.php' script does append ' (ed)' or ' (eds)' if appropriate,
        //  so we're just checking for these identifier strings here. Alternatively, we could check whether the editor field matches the author field]
        if (ereg("[ \r\n]*\(ed\)", $row['author'])) // single editor
        $author = $author . ", " . "ed";
        elseif (ereg("[ \r\n]*\(eds\)", $row['author'])) // multiple editors
        $author = $author . ", " . "eds";

        if (!ereg("\. *$", $author))
        $record .= $author . ".";
        else
        $record .= $author;
      }

      if (!empty($row['title']))      // title
      {
        if (!empty($row['author']))
        $record .= " ";

        if (!empty($row['thesis']))      // thesis
        {
          $record .= '"' . $row['title'];
          if (!ereg("[?!.]$", $row['title']))
          $record .= ".";
          $record .= '"';
        }
        else // not a thesis
        $record .= $markupPatternsArray["italic-prefix"] . $row['title'] . $markupPatternsArray["italic-suffix"];
      }

      if (!empty($row['editor']) && !ereg("[ \r\n]*\(eds?\)", $row['author']))      // editor (if different from author, see note above regarding the check for ' (ed)' or ' (eds)')
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
        $editor = reArrangeAuthorContents($row['editor'], // 1.
        true, // 2.
                                              " *; *", // 3.
                                              ", ", // 4.
                                              ", and ", // 5.
                                              " *, *", // 6.
                                              " ", // 7.
                                              " ", // 8.
                                              ". ", // 9.
        true, // 10.
        true, // 11.
        false, // 12.
                                              "3", // 13.
                                              "1", // 14.
                                              ", et al.", // 15.
        $encodeHTML); // 16.

        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $record))
        $record .= ".";

        if (ereg("^[^;\r\n]+(;[^;\r\n]+)+$", $row['editor'])) // there are at least two editors (separated by ';')
        $record .= " Eds. " . $editor;
        else // there's only one editor (or the editor field is malformed with multiple editors but missing ';' separator[s])
        $record .= " Ed. " . $editor;
      }

      if (!empty($row['edition']) && !preg_match("/^(1|1st|first|one)( ed\.?| edition)?$/i", $row['edition']))      // edition
      {
        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $record))
        $record .= ".";

        if (preg_match("/^\d{1,3}$/", $row['edition'])) // if the edition field contains a number of up to three digits, we assume it's an edition number (such as "2nd ed.")
        {
          if ($row['edition'] == "2")
          $editionSuffix = "nd";
          elseif ($row['edition'] == "3")
          $editionSuffix = "rd";
          else
          $editionSuffix = "th";
        }
        else
        $editionSuffix = "";

        if (preg_match("/^(Rev\.?|Revised)( ed\.?| edition)?$/i", $row['edition']))
        $row['edition'] = "Rev.";

        elseif (preg_match("/^(Abr\.?|Abridged)( ed\.?| edition)?$/i", $row['edition']))
        $row['edition'] = "Abr.";

        if (!preg_match("/( ed\.?| edition)$/i", $row['edition']))
        $editionSuffix .= " ed.";

        $record .= " " . $row['edition'] . $editionSuffix;
      }

      if (!empty($row['volume']))      // volume
      {
        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $record))
        $record .= ".";

        $record .= " Vol. " . $row['volume'];
      }

      if (!empty($row['abbrev_series_title']) OR !empty($row['series_title'])) // if there's either a full or an abbreviated series title
      {
        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $record))
        $record .= ".";

        $record .= " ";

        if (!empty($row['abbrev_series_title']))
        $record .= $row['abbrev_series_title'];      // abbreviated series title

        // if there's no abbreviated series title, we'll use the full series title instead:
        elseif (!empty($row['series_title']))
        $record .= $row['series_title'];      // full series title

        if (!empty($row['series_volume'])||!empty($row['series_issue']))
        $record .= ", ";

        if (!empty($row['series_volume']))      // series volume
        $record .= $row['series_volume'];

        if (!empty($row['series_issue']))      // series issue (I'm not really sure if -- for this cite style -- the series issue should be rather omitted here)
        $record .= "." . $row['series_issue']; // is it correct to format series issues similar to journal article issues?
      }

      if (!empty($row['thesis']))      // thesis (unpublished dissertation)
      {
        // TODO: a published dissertation needs to be formatted differently!
        //       see e.g. example at: <http://web.csustan.edu/english/reuben/pal/append/AXI.HTML>

        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $record))
        $record .= ".";

        // TODO: I've also seen MLA examples that separate thesis name, name of institution and year by dots. ?:-|
        //       Also, do we need to use the abbreviation "Diss." instead of "Ph.D. thesis"? What about other thesis types then?
        //       see e.g. <http://www.english.uiuc.edu/cws/wworkshop/writer_resources/citation_styles/mla/unpublished_diss.htm>
        $record .= " " . $row['thesis'];
        $record .= ", " . $row['publisher'];
      }
      else // not a thesis
      {
        if (!ereg("[?!.][ \"" . $markupPatternsArray["italic-suffix"] . "]*$", $record))
        $record .= ".";

        if (!empty($row['place']))      // place
        $record .= " " . $row['place'];

        if (!empty($row['publisher']))      // publisher
        {
          if (!empty($row['place']))
          $record .= ":";

          $record .= " " . $row['publisher'];
        }
      }

      if (!empty($row['year']))      // year
      $record .= ", ".$row['year'];

      if ($row['online_publication'] == "yes") // this record refers to an online article
      {
        $today = date("j M. Y");

        if (!empty($row['online_citation']))      // online_citation
        {
          if (!ereg("\. *$", $record))
          $record .= ".";

          $record .= " " . $row['online_citation'];
        }

        if (!empty($row['doi']))      // doi
        {
          if (!ereg("\. *$", $record))
          $record .= ".";

          if ($encodeHTML)
          $record .= " " . $today . encodeHTML(" <http://dx.doi.org/" . $row['doi'] . ">");
          else
          $record .= " " . $today . " <http://dx.doi.org/" . $row['doi'] . ">";
        }
        elseif (!empty($row['url']))      // url
        {
          if (!ereg("\. *$", $record))
          $record .= ".";

          if ($encodeHTML)
          $record .= " " . $today . encodeHTML(" <" . $row['url'] . ">");
          else
          $record .= " " . $today . " <" . $row['url'] . ">";
        }

      }

      if (!ereg("\. *$", $record))
      $record .= ".";

      //      if (isset ($node->biblio_year)) {
      //        $output .= check_plain($node->biblio_year) . ".&nbsp;&nbsp;";
      //      }
      //      $output .= '<span class="biblio-title">';
      //      $output .= $inline ? l("$node->title", "$base/viewinline/$node->nid") : l("$node->title", "node/$node->nid");
      //      $output .= (strpos($node->title, '?'))? " </span>" : ". </span>";  // if the title ends in a question mark, don't put a period after it.
      //      $output .= (!empty ($node->biblio_secondary_title)) ? check_plain($node->biblio_secondary_title) . '. ' : '';
      //      $output .= (!empty ($node->biblio_volume))          ? check_plain($node->biblio_volume) : '';
      //      $output .= (!empty ($node->biblio_issue))           ? '(' . check_plain($node->biblio_issue) . ')' : '';
      //      $output .= (!empty ($node->biblio_pages))           ? ':' . str_replace(" ", "", check_plain($node->biblio_pages)) . '.' : '';
      break; // generic
  }
  /*  if ($node->biblio_date) $output .= ', '. check_plain($node->biblio_date);
   if ($node->biblio_number) $output .= ', Number '. check_plain($node->biblio_number);

   if ($node->biblio_place_published) $output .= ', '. check_plain($node->biblio_place_published);
   */
  return $output;
}
function _mla_format_author($author) {
  $format = $author['prefix'] . ' ' . $author['lastname'] . ' ';
  $format .= !empty ($author['firstname']) ? ' ' . drupal_substr($author['firstname'], 0, 1) : '';
  $format .= !empty ($author['initials']) ? str_replace(' ', '', $author['initials']) : '';
  return $format;
}