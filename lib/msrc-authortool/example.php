<?php

/*
 * This file contains an example of how to integrate the
 * author disambiguate tool with Drupal a migrate script
 */

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//1. Include and register the autoloader
//
require_once('autoload.php');
spl_autoload_register('adAutoload', true, true);


//2. Load an array of 'organization' words
//   Here is a list of ones that works well on our
//   current database
//
$orgWords = array(
    'Midatlantic',
    'of',
    'VA',
    'study',
    'trial',
    'the',
    'for',
    'Disorder',
    'Millennium',
    'Millenium',
    'Cohort',
    'Organization',
    'Org',
    'Division',
    'Center',
    'Centers',
    'Health',
    'Group',
    'Team',
    'Agency',
    'Trust',
    'Prevention',
    'Survey',
    'Grp',
    'Force',
    'Inst',
    'Institute',
    'Office',
    'Consortium',
    'STATA',
    'Depression',
    'Support'
);


//3. Load up the necessary libraries
//
$analyzer = new \Analyzer\ContributorNames();
$analyzer->setOrgWords($orgWords);


//4. For each author, run the $analyzer->analyze() method...
//
//   This method works well on the biblio_contributor_data.name
//   field in the overlord database...
//
$authorObject  = $analyzer->analyze('Bob Jones, M.D.');
$anotherAuthor = $analyzer->analyze('Jr., Thomas E. Joiner');
$corpAuth      = $analyzer->analyze('Association of Research');
var_dump($corpAuth);


//5. Do what you will with the output objects...
//
//   Presumably map them to the new Biblio Contributor entities


/* EOF: example.php */