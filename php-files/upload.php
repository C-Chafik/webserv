#!/usr/bin/php-cgi
<?php
	// $f = fopen( 'php://stdin', 'r' );

	// while( $line = fgets( $f ) ) {
	// echo $line;
	// }

	// fclose( $f );
	var_dump($_ENV).'\n';
	var_dump($_POST).'\n';
?>