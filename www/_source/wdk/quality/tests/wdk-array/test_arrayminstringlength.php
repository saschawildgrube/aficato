<?php

	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test ArrayMinStringLength");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_ArrayMinStringLength(
			$arrayInput,
			$nExpectedValue)
		{ 
			$this->Trace("TestCase_ArrayMinStringLength");
		
			$nValue = ArrayMinStringLength($arrayInput);
		
			$this->Trace("Input:");
			$this->Trace(RenderValue($arrayInput));
		
			$this->Trace("ArrayMinStringLength = ".RenderValue($nValue));
		
			if ($nValue === $nExpectedValue)
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
			$this->TestCase_ArrayMinStringLength($arrayInput,false);

			$arrayInput = array("");
			$this->TestCase_ArrayMinStringLength($arrayInput,0);

			$arrayInput = array(
				"apples",
				"oranges");
			$this->TestCase_ArrayMinStringLength($arrayInput,6);

			$arrayInput = array(
				"first" => "apples",
				"second" => "oranges");
			$this->TestCase_ArrayMinStringLength($arrayInput,6);

			$arrayInput = array(
				23,
				123);
			$this->TestCase_ArrayMinStringLength($arrayInput,2);

			$arrayInput = array(
				23.45);
			$this->TestCase_ArrayMinStringLength($arrayInput,5);

			$arrayInput = array(
				23.45, 
				1000.01);
			$this->TestCase_ArrayMinStringLength($arrayInput,5);


			$arrayInput = "";
			$this->TestCase_ArrayMinStringLength($arrayInput,false);

			$arrayInput = 23.45;
			$this->TestCase_ArrayMinStringLength($arrayInput,false); 
			
			
		}
		
		
	}
	
	

		
