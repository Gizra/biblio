<?php

function adAutoload($classname)
{
    $ds = DIRECTORY_SEPARATOR;

    $classname = ltrim($classname, '\\');
    $filename = '';

    if ($lnp = strripos($classname, '\\')) {
            $namespace = substr($classname, 0, $lnp);
            $classname = substr($classname, $lnp + 1);
            $filename = str_replace('\\', $ds, $namespace) . $ds;
    }

    $filename .= str_replace('_', $ds, $classname) . '.php';

    $fullpath = __DIR__ . $ds . 'src' . $ds . $filename;

    if (is_readable($fullpath)) {
        require_once($fullpath);
        return;
    }
}

/* EOF: autoload.php */