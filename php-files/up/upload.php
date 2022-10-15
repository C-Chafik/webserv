#!/usr/bin/php-cgi
<?php
	$f = fopen( 'php://stdin', 'r' );

	while( $line = fgets( $f ) ) {
		echo $line;
		echo "</br>";
	}
	fclose( $f );

	echo "*******************</br>";
	var_dump($_POST);
	echo "</br>";
	var_dump($_FILES);
	echo "</br>";
	echo "*******************</br>";
?>