<?php


/**
 * Name Disambiguator
 *
 * @author Casey McLaughlin <caseyamcl@gmail.com>
 */

namespace Nametools;

/**
 * Normalize strings from common formats using REGEX
 */
class Normalize
{
    const PREPEND = 0;
    const APPEND  = 1;

    // --------------------------------------------------------------

    /**
     * @var array
     * Key/Value patterns and their matchPatterns
     */
    private $patterns;

    /**
     * @var MatchObject
     */
    private $matchObject;

    /**
     * @private RegexCounter
     */
    private $regexCounter;

    // --------------------------------------------------------------

    /**
     * Constructor
     *
     * @param MatchObject $matchObject
     *
     * @param array $patterns
     */
    public function __construct(MatchObject $matchObject, RegexCounter $regexCounter, $patterns = array())
    {
        //Setup array
        $this->patterns = array();

        //Dependencies and options
        $this->regexCounter = $regexCounter;
        $this->matchObject = $matchObject;
        $this->addPatterns($patterns);
    }

    // --------------------------------------------------------------

    /**
     * Add a regex pattern
     *
     * The regex pattern should be as specific as possible
     *
     * @param string $regex
     * The regex should contain the number of parentheses matching the match patterns
     *
     * @param array $matchPattern
     * Array of properties to convert the match patterns to
     *
     * @param int $where
     * self::PREPEND or self::APPEND
     */
    public function addPattern($regex, $matchPattern, $where = self::APPEND)
    {
        //Test to ensure the number of matches equals the number
        //of items in the matchPattern
        if ($this->regexCounter->count($regex) != count($matchPattern)) {
            throw new \InvalidArgumentException("The number of matches in the
                regular expression must match the number of elements in the matchPattern");
        }

        //Test to ensure the matchPattern contains only properties that
        //are in the actual match Class
        foreach($matchPattern as $propName) {
            if ( ! in_array($propName, array_keys(get_object_vars($this->matchObject)))) {
                throw new \InvalidArgumentException(sprintf("The match pattern should
                    only contain properties in the %s class!", get_class($this->matchObject) ));
            }
        }

        //Add it
        if ($where == self::PREPEND) {
            $this->patterns = array_merge(
                array($regex => $matchPattern),
                $this->patterns
            );
        }
        else {
            $this->patterns[$regex] = $matchPattern;
        }
    }

    // --------------------------------------------------------------

    /**
     * Append a pattern to this list, making it the last to check
     *
     * @param string $regex
     * @param array $matchPattern
     */
    public function appendPattern($regex, $matchPattern)
    {
        return $this->addPattern($regex, $matchPattern, self::APPEND);
    }

    // --------------------------------------------------------------

    /**
     * Prepend a pattern to the list, making it the first to check
     *
     * @param string $regex
     * @param array $matchPattern
     */
    public function prependPattern($regex, $matchPattern)
    {
        return $this->addPattern($regex, $matchPattern, self::PREPEND);
    }

    // --------------------------------------------------------------

    /**
     * Get the list of patterns and their match patterns
     *
     * @return array
     */
    public function getPatterns()
    {
        return $this->patterns;
    }

    // --------------------------------------------------------------

    /**
     * Initialize built-in and paramaterized patterns
     *
     * @param array $patterns
     */
    public function addPatterns($patterns = array())
    {
        //The patterns specified get higher priority
        foreach($patterns as $regex => $matchPattern) {
            $this->appendPattern($regex, $matchPattern);
        }
    }

    // --------------------------------------------------------------

    /**
     * Normalize
     *
     * @param string $str
     * The string to normalize
     *
     * @return MatchObject|false
     * A match object, or false if no match found
     */
    public function normalize($str)
    {
        //Run through the REGEXes in order.  If match found, match it
        foreach ($this->patterns as $regex => $pattern) {

            if ($res = $this->checkPattern($regex, $pattern, $str)) {
                return $res;
            }
        }

        //If match not found return false
        return false;
    }

    // --------------------------------------------------------------

    /**
     * Check Pattern
     *
     * @param string $regex
     * Regular Expression Pattern
     *
     * @param array $pattern
     * Match Pattern
     *
     * @param string $str
     * String to operate on
     *
     * @return MatchObject|false
     * False if no match
     */
    protected function checkPattern($regex, $pattern, $str)
    {
        if (preg_match($regex, $str, $matches) > 0) {
            return $this->applyPattern($matches, $pattern);
        }
        else {
            return false;
        }
    }

    // --------------------------------------------------------------

    /**
     * Apply a pattern if it has matched
     *
     * @param array $matches
     * Matches from Regex
     *
     * @param array $pattern
     * Match Pattern
     *
     * @return MatchObject
     * New match Object with values applied
     */
    protected function applyPattern($matches, $pattern)
    {
        $class = $this->matchObject;
        $outObj = $class::factory();

        $matches = array_slice($matches, 1);
        for ($i = 0; $i < count($matches); $i++) {
            $propName = $pattern[$i];

            $outObj->$propName = $matches[$i];
        }

        return $outObj;
    }
}

/* EOF: Nomralize.php */