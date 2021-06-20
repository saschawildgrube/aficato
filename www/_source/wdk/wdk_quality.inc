<?php

	function GetAssemblyDir($strAssemblyID)
	{ 
		switch ($strAssemblyID)
		{
		case "wdk":
			$strAssemblyDir = GetWDKDir();
			break;
		case "root":
			$strAssemblyDir = GetSourceDir();
			break;
		default:
			$strAssemblyDir = GetSourceDir()."assemblies/".$strAssemblyID."/";
			break;
		}
		return $strAssemblyDir;
	}

	function GetTestGroupsDir($strAssemblyID)
	{
		return GetAssemblyDir($strAssemblyID) . "quality/tests/";
	}
	
	function MakeTestFilePath($strAssemblyID,$strGroupID,$strTestID)
	{
		return GetTestGroupsDir($strAssemblyID).$strGroupID."/test_".$strTestID.".php";
	}
	
	function MakeTestPath(
		$strSitePath,
		$strAssemblyID,
		$strGroupID,
		$strTestID)
	{
		return $strSitePath.":".$strAssemblyID."/".$strGroupID."/".$strTestID;
	}
	
	function ParseTestPath($strTestPath)
	{
		$strTestPath = GetStringValue($strTestPath);
		$arrayTestPath = array();
		$arraySiteAndRest = explode(":",$strTestPath);
		$nParts = ArrayCount($arraySiteAndRest);
		if ($nParts == 2)
		{
			$strSitePath = $arraySiteAndRest[0];
			$strRest = $arraySiteAndRest[1];
		}
		else if ($nParts == 1)
		{
			$strSitePath = "local";
			$strRest = $arraySiteAndRest[0];
		}
		else
		{
			return false;	
		}
		
		$arrayRest = explode("/",$strRest);
		if (ArrayCount($arrayRest) != 3)
		{
			return false;	
		}
		$arrayTestPath["site_path"] = $strSitePath;
		$arrayTestPath["assembly_id"] = $arrayRest[0];
		$arrayTestPath["group_id"] = $arrayRest[1];
		$arrayTestPath["test_id"] = $arrayRest[2];
		return $arrayTestPath;
	}
	
