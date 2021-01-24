<?php

	require_once(GetWDKDir()."wdk_config.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test IsValidConfigID");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_IsValidConfigID(
			$value,
			$bExpectedValue)
		{ 
			$this->Trace("TestCase_IsValidConfigID");
		
			$bValue = IsValidConfigID($value);
		
			$this->Trace("IsValidConfigID(".RenderValue($value).") = ".RenderBool($bValue));
		
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
			$this->TestCase_IsValidConfigID("abc123",true);
			$this->TestCase_IsValidConfigID("1",true);
			$this->TestCase_IsValidConfigID("prod",true);
			$this->TestCase_IsValidConfigID("123",true);

			$this->TestCase_IsValidConfigID("",false);
			$this->TestCase_IsValidConfigID(false,false);
			$this->TestCase_IsValidConfigID("test@example.com",false);
			$this->TestCase_IsValidConfigID("-test-",false);
			$this->TestCase_IsValidConfigID("test-",false);
			$this->TestCase_IsValidConfigID("-test",false);
			$this->TestCase_IsValidConfigID(" test",false);
			$this->TestCase_IsValidConfigID("test.cfg",false);
			

		}
		
		
	}
	
	

		
