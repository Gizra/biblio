<?php


/**
 * Name Disambiguator
 *
 * @author Casey McLaughlin <caseyamcl@gmail.com>
 */

namespace Analyzer;

use Nametools\Normalize;

interface AnalyzerInterface
{
    /**
     * Constructor should optionally accept an injected normalizer
     * or set one up with the 'new' keyword in the constructor
     *
     * @param Normalize $normalizer
     */
    function __construct(Normalize $normalizer = null);

    // --------------------------------------------------------------

    /**
     * Analyze analyzes a string
     *
     * @param string $string
     *
     * @return Nametools\MatchObject
     */
    function analyze($string);
}

/* EOF: Analyzer.php */