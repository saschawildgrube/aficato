<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);

	require_once ("../../../_source/env.inc");
	require_once (GetSourceDir()."webservices_directory.inc");
	
	$strServiceDir = GetWebservicesDir(). "/system/test/";
	$strServiceSourceDir = GetWDKDir()."webservices/system/test/";
	require_once ($strServiceSourceDir."webservice_test.inc");

	$config = new CConfig();
	$config->AddConfigStoreLocation(GetConfigDir());
	$config->LoadConfig(GetDatabaseConfigID());

	$arrayConfig = array();	
	$arrayConfig["protocols"] = array("http","https");
	$arrayConfig["webservices"] = GetWebservicesDirectory();
	$arrayConfig["accesscodes"] = array($arrayConfig["webservices"]["system/test"]["accesscode"]);
	$arrayConfig["admin_email"] = GetAdminEmail();
	$arrayConfig["database"] = $config->GetDataArray();
	
	$arrayConfig["load_tolerance"] = array("testrun" => 3.0);

	$arrayConfig["database_support"] = true;

	$arrayConfig["alert_email"] = GetAdminEmail();
	$arrayConfig["tests_url"] = "http://".GetRootURL()."quality/";

 	$arrayConfig["status_history"]["active"] = true;
 	$arrayConfig["status_history"]["log"] = true; 
 	$arrayConfig["status_history"]["status_storagetime_hours"] = 48;
 	$arrayConfig["status_history"]["log_storagetime_hours"] = 3;
 	
 	$arrayConfig["test"]["request_timeout"] = 45;
 	$arrayConfig["test"]["max_attempts"] = 3;
 	
 	$arrayConfig["testrun"]["max_seconds"] = 10;    
 	$arrayConfig["testrun"]["max_tests"] = 2;
 	$arrayConfig["testrun"]["retry_on_request_error"] = 2; 

 	$arrayConfig["exclude"]["tests"] = array("server-status");  
 	
	$arrayParams = array();
	//$arrayParams ["trace"] = "1";
	
	$webservice = new CTestWebService(
		$strServiceSourceDir,
		$strServiceDir,
		$arrayConfig,
		$arrayParams);

