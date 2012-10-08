<?php

//Will use analyzer
use Analyzer\ContributorNames;

//
// Autoloader
//
require_once(__DIR__ . '/../autoload.php');
spl_autoload_register('adAutoload', true, true);

//
// Load up the file data
//
$authorList = file('authors.txt', FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
$orgWords   = file('orgwords.txt', FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);


//
// Load up the Analyzer
//
$analyzer = new ContributorNames();
$analyzer->setOrgWords($orgWords);

//
// Reorder by line length for easier debugging the skipped.txt output
//
usort($authorList, function($a, $b) {
    if (strlen($a) == strlen($b)) {
        $testArr = array($a, $b);
        $test2Arr = $testArr;
        sort($test2Arr);
        return ($test2Arr == $testArr) ? 1 : -1;
    }
    else {
        return (strlen($a) > strlen($b)) ? -1 : 1;
    }
});


//
// Out files
//
file_put_contents('output/found.txt', '');
file_put_contents('output/skipped.txt', '');

//
// Stats
//
$numFound = 0;;
$numSkipped = 0;

//
// Main Loop
//
foreach($authorList as $author) {

    $result = $analyzer->analyze($author);

    if ($result instanceOf \Analyzer\ContributorObject) {
        file_put_contents('output/found.txt', (string) $result . "\n", FILE_APPEND);
        $numFound++;
    }
    else {
        file_put_contents('output/skipped.txt', $author . "\n", FILE_APPEND);
        $numSkipped++;
    }
}

//Report
printf("\n\nDone.\n%d | Num Found: %s | Num Skipped: %s\n\n", time(), number_format($numFound, 0), number_format($numSkipped, 0));


/* EOF: analyze.php */