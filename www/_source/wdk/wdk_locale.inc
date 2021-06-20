<?php

	require_once("wdk_country.inc");
	require_once("wdk_language.inc");
	
	/*
	To check existing locales use this command:
	locale -a
	
	To install additional locales use this command:
	sudo locale-gen en_US.utf8
	*/
	
	
	function GetAllLocales()
	{
		$arrayLocales = array(
			"ARE" => array(
				"AR" => "ar_AE"
				),
			"AUT" => array(
				"DE" => "de_AT"
				),
			"BEL" => array(
				"NL" => "nl_BE",
				"FR" => "fr_BE",
				"DE" => "de_BE"
				),
			"CAN" => array(
				"EN" => "en_CA",
				"FR" => "fr_CA"
				),
			"CHE" => array(
				"DE" => "de_CH",
				"FR" => "fr_CH",
				"IT" => "it_CH"
				),
			"CHL" => array(
				"ES" => "es_CL"
				),
			"CHN" => array(
				"ZH" => "zh_CN"
				),
			"COL" => array(
				"ES" => "es_CO"
				),
			"CZE" => array(
				"CS" => "cs_CZ"
				),
			"DEU" => array(
				"DE" => "de_DE"
				),
			"DNK" => array(
				"DA" => "da_DK"
				),
			"ESP" => array(
				"ES" => "es_ES"
				),
			"FIN" => array(
				"FI" => "fi_FI"
				),
			"FRA" => array(
				"FR" => "fr_FR"
				),
			"GBR" => array(
				"EN" => "en_GB"
				),
			"GRC" => array(
				"EL" => "el_GR"
				),
			"ISL" => array(
				"IS" => "is_IS"
				),
			"ITA" => array(
				"IT" => "it_IT"
				),
			"IRL" => array(
				"EN" => "en_IE",
				"GA" => "ga_IE"
				),
			"JPN" => array(
				"JA" => "ja_JP"
				),
			"LUX" => array(
				"DE" => "de_LU",
				"FR" => "fr_LU"
				//,"LB" => "lb_LU"   locale for luxembourgish is missing in ubuntu
				),
			"MAR" => array(
				"AR" => "ar_MA"
				),
			"NLD" => array(
				"NL" => "nl_NL"
				),
			"NOR" => array(
				"NN" => "nn_NO"
				),
			"PHL" => array(
				"EN" => "en_PH"
				),
			"POL" => array(
				"PL" => "pl_PL"
				),
			"PRT" => array(
				"PT" => "pt_PT"
				),
			"SGP" => array(
				"EN" => "en_SG",
				"ZH" => "zh_SG"
				),
			"SWE" => array(
				"SV" => "sv_SE"
				),
			"USA" => array(
				"EN" => "en_US")
			);

		foreach ($arrayLocales as $strCountry => &$arrayLanguages)
		{
			foreach ($arrayLanguages as $strLanguage => &$strLocale)
			{
				$strLocale .= ".utf8";
			}	
		}	
		return $arrayLocales;
	}
	
	
	function GetLocale($strCountryID,$strLanguageID)
	{
		$arrayLocales = GetAllLocales();
		if ($strCountryID != '')
		{
			return  ArrayGetValue($arrayLocales,$strCountryID,$strLanguageID);
		}
		foreach ($arrayLocales as $strCountry => $arrayLanguages)
		{
			foreach ($arrayLanguages as $strLanguage => $strLocale)
			{
				if ($strLanguage == $strLanguageID)
				{
					return $strLocale;	
				}
			}	
		}	
		return '';
	}
	
	function GetCountriesByLanguage($strLanguageID)
	{
		$arrayLocales = GetAllLocales();
		$arrayCountries = array();
		foreach ($arrayLocales as $strCountryID => $arrayLanguages)
		{
			foreach ($arrayLanguages as $strLanguageKey => $strLocale)
			{
				if ($strLanguageKey == $strLanguageID)
				{
					$arrayCountries[] = $strCountryID;
					break;	
				}	
			}	
		}
		return $arrayCountries;
	}
	
	function GetLanguagesByCountry($strCountryID)
	{
		$arrayLocales = GetAllLocales();
		$arrayLanguages = ArrayGetValue($arrayLocales,$strCountryID);
		if (!is_array($arrayLanguages))
		{
			return array();	
		}
		$arrayLanguageIDs = array_keys($arrayLanguages);
		return $arrayLanguageIDs;
	}

	
	
	
	function SetMonetaryLocale($strCountryID,$strLanguageID)
	{
		$strLocale = GetLocale($strCountryID,$strLanguageID);
		$bReturn = setlocale(LC_MONETARY,$strLocale);
		if ($bReturn == false)
		{
			return false;
		}
		return true;
	}
	
	function GetLocaleSettings($strCountryID,$strLanguageID)
	{
		if (!IsValidCountryID($strCountryID))
		{
			return false;
		}
		if (!IsValidLanguageID($strLanguageID))
		{
			return false;
		}
		if (SetMonetaryLocale($strCountryID,$strLanguageID) == false)
		{
			return false;
		}
  	$arrayLocalConv = localeconv();
  	if (!is_array($arrayLocalConv))
  	{
  		return false;	
  	}
		$arraySettings = array();
		$arraySettings["COUNTRY"] = $strCountryID;
		$arraySettings["LANGUAGE"] = $strLanguageID;
		$arraySettings["DECIMAL_DELIMITER"] = ArrayGetValue($arrayLocalConv,"mon_decimal_point");
		if ($arraySettings["DECIMAL_DELIMITER"] == "")
		{
			$arraySettings["DECIMAL_DELIMITER"] = ".";
		}
		$arraySettings["DIGIT_GROUPING_SEPARATOR"] = ArrayGetValue($arrayLocalConv,"mon_thousands_sep");
		// This is to work-around a bug in earlier versions to php 5.4 where number_format does only use the first character of a multi-byte thousands separator.
		if (strlen($arraySettings["DIGIT_GROUPING_SEPARATOR"]) > 1)  
		{
			$arraySettings["DIGIT_GROUPING_SEPARATOR"] == "";
		}
		// And this is to work around an unclear bug in the norwegian locale  
		if ($strLanguageID == "NN")
		{
			$arraySettings["DIGIT_GROUPING_SEPARATOR"] = "";
		}
  	return $arraySettings;
	}
	
	function RenderNumber($fValue,$nPrecision = 0,$strMetric = '',$arraySettings = false)
	{
		if (!is_array($arraySettings))
		{
			$arraySettings = array();
		}
		$strDecimalDelimiter = ArrayGetValue($arraySettings,"DECIMAL_DELIMITER");
		if ($strDecimalDelimiter == "")
		{
			$strDecimalDelimiter = ".";
		}
		$strDigitGroupingSeparator = ArrayGetValue($arraySettings,"DIGIT_GROUPING_SEPARATOR");
		if ($strDigitGroupingSeparator == null)
		{
			$strDigitGroupingSeparator = "";
		}
		$fValue = floatval($fValue);
		$strAbbreviation = '';
		if ($nPrecision <= -3)
		{
			if (abs($fValue)>1000)
			{
				$fValue = round($fValue) / 1000;
				$strAbbreviation = 'K';
				if ($nPrecision <= -6)
				{
					if (abs($fValue)>1000)
					{
						$fValue = round($fValue) / 1000;
						$strAbbreviation = 'M';
					}
				}
			}
			$nPrecision = 0;
		}
		$strNumber = number_format($fValue,$nPrecision,$strDecimalDelimiter,$strDigitGroupingSeparator);
		if ($strAbbreviation != '')
		{
			$strNumber .= $strAbbreviation;	
		}
		if ($strMetric != '')
		{
			$strNumber .= ' '.$strMetric;	
		}
		return $strNumber; 
	}
	
	function GetLocalFloatValue($value,$strCountry,$strLanguage)
	{
		if (is_float($value))
		{
			return $value;
		}

		$strValue = strval($value);
   	$arraySettings = GetLocaleSettings($strCountry,$strLanguage); 
  	$strValue = ReplaceString($strValue,$arraySettings["DIGIT_GROUPING_SEPARATOR"] , ""); 
  	$strValue = ReplaceString($strValue,$arraySettings["DECIMAL_DELIMITER"] , "."); 
    return floatval($strValue); 
	} 
	



