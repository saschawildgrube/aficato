<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetBoolValue");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}
	
	
		function TestCase_GetBoolValue($value,$bExpectedResult)
		{
			$this->Trace("TestCase_GetBoolValue");
			$this->Trace("value = $value");
			$this->Trace("Expected Result: \"".RenderBool($bExpectedResult)."\"");
			$bResult = GetBoolValue($value);
			$this->Trace("GetBoolValue($value) = \"".RenderBool($bResult)."\"");
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
			

		
			$this->TestCase_GetBoolValue("true",true);
			$this->TestCase_GetBoolValue("false",false);

			$this->TestCase_GetBoolValue("1",true);
			$this->TestCase_GetBoolValue("0",false);

			$this->TestCase_GetBoolValue(1,true);
			$this->TestCase_GetBoolValue(0,false);

			$this->TestCase_GetBoolValue(true,true);
			$this->TestCase_GetBoolValue(false,false);

			$this->TestCase_GetBoolValue("TRUE",true);
			$this->TestCase_GetBoolValue("FALSE",false);

			$this->TestCase_GetBoolValue("whatever",true);
			$this->TestCase_GetBoolValue("",false);

		}
		

	}
	
	

		
