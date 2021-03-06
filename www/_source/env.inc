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
		return $strDocRoot.'';
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
		return $_SERVER['SERVER_NAME'].'/';	
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
		return 'sascha@wildgrube.com';	
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
		if (FindString(GetRootURL(),'aficato.com') != -1)
		{
			return 'prod';
		}
		
		return '';	
	}
	
	function GetEnvConfigID()
	{
		$strEnvConfigID = 'env';
		if (GetEnvID() != '')
		{
			$strEnvConfigID .= '-'.GetEnvID();
		}
		return $strEnvConfigID;
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
		return '/tmp/';	
	}

	function GetErrorLogPath()
	{
		if (GetEnvID() == 'dev')
		{
			return GetDocumentRootDir().'../../logs-dev/error_log';
		}
		return GetDocumentRootDir().'../../logs/error_log';	
	}


