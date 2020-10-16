<?php

	require_once("wdk_arraytextfile.inc");

	
	function IsValidTID($strID)
	{
		if (StringCheckCharSet($strID,CHARSET_TID) == false)
		{
			return false;
		}
		if (StringSection($strID,4) == "TID_")
		{
			return false;	
		}
		if (StringSection($strID,-1) == "_")
		{
			return false;	
		}
		if (StringSection($strID,-1) == "-")
		{
			return false;	
		}
		if (StringLength($strID) < 5)
		{
			return false;
		}
		return true;	
	}
	
	function GetInvalidTIDs($arrayText)
	{
		$arrayErrors = array();
		foreach ($arrayText as $strKey => $strValue)
		{
			if (IsValidTID($strKey) == false)
			{
				array_push($arrayErrors,$strKey);
			}
		}
		return $arrayErrors;	
	}
	
	function MakeTID($strUserFriendly)
	{
		$strID = $strUserFriendly;
		$strID = StringUpperCase($strID);
		$strID = StringTranslateCharacters($strID,"/ .","__-");
		$strID = StringFilter($strID,CHARSET_TID);
		$strID = StringRemoveLeadingCharacters($strID,"_-",true);
		$strID = StringRemoveTrailingCharacters($strID,"_-",true);
		if ($strID == "")
		{
			return "";
		}
		if (StringSection($strID,0,4) != "TID_")
		{
			$strID = "TID_".$strID;
		}
		return $strID;
	}

	class CLocalResources
	{
		private $m_arrayLocalStrings;
		private $m_bStringResourceFileLoaded = false;

		function __construct()
		{
			$this->m_arrayLocalStrings = array();
		}

	
		function LoadStringResourceFile($strLanguage,$strFile)
		{	
			if (is_array($this->m_arrayLocalStrings) != true)
			{
				$this->m_arrayLocalStrings = array();	
			}			
			$arrayNewStrings = LoadNamedArrayFromTextFileMultiline($strFile,"TID_");
			if (is_array($arrayNewStrings))
			{
				$arrayExistingStrings = ArrayGetValue($this->m_arrayLocalStrings,StringLowerCase($strLanguage));
				if (is_array($arrayExistingStrings)!=true)
				{
					$arrayExistingStrings = array();
				}
				$arrayExistingStrings = ArrayMerge($arrayExistingStrings,$arrayNewStrings);  
				$this->m_arrayLocalStrings[StringLowerCase($strLanguage)] = $arrayExistingStrings;
				
				$this->m_bStringResourceFileLoaded = true;
			}
			return $this->m_bStringResourceFileLoaded;
		}
		
		function GetStrings($strLanguage)
		{
			if (!IsValidLanguageID($strLanguage))
			{
				return array();	
			}
			$arrayStrings = ArrayGetValue($this->m_arrayLocalStrings,StringLowerCase($strLanguage));
			if (is_array($arrayStrings) == false)
			{
				$arrayStrings = array();
			}
			return $arrayStrings;
		}
		
		function LoadString($strID,$strLanguage)
		{
			if ($strID == "")
			{
				return "";	
			}
			if ($this->m_bStringResourceFileLoaded == false)
			{
				return false;
			}
			if (!IsValidTID($strID))
			{
				return false;	
			}
			$strString = ArrayGetValue(
				$this->m_arrayLocalStrings,
				StringLowerCase($strLanguage),  
				StringUpperCase($strID));
			return $strString;
		}
	}

