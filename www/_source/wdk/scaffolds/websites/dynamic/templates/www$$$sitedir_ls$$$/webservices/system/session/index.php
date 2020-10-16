<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);
	
	require_once ("../../../_source/env.inc");
	require_once (GetSourceDir()."webservices_directory.inc");
	
	$strServiceDir = GetWebservicesDir(). "/system/session/";
	$strServiceSourceDir = GetWDKDir()."webservices/system/session/"; 
	require_once ($strServiceSourceDir."webservice_session.inc");
	
	$config = new CConfig();
	$config->AddConfigStoreLocation(GetConfigDir());
	$config->LoadConfig(GetDatabaseConfigID());

	 
	$arrayConfig = array();	
	$arrayConfig["database"] = $config->GetDataArray();
	$arrayConfig["protocols"] = array("http","https");
	$arrayConfig["admin_email"] = GetAdminEmail();
	$arrayConfig["webservices"] = GetWebservicesDirectory();
	$arrayConfig["accesscodes"] = array($arrayConfig["webservices"]["system/session"]["accesscode"]);

	
	$arrayConfig["duration_max"] = intval(60 * 60 * 24); // Sessions may last 24 hours.
	//$arrayConfig["duration_max"] = intval(60 * 60 * 12); // Sessions may last 12 hours.
	$arrayConfig["data_maxlen"] = 65535; // Each session can handle 64 K bytes
	
	
	$arrayParams = array();
	//$arrayParams ["trace"] = "1";
	
	$webservice = new CSessionWebService(
		$strServiceSourceDir,
		$strServiceDir,
		$arrayConfig,
		$arrayParams);
