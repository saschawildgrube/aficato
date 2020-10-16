<?php

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringGetEndOfLineToken");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringGetEndOfLineToken($strString,$strExpectedResult)
		{
			$this->Trace("TestCase_StringGetEndOfLineToken");
			$this->Trace("Test Data:");
			$this->Trace("\"$strString\"");
			$this->Trace("Expected Result: \"".RenderHex($strExpectedResult)."\"");
			$strResult = StringGetEndOfLineToken($strString);
			$this->Trace("StringGetEndOfLineToken returns: \"".RenderHex($strExpectedResult)."\"");
			if ($strResult == $strExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
			}
			$this->Trace("");
			
		}


		
		function CallbackTest()
		{
			parent::CallbackTest();
			

			$this->TestCase_StringGetEndOfLineToken(
				"",
				"");

			$this->TestCase_StringGetEndOfLineToken(
				"123",
				"");

		
			$this->TestCase_StringGetEndOfLineToken(
				"123\n456\n123",
				"\n");

			
			$this->TestCase_StringGetEndOfLineToken(
				"\r\n123\r\n456\r\n123",
				"\r\n");
			
			$this->TestCase_StringGetEndOfLineToken(
				"123\n\r456\n\r123",
				"\n\r");


			$this->TestCase_StringGetEndOfLineToken(
				"123\n\r456\r\n123",
				"\r\n");

		}
		

	}
	
	

		
