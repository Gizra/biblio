<?php

require_once(__DIR__ . '/../src/Nametools/MatchObject.php');

class MatchObjectTest extends PHPUnit_Framework_TestCase
{
    // --------------------------------------------------------------

    public function testInstantiateAsObjectSucceeds()
    {
        $obj = new TestMatchObject();
        $this->assertInstanceOf('\Nametools\MatchObject', $obj);
    }

    // --------------------------------------------------------------

    public function testSetPropertiesWorksForExistingProperties()
    {
        $obj = new TestMatchObject();
        $obj->name = "Bob";

        $this->assertEquals("Bob", $obj->name);
    }

    // --------------------------------------------------------------

    public function testSetPropertiesFailsForNonexistentProperties()
    {
        $this->setExpectedException("\InvalidArgumentException");
        $obj = new TestMatchObject();
        $obj->doesNotExist = 'abc';
    }

    // --------------------------------------------------------------

    public function testCastAsStringReturnsJson()
    {
        $obj = new TestMatchObject();
        $obj->name = "Bob";
        $obj->email = "bob@example.com";

        $toMatch = '{"name":"Bob","email":"bob@example.com"}';
        $this->assertEquals($toMatch, $obj);
    }

    // --------------------------------------------------------------

    public function testFactoryReturnsObject()
    {
        $obj = new TestMatchObject();
        $classname = get_class($obj);
        $newobj = $classname::factory();

        $this->assertInstanceOf('TestMatchObject', $newobj);
    }
}

// ==================================================================

class TestMatchObject extends \Nametools\MatchObject
{
    public $name;
    public $email;
}

/* EOF: MatchObjectTest.php */