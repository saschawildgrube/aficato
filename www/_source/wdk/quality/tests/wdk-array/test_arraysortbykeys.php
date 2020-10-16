<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArraySortByKeys");
		}
		  

		function TestCase_ArraySortByKeys(
			$arrayInput,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArraySortByKeys");
			$arraySorted = ArraySortByKeys($arrayInput);
			$this->Trace($arraySorted);
			if (ArrayStrictCompare($arraySorted,$arrayExpectedResult))
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
					
			$this->SetResult(true);
			
			$arrayInput = array(
				1 => "z",
				2 => "x",
				3 => "a"
				);
			$arrayExpected = array(
				1 => "z",
				2 => "x",
				3 => "a"
				);
			$this->TestCase_ArraySortByKeys($arrayInput,$arrayExpected);

			$arrayInput = array(
				3 => "a",
				1 => "z",
				2 => "x",
				);
			$arrayExpected = array(
				1 => "z",
				2 => "x",
				3 => "a"
				);
			$this->TestCase_ArraySortByKeys($arrayInput,$arrayExpected);

			$arrayInput = array(
				"gamma" => "a",
				"beta" => "x",
				"alpha" => "z"
				);
			$arrayExpected = array(
				"alpha" => "z",
				"beta" => "x",
				"gamma" => "a"
				);
			$this->TestCase_ArraySortByKeys($arrayInput,$arrayExpected);


		}
	}
	
	

		
