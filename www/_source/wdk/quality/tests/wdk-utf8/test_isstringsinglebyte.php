<?php

	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test IsStringSingleByte");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_IsStringSingleByte(
			$strValue,
			$bExpectedValue)
		{ 
			$this->Trace("TestCase_IsStringSingleByte");
		
			$bValue = IsStringSingleByte($strValue);
		
			$this->Trace("IsStringSingleByte(\"$strValue\") = ".RenderBool($bValue));
		
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
			$this->TestCase_IsStringSingleByte("",true);
			$this->TestCase_IsStringSingleByte("abc",true);
			$this->TestCase_IsStringSingleByte(u("abc"),true);
			
			$this->TestCase_IsStringSingleByte(u("ƒ‹÷"),false);
			$this->TestCase_IsStringSingleByte(u("È·"),false);
			

		}
		
		
	}
	
	

		
