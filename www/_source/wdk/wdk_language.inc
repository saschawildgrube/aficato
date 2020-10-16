<?php

	function GetBrowserLanguage()
	{
		$arrayLanguages = GetAllLanguageIDs();

		$strHttpAcceptLanguage = ArrayGetValue($_SERVER,"HTTP_ACCEPT_LANGUAGE");
		$strBrowserLanguage = "";

		$nClosest = 1000;
		for ($nIndex = 0; $nIndex < ArrayCount($arrayLanguages);$nIndex++)
		{
			$nPos = FindString($strHttpAcceptLanguage,StringLowerCase($arrayLanguages[$nIndex]));
			if ($nPos != -1)
			{
				if ($nPos < $nClosest)
				{
					$strBrowserLanguage = $arrayLanguages[$nIndex];
					$nClosest = $nPos;
				}
			}
		}
		return StringUpperCase($strBrowserLanguage);	
	}



	function GetLanguage($strDefaultLanguage)
	{
		// If there is a default language, use it.
		if ($strDefaultLanguage != "")
		{
			return StringUpperCase($strDefaultLanguage);
		}

		// Maybe the browser tells us what to do
		$strLanguage = GetBrowserLanguage();
		if ($strLanguage != "")
		{ 
			return $strLanguage;
		}

		// We simply don't know
		return "";
	}
	
	function IsValidLanguageID($strLanguage)
	{
		return ArrayValueExists(GetAllLanguageIDs(),$strLanguage);	
	}

	function GetAllLanguageIDs()
	{
		return array(
			"AR",
			"CS",
			"DA",
			"DE",
			"EL",
			"EN",
			"ES",
			"FI",
			"FR",
			"GA",
			"IS",
			"IT",
			"JA",
			//"LB",   locale lb_LU.utf8 is missing in ubuntu
			"NL",			
			"NN",
			"PL",
			"PT",
			"SV",
			"ZH");
	}

