<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);
	
	require_once ("../../../_source/env.inc");
	require_once (GetSourceDir()."webservices_directory.inc");
	
	$strServiceDir = GetWebservicesDir(). "/system/entitlement/";
	$strServiceSourceDir = GetWDKDir()."webservices/system/entitlement/";
	require_once ($strServiceSourceDir."webservice_entitlement.inc");

	$config = new CConfig();
	$config->AddConfigStoreLocation(GetConfigDir());
	$config->LoadConfig(GetDatabaseConfigID());

	 
	$arrayConfig = array();	
	$arrayConfig["database"] = $config->GetDataArray();
	$arrayConfig["protocols"] = array("http","https");
	$arrayConfig["admin_email"] = GetAdminEmail();
	$arrayConfig["webservices"] = GetWebservicesDirectory();
	$arrayConfig["accesscodes"] = array($arrayConfig["webservices"]["system/entitlement"]["accesscode"]);
	
	$arrayParams = array();
	//$arrayParams ["trace"] = "1";
	
	$webservice = new CEntitlementWebService(
		$strServiceSourceDir,
		$strServiceDir,
		$arrayConfig,
		$arrayParams);
		
