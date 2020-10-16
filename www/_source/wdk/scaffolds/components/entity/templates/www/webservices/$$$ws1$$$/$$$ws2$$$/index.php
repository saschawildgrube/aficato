<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);

	require_once ("../../../_source/env.inc");
	
	$strServiceDir = GetWebservicesDir(). "$$$webservicename$$$/";
	$strServiceSourceDir = GetSourceDir()."assemblies/$$$a$$$/webservices/$$$webservicename$$$/";
	require_once ($strServiceSourceDir."webservice_$$$ws2$$$.inc");
	
	require_once (GetSourceDir()."webservices_directory.inc");
	
	$config = new CConfig();
	$config->AddConfigStoreLocation(GetConfigDir());
	$config->LoadConfig(GetDatabaseConfigID());

	/*
		The config array contains the configuration of a web service.
		Some settings are optional other are crucial for operations.
	*/ 
	$arrayConfig = array();	
	$arrayConfig["database"] = $config->GetDataArray();
	$arrayConfig["protocols"] = array("http","https");
	
	/*
		GetWebservicesDirectory() is usually implemented in web services_directory.inc.
		It returns a configuration array with details on web services contained
		in this application system.
	*/
	$arrayConfig["webservices"] = GetWebservicesDirectory();
	
	/*
		Any notification mails will be sent to this email address:
	*/
	$arrayConfig["admin_email"] = GetAdminEmail();
		
	/*
		Accesscodes protect web services from unauthorized use.
		A client must provide one out of many defined access codes.
		In this case we get the accesscode from the web service directory
		which is shared between website and web service source code.
	*/
	$arrayConfig["accesscodes"] = array($arrayConfig["webservices"]["$$$webservicename$$$"]["accesscode"]);
	
	/*
		You can specify a load tolerance for every single command of a web service.
		If a load tolerance is set for a command, it will only be executed if
		the one minute average load is below tolerance.
		This allows to create a balance between crucial activities and resource
		consumption on the server.
	*/
	$arrayConfig["load_tolerance"] = array("cleanup" => 3.0);
	
	/*
		Define which event severity level justifies to send an email each
		time the event is triggered. Anything more verbose than SEVERITY_ERROR
		(e.g. SEVERITY_WARNING) may spam the administrator's inbox.
	*/
	$arrayConfig["email_alert_severity_threshold"] = SEVERITY_ERROR;
	
	/*
		The parameter array can be used to immitate parameters which would
		otherwise be provided via GET or POST.
		For example, you could create versions of the web service index.php that
		always execute a specific command.
		The example below would always generate debug output no matter if the
		parameter "trace" is set to "1" or not.
	*/
	$arrayParams = array();
	//$arrayParams ["trace"] = "1";
	
	/*
		Creating an instance of the web service class triggers everything
		necessary to operate the web service - i.e. to respond to requests.
	*/
	$webservice = new C$$$EntityName$$$WebService(
		$strServiceSourceDir,
		$strServiceDir,
		$arrayConfig,
		$arrayParams);
		
