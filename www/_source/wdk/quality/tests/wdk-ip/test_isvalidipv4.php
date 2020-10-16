<?php

	require_once(GetWDKDir()."wdk_ip.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test IsValidIPv4");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_IsValidIPv4(
			$value,
			$bExpectedValue)
		{ 
			$this->Trace("TestCase_IsValidIPv4");
		
			$bValue = IsValidIPv4($value);
		
			$this->Trace("IsValidIPv4(".RenderValue($value).") = ".RenderBool($bValue));
		
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

		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->TestCase_IsValidIPv4("192.168.1.1",true);
			$this->TestCase_IsValidIPv4("255.255.255.255",true);
			$this->TestCase_IsValidIPv4("0.0.0.0",true);
			
			

			$this->TestCase_IsValidIPv4("",false);
			$this->TestCase_IsValidIPv4("http://www.example.com",false);
			$this->TestCase_IsValidIPv4("192.168.100",false);
			$this->TestCase_IsValidIPv4("256.1.1.1",false);
			$this->TestCase_IsValidIPv4(1,false);
			$this->TestCase_IsValidIPv4(false,false);
			
		}
		
		
	}
	
	

		
