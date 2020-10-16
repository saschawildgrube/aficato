<?php
	
	require_once(GetWDKDir()."wdk_locale.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK GetLanguagesByCountry");
		}
		  

		function TestCase_GetLanguagesByCountry(
			$strCountryID,
			$arrayExpectedLanguages)
		{ 
			$this->Trace("TestCase_GetLanguagesByCountry");
			$this->Trace("strCountryID    = \"$strCountryID\"");
			$this->Trace("arrayExpectedLanguages");
			$this->Trace($arrayExpectedLanguages);
			
			$arrayLanguages = GetLanguagesByCountry($strCountryID);
			
			$this->Trace($arrayLanguages);
			
			if ($arrayExpectedLanguages == $arrayLanguages)
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
			
			$this->TestCase_GetLanguagesByCountry(
				"DEU",
				array("DE"));

			$this->TestCase_GetLanguagesByCountry(
				"XXX",
				array());


		}
	}
	
	

		
