<?php

	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test IsFloatString");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_IsFloatString(
			$value,
			$bExpectedValue)
		{ 
			$this->Trace("TestCase_IsFloatString");
		
			$bValue = IsFloatString($value);
		
			$this->Trace("IsFloatString(".RenderValue($value).") = ".RenderBool($bValue));
			$this->Trace("Expected = ".RenderValue($bExpectedValue));
		
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
			$this->TestCase_IsFloatString("",false);
			
			$this->TestCase_IsFloatString("1.0abcd",false);
			$this->TestCase_IsFloatString("4.5g234",false);
			$this->TestCase_IsFloatString("4.5e234e76.8768",false);
			$this->TestCase_IsFloatString("4.5 ",false);
			$this->TestCase_IsFloatString(" 4.5",false);
			$this->TestCase_IsFloatString(1.0,false);
			$this->TestCase_IsFloatString("FALSE",false);
			$this->TestCase_IsFloatString(5,false);
			$this->TestCase_IsFloatString("bogus",false);
			$this->TestCase_IsFloatString(array(3.4),false);
			$this->TestCase_IsFloatString(array(),false);

			$this->TestCase_IsFloatString("1",true);
			$this->TestCase_IsFloatString("-.5",true);
			$this->TestCase_IsFloatString(".5",true);
			$this->TestCase_IsFloatString("34.",true);
			$this->TestCase_IsFloatString("1.0",true);
			$this->TestCase_IsFloatString("-1.0",true);
			$this->TestCase_IsFloatString("3.45e34",true);
			$this->TestCase_IsFloatString("-3.45e-1",true);
			$this->TestCase_IsFloatString("3.45e-1",true);
 
		}
		
		
	}
	
	

		
