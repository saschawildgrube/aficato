<?php

	require_once(GetWDKDir()."wdk_locale.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK GetCountriesByLanguage");
		}
		  

		function TestCase_GetCountriesByLanguage(
			$strLanguageID,
			$arrayExpectedCountries)
		{ 
			$this->Trace("TestCase_GetCountriesByLanguage");
			$this->Trace("strLanguageID    = \"$strLanguageID\"");
			$this->Trace("arrayExpectedCountries");
			$this->Trace($arrayExpectedCountries);
			
			$arrayCountries = GetCountriesByLanguage($strLanguageID);
			
			$this->Trace($arrayCountries);
			
			if ($arrayExpectedCountries == $arrayCountries)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);	
			}

			$this->Trace("");
			$this->Trace("");
		}


		function CallbackTest()
		{
			parent::CallbackTest();
					
			$this->SetResult(true);
			
			$this->TestCase_GetCountriesByLanguage(
				"DE",
				array("AUT","BEL","CHE","DEU","LUX"));

			$this->TestCase_GetCountriesByLanguage(
				"EN",
				array("CAN","GBR","IRL","PHL","SGP","USA"));

			$this->TestCase_GetCountriesByLanguage(
				"FR",
				array("BEL","CAN","CHE","FRA","LUX"));   


			$this->TestCase_GetCountriesByLanguage(
				"XX",
				array());


		}
	}
	
	

		
