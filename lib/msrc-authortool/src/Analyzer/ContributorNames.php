<?php

/**
 * Name Disambiguator
 *
 * @author Casey McLaughlin <caseyamcl@gmail.com>
 */

namespace Analyzer;

use Nametools\Normalize;
use Nametools\RegexCounter;

/**
 * Match Contributor Names with Predefined Regex
 */
class ContributorNames implements AnalyzerInterface
{
    /**
     * @var \Nametools\Normalize
     * Normalizer object
     */
    private $normalizer;

    /**
     * @var $orgWords
     */
    private $orgWords;

    // --------------------------------------------------------------

    /**
     * Normalizer
     *
     * @param \Nametools\Normalize $normalize
     * Or null to auto-use dependency
     *
     */
    public function __construct(Normalize $normalizer = null)
    {
        //Set Normalizer Object
        $this->normalizer = $normalizer ?: new Normalize(new ContributorObject, new RegexCounter);

        //Setup arrays
        $this->normalizedNames = array();
        $this->skippedNames = array();

        //Setup Patterns
        $this->setupPatterns();
    }

    // --------------------------------------------------------------

    /**
     * Setup Org Words
     *
     * @param array $orgWords
     * Array of words that denote organization
     */
    public function setOrgWords($orgWords)
    {
        $this->orgWords = array_map('strtolower', $orgWords);
    }

    // --------------------------------------------------------------

    /**
     * Analyze a string
     *
     * @param string $string
     *
     * @param return ContributorObject|boolean
     * false if not parsed
     */
    public function analyze($string)
    {
        //String Trim
        $string = trim($string);

        //Some names have HTML escape codes
        $string = str_replace('&apos;', "'", $string);
        $string = str_replace('&Ouml;', 'Ö', $string);

        //If it's [anon], ignore it
        if (strcasecmp("[anon]", $string) == 0) {
            return false;
        }

        //If it contains organization keywords, it is an organization
        if ($this->checkIsOrganization($string)) {
            $co = new ContributorObject();
            $co->organization = $string;
            return $co;
        }

        //Everything < 6 char w/no spaces and alphanum is organization
        if (strlen($string) < 6 && preg_match("/^[a-z0-9]+$/i", $string)) {
            $co = new ContributorObject();
            $co->organization = $string;
            return $co;
        }

        //Everything < 6 char w/no spaces and not-alphanum, ignore
        elseif (strlen($string) < 6 && ! preg_match("/[ +?]/", $string)) {
            return false;
        }

        //Before running the main regex patterns, see if there is a known lastName prefix,
        //and rip that out
        list($lnPrefix, $string) = $this->checkAndSetLastNamePrefix($string);

        //Everything else we'll try to parse using the Nametools
        $object = $this->normalizer->normalize($string);

        //Clean it up
        if ($object) {
            $object->lastNamePrefix = $lnPrefix;
            $object = $this->cleanUp($object);
        }

        //Return it
        return $object;
    }

    // --------------------------------------------------------------

    /**
     * Check if a string represents an organization
     *
     * Compares the string against known organization words
     * e.g. "Center" or "Team"
     *
     * @param string $str
     * @return boolean
     */
    protected function checkIsOrganization($str)
    {
        //Lowercase
        $str = strtolower($str);

        //Check against all of the org words
        foreach($this->orgWords as $word) {
            if (preg_match("/\b$word\b/i", $str)) {
                return true;
            }
        }

        //If made it here
        return false;
    }

    // --------------------------------------------------------------

    /**
     * Cleanup a matched object
     *
     * Normalizes suffixes, and fills in initials.
     *
     * @param ContributorObject $obj
     * @return ContributorObject
     */
    protected function cleanUp(ContributorObject $obj)
    {
        //If there is a suffix, attempt to normalize it
        if ($obj->suffix) {
            $obj->suffix = $obj->mapSuffix($obj->suffix);
        }

        //If middleName is a single character, and no middle initial,
        //move the middleName to the middleInitial
        if (strlen($obj->middleName) == 1 && ! $obj->middleInitial) {
            $obj->middleInitial = $obj->middleName;
            $obj->middleName = null;
        }

        //If firstName is a single character and no first initial,
        //move the firstName to the firstInitial
        if (strlen($obj->firstName) == 1 && ! $obj->firstInitial) {
            $obj->firstInitial = $obj->firstName;
            $obj->firstName = null;
        }

        //If middleName and no middle initial, fill that in
        if ($obj->middleName && ! $obj->middleInitial) {
            $obj->middleInitial = $obj->middleName{0};
        }

        //If firstName and no first initial, fill that in
        if ($obj->firstName && ! $obj->firstInitial) {
            $obj->firstInitial = $obj->firstName{0};
        }

        return $obj;
    }

