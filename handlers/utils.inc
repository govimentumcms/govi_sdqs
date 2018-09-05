<?php
function splitName($name) {
    $parts = explode(' ', $name);
    return array(
        'firstpart' => array_shift($parts),
        'secondpart' => join(' ', $parts)
    );
}
