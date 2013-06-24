<?php

/**
 * @file
 * BibTeX style.
 */

class BiblioStyleBibtex extends BiblioStyleBase {

  public function render($options = array(), $langcode = NULL) {
    $biblio = $this->biblio;
    $wrapper = entity_metadata_wrapper('biblio', $biblio);

    $output = '';
    $journal = $series = $booktitle = $school = $organization = $institution = NULL;
    $type = $this->typeMap();

    switch ($type) {
      case 100:
        $series = $wrapper->biblio_secondary_title->value();
        $organization = $wrapper->biblio_publisher->value();
        break;

      case 101:
      case 103:
        $booktitle = $wrapper->biblio_secondary_title->value();
        $organization = $wrapper->biblio_publisher->value();
        $series = $wrapper->biblio_tertiary_title->value();
        break;

      case 108:
        $school = $wrapper->biblio_publisher->value();
        $biblio->biblio_publisher = NULL;
        if (strpos($wrapper->biblio_type_of_work->value(), 'masters') === TRUE) {
          $type = 'mastersthesis';
        }
        break;

      case 109:
        $institution  = $wrapper->biblio_publisher->value();
        $biblio->biblio_publisher = NULL;
        break;

      case 102:
      default:
        $journal = $wrapper->biblio_secondary_title->value();
        break;
    }

    $output .= '@' . $type . ' {';
    $output .= isset($wrapper->biblio_citekey) ? $wrapper->biblio_citekey->value()  : '';
    $output .= $this->formatEntry('title');
    $output .= $this->formatEntry('journal', $journal);
    $output .= $this->formatEntry('booktitle', $booktitle);
    $output .= $this->formatEntry('series', $series);
    $output .= $this->formatEntry('volume');
    $output .= $this->formatEntry('number');
    $output .= $this->formatEntry('year');
    $output .= $this->formatEntry('note');
    $output .= $this->formatEntry('month');
    $output .= $this->formatEntry('pages');
    $output .= $this->formatEntry('publisher');
    $output .= $this->formatEntry('school', $school);
    $output .= $this->formatEntry('organization', $organization);
    $output .= $this->formatEntry('institution', $institution);
    $output .= $this->formatEntry('type');
    $output .= $this->formatEntry('edition');
    $output .= $this->formatEntry('chapter');
    $output .= $this->formatEntry('address');
    $output .= $this->formatEntry('abstract');

    $output .= $this->formatEntry('keywords');

    $output .= $this->formatEntry('isbn');
    $output .= $this->formatEntry('issn');
    $output .= $this->formatEntry('doi');
    $output .= $this->formatEntry('url');

    $output .= $this->formatEntry('attachments');

    /*

    $a = $e = $authors = array();
    if ($authors = biblio_get_contributor_category($biblio->biblio_contributors, 1)) {
      foreach ($authors as $author) {
        $a[] = trim($author['name']);
      }
    }
    if ($authors = biblio_get_contributor_category($biblio->biblio_contributors, 2)) {
      foreach ($authors as $author) {
        $e[] = trim($author['name']);
      }
    }
    $a = implode(' and ', $a);
    $e = implode(' and ', $e);

    if (!empty($a)) {
      $output .= $this->formatEntry('author', $a);
    }

    if (!empty($e)) {
      $output .= $this->formatEntry('editor', $e);
    }

    */

    $output .= "\n}\n";

    // Convert any special characters to the latex equivalents.
    $converter = new PARSEENTRIES();
    $output = $converter->searchReplaceText($this->getTranstab(), $output, FALSE);

    return $output;
  }

  /**
   * Format an entry.
   *
   * @param $key
   *   The BibTeX key name.
   * @param $value
   *   Optional; The value to format. If empty, try to get it from the Biblio
   *   entity. Defaults to NULL.
   * @return string
   */
  private function formatEntry($key, $value = NULL) {
    if (empty($value)) {
      $map = $this->getMapping();
      $map = $map['field'];

      if (empty($map[$key])) {
        return;
      }

      // @todo: Cache the wrapper.
      $wrapper = entity_metadata_wrapper('biblio', $this->biblio);

      $property_name = $map[$key]['property'];
      if (!isset($wrapper->{$property_name})) {
        return;
      }

      $method = $map[$key]['method'];

      if (!$value = $this->{$method}($wrapper, $property_name)) {
        return;
      }
    }

    return ",\n\t$key = {" . $value . "}";
  }

