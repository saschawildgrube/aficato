<?php

	require_once("wdk_shell.inc");

	function GetVersionInfoWget()
	{
		$arrayVersion = array();
		$arrayVersion["NAME"] = "wget";
		
		$arrayResult = array();
		$bResult = ShellExecute(
			"wget",
			array("-h"),
			array(),
			"",
			$arrayResult); 

		if ($bResult == true)
		{
			$strStdout = ArrayGetValue($arrayResult,"stdout");
			if ($strStdout != "")
			{
				$arrayResultLines = StringExplode($strStdout,"\n");
				foreach ($arrayResultLines as $strLine)
				{
					$nPos = FindString($strLine,"GNU Wget ");
					if ($nPos != -1)
					{
						$strVersion = StringSection($strLine,9);
						$arrayVersionLine = StringExplode($strVersion,",");
						$strVersion = ArrayGetValue($arrayVersionLine,0);
						$arrayVersion["VERSION"] = $strVersion;
						break;
					}
				}				
			}
		}
		
		

		$arrayResult = array();
		$bResult = ShellExecute(
			"apt-cache",
			array("show wget"),
			array(),
			"",
			$arrayResult); 

		if ($bResult == true)
		{
			$strStdout = ArrayGetValue($arrayResult,"stdout");
			if ($strStdout != "")
			{
				$arrayResultLines = StringExplode($strStdout,"\n");
				foreach ($arrayResultLines as $strLine)
				{
					$nPos = FindString($strLine,"Version: ");
					if ($nPos != -1)
					{
						$strVersion = StringSection($strLine,9);
						$arrayVersion["VERBOSE"] = $strVersion; 
						break;
					}
				}				
			}
		}


		return $arrayVersion;
	}

