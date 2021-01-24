<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringCutOff");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringCutOff($strString,$nMaxLength,$strTrailer,$strExpectedResult)
		{
			$this->Trace("TestCase_StringCutOff");
			$this->Trace("Test String    : \"$strString\"");
			$this->Trace("nMaxLength	  : \"$nMaxLength\"");
			$this->Trace("strTrailer	  : \"$strTrailer\"");
			$this->Trace("Expected Result: \"$strExpectedResult\"");
			$strResult = StringCutOff($strString,$nMaxLength,$strTrailer); 
			$this->Trace("StringCutOff returns: \"$strResult\"");
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

			$this->TestCase_StringCutOff(
				"1234 67890 23456789 1234 67890 23456789",
				18,
				"...",
				"1234 67890...");
			
		
			$this->TestCase_StringCutOff(
				"1234 67890 23456789 1234 67890 23456789",
				20,
				"...",
				"1234 67890 23456789...");

			$this->TestCase_StringCutOff(
				"1234 67890 23456789 1234 67890 23456789",
				22,
				"...",
				"1234 67890 23456789...");

			$this->TestCase_StringCutOff(
				"1234 67890 23456789 1234 67890 23456789",
				40,
				"...",
				"1234 67890 23456789 1234 67890 23456789");

		}
		

	}
	
	

		
