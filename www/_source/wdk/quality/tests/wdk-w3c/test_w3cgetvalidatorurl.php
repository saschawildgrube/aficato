<?php

	require_once(GetWDKDir()."wdk_w3c.inc");

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("w3cGetValidatorURL");
		}
		

		function TestCase_w3cGetValidatorURL(
			$strURL,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_w3cGetValidatorURL");
			$this->Trace("URL            : $strURL");
			$this->Trace("Expected Result: $strExpectedResult");

			$strResult = w3cGetValidatorURL($strURL);
			
			$this->Trace("Result         : $strResult");   
			
			if ($strResult != $strExpectedResult)
			{
				$this->Trace("Testcase FAILED!");	
				$this->Trace("");
				$this->Trace("");
				$this->SetResult(false);	
				return;
			}
	
			$this->Trace("Testcase PASSED!");
			$this->Trace("");
			$this->Trace("");
			
		}



		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->SetResult(true);
			
			$this->TestCase_w3cGetValidatorURL(
				"www.example.com",
				"https://validator.w3.org/nu/?doc=www.example.com");

		}
	}
	
	

		