  /**
   * Generic format entry.
   *
   * @param $wrapper
   * @param $property_name
   */
  private function formatEntryGeneric($wrapper, $property_name) {
    return $wrapper->{$property_name}->value();
  }

  /**
   * Taxonomy term format entry.
   *
   * @param $wrapper
   * @param $property_name
   */
  private function formatEntryTaxonomyTerms($wrapper, $property_name) {
    if (!$terms = $wrapper->{$property_name}->value()) {
      return;
    }

    $terms = is_array($terms) ? $terms : array($terms);
    $values = array();
    foreach ($terms as $term) {
      $values[] = $term->name;
    }

    return implode(', ', $values);
  }

  /**
   * File format entry.
   *
   * @param $wrapper
   * @param $property_name
   */
  private function formatEntryFiles($wrapper, $property_name) {
    if (!user_access('view uploaded files')) {
      return;
    }

    if (!$files =  $wrapper->{$property_name}->value()) {
      return;
    }

    $url = array();
    $files = !isset($files['fid']) ? $files : array($files);
    foreach ($files as $file) {
      $url[] = file_create_url($file['uri']);
    }

    return implode(' , ', $url);
  }



  /**
   * Get the BibTeX type from the Biblio entity.
   *
   * @return
   */
  private function typeMap() {
    $type = $this->biblio->type;
    $map = $this->getMapping();
    return !empty($map['type'][$type]) ? $map['type'][$type] : 'article';
  }


  /**
   * Mapping of Biblio and BibTeX.
   *
   * - type: Array with the Biblio type as key, and the BibTeX type as the
   *   value.
   * - field: Array with field mapping, keyed by BibTeX name, and the Biblio
   *   field as the value.
   */
  public function getMapping() {
    $return  = array(
      'type' => array(
        'article'       => 102,
        'book'          => 100,
        'booklet'       => 129,
        'conference'    => 103,
        'inbook'        => 101,
        'incollection'  => 101,
        'inproceedings' => 103,
        'manual'        => 129,
        'mastersthesis' => 108,
        'misc'          => 129,
        'phdthesis'     => 108,
        'proceedings'   => 104,
        'techreport'    => 129,
        'unpublished'   => 124,
      ),
      'field' => array(
        'title' => array('property' => 'title'),
        'volume' => array('property' => 'biblio_volume'),
        'number' => array('property' => 'biblio_number'),
        'year' => array('property' => 'biblio_year'),
        'note' => array('property' => 'biblio_notes'),
        'month' => array('property' => 'biblio_date'),
        'pages' => array('property' => 'biblio_pages'),
        'publisher' => array('property' => 'biblio_publisher'),
        'type' => array('property' => 'biblio_type_of_work'),
        'edition' => array('property' => 'biblio_edition'),
        'chapter' => array('property' => 'biblio_section'),
        'address' => array('property' => 'biblio_place_published'),
        'abstract' => array('property' => 'biblio_abstract'),
        'isbn' => array('property' => 'biblio_isbn'),
        'issn' => array('property' => 'biblio_issn'),
        'doi' => array('property' => 'biblio_doi'),
        // @todo: Is this the Biblio URL?
        'url' => array('property' => 'biblio_url'),

        'keywords' => array('property' => 'biblio_keywords', 'method' => 'formatEntryTaxonomyTerms'),

        // @todo: Use bilbio_file instead.
        'attachments' => array('property' => 'biblio_image', 'method' => 'formatEntryFiles'),
      ),
    );

    // Assign default method to format entry.
    foreach ($return as $key => $value) {
      if (empty($value['method'])) {
        $return[$key]['method'] = 'formatEntryGeneric';
      }
    }

    return $return;
  }


