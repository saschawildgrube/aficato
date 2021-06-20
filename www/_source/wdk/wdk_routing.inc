<?php

	require_once("wdk_csv.inc");

	function MakeRoutingAlias($strAlias,$bMakeLowerCase)
	{
		$strAlias = StringTranslateCharacters($strAlias,u(" 'ÁáÉéÍíÓóÚúÀàÈèÌìÒòÙùÂâÊêÎîÔôÛû"),"--AaEeIiOoUuAaEeIiOoUuAaEeIiOoUu");
		$strAlias = ReplaceString($strAlias,u("Ä"),"Ae");
		$strAlias = ReplaceString($strAlias,u("ä"),"ae");
		$strAlias = ReplaceString($strAlias,u("Ü"),"Ue");
		$strAlias = ReplaceString($strAlias,u("ü"),"ue");
		$strAlias = ReplaceString($strAlias,u("Ö"),"Oe");
		$strAlias = ReplaceString($strAlias,u("ö"),"oe");
		//$strAlias = iconv('UTF-8', 'ASCII//TRANSLIT', $strAlias);  
		$strAlias = StringFilter($strAlias,CHARSET_ALPHANUMERIC."-_");
		if ($bMakeLowerCase == true)
		{
			$strAlias = StringLowerCase($strAlias);
		}
		return $strAlias;
		
	}

	class CRoutingMap
	{
		private $m_arrayAliasToContext;
		private $m_arrayContextToAlias;

		function __construct()
		{
			$this->m_arrayAliasToContext = array();
			$this->m_arrayContextToAlias = array();
		}

		function LoadRoutingMapFile($strLanguage,$strFile)
		{	
			$arrayNewItems = FileReadCSV_WDK($strFile);
			if (is_array($arrayNewItems))
			{
				foreach ($arrayNewItems as $arrayItem)
				{
					if (is_array($arrayItem))
					{
						$strContextType = ArrayGetValue($arrayItem,"CONTEXTTYPE");
						$strContextID = ArrayGetValue($arrayItem,"CONTEXT");
						$strAlias = ArrayGetValue($arrayItem,"ALIAS");
						if ($strContextType != "" && $strContextID != "" && $strAlias != "")
						{
							$this->m_arrayContextToAlias[StringUpperCase($strLanguage)][$strContextType][$strContextID][] = $strAlias;
							$this->m_arrayAliasToContext[StringUpperCase($strLanguage)][$strContextType][MakeRoutingAlias($strAlias,true)] = $strContextID;
						}
					}
				}
			}
		}
		
		/*
		function GetStrings($strLanguage)
		{
			$arrayStrings = ArrayGetValue($this->m_arrayLocalStrings,StringLowerCase($strLanguage));
			if (is_array($arrayStrings) == false)
			{
				$arrayStrings = array();
			}
			return $arrayStrings;
		}
		*/
		
		function GetRoutingAlias($strLanguage,$strContextType,$strContextID,$nSeed)
		{
			$strContextID2 = $strContextID; 
			if ($strContextID2 == "" && $strContextType == "content")
			{
				$strContextID2 = "index";	  
			}
			$arrayContext = ArrayGetValue($this->m_arrayContextToAlias,StringUpperCase($strLanguage),$strContextType,$strContextID2);
			if (!is_array($arrayContext))
			{
				return $strContextID;	
			}
			$nAliasCount = ArrayCount($arrayContext);
			if ($nAliasCount == 1)  
			{
				$nIndex = 0;
			}
			else if ($nSeed < 0) // random
			{
				$nIndex = GetRandomInteger($nAliasCount -1);	
			}
			else
			{
				$nIndex = $nSeed % $nCount;	
			}
			return MakeRoutingAlias($arrayContext[$nIndex],false);
		}

		function GetRoutingContext($strLanguage,$strContextType,$strAlias)
		{
			$arrayContextType = ArrayGetValue($this->m_arrayAliasToContext,StringUpperCase($strLanguage),$strContextType);
			if (!is_array($arrayContextType))
			{
				return $strAlias;	
			}
			$strContext = ArrayGetValue($arrayContextType,StringLowerCase($strAlias));
			if ($strContext == "")
			{
				return $strAlias;	
			}
			return $strContext;
		}

		
	}

