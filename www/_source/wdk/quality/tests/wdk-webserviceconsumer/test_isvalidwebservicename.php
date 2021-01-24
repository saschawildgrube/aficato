<?php

	require_once(GetWDKDir()."wdk_webserviceconsumer.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test IsValidWebserviceName");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_IsValidWebserviceName(
			$value,
			$bExpectedValue)
		{ 
			$this->Trace("TestCase_IsValidWebserviceName");
		
			$bValue = IsValidWebserviceName($value);
		
			$this->Trace("IsValidWebserviceName(".RenderValue($value).") = ".RenderBool($bValue));
		
			if ($bValue == $bExpectedValue)
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
			
			$this->TestCase_IsValidWebserviceName("demo/demo",true);
			$this->TestCase_IsValidWebserviceName("a/b",true);
			$this->TestCase_IsValidWebserviceName("test123/123test",true);
 			
 			$this->TestCase_IsValidWebserviceName("",false);
 			$this->TestCase_IsValidWebserviceName("http://www.example.com/webservices/demo/demo/",false);
 			$this->TestCase_IsValidWebserviceName("http://www.example.com/webservices/demo/demo",false);
 			$this->TestCase_IsValidWebserviceName("demo",false);
 			$this->TestCase_IsValidWebserviceName("demo/",false);
 			$this->TestCase_IsValidWebserviceName("/demo",false);
 			
		
			
		}
		
		
	}
	
	

		
