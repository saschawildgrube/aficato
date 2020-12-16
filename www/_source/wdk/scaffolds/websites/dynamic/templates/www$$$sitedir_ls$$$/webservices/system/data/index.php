<?php
	
	error_reporting (E_ALL);
	ini_set('display_errors', 1);
	
	require_once ('../../../_source/env.inc');
	require_once (GetSourceDir().'webservices_directory.inc');
	
	$strServiceDir = GetWebservicesDir(). '/system/data/';
	$strServiceSourceDir = GetWDKDir().'webservices/system/data/';
	require_once ($strServiceSourceDir.'webservice_data.inc');
	
	$arrayConfig = array();
	$arrayConfig['protocols'] = array('http','https');
	$arrayConfig['admin_email'] = GetAdminEmail();
	$arrayConfig['webservices'] = GetWebservicesDirectory();
	$arrayConfig['max_content_size'] = 60000;
	
	$arrayParams = array();
	//$arrayParams ['trace'] = '1';
	
	$webservice = new CDataWebService(
		$strServiceSourceDir,
		$strServiceDir,
		$arrayConfig,
		$arrayParams);
		
