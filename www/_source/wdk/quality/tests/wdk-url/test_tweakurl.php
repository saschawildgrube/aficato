<?php

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("TweakURL");
		}
		

		function TestCase_TweakURL(
			$strURL,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_TweakURL");
			$this->Trace("URL: $strURL");
			$this->Trace("Expected Result: $strExpectedResult");

			$strResult = TweakURL($strURL);
			
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
			
			$this->TestCase_TweakURL(
				"www.example.com",
				"http://www.example.com");
				
				/*
			$this->TestCase_TweakURL(
				"example.com",
				"http://www.example.com");
				*/

		}
	}
	
	

		