  /**
   * This is a translation table for best-effort conversion from Unicode to
   * LaTeX/BibTeX entities. It contains a comprehensive list of substitution
   * strings for Unicode characters, which can be used with the 'T1' font
   * encoding.
   *
   * Uses commands from the 'textcomp' package. Characters that can't be matched
   * are converted to ASCII equivalents.
   *
   * Adopted from 'transtab' by Markus Kuhn (transtab.utf v1.8 2000-10-12
   * 11:01:28+01 mgk25 Exp);
   *
   * see <http://www.cl.cam.ac.uk/~mgk25/unicode.html> for more info about
   * Unicode and transtab.
   */
  private function getTranstab() {

    return array(
      "(?<!\\\\)#" => '$\\#$',
      "(?<!\\\\)%" => "\\%",
      "(?<!\\\\)&" => "\\&",
      "(?<!\\\\)'" => "{\\textquoteright}",
      "(?<!\\\\)`" => "{\\textquoteleft}",
      " " => "~",
      "¡" => "{\\textexclamdown}",
      "¢" => "{\\textcent}",
      "£" => "{\\textsterling}",
      "¥" => "{\\textyen}",
      "¦" => "{\\textbrokenbar}",
      "§" => "{\\textsection}",
      "¨" => "{\\textasciidieresis}",
      "©" => "{\\textcopyright}",
      "ª" => "{\\textordfeminine}",
      "«" => "{\\guillemotleft}",
      "¬" => "{\\textlnot}",
      "­" => "-",
      "®" => "{\\textregistered}",
      "¯" => "{\\textasciimacron}",
      "°" => "{\\textdegree}",
      "±" => "{\\textpm}",
      "²" => "{\\texttwosuperior}",
      "³" => "{\\textthreesuperior}",
      "´" => "{\\textasciiacute}",
      "µ" => "{\\textmu}",
      "¶" => "{\\textparagraph}",
      "·" => "{\\textperiodcentered}",
      "¸" => "{\\c\\ }",
      "¹" => "{\\textonesuperior}",
      "º" => "{\\textordmasculine}",
      "»" => "{\\guillemotright}",
      "¼" => "{\\textonequarter}",
      "½" => "{\\textonehalf}",
      "¾" => "{\\textthreequarters}",
      "¿" => "{\\textquestiondown}",
      "À" => "{\\`A}",
      "Á" => "{\\'A}",
      "Â" => "{\\^A}",
      "Ã" => "{\\~A}",
      "Ä" => "{\\\"A}",
      "Å" => "{\\r A}",
      "Æ" => "{\\AE}",
      "Ç" => "{\\c C}",
      "È" => "{\\`E}",
      "É" => "{\\'E}",
      "Ê" => "{\\^E}",
      "Ë" => "{\\\"E}",
      "Ì" => "{\\`I}",
      "Í" => "{\\'I}",
      "Î" => "{\\^I}",
      "Ï" => "{\\\"I}",
      "Ð" => "{\\DH}",
      "Ñ" => "{\\~N}",
      "Ò" => "{\\`O}",
      "Ó" => "{\\'O}",
      "Ô" => "{\\^O}",
      "Õ" => "{\\~O}",
      "Ö" => "{\\\"O}",
      "×" => "{\\texttimes}",
      "Ø" => "{\\O}",
      "Ù" => "{\\`U}",
      "Ú" => "{\\'U}",
      "Û" => "{\\^U}",
      "Ü" => "{\\\"U}",
      "Ý" => "{\\'Y}",
      "Þ" => "{\\TH}",
      "ß" => "{\\ss}",
      "à" => "{\\`a}",
      "á" => "{\\'a}",
      "â" => "{\\^a}",
      "ã" => "{\\~a}",
      "ä" => "{\\\"a}",
      "å" => "{\\r a}",
      "æ" => "{\\ae}",
      "ç" => "{\\c c}",
      "è" => "{\\`e}",
      "é" => "{\\'e}",
      "ê" => "{\\^e}",
      "ë" => "{\\\"e}",
      "ì" => "{\\`\\i}",
      "í" => "{\\'\\i}",
      "î" => "{\\^\\i}",
      "ï" => "{\\\"\\i}",
      "ð" => "{\\dh}",
      "ñ" => "{\\~n}",
      "ò" => "{\\`o}",
      "ó" => "{\\'o}",
      "ô" => "{\\^o}",
      "õ" => "{\\~o}",
      "ö" => "{\\\"o}",
      "÷" => "{\\textdiv}",
      "ø" => "{\\o}",
      "ù" => "{\\`u}",
      "ú" => "{\\'u}",
      "û" => "{\\^u}",
      "ü" => "{\\\"u}",
      "ý" => "{\\'y}",
      "þ" => "{\\th}",
      "ÿ" => "{\\\"y}",
      "Ā" => "A",
      "ā" => "{\\={a}}",
      "Ă" => "{\\u A}",
      "ă" => "{\\u a}",
      "Ą" => "{\\k A}",
      "ą" => "{\\k a}",
      "Ć" => "{\\'C}",
      "ć" => "{\\'c}",
      "Ĉ" => "Ch",
      "ĉ" => "ch",
      "Ċ" => "C",
      "ċ" => "c",
      "Č" => "{\\v C}",
      "č" => "{\\v c}",
      "Ď" => "{\\v D}",
      "ď" => "{\\v d}",
      "Đ" => "{\\DJ}",
      "đ" => "{\\dj}",
      "Ē" => "E",
      "ē" => "e",
      "Ĕ" => "E",
      "ĕ" => "e",
      "Ė" => "E",
      "ė" => "e",
      "Ę" => "{\\k E}",
      "ę" => "{\\k e}",
      "Ě" => "{\\v E}",
      "ě" => "{\\v e}",
      "Ĝ" => "Gh",
      "ĝ" => "gh",
      "Ğ" => "{\\u G}",
      "ğ" => "{\\u g}",
      "Ġ" => "G",
      "ġ" => "g",
      "Ģ" => "G",
      "ģ" => "g",
      "Ĥ" => "Hh",
      "ĥ" => "hh",
      "Ħ" => "H",
      "ħ" => "h",
      "Ĩ" => "I",
      "ĩ" => "i",
      "Ī" => "I",
      "ī" => "i",
      "Ĭ" => "I",
      "ĭ" => "i",
      "Į" => "I",
      "į" => "i",
      "İ" => "{\\.I}",
      "ı" => "{\\i}",
      "Ĳ" => "IJ",
      "ĳ" => "ij",
      "Ĵ" => "Jh",
      "ĵ" => "jh",
      "Ķ" => "K",
      "ķ" => "k",
      "ĸ" => "k",
      "Ĺ" => "{\\'L}",
      "ĺ" => "{\\'l}",
      "Ļ" => "L",
      "ļ" => "l",
      "Ľ" => "{\\v L}",
      "ľ" => "{\\v l}",
      "Ŀ" => "L·",
      "ŀ" => "l·",
      "Ł" => "{\\L}",
      "ł" => "{\\l}",
      "Ń" => "{\\'N}",
      "ń" => "{\\'n}",
      "Ņ" => "N",
      "ņ" => "n",
      "Ň" => "{\\v N}",
      "ň" => "{\\v n}",
      "ŉ" => "'n",
      "Ŋ" => "{\\NG}",
      "ŋ" => "{\\ng}",
      "Ō" => "O",
      "ō" => "o",
      "Ŏ" => "O",
      "ŏ" => "o",
      "Ő" => "{\\H O}",
      "ő" => "{\\H o}",
      "Œ" => "{\\OE}",
      "œ" => "{\\oe}",
      "Ŕ" => "{\\'R}",
      "ŕ" => "{\\'r}",
      "Ŗ" => "R",
      "ŗ" => "r",
      "Ř" => "{\\v R}",
      "ř" => "{\\v r}",
      "Ś" => "{\\'S}",
      "ś" => "{\\'s}",
      "Ŝ" => "Sh",
      "ŝ" => "sh",
      "Ş" => "{\\c S}",
      "ş" => "{\\c s}",
      "Š" => "{\\v S}",
      "š" => "{\\v s}",
      "Ţ" => "{\\c T}",
      "ţ" => "{\\c t}",
      "Ť" => "{\\v T}",
      "ť" => "{\\v t}",
      "Ŧ" => "T",
      "ŧ" => "t",
      "Ũ" => "U",
      "ũ" => "u",
      "Ū" => "U",
      "ū" => "u",
      "Ŭ" => "U",
      "ŭ" => "u",
      "Ů" => "{\\r U}",
      "ů" => "{\\r u}",
      "Ű" => "{\\H U}",
      "ű" => "{\\H u}",
      "Ų" => "U",
      "ų" => "u",
      "Ŵ" => "W",
      "ŵ" => "w",
      "Ŷ" => "Y",
      "ŷ" => "y",
      "Ÿ" => "{\\\"Y}",
      "Ź" => "{\\'Z}",
      "ź" => "{\\'z}",
      "Ż" => "{\\.Z}",
      "ż" => "{\\.z}",
      "Ž" => "{\\v Z}",
      "ž" => "{\\v z}",
      "ſ" => "s",
      "ƒ" => "{\\textflorin}",
      "Ș" => "S",
      "ș" => "s",
      "Ț" => "T",
      "ț" => "t",
      "ʹ" => "'",
      "ʻ" => "'",
      "ʼ" => "'",
      "ʽ" => "'",
      "ˆ" => "{\\textasciicircum}",
      "ˈ" => "'",
      "ˉ" => "-",
      "ˌ" => ",",
      "ː" => ":",
      "˚" => "o",
      "˜" => "\\~{}",
      "˝" => "{\\textacutedbl}",
      "ʹ" => "'",
      "͵" => ",",
      ";" => ";",
      "Ḃ" => "B",
      "ḃ" => "b",
      "Ḋ" => "D",
      "ḋ" => "d",
      "Ḟ" => "F",
      "ḟ" => "f",
      "Ṁ" => "M",
      "ṁ" => "m",
      "Ṗ" => "P",
      "ṗ" => "p",
      "Ṡ" => "S",
      "ṡ" => "s",
      "Ṫ" => "T",
      "ṫ" => "t",
      "Ẁ" => "W",
      "ẁ" => "w",
      "Ẃ" => "W",
      "ẃ" => "w",
      "Ẅ" => "W",
      "ẅ" => "w",
      "Ỳ" => "Y",
      "ỳ" => "y",
      " " => " ",
      " " => "  ",
      " " => " ",
      " " => "  ",
      " " => " ",
      " " => " ",
      " " => " ",
      " " => " ",
      " " => " ",
      " " => " ",
      "‐" => "-",
      "‑" => "-",
      "‒" => "-",
      "–" => "{\\textendash}",
      "—" => "{\\textemdash}",
      "―" => "--",
      "‖" => "{\\textbardbl}",
      "‗" => "{\\textunderscore}",
      "‘" => "{\\textquoteleft}",
      "’" => "{\\textquoteright}",
      "‚" => "{\\quotesinglbase}",
      "‛" => "'",
      "“" => "{\\textquotedblleft}",
      "”" => "{\\textquotedblright}",
      "„" => "{\\quotedblbase}",
      "‟" => "\"",
      "†" => "{\\textdagger}",
      "‡" => "{\\textdaggerdbl}",
      "•" => "{\\textbullet}",
      "‣" => ">",
      "․" => ".",
      "‥" => "..",
      "…" => "{\\textellipsis}",
      "‧" => "-",
      " " => " ",
      "‰" => "{\\textperthousand}",
      "′" => "'",
      "″" => "\"",
      "‴" => "'''",
      "‵" => "`",
      "‶" => "``",
      "‷" => "```",
      "‹" => "{\\guilsinglleft}",
      "›" => "{\\guilsinglright}",
      "‼" => "!!",
      "‾" => "-",
      "⁃" => "-",
      "⁄" => "{\\textfractionsolidus}",
      "⁈" => "?!",
      "⁉" => "!?",
      "⁊" => "7",
      "⁰" => '$^{0}$',
      "⁴" => '$^{4}$',
      "⁵" => '$^{5}$',
      "⁶" => '$^{6}$',
      "⁷" => '$^{7}$',
      "⁸" => '$^{8}$',
      "⁹" => '$^{9}$',
      "⁺" => '$^{+}$',
      "⁻" => '$^{-}$',
      "⁼" => '$^{=}$',
      "⁽" => '$^{(}$',
      "⁾" => '$^{)}$',
      "ⁿ" => '$^{n}$',
      "₀" => '$_{0}$',
      "₁" => '$_{1}$',
      "₂" => '$_{2}$',
      "₃" => '$_{3}$',
      "₄" => '$_{4}$',
      "₅" => '$_{5}$',
      "₆" => '$_{6}$',
      "₇" => '$_{7}$',
      "₈" => '$_{8}$',
      "₉" => '$_{9}$',
      "₊" => '$_{+}$',
      "₋" => '$_{-}$',
      "₌" => '$_{=}$',
      "₍" => '$_{(}$',
      "₎" => '$_{)}$',
      "€" => "{\\texteuro}",
      "℀" => "a/c",
      "℁" => "a/s",
      "℃" => "{\\textcelsius}",
      "℅" => "c/o",
      "℆" => "c/u",
      "℉" => "F",
      "ℓ" => "l",
      "№" => "{\\textnumero}",
      "℗" => "{\\textcircledP}",
      "℠" => "{\\textservicemark}",
      "℡" => "TEL",
      "™" => "{\\texttrademark}",
      "Ω" => "{\\textohm}",
      "K" => "K",
      "Å" => "A",
      "℮" => "{\\textestimated}",
      "⅓" => " 1/3",
      "⅔" => " 2/3",
      "⅕" => " 1/5",
      "⅖" => " 2/5",
      "⅗" => " 3/5",
      "⅘" => " 4/5",
      "⅙" => " 1/6",
      "⅚" => " 5/6",
      "⅛" => " 1/8",
      "⅜" => " 3/8",
      "⅝" => " 5/8",
      "⅞" => " 7/8",
      "⅟" => " 1/",
      "Ⅰ" => "I",
      "Ⅱ" => "II",
      "Ⅲ" => "III",
      "Ⅳ" => "IV",
      "Ⅴ" => "V",
      "Ⅵ" => "VI",
      "Ⅶ" => "VII",
      "Ⅷ" => "VIII",
      "Ⅸ" => "IX",
      "Ⅹ" => "X",
      "Ⅺ" => "XI",
      "Ⅻ" => "XII",
      "Ⅼ" => "L",
      "Ⅽ" => "C",
      "Ⅾ" => "D",
      "Ⅿ" => "M",
      "ⅰ" => "i",
      "ⅱ" => "ii",
      "ⅲ" => "iii",
      "ⅳ" => "iv",
      "ⅴ" => "v",
      "ⅵ" => "vi",
      "ⅶ" => "vii",
      "ⅷ" => "viii",
      "ⅸ" => "ix",
      "ⅹ" => "x",
      "ⅺ" => "xi",
      "ⅻ" => "xii",
      "ⅼ" => "l",
      "ⅽ" => "c",
      "ⅾ" => "d",
      "ⅿ" => "m",
      "←" => "{\\textleftarrow}",
      "↑" => "{\\textuparrow}",
      "→" => "{\\textrightarrow}",
      "↓" => "{\\textdownarrow}",
      "↔" => "<->",
      "⇐" => "<=",
      "⇒" => "=>",
      "⇔" => "<=>",
      "−" => "-",
      "∕" => "/",
      "∖" => "\\",
      "∗" => "*",
      "∘" => "o",
      "∙" => ".",
      "∞" => '$\\infty$',
      "∣" => "|",
      "∥" => "||",
      "∶" => ":",
      "∼" => "\\~{}",
      "≠" => "/=",
      "≡" => "=",
      "≤" => "<=",
      "≥" => ">=",
      "≪" => "<<",
      "≫" => ">>",
      "⊕" => "(+)",
      "⊖" => "(-)",
      "⊗" => "(x)",
      "⊘" => "(/)",
      "⊢" => "|-",
      "⊣" => "-|",
      "⊦" => "|-",
      "⊧" => "|=",
      "⊨" => "|=",
      "⊩" => "||-",
      "⋅" => ".",
      "⋆" => "*",
      "⋕" => '$\\#$',
      "⋘" => "<<<",
      "⋙" => ">>>",
      "⋯" => "...",
      "〈" => "{\\textlangle}",
      "〉" => "{\\textrangle}",
      "␀" => "NUL",
      "␁" => "SOH",
      "␂" => "STX",
      "␃" => "ETX",
      "␄" => "EOT",
      "␅" => "ENQ",
      "␆" => "ACK",
      "␇" => "BEL",
      "␈" => "BS",
      "␉" => "HT",
      "␊" => "LF",
      "␋" => "VT",
      "␌" => "FF",
      "␍" => "CR",
      "␎" => "SO",
      "␏" => "SI",
      "␐" => "DLE",
      "␑" => "DC1",
      "␒" => "DC2",
      "␓" => "DC3",
      "␔" => "DC4",
      "␕" => "NAK",
      "␖" => "SYN",
      "␗" => "ETB",
      "␘" => "CAN",
      "␙" => "EM",
      "␚" => "SUB",
      "␛" => "ESC",
      "␜" => "FS",
      "␝" => "GS",
      "␞" => "RS",
      "␟" => "US",
      "␠" => "SP",
      "␡" => "DEL",
      "␣" => "{\\textvisiblespace}",
      "␤" => "NL",
      "␥" => "///",
      "␦" => "?",
      "①" => "(1)",
      "②" => "(2)",
      "③" => "(3)",
      "④" => "(4)",
      "⑤" => "(5)",
      "⑥" => "(6)",
      "⑦" => "(7)",
      "⑧" => "(8)",
      "⑨" => "(9)",
      "⑩" => "(10)",
      "⑪" => "(11)",
      "⑫" => "(12)",
      "⑬" => "(13)",
      "⑭" => "(14)",
      "⑮" => "(15)",
      "⑯" => "(16)",
      "⑰" => "(17)",
      "⑱" => "(18)",
      "⑲" => "(19)",
      "⑳" => "(20)",
      "⑴" => "(1)",
      "⑵" => "(2)",
      "⑶" => "(3)",
      "⑷" => "(4)",
      "⑸" => "(5)",
      "⑹" => "(6)",
      "⑺" => "(7)",
      "⑻" => "(8)",
      "⑼" => "(9)",
      "⑽" => "(10)",
      "⑾" => "(11)",
      "⑿" => "(12)",
      "⒀" => "(13)",
      "⒁" => "(14)",
      "⒂" => "(15)",
      "⒃" => "(16)",
      "⒄" => "(17)",
      "⒅" => "(18)",
      "⒆" => "(19)",
      "⒇" => "(20)",
      "⒈" => "1.",
      "⒉" => "2.",
      "⒊" => "3.",
      "⒋" => "4.",
      "⒌" => "5.",
      "⒍" => "6.",
      "⒎" => "7.",
      "⒏" => "8.",
      "⒐" => "9.",
      "⒑" => "10.",
      "⒒" => "11.",
      "⒓" => "12.",
      "⒔" => "13.",
      "⒕" => "14.",
      "⒖" => "15.",
      "⒗" => "16.",
      "⒘" => "17.",
      "⒙" => "18.",
      "⒚" => "19.",
      "⒛" => "20.",
      "⒜" => "(a)",
      "⒝" => "(b)",
      "⒞" => "(c)",
      "⒟" => "(d)",
      "⒠" => "(e)",
      "⒡" => "(f)",
      "⒢" => "(g)",
      "⒣" => "(h)",
      "⒤" => "(i)",
      "⒥" => "(j)",
      "⒦" => "(k)",
      "⒧" => "(l)",
      "⒨" => "(m)",
      "⒩" => "(n)",
      "⒪" => "(o)",
      "⒫" => "(p)",
      "⒬" => "(q)",
      "⒭" => "(r)",
      "⒮" => "(s)",
      "⒯" => "(t)",
      "⒰" => "(u)",
      "⒱" => "(v)",
      "⒲" => "(w)",
      "⒳" => "(x)",
      "⒴" => "(y)",
      "⒵" => "(z)",
      "Ⓐ" => "(A)",
      "Ⓑ" => "(B)",
      "Ⓒ" => "(C)",
      "Ⓓ" => "(D)",
      "Ⓔ" => "(E)",
      "Ⓕ" => "(F)",
      "Ⓖ" => "(G)",
      "Ⓗ" => "(H)",
      "Ⓘ" => "(I)",
      "Ⓙ" => "(J)",
      "Ⓚ" => "(K)",
      "Ⓛ" => "(L)",
      "Ⓜ" => "(M)",
      "Ⓝ" => "(N)",
      "Ⓞ" => "(O)",
      "Ⓟ" => "(P)",
      "Ⓠ" => "(Q)",
      "Ⓡ" => "(R)",
      "Ⓢ" => "(S)",
      "Ⓣ" => "(T)",
      "Ⓤ" => "(U)",
      "Ⓥ" => "(V)",
      "Ⓦ" => "(W)",
      "Ⓧ" => "(X)",
      "Ⓨ" => "(Y)",
      "Ⓩ" => "(Z)",
      "ⓐ" => "(a)",
      "ⓑ" => "(b)",
      "ⓒ" => "(c)",
      "ⓓ" => "(d)",
      "ⓔ" => "(e)",
      "ⓕ" => "(f)",
      "ⓖ" => "(g)",
      "ⓗ" => "(h)",
      "ⓘ" => "(i)",
      "ⓙ" => "(j)",
      "ⓚ" => "(k)",
      "ⓛ" => "(l)",
      "ⓜ" => "(m)",
      "ⓝ" => "(n)",
      "ⓞ" => "(o)",
      "ⓟ" => "(p)",
      "ⓠ" => "(q)",
      "ⓡ" => "(r)",
      "ⓢ" => "(s)",
      "ⓣ" => "(t)",
      "ⓤ" => "(u)",
      "ⓥ" => "(v)",
      "ⓦ" => "(w)",
      "ⓧ" => "(x)",
      "ⓨ" => "(y)",
      "ⓩ" => "(z)",
      "⓪" => "(0)",
      "─" => "-",
      "━" => "=",
      "│" => "|",
      "┃" => "|",
      "┄" => "-",
      "┅" => "=",
      "┆" => "|",
      "┇" => "|",
      "┈" => "-",
      "┉" => "=",
      "┊" => "|",
      "┋" => "|",
      "┌" => "+",
      "┍" => "+",
      "┎" => "+",
      "┏" => "+",
      "┐" => "+",
      "┑" => "+",
      "┒" => "+",
      "┓" => "+",
      "└" => "+",
      "┕" => "+",
      "┖" => "+",
      "┗" => "+",
      "┘" => "+",
      "┙" => "+",
      "┚" => "+",
      "┛" => "+",
      "├" => "+",
      "┝" => "+",
      "┞" => "+",
      "┟" => "+",
      "┠" => "+",
      "┡" => "+",
      "┢" => "+",
      "┣" => "+",
      "┤" => "+",
      "┥" => "+",
      "┦" => "+",
      "┧" => "+",
      "┨" => "+",
      "┩" => "+",
      "┪" => "+",
      "┫" => "+",
      "┬" => "+",
      "┭" => "+",
      "┮" => "+",
      "┯" => "+",
      "┰" => "+",
      "┱" => "+",
      "┲" => "+",
      "┳" => "+",
      "┴" => "+",
      "┵" => "+",
      "┶" => "+",
      "┷" => "+",
      "┸" => "+",
      "┹" => "+",
      "┺" => "+",
      "┻" => "+",
      "┼" => "+",
      "┽" => "+",
      "┾" => "+",
      "┿" => "+",
      "╀" => "+",
      "╁" => "+",
      "╂" => "+",
      "╃" => "+",
      "╄" => "+",
      "╅" => "+",
      "╆" => "+",
      "╇" => "+",
      "╈" => "+",
      "╉" => "+",
      "╊" => "+",
      "╋" => "+",
      "╌" => "-",
      "╍" => "=",
      "╎" => "|",
      "╏" => "|",
      "═" => "=",
      "║" => "|",
      "╒" => "+",
      "╓" => "+",
      "╔" => "+",
      "╕" => "+",
      "╖" => "+",
      "╗" => "+",
      "╘" => "+",
      "╙" => "+",
      "╚" => "+",
      "╛" => "+",
      "╜" => "+",
      "╝" => "+",
      "╞" => "+",
      "╟" => "+",
      "╠" => "+",
      "╡" => "+",
      "╢" => "+",
      "╣" => "+",
      "╤" => "+",
      "╥" => "+",
      "╦" => "+",
      "╧" => "+",
      "╨" => "+",
      "╩" => "+",
      "╪" => "+",
      "╫" => "+",
      "╬" => "+",
      "╭" => "+",
      "╮" => "+",
      "╯" => "+",
      "╰" => "+",
      "╱" => "/",
      "╲" => "\\",
      "╳" => "X",
      "╼" => "-",
      "╽" => "|",
      "╾" => "-",
      "╿" => "|",
      "○" => "o",
      "◦" => "{\\textopenbullet}",
      "★" => "*",
      "☆" => "*",
      "☒" => "X",
      "☓" => "X",
      "☹" => ":-(",
      "☺" => ":-)",
      "☻" => "(-:",
      "♭" => "b",
      "♯" => '$\\#$',
      "✁" => '$\\%<$',
      "✂" => '$\\%<$',
      "✃" => '$\\%<$',
      "✄" => '$\\%<$',
      "✌" => "V",
      "✓" => "v",
      "✔" => "V",
      "✕" => "x",
      "✖" => "x",
      "✗" => "X",
      "✘" => "X",
      "✙" => "+",
      "✚" => "+",
      "✛" => "+",
      "✜" => "+",
      "✝" => "+",
      "✞" => "+",
      "✟" => "+",
      "✠" => "+",
      "✡" => "*",
      "✢" => "+",
      "✣" => "+",
      "✤" => "+",
      "✥" => "+",
      "✦" => "+",
      "✧" => "+",
      "✩" => "*",
      "✪" => "*",
      "✫" => "*",
      "✬" => "*",
      "✭" => "*",
      "✮" => "*",
      "✯" => "*",
      "✰" => "*",
      "✱" => "*",
      "✲" => "*",
      "✳" => "*",
      "✴" => "*",
      "✵" => "*",
      "✶" => "*",
      "✷" => "*",
      "✸" => "*",
      "✹" => "*",
      "✺" => "*",
      "✻" => "*",
      "✼" => "*",
      "✽" => "*",
      "✾" => "*",
      "✿" => "*",
      "❀" => "*",
      "❁" => "*",
      "❂" => "*",
      "❃" => "*",
      "❄" => "*",
      "❅" => "*",
      "❆" => "*",
      "❇" => "*",
      "❈" => "*",
      "❉" => "*",
      "❊" => "*",
      "❋" => "*",
      "ﬀ" => "ff",
      "ﬁ" => "fi",
      "ﬂ" => "fl",
      "ﬃ" => "ffi",
      "ﬄ" => "ffl",
      "ﬅ" => "st",
      "ﬆ" => "st"
    );
  }
}