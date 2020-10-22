<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);

	require_once ("../../../_source/env.inc");
	require_once (GetSourceDir()."webservices_directory.inc");
	
	$strServiceDir = GetWebservicesDir(). "/system/log/";
	$strServiceSourceDir = GetWDKDir()."webservices/system/log/";
	require_once ($strServiceSourceDir."webservice_log.inc");
	
	$arrayConfig = array();
	$arrayConfig["protocols"] = array("http","https");
	$arrayConfig["admin_email"] = GetAdminEmail();
	$arrayConfig["webservices"] = GetWebservicesDirectory();
	
	//$arrayConfig["hashsink_email"] = GetAdminEmail();
	//$arrayConfig["logmail_active"] = true; 
	//$arrayConfig["logmail_email"] = GetAdminEmail();
	

	// Whitelists
	
	$arrayConfig["severity_whitelist"] = array(
		"EMERGENCY",
		"ALERT",
		"CRITICAL",
		"ERROR",
		"WARNING",
		"NOTICE",
		"INFORMATIONAL",
		"DEBUG");
		
	$arrayConfig["reporterid_whitelist"] = array(
		"WWW",
		"SYSTEM/SCHEDULER",
		"SYSTEM/USER",
		"SYSTEM/TEST");
		
	$arrayConfig["eventid_whitelist"] = array(
		//"DOTEST",
		"CRONTAB",		
		"REQUEST",
		"LOGIN",
		"LOGOUT"
		);
		
	
	
	
	// Blacklists (override whitelists)
	
	$arrayConfig["severity_blacklist"] = array(
		);
	
	$arrayConfig["reporterid_blacklist"] = array(
		);	
	
	$arrayConfig["eventid_blacklist"] = array(
		"SESSION_CREATE");	
			
			
	// For testing purposes
	$arrayConfig["severity_whitelist"][] = "TESTWHITELIST";
	$arrayConfig["severity_whitelist"][] = "TEST";
	$arrayConfig["severity_blacklist"][] = "TESTBLACKLIST";
	$arrayConfig["reporterid_whitelist"][] = "TEST";
	$arrayConfig["reporterid_blacklist"][] = "TESTBLACKLIST";
	$arrayConfig["eventid_whitelist"][] = "TEST";
	$arrayConfig["eventid_blacklist"][] = "TESTBLACKLIST";
	
	
	$arrayParams = array();
	//$arrayParams ["trace"] = "1";
	
	$webservice = new CLogWebService(
		$strServiceSourceDir,
		$strServiceDir,
		$arrayConfig,
		$arrayParams);
		
