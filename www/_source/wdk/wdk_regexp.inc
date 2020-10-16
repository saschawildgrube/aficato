<?php

	function RegExpMatch($strSubject,$strPattern)
	{
		$strSubject = StringEncodeUTF8($strSubject);
		$strPattern = StringEncodeUTF8($strPattern);
		$ea = new CErrorAmnesty();
		$ea->Start();
		$nResult = preg_match($strPattern, $strSubject);
		$ea->End();
		return GetBoolValue($nResult);
	}
	
	function RegExpMatchMultiple($strSubject,$strPattern,$bIncludePositions = false)
	{
		$flags = PREG_PATTERN_ORDER;
		if ($bIncludePositions == true)
		{
			$flags = PREG_OFFSET_CAPTURE;
		}
		$arrayMatches = array();
		preg_match_all($strPattern,$strSubject,$arrayMatches, $flags, 0);	
		$arrayResults = false;
		if ($flags == PREG_PATTERN_ORDER)
		{
			$arrayResults = ArrayGetValue($arrayMatches,0);
		}
		if ($flags == PREG_OFFSET_CAPTURE)  
		{
			$arrayResults = $arrayMatches;
		}
		
		return $arrayResults;
	}
	 	
	function RegExpReplace($strSubject,$strPattern,$strReplace)
	{
		$strSubject = StringEncodeUTF8($strSubject);
		$strPattern = StringEncodeUTF8($strPattern);
		$strReplace = StringEncodeUTF8($strReplace);
		$ea = new CErrorAmnesty();
		$ea->Start();
		$strResult = preg_replace($strPattern, $strReplace, $strSubject);
		$ea->End();
		return $strResult;
	}
	
