<?php
if (getenv('REQUEST_METHOD') == 'POST'){

	echo getenv('CONTENT_LENGTH')."\n";
	echo var_dump($_POST);
	$f = fopen( 'php://stdin', 'r' );
	
	while( $line = fgets( $f ) ) {
		echo $line;
	}
	
	fclose( $f );
}
else
	echo var_dump($_GET);