    // --------------------------------------------------------------

    /**
     * Check if the lastname has a prefix and set it
     *
     * If the lastname has a prefix, it will either be captured as part
     * of the lastname string, or as the middleName
     *
     * @param string $str
     * @return array [0] is the prefix and [1] is the modified string
     */
    protected function checkAndSetLastNamePrefix($str)
    {
        $regex = "/\b(von|van der|van den|van de|van|le|el|dos|de|de la)\s[\p{L}]/i";

        if (preg_match($regex, $str, $matches)) {

            //printf("\nFOUND %s: %s", $matches[1], $str);

            $prefix = $matches[1];
            $str = preg_replace("/" . $matches[1] . "/", '', $str, 1);

            //Fix double spaces caused by removal
            while(strpos($str, '  ')) {
                $str = str_replace('  ', ' ', $str);
            }

            //printf(" ... pre: %s ... ln: %s", $matches[1], $str);
        }
        else {
            $prefix = null;
        }

        return array($prefix, $str);
    }

    // --------------------------------------------------------------

    /**
     * Setup the patterns
     */
    protected function setupPatterns()
    {
        $ppLastNamePattern       = "([\p{L}\p{Ll}-' ]+)"; //"([a-zA-Z-' ]+)"; //Unicode form: ([\p{L}\p{Ll}-' ]+)
        $lastNamePattern         = "([\p{L}\p{Ll}-']+)";  //"([a-zA-Z-']+)";  //Unicode form:  ([\p{L}\p{Ll}-']+)
        $suffixPattern           = "(?i)(Jr|Sr|Esq|Ph\.?D|2nd|3rd|Psy\.D|M\.S|II|III|IV)\.?";

        //Where we have initials only, it's pretty clear

        //Lastname, FMS, Suf.
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern},[ +?]?([A-Z])\.?[ +?]?([A-Z])\.?[ +?]?([A-Z])\.?,[ +?]?{$suffixPattern}/u",
            array('lastName', 'firstInitial', 'middleInitial', 'secondMiddleInitial', 'suffix')
        );

        //Lastname, FM, Suf.
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern},[ +?]?([A-Z])\.?[ +?]?([A-Z])\.?,[ +?]?{$suffixPattern}/u",
            array('lastName', 'firstInitial', 'middleInitial', 'suffix')
        );

        //Lastname, F, Suf.
        $this->normalizer->appendPattern(
            "/{$ppLastNamePattern},[ +?]?([A-Z])\.?(?![a-z'-]),[ +?]?{$suffixPattern}/u",
            array('lastName', 'firstInitial', 'suffix')
        );

        //Lastname Suf., FMS
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern} {$suffixPattern}?,[ +?]?([A-Z])\.?[ +?]?([A-Z])\.?[ +?]?([A-Z])\.?$/u",
            array('lastName', 'suffix', 'firstInitial', 'middleInitial', 'secondMiddleInitial')
        );

        //Lastname Suf., FM
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern} {$suffixPattern}?,[ +?]?([A-Z])\.?[ +?]?([A-Z])\.?(?![a-z'-])\.?$/u",
            array('lastName', 'suffix', 'firstInitial', 'middleInitial')
        );

        //Lastname Suf., F
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern} {$suffixPattern}?,[ +?]?([A-Z])\.?(?![a-z'-])\.?$/u",
            array('lastName', 'suffix', 'firstInitial')
        );

        //Lastname, FMS
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern},[ +?]?([A-Z])\.?[ +?]?([A-Z])\.?[ +?]?([A-Z])\.?$/u",
            array('lastName', 'firstInitial', 'middleInitial', 'secondMiddleInitial')
        );

        //Lastname, FM
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern},[ +?]?([A-Z])\.?[ +?]?([A-Z])\.?(?![a-zA-Z'-])\.?$/u",
            array('lastName', 'firstInitial', 'middleInitial')
        );

        //Lastname, F
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern},[ +?]?([A-Z])\.?(?![a-z'-])\.?$/u",
            array('lastName', 'firstInitial')
        );

        //All FirstName Patterns are incorrect below this line!

        //?? Add ??: Lastname, Firstname, M[\.| ]?S[\.]?

        //Lastname, FirstName, MiddleName S, Suf.
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern},[ +?]?([\p{L}-]+)\.?[ +?]([\p{L}]+)\.?[ +?]([A-Z])\.?,[ +?]?{$suffixPattern}/ui",
            array('lastName', 'firstName', 'middleName', 'secondMiddleInitial', 'suffix')
        );

        //Lastname, Firstname, MiddleName, Suf.
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern},[ +?]?([\p{L}-]+)\.?[ +?]([\p{L}]+)\.?,[ +?]?{$suffixPattern}/ui",
            array('lastName', 'firstName', 'middleName', 'suffix')
        );

        //Lastname, Firstname, Suf.
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern},[ +?]?([\p{L}-]+)\.?,[ +?]?{$suffixPattern}/ui",
            array('lastName', 'firstName', 'suffix')
        );

        //Lastname, FirstName MiddleName S
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern},[ +?]?([\p{L}-]+)\.?[ +?]([\p{L}]+)\.?[ +?]([A-Z])\.?/ui",
            array('lastName', 'firstName', 'middleName', 'secondMiddleInitial')
        );

        //Lastname, Firstname, MiddleName
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern},[ +?]?([\p{L}-]+)\.?[ +?]([\p{L}]+)\.?/ui",
            array('lastName', 'firstName', 'middleName')
        );

        //Lastname, Firstname
        $this->normalizer->appendPattern(
            "/^{$ppLastNamePattern},[ +?]?([\p{L}-]+)\.?/ui",
            array('lastName', 'firstName')
        );

        //F.M.S. Lastname
        $this->normalizer->appendPattern(
            "/^([a-zA-Z])[\. ]([a-zA-Z])[\. ]([a-zA-Z])[\.]? {$lastNamePattern}/u",
            array('firstInitial', 'middleInitial', 'secondMiddleInitial', 'lastName')
        );

        //F.M. Lastname
        $this->normalizer->appendPattern(
            "/^([a-zA-Z])[\. ]([a-zA-Z])[\.]? {$lastNamePattern}/u",
            array('firstInitial', 'middleInitial', 'lastName')
        );


        //Firstname MS Lastname, Suf.
        $this->normalizer->appendPattern(
            "/^([\p{L}-]+)\.? ([A-Z])[\.| ]?([A-Z])\.? {$lastNamePattern},[ +?]?{$suffixPattern}/u",
            array('firstName', 'middleName', 'secondMiddleInitial', 'lastName', 'suffix')
        );

        //Firstname Middle S Lastname, Suf.
        $this->normalizer->appendPattern(
            "/^([\p{L}-]+)\.?[ +?]([a-z][a-z]+)[ +?]([A-Z])\.?[ +?]{$lastNamePattern},[ +?]?{$suffixPattern}/ui",
            array('firstName', 'middleName', 'secondMiddleInitial', 'lastName', 'suffix')
        );

        //Firstname Middle Lastname, Suf.
        $this->normalizer->appendPattern(
            "/^([\p{L}-]+)\.?[ +?]([a-z][a-z]+)[ +?]{$lastNamePattern},[ +?]?{$suffixPattern}/ui",
            array('firstName', 'middleName', 'lastName', 'suffix')
        );

        //Firstname Lastname, Suf.
        $this->normalizer->appendPattern(
            "/^([\p{L}-]+)\.?[ +?]{$lastNamePattern},[ +?]?{$suffixPattern}/ui",
            array('firstName', 'lastName', 'suffix')
        );

        //Firstname MS Lastname
        $this->normalizer->appendPattern(
            "/^([\p{L}-]+)\.?[ +?]([A-Z])[\.| ]?([A-Z])\.?[ +?]{$lastNamePattern}/u",
            array('firstName', 'middleName', 'secondMiddleInitial', 'lastName')
        );

        //Firstname Middle S Lastname
        $this->normalizer->appendPattern(
            "/^([\p{L}-]+)\.?[ +?]([a-z][a-z]+)[ +?]([A-Z])\.?[ +?]{$lastNamePattern}/ui",
            array('firstName', 'middleName', 'secondMiddleInitial', 'lastName')
        );

        //Firstname Middle Lastname
        $this->normalizer->appendPattern(
            "/^([\p{L}-]+)\.?[ +?]([a-z][a-z]+)[ +?]{$lastNamePattern}/ui",
            array('firstName', 'middleName', 'lastName')
        );

        //Firstname Lastname
        $this->normalizer->appendPattern(
            "/^([\p{L}-]+)\.?[ +?]{$lastNamePattern}/ui",
            array('firstName', 'lastName')
        );

        //Suf.,Firstname M. Lastname
        $this->normalizer->appendPattern(
            "/^{$suffixPattern},\s?([\p{L}-]+)\.?[ +?]([a-z]+)\.?[ +?]{$lastNamePattern}/ui",
            array('suffix', 'firstName', 'middleInitial', 'lastName')
        );
    }
}

/* EOF: Analyzer.php */