<?php

require_once(__DIR__ . '/../src/Nametools/MatchObject.php');
require_once(__DIR__ . '/../src/Nametools/Normalize.php');
require_once(__DIR__ . '/../src/Nametools/RegexCounter.php');
require_once(__DIR__ . '/../src/Nametools/RegexCounterException.php');

class NormalizeTest extends PHPUnit_Framework_TestCase
{
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
        $obj = new \Nametools\Normalize(new NormalizerTestMatchObject, new \Nametools\RegexCounter);
        $this->assertInstanceOf('\Nametools\Normalize', $obj);

    }

    // --------------------------------------------------------------

    public function testAddPatternAddsPattern()
    {
        $obj = new \Nametools\Normalize(new NormalizerTestMatchObject, new \Nametools\RegexCounter);
        $obj->addPattern("/^(.+?)\s(.+?)$/", array('name', 'email'));

        $shouldMatch = array("/^(.+?)\s(.+?)$/" => array('name', 'email'));
        $this->assertEquals($shouldMatch, $obj->getPatterns());
    }

    // --------------------------------------------------------------

    public function testAddPatternAppendsPatternByDefault()
    {
        $obj = new \Nametools\Normalize(new NormalizerTestMatchObject, new \Nametools\RegexCounter);
        $obj->addPattern("/^(.+?)\s(.+?)$/", array('name', 'email'));
        $obj->addPattern("/^(.+?)\s<(.+?)>$/", array('name', 'email'));

        $shouldMatch = array(
            "/^(.+?)\s(.+?)$/"   => array('name', 'email'),
            "/^(.+?)\s<(.+?)>$/" => array('name', 'email')
        );
        $this->assertEquals($shouldMatch, $obj->getPatterns());
    }

    // --------------------------------------------------------------

    public function testAppendPatternAppendsPattern()
    {
        $obj = new \Nametools\Normalize(new NormalizerTestMatchObject, new \Nametools\RegexCounter);
        $obj->appendPattern("/^(.+?)\s(.+?)$/", array('name', 'email'));
        $obj->appendPattern("/^(.+?)\s<(.+?)>$/", array('name', 'email'));

        $shouldMatch = array(
            "/^(.+?)\s(.+?)$/"   => array('name', 'email'),
            "/^(.+?)\s<(.+?)>$/" => array('name', 'email')
        );
        $this->assertEquals($shouldMatch, $obj->getPatterns());
    }

    // --------------------------------------------------------------

    public function testPrependPatternPrependsPattern()
    {
        $obj = new \Nametools\Normalize(new NormalizerTestMatchObject, new \Nametools\RegexCounter);
        $obj->prependPattern("/^(.+?)\s(.+?)$/", array('name', 'email'));
        $obj->prependPattern("/^(.+?)\s<(.+?)>$/", array('name', 'email'));

        $shouldMatch = array(
            "/^(.+?)\s<(.+?)>$/" => array('name', 'email'),
            "/^(.+?)\s(.+?)$/"   => array('name', 'email')
        );

        $this->assertEquals($shouldMatch, $obj->getPatterns());
    }

    // --------------------------------------------------------------

    public function testAddPatternWithImproperNumberOfMatchesThrowsException()
    {
        $this->setExpectedException("\InvalidArgumentException");

        $obj = new \Nametools\Normalize(new NormalizerTestMatchObject, new \Nametools\RegexCounter);
        $obj->prependPattern("/^(.+?)$/", array('name', 'email'));
    }

    // --------------------------------------------------------------

    public function testAddPatternWithImproperPropertyNamesThrowsException()
    {
        $this->setExpectedException("\InvalidArgumentException");

        $obj = new \Nametools\Normalize(new NormalizerTestMatchObject, new \Nametools\RegexCounter);
        $obj->prependPattern("/^(.+?)\s(.+?)$/", array('name', 'bloogedybloo'));
    }

    // --------------------------------------------------------------

    /**
     * Implictely tests the addPatterns() method
     */
    public function testConstructWithPatternsAddsPatterns()
    {
        $patternList = array(
            "/^(.+?)\s<(.+?)>$/" => array('name', 'email'),
            "/^(.+?)\s(.+?)$/"   => array('name', 'email')
        );

        $obj = new \Nametools\Normalize(new NormalizerTestMatchObject, new \Nametools\RegexCounter, $patternList);
        $this->assertEquals($patternList, $obj->getPatterns());
    }

    // --------------------------------------------------------------

    public function testNormalizeNormalizeMethodWorks()
    {
        $obj = new \Nametools\Normalize(new NormalizerTestMatchObject, new \Nametools\RegexCounter);
        $obj->addPattern("/^(.+?)\s(.+?)$/", array('name', 'email'));

        $result = $obj->normalize("John john@example.com");
        $toMatch = new \NormalizerTestMatchObject();
        $toMatch->name = "John";
        $toMatch->email = "john@example.com";

        $this->assertEquals($toMatch, $result);
    }

    // --------------------------------------------------------------

    public function testNormalizeMethodReturnsFalseForUnmatchedPattern()
    {
        $obj = new \Nametools\Normalize(new NormalizerTestMatchObject, new \Nametools\RegexCounter);
        $obj->addPattern("/^(.+?)\s(.+?)$/", array('name', 'email'));

        $result = $obj->normalize("John");
        $this->assertFalse($result);
    }

    // --------------------------------------------------------------

    public function testNormalizeMethodNormalizesWithArrayOfPatterns()
    {
        $obj = new \Nametools\Normalize(new NormalizerTestMatchObject, new \Nametools\RegexCounter);
        $obj->addPattern("/^EMAIL:(.+?)::NAME:(.+?)$/", array('email', 'name'));
        $obj->addPattern("/^(.+?)\s(.+?)$/", array('name', 'email'));

        $toMatch = new \NormalizerTestMatchObject();
        $toMatch->name = "John";
        $toMatch->email = "john@example.com";

        $result = $obj->normalize("John john@example.com");
        $this->assertEquals($toMatch, $result);

        $result = $obj->normalize("EMAIL:john@example.com::NAME:John");
        $this->assertEquals($toMatch, $result);
    }
}

// ==================================================================

class NormalizerTestMatchObject extends \Nametools\MatchObject
{
    public $name;
    public $email;
}

/* EOF: NormalizeTest.php */