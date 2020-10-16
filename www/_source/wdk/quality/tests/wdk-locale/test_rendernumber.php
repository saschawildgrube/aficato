<?php
	
	require_once(GetWDKDir()."wdk_locale.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK RenderNumber");
		}
		  

		function TestCase_RenderNumber(
			$fValue,
			$nPrecision,
			$strMetric,
			$arrayLocaleSettings,
			$strExpected)
		{ 
			$this->Trace("TestCase_RenderNumber");
			$this->Trace("strExpected	          = \"$strExpected\"");
			$this->Trace("fValue 		          = $fValue");
			$this->Trace("nPrecision             = $nPrecision");
			$this->Trace("arrayLocaleSettings");
			$this->Trace($arrayLocaleSettings);
			
			$strNumber = RenderNumber(
				$fValue,
				$nPrecision,
				$strMetric,
				$arrayLocaleSettings);
			
			$this->Trace("Result = \"$strNumber\"");
			
			if ($strExpected == $strNumber)
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
			
			// Positive Values
			
			$arrayLocaleSettings = GetLocaleSettings("DEU","DE");			
			$this->TestCase_RenderNumber(
				1,
				0,
				"",
				$arrayLocaleSettings,
				"1");

			$arrayLocaleSettings = GetLocaleSettings("DEU","DE");
			$this->TestCase_RenderNumber(
				123.23,
				3,
				"",
				$arrayLocaleSettings,
				"123,230");

			$arrayLocaleSettings = GetLocaleSettings("DEU","DE");
			$this->TestCase_RenderNumber(
				123.23,
				2,
				"m",
				$arrayLocaleSettings,
				"123,23 m");
				
			$this->TestCase_RenderNumber(
				123.23,
				2,
				"m",
				array(),
				"123.23 m");				
				
			$this->TestCase_RenderNumber(
				123000.23,
				2,
				"m",
				array(),
				"123000.23 m");

			$this->TestCase_RenderNumber(
				12300.23,
				-3,
				"m",
				array(),
				"12K m");

			$this->TestCase_RenderNumber(
				12500.23,
				-3,
				"m",
				array(),
				"13K m");

			$this->TestCase_RenderNumber(
				1250000.23,
				-6,
				"m",
				array(),
				"1M m");
				
			$arrayLocaleSettings = GetLocaleSettings("GBR","EN");
			$this->TestCase_RenderNumber(
				123000.23,
				2,
				"m",
				$arrayLocaleSettings,
				"123,000.23 m");

			// Negative Values

			$arrayLocaleSettings = GetLocaleSettings("DEU","DE");		
			$this->TestCase_RenderNumber(
				-1,
				0,
				"",
				$arrayLocaleSettings,
				"-1");

			$arrayLocaleSettings = GetLocaleSettings("DEU","DE");
			$this->TestCase_RenderNumber(
				-123.23,
				3,
				"",
				$arrayLocaleSettings,
				"-123,230");

			$arrayLocaleSettings = GetLocaleSettings("DEU","DE");
			$this->TestCase_RenderNumber(
				-123.23,
				2,
				"m",
				$arrayLocaleSettings,
				"-123,23 m");
				
			$this->TestCase_RenderNumber(
				-123.23,
				2,
				"m",
				array(),
				"-123.23 m");				
				
			$this->TestCase_RenderNumber(
				-123000.23,
				2,
				"m",
				array(),
				"-123000.23 m");

				
			$this->TestCase_RenderNumber(
				-12300.23,
				-3,
				"m",
				array(),
				"-12K m");

			$this->TestCase_RenderNumber(
				-12500.23,
				-3,
				"m",
				array(),
				"-13K m");

			$this->TestCase_RenderNumber(
				-1250000.23,
				-6,
				"m",
				array(),
				"-1M m");
				
			$arrayLocaleSettings = GetLocaleSettings("GBR","EN");
			$this->TestCase_RenderNumber(
				-123000.23,
				2,
				"m",
				$arrayLocaleSettings,
				"-123,000.23 m");
				
			// Special cases

			$arrayLocaleSettings = GetLocaleSettings("NOR","NN");
			if (ArrayCount($arrayLocaleSettings) == 0)
			{
				$this->Trace("Locale setting for NOR/NN is missing. Check out if all locales are installed!");
			}
			else
			{
				$this->TestCase_RenderNumber(
					123000.23,
					2,
					"",
					$arrayLocaleSettings,
					"123000,23");
			}
		}
	}
	
	

		
