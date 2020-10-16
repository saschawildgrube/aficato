<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);

	require_once ("../../../_source/env.inc");
	require_once (GetSourceDir()."webservices_directory.inc");
	
	$strServiceDir = GetRootURL()."/quality/testsite/";
	$strServiceSourceDir = GetWDKDir()."webservices/system/test/";
	require_once ($strServiceSourceDir."webservice_test.inc");


	$arrayConfig = array();	
	$arrayConfig["protocols"] = array("http","https");
	$arrayConfig["tests_url"] = "http://".GetRootURL()."quality/testsite/testhub/";
	$arrayConfig["webservices"] = GetWebservicesDirectory();
	$arrayConfig["accesscodes"] = array("1");	
	$arrayConfig["exclude"]["assemblies"] = array("wdk");
	
	$arrayConfig["database_support"] = false; 
 
 	$arrayParams = array();
	//$arrayParams ["trace"] = "1";
	
	$webservice = new CTestWebService(
		$strServiceSourceDir,
		$strServiceDir,
		$arrayConfig,
		$arrayParams);

