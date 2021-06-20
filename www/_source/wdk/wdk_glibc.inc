<?php

	require_once("wdk_shell.inc");

	function GetVersionInfoGlibc()
	{
		$arrayVersion = array();
		$arrayVersion["NAME"] = "glibc";
		
		$arrayResult = array();
		$bResult = ShellExecute(
			"ldd",
			array("--version"),
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
					$nPos = FindString($strLine,"ldd ");
					if ($nPos != -1)
					{
						$nPosClosingBracet = FindString($strLine,") ");
						if ($nPosClosingBracet != -1)
						{
							$strVersion = StringSection($strLine,$nPosClosingBracet+2);
							$arrayVersion["VERSION"] = $strVersion;
						}
						
						$strPackageVersion = StringGetFromEmbracingTags($strLine,"EGLIBC ",") ");
						if ($strPackageVersion != "")
						{
							$arrayVersion["VERBOSE"] = $strPackageVersion; 
						}
					}
					break;					
				}				
			}
		}
		
		/*

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
						$arrayVersion["WGET_PACKAGE_VERSION"] = $strVersion; 
						break;
					}
				}				
			}
		}
*/

		return $arrayVersion;
	}

