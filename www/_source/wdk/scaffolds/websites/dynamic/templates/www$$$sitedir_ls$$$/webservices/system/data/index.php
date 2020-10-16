<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);
	
	require_once ("../../../_source/env.inc");
	require_once (GetSourceDir()."webservices_directory.inc");
	
	$strServiceDir = GetWebservicesDir(). "/system/data/";
	$strServiceSourceDir = GetWDKDir()."webservices/system/data/";
	require_once ($strServiceSourceDir."webservice_data.inc");
	
	$config = new CConfig();
	$config->AddConfigStoreLocation(GetConfigDir());
	$config->LoadConfig(GetDatabaseConfigID());

	 
	$arrayConfig = array();	
	$arrayConfig["database"] = $config->GetDataArray();
	$arrayConfig["protocols"] = array("http","https");
	$arrayConfig["admin_email"] = GetAdminEmail();
	$arrayConfig["webservices"] = GetWebservicesDirectory();
	$arrayConfig["accesscodes"] = array($arrayConfig["webservices"]["system/data"]["accesscode"]);
	
	$arrayConfig["max_content_size"] = 60000;
	
	 
	$arrayParams = array();
	//$arrayParams ["trace"] = "1";
	
	$webservice = new CDataWebService(
		$strServiceSourceDir,
		$strServiceDir,
		$arrayConfig,
		$arrayParams);
		
