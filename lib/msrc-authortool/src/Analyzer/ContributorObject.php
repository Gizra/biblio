<?php

/**
 * Name Disambiguator
 *
 * @author Casey McLaughlin <caseyamcl@gmail.com>
 */

namespace Analyzer;

use \Nametools\MatchObject;

/**
 * Contributor Match Object
 *
 * A simple value object that contains the properties for a contributor
 */
class ContributorObject extends MatchObject
{
    /**
     * @var string
     * First Name
     */
    public $firstName;

    /**
     * @var string
     * Last Name
     */
    public $lastName;

    /**
     * @var string
     * Middle Name
     */
    public $middleName;

    /**
     * @var string
     * First Initial
     */
    public $firstInitial;

    /**
     * @var string
     * Middle Initial
     */
    public $middleInitial;

    /**
     * @var string
     * Second Middle Initial
     */
    public $secondMiddleInitial;

    /**
     * @var string
     * Organization
     */
    public $organization;

    /**
     * @var string
     * Normalized suffixes (jr., sr. or whatever)
     */
    public $suffix;

    /**
     * @var string
     * Prefix for last name (von, van, van der, or whatever)
     */
    public $lastNamePrefix;

    /**
     * @var string
     * Original string
     */
    public $originalString;

    // --------------------------------------------------------------

    /**
     * Map a suffix
     *
     * @param string $suffix  A common suffix (Jr., Sr. etc)
     * @return string         A normalized version of that suffix
     */
    public function mapSuffix($suffix)
    {
        //get the letters and numbers only
        preg_match("/[a-z0-9]/i", $suffix, $matches);
        $suffix = implode('', array_slice($matches, 1));

        $suffixMap = $this->getSuffixMap();
        return (isset($suffixMap[$suffix])) ? $suffixMap[$suffix] : false;
    }

    // --------------------------------------------------------------

    /**
     * Get an array of common suffixes
     *
     * @return array  Keys are normalized suffixes, values are proper suffixes
     */
    public function getSuffixMap()
    {
        return array(
            '2nd'   => '2nd',
            '3rd'   => '3rd',
            'phd'   => 'Ph.D.',
            'ms'    => 'M.S.',
            'ma'    => 'M.A.',
            'jr'    => 'Jr.',
            'sr'    => 'Sr.',
            'psyd'  => 'Psy.D.',
            'frcpi' => 'F.R.C.P.I.',
            '2'     => '2nd',
            'II'    => '2nd',
            'III'   => '3rd',
            'IV'    => '4th'
        );
    }

}

/* EOF: ContributorObject.php */