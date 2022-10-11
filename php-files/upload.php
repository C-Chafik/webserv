<?php
	// echo "test";
	echo file_get_contents("/tmp/cgi_post.log");
	echo var_dump($_POST);