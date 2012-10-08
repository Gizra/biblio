<?php

require_once(__DIR__ . '/../src/Nametools/MatchObject.php');
require_once(__DIR__ . '/../src/Analyzer/ContributorObject.php');

class ContributorObjectTest extends PHPUnit_Framework_TestCase {

    // --------------------------------------------------------------

    function setUp()
    {
        parent::setUp();
    }

    // --------------------------------------------------------------

    function tearDown()
    {
        parent::tearDown();
    }

    // --------------------------------------------------------------

    public function testInstantiateAsObjectSucceeds()
    {
        $obj = new \Analyzer\ContributorObject();
        $this->assertInstanceOf('\Analyzer\ContributorObject', $obj);
    }

    // --------------------------------------------------------------

    public function testSetPropertiesWorksForExistingProperties()
    {
        $obj = new \Analyzer\ContributorObject();
        $obj->firstName           = 'Bob';
        $obj->lastName            = 'Jones';
        $obj->middleName          = 'Roy';
        $obj->firstInitial        = 'B';
        $obj->middleInitial       = 'R';
        $obj->organization        = 'Some Place';
        $obj->suffix              = "Jr.";
        $obj->secondMiddleInitial = 'N';
        $obj->lastNamePrefix      = null;

        $checkArray = array(
            'firstName'           => 'Bob',
            'lastName'            => 'Jones',
            'middleName'          => 'Roy',
            'firstInitial'        => 'B',
            'middleInitial'       => 'R',
            'organization'        => 'Some Place',
            'suffix'              => 'Jr.',
            'secondMiddleInitial' => 'N',
            'lastNamePrefix'      => null,
            'originalString'      => null
        );

        $this->assertEquals($checkArray, get_object_vars($obj));
    }

    // --------------------------------------------------------------

    public function testSetPropertiesFailsForNonexistentProperties()
    {
        $this->setExpectedException("\InvalidArgumentException");
        $obj = new \Analyzer\ContributorObject();
        $obj->doesNotExist = 'abc';
    }
}

/* EOF: ContributorObjectTest.php */