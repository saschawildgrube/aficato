<?php

	function GetRandomInteger($nMax = 0)
	{
		$nRandMax = getrandmax();
		if ($nMax == 0 || $nMax > $nRandMax)
		{
			$nMax = $nRandMax;
		}
		return rand(0,$nMax);	
	}


	function MakeNumericToken($nLength)
	{
		if ($nLength <= 0)
		{
			return "";	
		}
		$strToken = sha1(microtime(true).rand());
		$strToken = StringLowerCase($strToken);
		$strToken = strtr($strToken,"abcdef","123456");
		$strToken = StringSection($strToken,0,min(intval($nLength),40));
		return $strToken;
	}
	
	function MakeHexToken($nLength)
	{
		if ($nLength <= 0)
		{
			return "";	
		}
		$strToken = sha1(microtime(true).rand());
		$strToken = StringLowerCase($strToken);
		$strToken = StringSection($strToken,0,min(intval($nLength),40));
		return $strToken;
	}	
	
	function MakeUniqueToken($strHaystack, $arrayTokens)
	{
		if (!is_array($arrayTokens))
		{
			$arrayTokens = array();
		}
		do
		{
			$bDone = true;
			$strUniqueToken = MakeNumericToken(10);
			if ($strHaystack != "")
			{
				if (FindString($strHaystack,$strUniqueToken) != -1)
				{
					$bDone = false;
				}
			}
			if ($bDone == true)
			{
				foreach ($arrayTokens as $strToken)
				{
					if ($strToken != "")
					{
						if (FindString($strUniqueToken,$strToken) != -1)
						{
							$bDone = false;
						}
					}
				}
			}
		} while(!$bDone);	
		
		return $strUniqueToken;
	}
	

