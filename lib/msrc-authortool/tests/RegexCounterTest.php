<?php

require_once(__DIR__ . '/../src/Nametools/RegexCounter.php');
require_once(__DIR__ . '/../src/Nametools/RegexCounterException.php');

class RegexCounterTest extends PHPUnit_Framework_TestCase
{
    // --------------------------------------------------------------

    /**
     * @dataProvider provider
     */
    public function testCountGroupsReturnsExpectedResults($pattern, $expectedResult)
    {
        $obj = new \Nametools\RegexCounter();
        $this->assertEquals($expectedResult, $obj->countGroups($pattern, $dummy, $dummy));
    }

    // --------------------------------------------------------------

    /**
     * @dataProvider provider
     */
    public function testCountReturnsExpectedResults($pattern, $expectedResult)
    {
        $obj = new \Nametools\RegexCounter();
        $this->assertEquals($expectedResult, $obj->count($pattern));
    }

    // --------------------------------------------------------------

    public function provider()
    {
        return array(
            array('the ((?:red|white) (king|queen))', 2),
            array('(?<date>(?<year>(\d\d)?\d\d)-(?<month>\d\d)-(?<day>\d\d))', 9),
            array('(?J:(?<DN>Mon|Fri|Sun)(?:day)?|(?<DN>Tue)(?:sday)?|(?<DN>Wed)(?:nesday)?|(?<DN>Thu)(?:rsday)?|(?<DN>Sat)(?:urday)?)', 6),
            array('(?|(Sat)ur|(Sun))day', 1)
        );
    }
}

/* EOF: RegexCounterTest.php */