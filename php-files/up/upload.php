#!/usr/bin/php-cgi
<?php
	$f = fopen( 'php://stdin', 'r' );

	while( $line = fgets( $f ) ) {
		echo $line;
		echo "</br>";
	}
	fclose( $f );

	if (isset($_POST)){
		echo "<h2>POST :</h2>";
		echo "<p>";
		var_dump($_POST);
		echo "</p>";
	}
	if (isset($_FILES)){
		echo "<h2>FILES :</h2>";
		echo "<p>";
		var_dump($_FILES);
		echo "</p>";
	}
?>