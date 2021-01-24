<?php

	require_once(GetWDKDir()."wdk_string.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test IsUnsignedIntegerString");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_IsUnsignedIntegerString(
			$value,
			$bExpectedValue)
		{ 
			$this->Trace("TestCase_IsUnsignedIntegerString");
		
			$bValue = IsUnsignedIntegerString($value);
		
			$this->Trace("IsUnsignedIntegerString(".RenderValue($value).") = ".RenderBool($bValue));
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
			$this->TestCase_IsUnsignedIntegerString("",false);
			$this->TestCase_IsUnsignedIntegerString(" ",false);
			$this->TestCase_IsUnsignedIntegerString("a",false);
			$this->TestCase_IsUnsignedIntegerString(u("ä"),false);
			$this->TestCase_IsUnsignedIntegerString("1.0abcd",false);
			$this->TestCase_IsUnsignedIntegerString("4.5g234",false);
			$this->TestCase_IsUnsignedIntegerString("4.5e234e76.8768",false);
			$this->TestCase_IsUnsignedIntegerString("4.5 ",false);
			$this->TestCase_IsUnsignedIntegerString(" 4.5",false);
			$this->TestCase_IsUnsignedIntegerString(1.0,false);
			$this->TestCase_IsUnsignedIntegerString("FALSE",false);
			$this->TestCase_IsUnsignedIntegerString(5,false);
			$this->TestCase_IsUnsignedIntegerString("bogus",false);
			$this->TestCase_IsUnsignedIntegerString(array(3.4),false);
			$this->TestCase_IsUnsignedIntegerString(array(),false);
			$this->TestCase_IsUnsignedIntegerString("-1",false);
			$this->TestCase_IsUnsignedIntegerString("1 ",false);
			$this->TestCase_IsUnsignedIntegerString(" 1",false);

			$this->TestCase_IsUnsignedIntegerString("0",true);
			$this->TestCase_IsUnsignedIntegerString("1",true);
			$this->TestCase_IsUnsignedIntegerString("2",true);
			$this->TestCase_IsUnsignedIntegerString("10",true);
			$this->TestCase_IsUnsignedIntegerString("698769879786986",true);
		}
		
		
	}
	
	

		
