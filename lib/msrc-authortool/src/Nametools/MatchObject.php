<?php

/**
 * Name Disambiguator
 *
 * @author Casey McLaughlin <caseyamcl@gmail.com>
 */

namespace Nametools;

/**
 * MatchObject Abstract Class
 */
abstract class MatchObject
{
    // --------------------------------------------------------------

    /**
     * Factory method
     *
     * @return MatchObject
     */
    public static function factory()
    {
        $classname = get_called_class();
        return new $classname;
    }

    // --------------------------------------------------------------

    /**
     * Set magic method prevents setting undefined variables
     *
     * @param string $item
     * @param string $value
     */
    public function __set($item, $val)
    {
        if ( ! in_array($item, array_keys(get_object_vars($this)))) {
            throw new \InvalidArgumentException("Undefined Property: {$item}");
        }
    }

    // --------------------------------------------------------------

    /**
     * To string magic method
     *
     * @return string
     */
    public function __toString()
    {
        return json_encode(get_object_vars($this));
    }

}

/* EOF: MatchObject.php */