<?php

	function GetTopLevelDomain($strServerName)
	{
		$nLastDotPos = strrpos($strServerName,".");
		
		if (is_string($nLastDotPos) && !$nLastDotPos)
		{
			return "";
		}

		$strTopLevelDomain = StringSection($strServerName,$nLastDotPos+1,StringLength($strServerName)-$nLastDotPos-1);
		$strTopLevelDomain = StringLowerCase($strTopLevelDomain);
		return $strTopLevelDomain;
	}

	function GetSubDomain($strHttpHost)
	{
		$nPosDot = FindString($strHttpHost,".");
		if ($nPosDot == -1)
		{
			return "";	
		}
		$strSubDomain = StringSection($strHttpHost,0,$nPosDot);
		return $strSubDomain;
	}

