<?php

	// Static calls share a PHP process; headers belong to the outer request only.
	class CWebServiceTransportContext
	{
		public static $nDepth = 0;
	}

	function WebServiceTransportHeader($strHeader)
	{
		if (CWebServiceTransportContext::$nDepth == 0)
		{
			header($strHeader);
		}
	}

	function GetStaticWebserviceEntryPoint($strURL)
	{
		if (!function_exists('GetWebservicesURL') || !function_exists('GetWebservicesDir'))
		{
			return false;
		}
		$strRootURL = GetWebservicesURL();
		if (strpos($strRootURL,'://') === false)
		{
			$strRootURL = 'https://'.$strRootURL;
		}
		$arrayRoot = parse_url($strRootURL);
		$arrayURL = parse_url($strURL);
		if ($arrayRoot === false || $arrayURL === false
			|| !isset($arrayURL['host'],$arrayRoot['host'],$arrayURL['scheme'])
			|| !in_array($arrayURL['scheme'],array('http','https'))
			|| strcasecmp($arrayURL['host'],$arrayRoot['host']) != 0
			|| isset($arrayURL['user']) || isset($arrayURL['pass'])
			|| (isset($arrayURL['port']) ? $arrayURL['port'] : ($arrayURL['scheme'] == 'https' ? 443 : 80))
				!= (isset($arrayRoot['port']) ? $arrayRoot['port'] : ($arrayURL['scheme'] == 'https' ? 443 : 80)))
		{
			return false;
		}
		$strRootPath = rtrim(isset($arrayRoot['path']) ? $arrayRoot['path'] : '/','/').'/';
		$strPath = isset($arrayURL['path']) ? $arrayURL['path'] : '';
		if (strpos($strPath,$strRootPath) !== 0)
		{
			return false;
		}
		$strService = substr($strPath,strlen($strRootPath));
		$strService = preg_replace('~/index\.php$~','',$strService);
		$strService = rtrim($strService,'/');
		if (!preg_match('~^[a-zA-Z0-9_-]+(?:/[a-zA-Z0-9_-]+)+$~D',$strService))
		{
			return false;
		}
		// Reject relative configuration rather than resolving it against the caller's directory.
		$strConfiguredRoot = str_replace('\\','/',GetWebservicesDir());
		if (substr($strConfiguredRoot,0,1) != '/' && !preg_match('~^[A-Za-z]:/~',$strConfiguredRoot))
		{
			return false;
		}
		$strRoot = realpath($strConfiguredRoot);
		if ($strRoot === false)
		{
			return false;
		}
		$strEntry = realpath($strRoot.'/'.$strService.'/index.php');
		if ($strRoot === false || $strEntry === false || !is_file($strEntry))
		{
			return false;
		}
		$strPrefix = rtrim(str_replace('\\','/',$strRoot),'/').'/';
		$strEntryNormalized = str_replace('\\','/',$strEntry);
		if (DIRECTORY_SEPARATOR == '\\')
		{
			$strPrefix = strtolower($strPrefix);
			$strEntryNormalized = strtolower($strEntryNormalized);
		}
		if (strpos($strEntryNormalized,$strPrefix) !== 0)
		{
			return false;
		}
		return $strEntry;
	}

	function IncludeStaticWebserviceEntryPoint($strEntryPoint,&$strFailure = "",$strCallerScript = "")
	{
		$strFailure = '';
		// Shutdown destructors may start outside the site's permitted directories.
		// Verify the return directory before executing a service with side effects.
		$strCallerDir = getcwd();
		try
		{
			if ($strCallerDir === false || !@chdir($strCallerDir)) 
			{
				$strScriptPath = str_replace('\\','/',$strCallerScript);
				if ($strScriptPath == '' || (substr($strScriptPath,0,1) != '/'
					&& !preg_match('~^[A-Za-z]:/~',$strScriptPath)))
				{
					$strFailure = 'CALLER_DIRECTORY_UNAVAILABLE';
					return false;
				}
				$strFallbackDir = realpath(dirname($strScriptPath));
				if ($strFallbackDir === false || !@chdir($strFallbackDir))
				{
					$strFailure = 'CALLER_SCRIPT_DIRECTORY_UNAVAILABLE';
					return false;
				}
				$strCallerDir = $strFallbackDir;
			}
		}
		catch (Throwable $error)
		{
			$strFailure = 'PREPARE_CALLER_DIRECTORY: '.get_class($error).' in '.$error->getFile().':'.$error->getLine();
			return false;
		}
		$bResult = false;
		$strStage = 'ENTER_SERVICE_DIRECTORY';
		try
		{
			if (@chdir(dirname($strEntryPoint)))
			{
				$strStage = 'EXECUTE_ENTRY_POINT';
				$bResult = ExecuteStaticWebserviceEntryPoint($strEntryPoint);
				if (!$bResult)
				{
					$strFailure = 'ENTRY_POINT_RETURNED_FALSE';
				}
			}
			else
			{
				$strFailure = 'SERVICE_DIRECTORY_UNAVAILABLE';
			}
		}
		catch (Throwable $error)
		{
			$strFailure = $strStage.': '.get_class($error).' in '.$error->getFile().':'.$error->getLine();
			$bResult = false;
		}
		finally
		{
			try
			{
				if (!@chdir($strCallerDir))
				{
					$strFailure .= ' RESTORE_CALLER_DIRECTORY_FAILED';
					$bResult = false;
				}
			}
			catch (Throwable $error)
			{
				$strFailure .= ' RESTORE_CALLER_DIRECTORY: '.get_class($error).' in '.$error->getFile().':'.$error->getLine();
				$bResult = false;
			}
		}
		return $bResult;
	}

	function ExecuteStaticWebserviceEntryPoint($strEntryPoint)
	{
		// Entry-point variables must not overwrite the saved caller directory.
		return (@include($strEntryPoint)) !== false;
	}

	function StaticWebserviceRequest($strURL,$arrayParams,&$strFailure = "")
	{
		$strFailure = '';
		$strEntryPoint = GetStaticWebserviceEntryPoint($strURL);
		if ($strEntryPoint === false || CWebServiceTransportContext::$nDepth >= 32)
		{
			$strFailure = ($strEntryPoint === false) ? 'LOCAL_ENTRY_POINT_UNAVAILABLE' : 'STATIC_CALL_DEPTH_EXCEEDED';
			return false;
		}
		// Capture before installing the service's synthetic SERVER values.
		$strCallerScript = isset($_SERVER['SCRIPT_FILENAME']) ? $_SERVER['SCRIPT_FILENAME'] : '';
		$arraySavedGlobals = array();
		foreach (array('_GET','_POST','_REQUEST','_COOKIE','_FILES','_SERVER') as $strKey)
		{
			if (array_key_exists($strKey,$GLOBALS))
			{
				$arraySavedGlobals[$strKey] = $GLOBALS[$strKey];
			}
		}
		$strIncludePath = get_include_path();
		$strTimeZone = date_default_timezone_get();
		$nErrorReporting = error_reporting();
		$arrayIni = array();
		foreach (array('display_errors','allow_url_fopen','max_execution_time','expose_php') as $strKey)
		{
			$arrayIni[$strKey] = ini_get($strKey);
		}
		$arrayHeaders = headers_list();
		$nStatus = http_response_code();
		$nBufferLevel = ob_get_level();
		CWebServiceTransportContext::$nDepth++;
		try
		{
			$arrayURL = parse_url($strURL);
			$_GET = array();
			parse_str(isset($arrayURL['query']) ? $arrayURL['query'] : '',$_GET);
			$_POST = array();
			parse_str(is_array($arrayParams) ? MakeURLParameters($arrayParams) : GetStringValue($arrayParams),$_POST);
			$_REQUEST = array_merge($_GET,$_POST);
			$_COOKIE = array();
			$_FILES = array();
			// Do not inherit the visitor's authentication, cookies or proxy headers.
			foreach (array_keys($_SERVER) as $strKey)
			{
				if (strpos($strKey,'HTTP_') === 0 || strpos($strKey,'PHP_AUTH_') === 0
					|| in_array($strKey,array('AUTH_TYPE','REMOTE_USER','REDIRECT_REMOTE_USER')))
				{
					unset($_SERVER[$strKey]);
				}
			}
			$_SERVER['SERVER_NAME'] = $arrayURL['host'];
			$_SERVER['HTTP_HOST'] = $arrayURL['host'].(isset($arrayURL['port']) ? ':'.$arrayURL['port'] : '');
			$_SERVER['HTTPS'] = $arrayURL['scheme'] == 'https' ? 'on' : '';
			$_SERVER['SERVER_PORT'] = isset($arrayURL['port']) ? $arrayURL['port'] : ($arrayURL['scheme'] == 'https' ? 443 : 80);
			$_SERVER['REQUEST_METHOD'] = 'POST';
			$_SERVER['QUERY_STRING'] = isset($arrayURL['query']) ? $arrayURL['query'] : '';
			$_SERVER['REQUEST_URI'] = $arrayURL['path'].($_SERVER['QUERY_STRING'] != '' ? '?'.$_SERVER['QUERY_STRING'] : '');
			$_SERVER['SCRIPT_NAME'] = rtrim($arrayURL['path'],'/');
			if (substr($_SERVER['SCRIPT_NAME'],-10) != '/index.php')
			{
				$_SERVER['SCRIPT_NAME'] .= '/index.php';
			}
			$_SERVER['PHP_SELF'] = $_SERVER['SCRIPT_NAME'];
			$_SERVER['SCRIPT_FILENAME'] = $strEntryPoint;
			$_SERVER['SCRIPT_URI'] = $arrayURL['scheme'].'://'.$_SERVER['HTTP_HOST'].$arrayURL['path'];
			$_SERVER['REMOTE_ADDR'] = isset($_SERVER['SERVER_ADDR']) ? $_SERVER['SERVER_ADDR'] : '127.0.0.1';
			$_SERVER['REQUEST_TIME'] = time();
			$_SERVER['REQUEST_TIME_FLOAT'] = microtime(true);
			$_SERVER['CONTENT_TYPE'] = 'application/x-www-form-urlencoded';
			$_SERVER['CONTENT_LENGTH'] = strlen(is_array($arrayParams) ? MakeURLParameters($arrayParams) : GetStringValue($arrayParams));
			// Resolve bare includes against explicit service and framework directories.
			set_include_path(dirname($strEntryPoint).PATH_SEPARATOR.__DIR__.PATH_SEPARATOR.GetSourceDir());
			ob_start();
			if (!IncludeStaticWebserviceEntryPoint($strEntryPoint,$strFailure,$strCallerScript))
			{
				return false;
			}
			while (ob_get_level() > $nBufferLevel + 1)
			{
				ob_end_flush();
			}
			if (ob_get_level() != $nBufferLevel + 1)
			{
				$strFailure = 'RESPONSE_BUFFER_MISSING';
				return false;
			}
			$strOutput = ob_get_contents();
			if ($strOutput === '')
			{
				$strFailure = 'EMPTY_SERVICE_RESPONSE';
			}
			return $strOutput;
		}
		catch (Throwable $error)
		{
			$strFailure = 'STATIC_REQUEST: '.get_class($error).' in '.$error->getFile().':'.$error->getLine();
			return false;
		}
		finally
		{
			while (ob_get_level() > $nBufferLevel)
			{
				ob_end_clean();
			}
			foreach (array('_GET','_POST','_REQUEST','_COOKIE','_FILES','_SERVER') as $strKey)
			{
				if (array_key_exists($strKey,$arraySavedGlobals))
				{
					$GLOBALS[$strKey] = $arraySavedGlobals[$strKey];
				}
				else
				{
					unset($GLOBALS[$strKey]);
				}
			}
			set_include_path($strIncludePath);
			date_default_timezone_set($strTimeZone);
			foreach ($arrayIni as $strKey => $strValue)
			{
				if (ini_get($strKey) !== $strValue)
				{
					ini_set($strKey,$strValue);
				}
			}
			error_reporting($nErrorReporting);
			if (!headers_sent())
			{
				header_remove();
				foreach ($arrayHeaders as $strHeader)
				{
					header($strHeader,false);
				}
				if ($nStatus !== false)
				{
					http_response_code($nStatus);
				}
			}
			CWebServiceTransportContext::$nDepth--;
		}
	}
