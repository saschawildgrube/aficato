<?php

	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test IsBoolString");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_IsBoolString(
			$value,
			$bExpectedValue)
		{ 
			$this->Trace("TestCase_IsBoolString");
		
			$bValue = IsBoolString($value);
		
			$this->Trace("IsBoolString(".RenderValue($value).") = ".RenderBool($bValue));
		
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
			$this->TestCase_IsBoolString("0",true);
			$this->TestCase_IsBoolString("1",true);
			$this->TestCase_IsBoolString("TRUE",true);
			$this->TestCase_IsBoolString("FALSE",true);
			$this->TestCase_IsBoolString("true",true);
			$this->TestCase_IsBoolString("false",true);
			$this->TestCase_IsBoolString("True",true);
			$this->TestCase_IsBoolString("False",true);
			
			$this->TestCase_IsBoolString(0,false);
			$this->TestCase_IsBoolString(1,false);
			$this->TestCase_IsBoolString(true,false);
			$this->TestCase_IsBoolString(false,false);
			$this->TestCase_IsBoolString("wahr",false);
			$this->TestCase_IsBoolString("falsch",false);
			$this->TestCase_IsBoolString("",false);
			$this->TestCase_IsBoolString(0.0,false);
			$this->TestCase_IsBoolString(array(),false);

		}
		
		
	}
	
	

		
