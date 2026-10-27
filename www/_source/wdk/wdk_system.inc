<?php

	require_once(GetWDKDir()."wdk_shell.inc");

	function GetSystemHostname()
	{
		return php_uname("n");
	}



	function GetVersionInfoOS()
	{
		$arrayOperatingSystem = array(
			"NAME" => php_uname("s"),
			"VERSION" => php_uname("r"));

		$arrayResult = array();
		$bResult = ShellExecute(
			"cat",
			array("/etc/os-release"),
			array(),
			"",
			$arrayResult);
		if ($bResult == false || ArrayGetValue($arrayResult,"return") != 0)
		{
			return $arrayOperatingSystem;
		}

		$arrayValues = array();
		$arrayLines = StringExplode(ArrayGetValue($arrayResult,"stdout"),"\n",true);
		foreach ($arrayLines as $strLine)
		{
			if ($strLine == "" || FindString($strLine,"#") == 0)
			{
				continue;
			}

			$nSeparatorPos = FindString($strLine,"=");
			if ($nSeparatorPos == -1)
			{
				continue;
			}

			$strKey = StringTrim(StringSection($strLine,0,$nSeparatorPos));
			$strValue = StringTrim(StringSection($strLine,$nSeparatorPos+1));
			$nValueLength = strlen($strValue);
			if ($nValueLength >= 2)
			{
				$strFirstCharacter = substr($strValue,0,1);
				$strLastCharacter = substr($strValue,-1);
				if (($strFirstCharacter == '"' && $strLastCharacter == '"') ||
					($strFirstCharacter == "'" && $strLastCharacter == "'"))
				{
					$strValue = substr($strValue,1,$nValueLength-2);
				}
			}
			$arrayValues[$strKey] = $strValue;
		}

		$strName = ArrayGetValue($arrayValues,"NAME");
		if ($strName != "")
		{
			$arrayOperatingSystem["NAME"] = $strName;
		}
		$strVersion = ArrayGetValue($arrayValues,"VERSION_ID");
		if ($strVersion == "")
		{
			$strVersion = ArrayGetValue($arrayValues,"VERSION");
		}
		if ($strVersion != "")
		{
			$arrayOperatingSystem["VERSION"] = $strVersion;
		}

		return $arrayOperatingSystem;
	}

	function GetSystemAverageLoad(&$arrayAverageLoad)
	{
		$arrayAverageLoad = array();
		$arrayResult = array();
		$bResult = ShellExecute(
			"uptime",
			array(),
			array(),
			"",
			$arrayResult);
		if ($bResult == false)
		{
			return false;	
		}
		$strStdOut = $arrayResult["stdout"];
		$nLoadPos = FindString($strStdOut,"load average:");
		if ($nLoadPos == -1)
		{
			return false;	
		}
		$strAverageLoad = StringSection($strStdOut,$nLoadPos+13);
		$arrayAverageLoadRaw = explode(",",$strAverageLoad);
		if (ArrayCount($arrayAverageLoadRaw) != 3)
		{
			return false;	
		}
		$arrayAverageLoad["1minute"] = floatval($arrayAverageLoadRaw[0]);
		$arrayAverageLoad["5minutes"] = floatval($arrayAverageLoadRaw[1]);
		$arrayAverageLoad["15minutes"] = floatval($arrayAverageLoadRaw[2]);
		return true;
	}
	

	
	function GetSystemUser()
	{
		$arrayResult = array();
		$bResult = ShellExecute(
			"whoami",
			array(),
			array(),
			"",
			$arrayResult);
		if ($bResult == false)
		{
			return "";	
		}
		$strName = chop($arrayResult["stdout"]);
		return $strName;
	}



	function GetPleskVersion()
	{
		$arrayResult = array();
		$bResult = ShellExecute(
			"plesk",
			array("version"),
			array(),
			"",
			$arrayResult);
		if ($bResult == false || ArrayGetValue($arrayResult,"return") != 0)
		{
			return "";
		}

		$strStdOut = ArrayGetValue($arrayResult,"stdout");
		$arrayLines = StringExplode($strStdOut,"\n",true);
		foreach ($arrayLines as $strLine)
		{
			if (FindString($strLine,"Product version:") == 0)
			{
				$strProductVersion = StringTrim(StringSection($strLine,16));
				$arrayMatches = array();
				if (preg_match('/[0-9]+(?:\.[0-9]+)+/', $strProductVersion, $arrayMatches) == 1)
				{
					return ArrayGetValue($arrayMatches,0);
				}
				return "";
			}
		}
		return "";
	}



	function GetMailQueueSize()
	{
		$arrayResult = array();
		$bResult = ShellExecute( 
			"sudo",
			array("-u root", "/var/qmail/bin/qmail-qstat"),
			array(),
			"",
			$arrayResult);
		
		if ($bResult == false)
		{
			return false;	
		}
		if (ArrayGetValue($arrayResult,"stderr") != "")
		{
			return false;	
		}
		$strStdout = ArrayGetValue($arrayResult,"stdout");
		if ($strStdout == "")
		{
			return false;	
		}
		$arrayLines = StringExplode($strStdout,"\n");
		$strLine1 = ArrayGetValue($arrayLines,0);
		$nMailQueueSize = intval(StringSection($strLine1,19));
		return $nMailQueueSize;
	}
	
	
	function GetCoreDumpFiles()
	{
		$arrayResult = array();
		ShellExecute(
			"sudo",
			array("/bin/ls","-1","/core.*"),
			array(),
			"",
			$arrayResult);
			
			
		$nReturn = ArrayGetValue($arrayResult,"return");  
		if ($nReturn == 1)
		{
			return false;	
		}
		
		$strStdOut = ArrayGetValue($arrayResult,"stdout");
		if ($strStdOut != "")
		{
			$arrayCoreDumpFiles = StringExplode($strStdOut,"\n",true);
			if (is_array($arrayCoreDumpFiles))
			{
				return $arrayCoreDumpFiles;
			}
		}
		return array();
	}
	
	
	
