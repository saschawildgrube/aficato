<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("RenderInteger");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_RenderInteger($value,$strExpectedResult)
		{
			$this->Trace("TestCase_RenderInteger");
			$this->Trace("RenderValue    : ".RenderValue($value));  
			$this->Trace("Expected result: \"$strExpectedResult\"");
			$strResult = RenderInteger($value); 
			$this->Trace("RenderInteger returns: \"$strResult\"");
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


		function CallbackTest()
		{
			parent::CallbackTest(); 

			$this->TestCase_RenderInteger(
				0,
				"0");

			$this->TestCase_RenderInteger(
				1,
				"1");

			$this->TestCase_RenderInteger(
				-1,
				"-1");

			$this->TestCase_RenderInteger(
				10000.0,
				"10000");

			$this->TestCase_RenderInteger(
				10000.01,
				"10000");

			$this->TestCase_RenderInteger(
				10000.6,
				"10001");
				
			$this->TestCase_RenderInteger(
				false,
				"0");

			$this->TestCase_RenderInteger(
				true,
				"1");

			$this->TestCase_RenderInteger(
				"",
				"0");
				
			$this->TestCase_RenderInteger(
				"abc",
				"0");

			$this->TestCase_RenderInteger(
				"1",
				"1");

			$this->TestCase_RenderInteger(
				"-1",
				"-1");

			$this->TestCase_RenderInteger(
				"1KB",
				"1024");
				
		
		}
		

	}
	
	

		
