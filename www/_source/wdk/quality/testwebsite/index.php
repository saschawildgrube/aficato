<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);

	if (!function_exists("GetWDKDir"))
	{
		print("You must include an env.inc in your index.php");
		exit();
	}
	
	require_once (GetSourceDir()."website_test.inc");
		
	$arrayParams = array();
	//$arrayParams ["trace"] = "1";
	//$arrayParams ["language"] = "en";
	$website = new CTestWebSite($arrayParams);
		
