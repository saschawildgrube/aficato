<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringSection");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringSection($strString,$nStart,$nLength,$strExpectedResult)
		{
			$this->Trace("TestCase_StringSection");
			$this->Trace("strString	: \"$strString\"");
			$this->Trace("nStart		: $nStart");
			if ($nLength===false)
			{
				$this->Trace("nLength	: false");
			}
			else
			{
				$this->Trace("nLength	: $nLength");
			}
			$this->Trace("Expected Result: \"$strExpectedResult\"");
			$strResult = StringSection($strString,$nStart,$nLength); 
			$this->Trace("StringSection returns: \"$strResult\"");
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

			$this->TestCase_StringSection(
				"",
				0,
				0,
				"");

			$this->TestCase_StringSection(
				"",
				100,
				0,
				"");

			$this->TestCase_StringSection(
				"",
				100,
				-40,
				"");


			$this->TestCase_StringSection(
				"12345678901234567890",
				0,
				false,
				"12345678901234567890");

			$this->TestCase_StringSection(
				"12345678901234567890",
				4,
				false,
				"5678901234567890");

			$this->TestCase_StringSection(
				"12345678901234567890",
				4,
				5,
				"56789");

			$this->TestCase_StringSection(
				"12345678901234567890",
				-3,
				false,
				"890");

			$this->TestCase_StringSection(
				"12345678901234567890",
				-3,
				0,
				"");

			$this->TestCase_StringSection(
				"12345678901234567890",
				-3,
				5,
				"890");
 
			$this->TestCase_StringSection(
				"12345",
				-5,
				-5,
				"");

			$this->TestCase_StringSection(
				u("ÄÖÜÄÖÜ"),
				1,
				2,
				u("ÖÜ"));
			
			$this->TestCase_StringSection(
				"12345678901234567890",
				0,
				-3,
				"12345678901234567");			

			$this->TestCase_StringSection(
				"12345678901234567890",
				2,
				-3,
				"345678901234567");			

		
		}
		

	}
	
	

		
