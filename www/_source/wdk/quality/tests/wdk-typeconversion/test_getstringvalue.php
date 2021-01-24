<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetStringValue");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}
	
	
		function TestCase_GetStringValue($value,$strExpectedResult)
		{
			$this->Trace("TestCase_GetStringValue");
			$strResult = GetStringValue($value);
			
			$this->Trace("RenderValue() input : ".RenderValue($value));
			$this->Trace("Expected result     : \"".$strExpectedResult."\"");
			$this->Trace("GetStringValue()    : \"".$strResult."\"");
			
			if ($strResult === $strExpectedResult)
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
			

		
			$this->TestCase_GetStringValue("","");
			$this->TestCase_GetStringValue(null,"");
			$this->TestCase_GetStringValue("abc","abc");
			$this->TestCase_GetStringValue(true,"1");
			$this->TestCase_GetStringValue(false,"0");
			$this->TestCase_GetStringValue(0,"0");
			$this->TestCase_GetStringValue(123,"123");
			$this->TestCase_GetStringValue(123.4,"123.4");
			$this->TestCase_GetStringValue(array(),"");
			$this->TestCase_GetStringValue(array(1,"Test"),"");


	
		}
		

	}
	
	

		
