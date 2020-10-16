<?php

	require_once(GetWDKDir()."wdk_compare.inc");

	function ParseVersion($strVerboseVersion,$nComponents = 4)
	{
		
		$arrayVerboseVersionComponents = StringExplode($strVerboseVersion,"-",true,2);
		$strVersion = ArrayGetValue($arrayVerboseVersionComponents,0);
		$strPackage = ArrayGetValue($arrayVerboseVersionComponents,1);
		$strPackage = StringTrim($strPackage);
		
		$arrayVersion = array();
		if ($nComponents < 1)
		{
			$nComponents = 1;
		}
		if ($nComponents > 4)
		{
			$nComponents = 4;
		}
		$arrayVersion["VERSION"] = ""; // To control the order of result values
		$arrayVersionComponents = StringExplode($strVersion,".",true,$nComponents+1);  
		$arrayVersion["MAJOR"] = intval(ArrayGetValue($arrayVersionComponents,0));
		$strVersion = $arrayVersion["MAJOR"];
		if ($nComponents > 1)
		{
			$arrayVersion["MINOR"] = intval(ArrayGetValue($arrayVersionComponents,1));
			$strVersion .= ".".$arrayVersion["MINOR"];
		}
		if ($nComponents > 2)
		{
			$arrayVersion["PATCH"] = intval(ArrayGetValue($arrayVersionComponents,2));
			$strVersion .= ".".$arrayVersion["PATCH"];
		}
		if ($nComponents > 3)
		{
			$arrayVersion["BUILD"] = intval(ArrayGetValue($arrayVersionComponents,3));
			$strVersion .= ".".$arrayVersion["BUILD"];
		}
		$arrayVersion["VERSION"] = $strVersion;
		if ($strPackage != "")
		{
			$arrayVersion["PACKAGE"] = $strPackage;
		}
		return $arrayVersion;
	}
	
	function CompareVersion($vVersion1, $vVersion2)
	{
		if (!is_array($vVersion1))
		{
			$arrayVersion1 = ParseVersion($vVersion1);
		}
		else
		{
			$arrayVersion1 = $vVersion1;	
		}
		if (!is_array($vVersion2))
		{
			$arrayVersion2 = ParseVersion($vVersion2);
		}
		else
		{
			$arrayVersion2 = $vVersion2;	
		}
		$nMajorCompare = CompareInteger(
			ArrayGetValue($arrayVersion1,"MAJOR"),
			ArrayGetValue($arrayVersion2,"MAJOR"));
		if ($nMajorCompare != 0)
		{
			return $nMajorCompare;	
		}
		$nMinorCompare = CompareInteger(
			ArrayGetValue($arrayVersion1,"MINOR"),
			ArrayGetValue($arrayVersion2,"MINOR"));
		if ($nMinorCompare != 0)
		{
			return $nMinorCompare;	
		}
		$nPatchCompare = CompareInteger(
			ArrayGetValue($arrayVersion1,"PATCH"),
			ArrayGetValue($arrayVersion2,"PATCH"));
		if ($nPatchCompare != 0)
		{
			return $nPatchCompare;	
		}
		$nBuildCompare = CompareInteger(
			ArrayGetValue($arrayVersion1,"BUILD"),
			ArrayGetValue($arrayVersion2,"BUILD"));
		if ($nBuildCompare != 0)
		{
			return $nBuildCompare;	
		}
		$nPackageCompare = CompareString(
			ArrayGetValue($arrayVersion1,"PACKAGE"),
			ArrayGetValue($arrayVersion2,"PACKAGE"));
		if ($nPackageCompare != 0)
		{
			return $nPackageCompare;	
		}
		return 0;	
	}


