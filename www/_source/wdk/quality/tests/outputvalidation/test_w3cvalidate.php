<?php
	
	require_once(GetWDKDir()."wdk_w3c.inc");	
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("W3C Validator");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		
		function TestCase_w3cValidateURL(
			$strURL,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_w3cValidateURL");
	
			$this->Trace("strURL = \"$strURL\"");
			$this->Trace("strExpectedResult = \"$strExpectedResult\"");

			$strValidatorURL = w3cGetValidatorURL($strURL);
			$this->Trace("Validator URL: \"$strValidatorURL\"");
			
			$strResult = w3cValidateURL($strURL);
			$this->Trace("Testcase w3cValidateURL returns: \"$strResult\"");
			
			if ($strResult == $strExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else if ($strResult == "UNDEFINED")
			{
				$this->Trace("Testcase UNDEFINED! This is because the w3c validator did not yield any defined result.");
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
			
			$this->TestCase_w3cValidateURL(
				"http://".GetRootURL(),
				"PASSED");

			$this->TestCase_w3cValidateURL(
				"http://".GetRootURL()."?trace=1",
				"PASSED");
		}

		
		
	}
	
	

		
