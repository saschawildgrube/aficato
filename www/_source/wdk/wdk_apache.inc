<?php


	function GetVersionInfoApache()
	{
		$arrayVersion = array();
		$arrayVersion["NAME"] = "Apache";
		
		require_once("wdk_shell.inc");
		$arrayResult = array();
		ShellExecute(
			"apachectl",
			array("-v"),
			array(),
			"",
			$arrayResult);
		$strStdOut = ArrayGetValue($arrayResult,"stdout");
		
		$arrayApacheStatus = StringExplode($strStdOut,"\n",true);
		$strApacheVersionLine = '';
		foreach ($arrayApacheStatus as $strLine)
		{
			if (FindString($strLine,'Server version: ') == 0)
			{
				$strApacheVersionLine = StringSection($strLine,16);
				continue;
			}
		}
		
		$arrayVersionLine = StringExplode($strApacheVersionLine," ");
		$strApacheVersion = '';
		foreach ($arrayVersionLine as $strLine)
		{
			if (FindString($strLine,"Apache/") == 0)
			{
				$strApacheVersion = StringSection($strLine,7);
				continue;
			}
		}
		$arrayVersion["VERSION"] = $strApacheVersion;
		return $arrayVersion;
	}

