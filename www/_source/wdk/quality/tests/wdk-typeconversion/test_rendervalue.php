<?php
	
	
	class CExample
	{
		public $m_strExample;
	}
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("RenderValue");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);	
			return true;
		}
	
	
		function TestCase_RenderValue($value,$strExpectedResult)
		{
			$this->Trace("TestCase_RenderValue");
			$strResult = RenderValue($value);
			$this->Trace("RenderValue():");
			$this->Trace($strResult);
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
			

		
			$this->TestCase_RenderValue(true,'true');
			$this->TestCase_RenderValue(false,"false");
			$this->TestCase_RenderValue("","\"\"");
			$this->TestCase_RenderValue("123","\"123\"");
			$this->TestCase_RenderValue(123,"123");
			$this->TestCase_RenderValue("Test","\"Test\"");
			$this->TestCase_RenderValue(array(1,"Test"),"Array\n(\n    [0] => 1\n    [1] => Test\n)\n");
			
			$example = new CExample();
			$example->m_strExample = 'Hello World';
			$this->TestCase_RenderValue($example,"CExample Object\n(\n    [m_strExample] => Hello World\n)\n");  
			


	
		}
		

	}
	
	

		
