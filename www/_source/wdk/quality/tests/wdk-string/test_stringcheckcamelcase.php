<?php
	
	require_once(GetWDKDir()."wdk_parameter.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test StringCheckCamelCase");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}

		function TestCase_StringCheckCamelCase($strString,$bExpectedResult)
		{
			$this->Trace("TestCase_StringCheckCamelCase");
			$this->Trace("strString = \"$strString\"");
			$this->Trace("Expected Result: ".RenderBool($bExpectedResult)."");
			$bResult = StringCheckCamelCase($strString);    
			$this->Trace("StringCheckCamelCase returns: ".RenderBool($bResult));
			if ($bResult == $bExpectedResult)
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
			
			$this->TestCase_StringCheckCamelCase("CamelCase",true);
			$this->TestCase_StringCheckCamelCase("WDK",true);
			$this->TestCase_StringCheckCamelCase("A",true);
			
			$this->TestCase_StringCheckCamelCase("a",false);
			$this->TestCase_StringCheckCamelCase("wdk",false);
			$this->TestCase_StringCheckCamelCase(u("Olé"),false);
			$this->TestCase_StringCheckCamelCase(u("Äüö"),false);
			$this->TestCase_StringCheckCamelCase("123mine",false);
			$this->TestCase_StringCheckCamelCase("camelCase",false);
			
		}
		
	}
	
	

		
