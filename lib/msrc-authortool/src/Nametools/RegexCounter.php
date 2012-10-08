<?php

namespace Nametools;

/**
 * Regexcounter is a class that counts the expected
 * number of matches for a given REGEX pattern
 *
 * It was ported entirely from:
 * @link http://noteslog.com/post/how-to-count-expected-matches-of-a-php-regular-expression/
 */
class RegexCounter
{
    // --------------------------------------------------------------

    /**
     * Returns how many groups (non-named) there are in the given pattern
     *
     * Ignores namesGroups and numberedGroups for simplicity
     *
     * @param string $pattern
     * @return int
     */
    public function count($pattern) {

        return $this->countGroups($pattern, $null, $null);
    }

    // --------------------------------------------------------------

    /**
     * Returns how many groups (numbered or named) there are in the given $pattern
     *
     * @param string $pattern
     * @param array  $namedGroups
     * @param array  $numberedGroups
     * @return int
     */
    public function countGroups($pattern, &$namedGroups, &$numberedGroups)
    {
        $result = $this->countGroupsIgnoreHellternations($pattern, $namedGroups, $numberedGroups);
        $hellternations = $this->findHellternations($pattern);

        if (empty($hellternations)) {
            return $result;
        }

        foreach ($hellternations as $hellternation) {
            $count = array();
            $pieces = $this->explodeAlternation($hellternation);

            foreach ($pieces as $piece) {
                $count[] = $this->countGroups($piece, $dummy, $dummy);
            }

            $max = max($count);
            $easy = $this->countGroupsIgnoreHellternations($hellternation, $dummy, $dummy);
            $result = $result - $easy + $max;
        }

        return $result;
    }

   // --------------------------------------------------------------

    /**
     * Returns how many groups (numbered or named) there are in the given $pattern,
     * ignoring hellternations (?|...|...)
     *
     * @param string $pattern
     * @param array  $namedGroups
     * @param array  $numberedGroups
     *
     * @return integer
     */
    function countGroupsIgnoreHellternations(&$pattern, &$namedGroups, &$numberedGroups)
    {
        $findExplicitelyEscaped = '/\\\\./';
        $pattern = preg_replace($findExplicitelyEscaped, '%', $pattern);

        $findImplicitelyEscaped = '/\[[^\]]*\]/';
        $pattern = preg_replace($findImplicitelyEscaped, '%', $pattern);

        $findNumberedGroups  = '/\((?!\?)/';
        $numberedGroups_count = preg_match_all($findNumberedGroups, $pattern, $numberedGroups, PREG_SET_ORDER | PREG_OFFSET_CAPTURE);

        $findConditions = '/\(\?\(/';
        $conditions_count = preg_match_all($findConditions, $pattern, $dummy);

        $numberedGroups_count -= $conditions_count;

        $find_namedGroups  = '/\(\?P?(?:(?:<([^>]+)>)|(?:\'([^\']+)\'))/';
        $namedGroups_count = preg_match_all($find_namedGroups, $pattern, $namedGroups, PREG_SET_ORDER | PREG_OFFSET_CAPTURE);

        $numberedGroups_count += $namedGroups_count;  //named groups also add as many numbered groups

        $dupnames = array();
        foreach ($namedGroups as $named_group) {

            $name = $named_group[1][0];
            if (isset($dupnames[$name])) {
                $dupnames[$name] += 1;
            }
            else {
                $dupnames[$name] = 0;
            }

        }
        $dupnames_count = array_sum($dupnames);
        $namedGroups_count -= $dupnames_count;  //duplicate names are added only once

        $result = $numberedGroups_count + $namedGroups_count;
        return $result;
    }

    // --------------------------------------------------------------

    /**
     * Returns the hellternations which are siblings to each other.
     * NOTE: the given $pattern is assumed to not contain escaped parentheses.
     *
     * @param string $pattern a string of alternations wrapped into (?|...)
     * @return array
     * @throws RegexCounterException
     */
    protected function findHellternations($pattern)
    {
        $result = array();
        $token = '(?|';
        $tokenLen = strlen($token);
        $offset = 0;

        do {
            $start = strpos($pattern, $token, $offset);
            if (FALSE === $start) {
                return $result;
            }
            $open = 1;
            $start += $tokenLen;
            for ($i = $start, $iTop = strlen($pattern); $i < $iTop; $i++) {
                //$current = $pattern[$i];
                if ($pattern[$i] == '(') {
                    $open += 1;
                }
                elseif ($pattern[$i] == ')') {
                    $open -= 1;
                    if (0 == $open) {
                        $result[$start] = substr($pattern, $start, $i - $start);
                        $offset = $i + 1;
                        break;
                    }
                }
            }
        }
        while ($i < $iTop);

        if (0 != $open) {
            throw new RegexCounterException('Unbalanced parentheses.');
        }

        return $result;
    }

    // --------------------------------------------------------------

    /**
     * Explodes an alternation on outer pipes.
     * NOTE: the given $pattern is assumed to not contain escaped parentheses nor escaped pipes.
     *
     * @param string $pattern a string with balanced (possibly nested) parentheses and pipes
     * @return array
     * @throws RegexCounterException
     */
    protected function explodeAlternation($pattern)
    {
        $result = array();
        $token = '|';
        $open = 0;
        $start = 0;

        for ($i = $start, $iTop = strlen($pattern); $i < $iTop; $i++) {

            //$current = $pattern[$i];
            if ($pattern[$i] == '(') {
                $open += 1;
            }
            elseif ($pattern[$i] == ')') {
                $open -= 1;
            }
            elseif ($pattern[$i] == '|') {
                if (0 == $open) {
                    $result[$start] = substr($pattern, $start, $i - $start);
                    $start = $i + 1;
                }
            }
        }

        $result[$start] = substr($pattern, $start);

        if (0 != $open) {
            throw new RegexCounterException('Unbalanced parentheses.');
        }

        return $result;
    }
}

/* EOF: RegexCounter.php */