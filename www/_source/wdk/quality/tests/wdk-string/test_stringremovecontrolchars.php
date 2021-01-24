<?php

	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringRemoveControlChars");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringRemoveControlChars($strString,$strExpectedResult)
		{
			$this->Trace("TestCase_StringRemoveControlChars");
			$this->Trace("Test String    : \"$strString\"");
			$this->Trace("Expected Result: \"$strExpectedResult\"");
			$strResult = StringRemoveControlChars($strString);
			$this->Trace("StringRemoveControlChars returns: \"$strResult\"");
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


		function OnTest()
		{
			parent::OnTest();
			
		
			$this->TestCase_StringRemoveControlChars(
				"123test123",
				"123test123");

			
			$this->TestCase_StringRemoveControlChars(
				"123\ttest\n\r123",
				"123test123");
			
			$this->TestCase_StringRemoveControlChars(
				"\t\t\n\n\r\r",
				"");
			
			

		}
		

	}
	
	

		
