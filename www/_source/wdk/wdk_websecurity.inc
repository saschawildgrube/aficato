<?php

	function SecureOutput($var)
	{
		if (is_array($var))
		{
			$arrayReturn = array();
			foreach ($var as $key => $value)
			{
				$arrayReturn[$key] = SecureOutput($value);	
			}
			return $arrayReturn;
		}
		return HtmlEncode($var);	
	}

	function CheckParameterName($strParamName)
	{
		if (StringCheckCharSet($strParamName,CHARSET_PARAMETERNAME) == false)
		{
			return false;
		}
		return true;
	}


	function SecureParameterArray($arrayParams)
	{
		$arrayParamsChecked = array();
		foreach ($arrayParams as $key => $value)
		{
			if (is_scalar($value))
			{
				if (CheckParameterName($key) == true)
				{
					$arrayParamsChecked[$key] = $value;
				}
			}			
		}
		return $arrayParamsChecked;
	}
	
	function LockParameterSources()
	{
		unset($_GET);
		unset($_POST);
		unset($_REQUEST);
		unset($_COOKIE);
		unset($GLOBALS["_POST"]);
		unset($GLOBALS["_GET"]);
	}



