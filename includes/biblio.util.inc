<?php
function biblio_normalize_title($title) {
  $stop_words = array('a', 'an', 'the');
  $title = trim(_strip_punctuation($title));
  mb_regex_encoding( "utf-8" );
  $title_words = mb_split( ' +', $title );
  while (array_search(drupal_strtolower($title_words[0]), $stop_words) !== FALSE) {
    array_shift($title_words);
  }
  return drupal_substr(implode(' ', $title_words), 0, 64);
}
/**
 * Copyright (c) 2008, David R. Nadeau, NadeauSoftware.com.
 * All rights reserved.
 *
 * Strip punctuation characters from UTF-8 text.
 *
 * Characters stripped from the text include characters in the following
 * Unicode categories:
 *
 * 	Separators
 * 	Control characters
 *	Formatting characters
 *	Surrogates
 *	Open and close quotes
 *	Open and close brackets
 *	Dashes
 *	Connectors
 *	Numer separators
 *	Spaces
 *	Other punctuation
 *
 * Exceptions are made for punctuation characters that occur withn URLs
 * (such as [ ] : ; @ & ? and others), within numbers (such as . , % # '),
 * and within words (such as - and ').
 *
 * Parameters:
 * 	text		the UTF-8 text to strip
 *
 * Return values:
 * 	the stripped UTF-8 text.
 *
 * See also:
 * 	http://nadeausoftware.com/articles/2007/9/php_tip_how_strip_punctuation_characters_web_page
 */
function _strip_punctuation( $text )
{
	$urlbrackets    = '\[\]\(\)';
	$urlspacebefore = ':;\'_\*%@&?!' . $urlbrackets;
	$urlspaceafter  = '\.,:;\'\-_\*@&\/\\\\\?!#' . $urlbrackets;
	$urlall         = '\.,:;\'\-_\*%@&\/\\\\\?!#' . $urlbrackets;

	$specialquotes = '\'"\*<>';

	$fullstop      = '\x{002E}\x{FE52}\x{FF0E}';
	$comma         = '\x{002C}\x{FE50}\x{FF0C}';
	$arabsep       = '\x{066B}\x{066C}';
	$numseparators = $fullstop . $comma . $arabsep;

	$numbersign    = '\x{0023}\x{FE5F}\x{FF03}';
	$percent       = '\x{066A}\x{0025}\x{066A}\x{FE6A}\x{FF05}\x{2030}\x{2031}';
	$prime         = '\x{2032}\x{2033}\x{2034}\x{2057}';
	$nummodifiers  = $numbersign . $percent . $prime;

	return preg_replace(
		array(
		// Remove separator, control, formatting, surrogate,
		// open/close quotes.
			'/[\p{Z}\p{Cc}\p{Cf}\p{Cs}\p{Pi}\p{Pf}]/u',
		// Remove other punctuation except special cases
			'/\p{Po}(?<![' . $specialquotes .
				$numseparators . $urlall . $nummodifiers . '])/u',
		// Remove non-URL open/close brackets, except URL brackets.
			'/[\p{Ps}\p{Pe}](?<![' . $urlbrackets . '])/u',
		// Remove special quotes, dashes, connectors, number
		// separators, and URL characters followed by a space
			'/[' . $specialquotes . $numseparators . $urlspaceafter .
				'\p{Pd}\p{Pc}]+((?= )|$)/u',
		// Remove special quotes, connectors, and URL characters
		// preceded by a space
			'/((?<= )|^)[' . $specialquotes . $urlspacebefore . '\p{Pc}]+/u',
		// Remove dashes preceded by a space, but not followed by a number
			'/((?<= )|^)\p{Pd}+(?![\p{N}\p{Sc}])/u',
		// Remove consecutive spaces
			'/ +/',
		),
		' ',
		$text );
}

