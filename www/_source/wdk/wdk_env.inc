<?php

	function CheckEnvironment()
	{
		if (!function_exists('GetDocumentRootDir'))
		{
			return false;	
		}
		if (!function_exists('GetSourceDir'))
		{
			return false;	
		}
		if (!function_exists('GetWDKDir'))
		{
			return false;	
		}
		if (!function_exists('GetConfigDir'))
		{
			return false;	
		}
		if (!function_exists('GetWebservicesDir'))
		{
			return false;	
		}
		if (!function_exists('GetQualityDir'))
		{
			return false;	
		}
		if (!function_exists('GetRootURL'))
		{
			return false;	
		}
		if (!function_exists('GetWebservicesURL'))
		{
			return false;	
		}
		if (!function_exists('GetAdminEmail'))
		{
			return false;	
		}
		if (!function_exists('GetSystemEmail'))
		{
			return false;	
		}
		if (!function_exists('GetSystemEmailSenderName'))
		{
			return false;	
		}
		if (!function_exists('GetMailDomain'))
		{
			return false;	
		}		
		if (!function_exists('GetEnvID'))
		{
			return false;	
		}		
		if (!function_exists('GetEnvConfigID'))
		{
			return false;	
		}		
		if (!function_exists('GetOperationMode'))
		{
			return false;	
		}		
		if (!function_exists('GetGpgPath'))
		{
			return false;	
		}		
		if (!function_exists('GetTempDir'))
		{
			return false;	
		}		
		return true;
	}
	
	function CheckSafeMode($bSafeModeOffRequired = false)
	{
		$bValue = GetBoolValue(ini_get('safe_mode'));
		if ($bSafeModeOffRequired == true && $bValue == true)
		{
			FatalError('Safe Mode is required to be off but not configured.');	
		}
		return $bValue;
	}
	
	function GetMemoryLimit()
	{
		return GetIntegerValue(ini_get('memory_limit'));	
	}

	function GetMaxExecutionTime()
	{
		return GetIntegerValue(ini_get('max_execution_time'));	
	}
	
	function InitIsFeatureActive()
	{
		if (!function_exists('IsFeatureActive'))
		{
			function IsFeatureActive($strFeature)
			{
				return true;
			}
		}		
	}


