#!/usr/bin/php-cgi
<?php
    if (!isset($_SERVER['REQUEST_METHOD'])) {
        echo("Ho no, no request method :(");
    }
    else if ($_SERVER['REQUEST_METHOD'] == 'POST'
        && isset($_POST['first_name'])) {
        $first_name = $_POST['first_name'];
        echo($first_name);
    }
    else if ($_SERVER['REQUEST_METHOD'] == 'GET'
        && isset($_GET['first_name'])) {
        $first_name = $_GET['first_name'];
        echo($first_name);
    }
?>