/**
 * Copyright (c) 2008, David R. Nadeau, NadeauSoftware.com.
 * All rights reserved.
 *
 * Strip symbol characters from UTF-8 text.
 *
 * Characters stripped from the text include characters in the following
 * Unicode categories:
 *
 * 	Modifier symbols
 * 	Private use symbols
 * 	Math symbols
 * 	Other symbols
 *
 * Exceptions are made for math symbols embedded within numbers (such as
 * + - /), math symbols used within URLs (such as = ~), units of measure
 * symbols, and ideograph parts.  Currency symbols are not removed.
 *
 * Parameters:
 * 	text		the UTF-8 text to strip
 *
 * Return values:
 * 	the stripped UTF-8 text.
 *
 * See also:
 *	http://nadeausoftware.com/articles/2007/09/php_tip_how_strip_symbol_characters_web_page
 */
function _strip_symbols( $text )
{
	$plus   = '\+\x{FE62}\x{FF0B}\x{208A}\x{207A}';
	$minus  = '\x{2012}\x{208B}\x{207B}';

	$units  = '\\x{00B0}\x{2103}\x{2109}\\x{23CD}';
	$units .= '\\x{32CC}-\\x{32CE}';
	$units .= '\\x{3300}-\\x{3357}';
	$units .= '\\x{3371}-\\x{33DF}';
	$units .= '\\x{33FF}';

	$ideo   = '\\x{2E80}-\\x{2EF3}';
	$ideo  .= '\\x{2F00}-\\x{2FD5}';
	$ideo  .= '\\x{2FF0}-\\x{2FFB}';
	$ideo  .= '\\x{3037}-\\x{303F}';
	$ideo  .= '\\x{3190}-\\x{319F}';
	$ideo  .= '\\x{31C0}-\\x{31CF}';
	$ideo  .= '\\x{32C0}-\\x{32CB}';
	$ideo  .= '\\x{3358}-\\x{3370}';
	$ideo  .= '\\x{33E0}-\\x{33FE}';
	$ideo  .= '\\x{A490}-\\x{A4C6}';

	return preg_replace(
		array(
		// Remove modifier and private use symbols.
			'/[\p{Sk}\p{Co}]/u',
		// Remove math symbols except + - = ~ and fraction slash
			'/\p{Sm}(?<![' . $plus . $minus . '=~\x{2044}])/u',
		// Remove + - if space before, no number or currency after
			'/((?<= )|^)[' . $plus . $minus . ']+((?![\p{N}\p{Sc}])|$)/u',
		// Remove = if space before
			'/((?<= )|^)=+/u',
		// Remove + - = ~ if space after
			'/[' . $plus . $minus . '=~]+((?= )|$)/u',
		// Remove other symbols except units and ideograph parts
			'/\p{So}(?<![' . $units . $ideo . '])/u',
		// Remove consecutive white space
			'/ +/',
		),
		' ',
		$text );
}
/**
 * Remove HTML tags, including invisible text such as style and
 * script code, and embedded objects.  Add line breaks around
 * block-level tags to prevent word joining after tag removal.
 */
function _strip_html_tags( $text )
{
    $text = preg_replace(
        array(
          // Remove invisible content
            '@<head[^>]*?>.*?</head>@siu',
            '@<style[^>]*?>.*?</style>@siu',
            '@<script[^>]*?.*?</script>@siu',
            '@<object[^>]*?.*?</object>@siu',
            '@<embed[^>]*?.*?</embed>@siu',
            '@<applet[^>]*?.*?</applet>@siu',
            '@<noframes[^>]*?.*?</noframes>@siu',
            '@<noscript[^>]*?.*?</noscript>@siu',
            '@<noembed[^>]*?.*?</noembed>@siu',
          // Add line breaks before and after blocks
            '@</?((address)|(blockquote)|(center)|(del))@iu',
            '@</?((div)|(h[1-9])|(ins)|(isindex)|(p)|(pre))@iu',
            '@</?((dir)|(dl)|(dt)|(dd)|(li)|(menu)|(ol)|(ul))@iu',
            '@</?((table)|(th)|(td)|(caption))@iu',
            '@</?((form)|(button)|(fieldset)|(legend)|(input))@iu',
            '@</?((label)|(select)|(optgroup)|(option)|(textarea))@iu',
            '@</?((frameset)|(frame)|(iframe))@iu',
        ),
        array(
            ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            "\n\$0", "\n\$0", "\n\$0", "\n\$0", "\n\$0", "\n\$0",
            "\n\$0", "\n\$0",
        ),
        $text );
    return strip_tags( $text );
}