<?php

	require_once(GetWDKDir()."wdk_ip.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test IsValidHostName");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_IsValidHostName(
			$value,
			$bExpectedValue)
		{ 
			$this->Trace("TestCase_IsValidHostName");
		
			$bValue = IsValidHostName($value);
		
			$this->Trace("IsValidHostName(".RenderValue($value).") = ".RenderBool($bValue));
		
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
			
			$this->TestCase_IsValidHostName("host123.tld",true);
			$this->TestCase_IsValidHostName("localhost",true);
			$this->TestCase_IsValidHostName("example.com",true);
			$this->TestCase_IsValidHostName("255.255.255.255",false);
			$this->TestCase_IsValidHostName("0.0.0.0",false);
			
			$this->TestCase_IsValidHostName("",false);
			$this->TestCase_IsValidHostName(" ",false);
			$this->TestCase_IsValidHostName("http://www.example.com",false);
			$this->TestCase_IsValidHostName("192.168.100",false);
			$this->TestCase_IsValidHostName("256.1.1.1",false);
			$this->TestCase_IsValidHostName(1,false);
			$this->TestCase_IsValidHostName(false,false);
			
		}
		
		
	}
	
	

		
