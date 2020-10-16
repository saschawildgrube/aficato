<?php

	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test ArrayCount");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_ArrayCount(
			$arrayInput,
			$nExpectedValue)
		{ 
			$this->Trace("TestCase_ArrayCount");
		
			$nValue = ArrayCount($arrayInput);
		
			$this->Trace("Input Array:");
			$this->Trace($arrayInput);
		
			$this->Trace("ArrayCount() = ".RenderValue($nValue));
		
			if ($nValue == $nExpectedValue)
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

		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$arrayInput = array();
			$this->TestCase_ArrayCount($arrayInput,0);
			
			$arrayInput = array(
				"apples",
				"oranges");
			$this->TestCase_ArrayCount($arrayInput,2);

			$arrayInput = array(
				"first" => "apples",
				"second" => "oranges");
			$this->TestCase_ArrayCount($arrayInput,2);

			$this->TestCase_ArrayCount("hallo",0);
			$this->TestCase_ArrayCount(42,0);
			
			
		}
		
		
	}
	
	

		
