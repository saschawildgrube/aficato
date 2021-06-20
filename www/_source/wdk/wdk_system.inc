<?php

	require_once(GetWDKDir()."wdk_shell.inc");

	function GetSystemHostname()
	{
		return php_uname("n");
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
	
	
	
