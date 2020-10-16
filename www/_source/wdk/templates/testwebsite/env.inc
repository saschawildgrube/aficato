<?php


	function GetDocumentRootDir()
	{
		return $_SERVER["DOCUMENT_ROOT"] . "/quality/testwebsite/";	
	}

	function GetSourceDir()
	{
		return $_SERVER["DOCUMENT_ROOT"]. "/_source/wdk/quality/testwebsite/_source/";
	}
	
	function GetWDKDir()
	{
		return $_SERVER["DOCUMENT_ROOT"]. "/_source/wdk/";
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
		return GetRootURL()." (".GetStage().")";	
	}


	function GetMailDomain()
	{
		$strMailHost = $_SERVER["SERVER_NAME"];
		$strMailHost = ReplaceString($strMailHost,"www.","");
		return $strMailHost;	
	}

	
	function GetStage()
	{
		return "prod";	
	}
	
	function GetDatabaseConfigID()
	{
		return "";
	}
	
	function GetOperationMode()
	{
		return "normal";	
	}
	
	function GetGpgPath()
	{
		return "";
	}
	
	function GetTempDir()
	{
		return "";
	}




