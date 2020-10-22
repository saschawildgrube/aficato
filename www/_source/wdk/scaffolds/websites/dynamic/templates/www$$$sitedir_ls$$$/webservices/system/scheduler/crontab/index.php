<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);
	
	require_once ("../../../../_source/env.inc");
	require_once (GetSourceDir()."webservices_directory.inc");
	
	$strServiceDir = GetWebservicesDir(). "/system/scheduler/";
	$strServiceSourceDir = GetWDKDir()."webservices/system/scheduler/";
	require_once ($strServiceSourceDir."webservice_scheduler.inc");

	$arrayConfig = array();
	$arrayConfig['accesscodes'] = false;
	$arrayConfig["protocols"] = array("http","https");
	$arrayConfig["admin_email"] = GetAdminEmail();
	$arrayConfig["webservices"] = GetWebservicesDirectory();
	$arrayConfig["load_tolerance"] = array("crontab" => 3.0);
	$arrayConfig["crontab_heartbeat"] = true; 
	$arrayConfig["disable_log"] = true;
	$arrayConfig["max_timeout"] = 90;

	$arrayParams = array();
	$arrayParams["trace"] = 'false';
	$arrayParams["command"] = "crontab";
	
	$webservice = new CSchedulerWebService(
		$strServiceSourceDir,
		$strServiceDir,
		$arrayConfig,
		$arrayParams);
		
		
