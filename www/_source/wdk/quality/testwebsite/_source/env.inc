<?php

	/*
	
	If the path settings in this env.inc file do not fit your environment,
	make a copy into your code space and include it from there.
	
	Do not make changes here as this file will be replaced when you update your WDK
	files.
		
	*/


	function GetDocumentRootDir()
	{
		return $_SERVER["DOCUMENT_ROOT"] . "quality/testwebsite/";	
	}

	function GetWDKDir()
	{
		return $_SERVER["DOCUMENT_ROOT"]. "_source/wdk/";
	}

	function GetSourceDir()
	{
		return GetWDKDir()."quality/testwebsite/_source/";
	}
	

	
	function GetConfigDir()
	{
		return GetSourceDir();	
	}

	function GetWebservicesDir()
	{
		return GetDocumentRootDir() . "webservices/";
	}
	
	function GetQualityDir()
	{
		return GetDocumentRootDir() . "quality/";
	}
	
	function GetRootURL()
	{
		return $_SERVER["SERVER_NAME"]."/quality/testwebsite/";	
	}

	function GetWebservicesURL()
	{
		return GetRootURL()."webservices/";	
	}

	function GetAdminEmail()
	{
		return "admin@".GetMailDomain();	
	}
	
	function GetSystemEmail()
	{
		return "noreply@".GetMailDomain();	
	}

	function GetSystemEmailSenderName()
	{
		return GetRootURL()." (".GetEnvID().")";	
	}


	function GetMailDomain()
	{
		$strMailHost = $_SERVER["SERVER_NAME"];
		$strMailHost = ReplaceString($strMailHost,"www.","");
		return $strMailHost;	
	}

	
	function GetEnvID()
	{
		return "prod";	
	}
	
	function GetEnvConfigID()
	{
		return 'env';
	}
	
	function GetOperationMode()
	{
		return 'normal';	
	}
	
	function GetGpgPath()
	{
		return "";
	}
	
	function GetTempDir()
	{
		return "";
	}

	function GetErrorLogPath()
	{
		return "";	
	}


