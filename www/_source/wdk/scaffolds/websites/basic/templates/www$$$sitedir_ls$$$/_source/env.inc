<?php

	require_once('features.inc');

	function GetDocumentRootDir()
	{
		$strDocRoot = $_SERVER['DOCUMENT_ROOT'];
		/*
			Unfortunately some web servers add the trailing slash to the document root
			path while others do not. The code below makes sure this works in all cases.
			However, once you found out how your server is configured, you should remove
			this code.
		*/
		if (substr($strDocRoot,-1,1) !=  '/')
		{
			$strDocRoot.='/';
		}
		return $strDocRoot.'$$$sitedir$$$';
	}

	function GetSourceDir()
	{
		return GetDocumentRootDir().'_source/';
	}
	
	function GetWDKDir()
	{
		return GetSourceDir().'wdk/';
	}
	
	function GetConfigDir()
	{
		return GetSourceDir();	
	}

	function GetWebservicesDir()
	{
		return GetDocumentRootDir() . 'webservices/';
	}
	
	function GetQualityDir()
	{
		return GetDocumentRootDir() . 'quality/';
	}
	
	function GetRootURL()
	{
		return $_SERVER['SERVER_NAME'].'/$$$sitedir$$$';	
		/*
			If there is no dev environment this could be used, too:		
			return '$$$rooturl$$$/$$$sitedir$$$';
		*/
	}

	function GetWebservicesURL()
	{
		return GetRootURL().'webservices/';	
	}
	
	function GetWebservicesDefaultFormat()
	{
		return 'json';	
	}

	function GetAdminEmail()
	{
		return '$$$adminemail$$$';	
	}
	
	function GetSystemEmail()
	{
		return 'noreply@'.GetMailDomain();	
	}

	function GetSystemEmailSenderName()
	{
		return GetRootURL().' ('.GetEnvID().')';	
	}


	function GetMailDomain()
	{
		$strMailHost = $_SERVER['SERVER_NAME'];
		$strMailHost = ReplaceString($strMailHost,'www.','');
		$strMailHost = ReplaceString($strMailHost,'dev.','');
		return $strMailHost;	
	}

	
	function GetEnvID()
	{
		if (FindString(GetRootURL(),'dev.') == 0)
		{
			return 'dev';
		}
		return 'prod';	
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
		return '';
	}
	
	function GetTempDir()
	{
		return '$$$tmpdir$$$';	
	}

	function GetErrorLogPath()
	{
		/*
			This will work in some cases but not for all.
			Find out the real location of the error_log
			or return an empty string.
		*/
		return GetDocumentRootDir().'../../logs/error_log';	
	}


