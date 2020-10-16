<?php


	function GetVersionInfoPHP()
	{
		$arrayVersion = array();
		$arrayVersion["NAME"] = "php";
		
		$strVersionVerbose = phpversion();
		if ($strVersionVerbose != "")
		{
			$arrayVersion["VERBOSE"] = $strVersionVerbose;			
		}
		
		$nDashPos = FindString($strVersionVerbose,"-");
		if ($nDashPos != -1)
		{
			$strVersion = StringSection($strVersionVerbose,0,$nDashPos);
		}
		else
		{
			$strVersion = $strVersionVerbose;
		}
		$arrayVersion["VERSION"] = $strVersion;
		return $arrayVersion;
	}

