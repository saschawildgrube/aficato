<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);

	require_once ("../../../_source/env.inc");
	require_once (GetSourceDir()."webservices_directory.inc");
	
	$strServiceDir = GetWebservicesDir(). "/system/server/";
	$strServiceSourceDir = GetWDKDir()."webservices/system/server/";
	require_once ($strServiceSourceDir."webservice_server.inc");	
	
	
	$arrayConfig = array();
	$arrayConfig["protocols"] = array("http","https");
	$arrayConfig["admin_email"] = GetAdminEmail();
	$arrayConfig["webservices"] = GetWebservicesDirectory();
	$arrayConfig["accesscodes"] = array($arrayConfig["webservices"]["system/server"]["accesscode"]);
	
	$arrayParams = array();

	$webservice = new CServerWebService(
		$strServiceSourceDir,
		$strServiceDir,
		$arrayConfig,
		$arrayParams);
	
