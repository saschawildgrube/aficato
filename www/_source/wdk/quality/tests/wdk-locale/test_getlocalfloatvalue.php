<?php
	
	require_once(GetWDKDir()."wdk_locale.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK GetLocalFloatValue");
		}
		  

		

		function TestCase_GetLocalFloatValue(
			$value,
			$strCountry,
			$strLanguage,
			$fExpected)
		{ 
			$this->Trace("TestCase_GetLocalFloatValue");
			$this->Trace("fExpected	          	 = $fExpected");
			$this->Trace("value 		             = $value");
			$this->Trace("strCountry             = $strCountry");
			$this->Trace("strLanguage            = $strLanguage");
		
			$fResult = GetLocalFloatValue(
				$value,
				$strCountry,
				$strLanguage);
			
			$this->Trace("fResult = $fResult");
			
			if ($fResult == $fExpected)
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


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);
			
			
			$this->TestCase_GetLocalFloatValue(
				floatval(1.5),
				"DEU",
				"DE",
				1.5);

			$this->TestCase_GetLocalFloatValue(
				"1,5",
				"DEU",
				"DE",
				1.5);

			$this->TestCase_GetLocalFloatValue(
				"bogus",
				"DEU",
				"DE",
				0);

			$this->TestCase_GetLocalFloatValue(
				"",
				"DEU",
				"DE",
				0);

			$this->TestCase_GetLocalFloatValue(
				0,
				"DEU",
				"DE",
				0);

			$this->TestCase_GetLocalFloatValue(
				"1,000.05",
				"USA",
				"EN",
				1000.05);

		}
	}
	
	

		
