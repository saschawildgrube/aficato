<?php

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringURLEncode");
		}
		

		function TestCase_StringURLEncode(
			$strRaw,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_StringURLEncode");
			$this->Trace("Raw: $strRaw");
			$this->Trace("Expected Result: $strExpectedResult");

			$strResult = StringURLEncode($strRaw);
			
			$this->Trace("Result: $strResult");
			
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



		function OnTest()
		{
			parent::OnTest();
			
			$this->SetResult(true);
			
			$this->TestCase_StringURLEncode(
				"",
				"");

			$this->TestCase_StringURLEncode(
				"a&b",
				"a%26b"); 

			$this->TestCase_StringURLEncode(
				"a b",
				"a+b");

			$this->TestCase_StringURLEncode(
				"a:b",
				"a%3Ab"); 

			$this->TestCase_StringURLEncode(
				"a?b",
				"a%3Fb");

				


		}
	}
	
	

		
