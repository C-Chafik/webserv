<?php
	print "Bonjour, ";
	echo getenv('name')."\n";
	echo getenv('QUERY_STRING')."\n";
	echo getenv('CONTENT_LENGTH')."\n";
	$_SERVER['QUERY_STRING']
?>