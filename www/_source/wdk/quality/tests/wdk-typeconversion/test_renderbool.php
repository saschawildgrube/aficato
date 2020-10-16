<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("RenderBool");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);	
			return true;
		}
	
	
		function TestCase_RenderBool($bValue,$bUppercase,$strExpectedResult)
		{
			$this->Trace("TestCase_RenderBool");
			$this->Trace("bValue = \"$bValue\", bUppercase = \"$bUppercase\"");
			$this->Trace("Expected Result: ".RenderValue($strExpectedResult));
			$strResult = RenderBool($bValue,$bUppercase);
			$this->Trace("RenderBool($bValue) = \"$strResult\"");
			if ($strResult == $strExpectedResult)
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
		
		function CallbackTest()
		{
			parent::CallbackTest();
			

		
			$this->TestCase_RenderBool(true,false,"true");
			$this->TestCase_RenderBool(true,true,"TRUE");
			$this->TestCase_RenderBool(false,false,"false");
			$this->TestCase_RenderBool(false,true,"FALSE");
		
			$this->TestCase_RenderBool("1",false,"true");
			$this->TestCase_RenderBool("1",true,"TRUE");
			$this->TestCase_RenderBool("0",false,"false");
			$this->TestCase_RenderBool("0",true,"FALSE");
		
			$this->TestCase_RenderBool("true",false,"true");
			$this->TestCase_RenderBool("true",true,"TRUE");
			$this->TestCase_RenderBool("false",false,"false");
			$this->TestCase_RenderBool("false",true,"FALSE");
		
						
			$this->TestCase_RenderBool("TRUE",false,"true");
			$this->TestCase_RenderBool("TRUE",true,"TRUE");
			$this->TestCase_RenderBool("FALSE",false,"false");
			$this->TestCase_RenderBool("FALSE",true,"FALSE");
			
		}
		

	}
	
	

		
