<?php
	
	require_once(GetWDKDir()."wdk_locale.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK GetLocalSettings");
		}
		  

		function TestCase_GetLocaleSettings(
			$strCountryID,
			$strLanguageID,
			$bExpectedResult,
			$strExpectedKey,
			$strExpectedValue)
		{ 
			$this->Trace("TestCase_GetLocaleSettings");
			$this->Trace("strCountryID     = \"$strCountryID\"");
			$this->Trace("strLanguageID    = \"$strLanguageID\"");
			$this->Trace("bExpectedResult  = ".RenderBool($bExpectedResult));
			$this->Trace("strExpectedKey   = \"$strExpectedKey\"");
			$this->Trace("strExpectedValue = \"$strExpectedValue\"");
			
	
			$arraySettings = GetLocaleSettings($strCountryID,$strLanguageID);
			
			if ($bExpectedResult == false)
			{
				if ($arraySettings == false)
				{
					$this->Trace("Testcase PASSED!");
					return;
				}
				else
				{
					$this->Trace("Testcase FAILED!");	
					$this->SetResult(false);
					return;
				}
			}
			
			$this->Trace("RenderBool(arraySettings) = ".RenderBool($arraySettings));
			$this->Trace($arraySettings);
			
			if (ArrayGetValue($arraySettings,$strExpectedKey) == $strExpectedValue)
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
			
			$this->TestCase_GetLocaleSettings(
				"DEU",
				"DE",
				true,
				"DECIMAL_DELIMITER",
				",");

			$this->TestCase_GetLocaleSettings(
				"USA",
				"EN",
				true,
				"DECIMAL_DELIMITER",
				".");

			$this->TestCase_GetLocaleSettings(
				"XXX",
				"YY",
				false,
				"",
				"");
		}
	}
	
	

		
