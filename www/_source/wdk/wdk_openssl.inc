<?php

	require_once("wdk_shell.inc");

	function GetVersionInfoOpenSSL()
	{
		$arrayVersion = array();
		$arrayVersion["NAME"] = "OpenSSL";
		
		$strVersionText = OPENSSL_VERSION_TEXT;
		if ($strVersionText != "")
		{
			$arrayVersion["VERBOSE"] = $strVersionText;			
		}
		
		$arrayVersionText = StringExplode($strVersionText," ");
		
		$strVersion = ArrayGetValue($arrayVersionText,1);
		if ($strVersion != "")
		{
			$arrayVersion["VERSION"] = $strVersion;
		}

		$strVersionDate = ArrayGetValue($arrayVersionText,2).ArrayGetValue($arrayVersionText,3).ArrayGetValue($arrayVersionText,4);
		if ($strVersionDate != "")
		{
			$timeVersion = strtotime($strVersionDate);
			$strVersionDate = RenderDate($timeVersion);
			if ($strVersionDate != "")
			{
				$arrayVersion["VERSION_DATE"] = $strVersionDate;
			}
		}
		
		$arrayResult = array();
		$bResult = SudoShellExecute(
			"openssl",
			array("version", "-a"),
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
					$nPos = FindString($strLine,"built on: ");
					if ($nPos != -1)
					{
						$strBuildDateRaw = StringSection($strLine,10);
						if (FindString($strBuildDateRaw,"reproducible build, date unspecified") != -1)
						{
							$arrayVersion["BUILD_DATE"] = "2016-09-26";
						}
						else
						{
							$timeBuild = strtotime($strBuildDateRaw);
							$strBuildDate = RenderDate($timeBuild);
							if ($strBuildDate != "")
							{
								$arrayVersion["BUILD_DATE"] = $strBuildDate;
							}
						}
						break;	
					}
				}				
			}
		}

		return $arrayVersion;
	}

