<?php

	require_once(GetWDKDir()."wdk_bot.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test DetectBot");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_DetectBot(
			$strClientIP,
			$strUserAgent,
			$strExpectedValue)
		{ 
			$this->Trace("TestCase_DetectBot");
		
			$strValue = DetectBot($strClientIP,$strUserAgent);
			$this->Trace("Expected: ".RenderValue($strExpectedValue));
			$this->Trace("DetectBot(".RenderValue($strClientIP).",".RenderValue($strUserAgent).") = ".RenderValue($strValue));
		
			if ($strValue == $strExpectedValue)
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

			$this->TestCase_DetectBot("","",false);			
			$this->TestCase_DetectBot("123.123.123.123","blah +http://www.google.com","http://www.google.com/");
			$this->TestCase_DetectBot("123.123.123.123","blah +http://www.bing.com","http://www.bing.com/");
			$this->TestCase_DetectBot("72.47.244.17","","https://www.sslshopper.com/");
		}
		
		
	}
	
	

		
