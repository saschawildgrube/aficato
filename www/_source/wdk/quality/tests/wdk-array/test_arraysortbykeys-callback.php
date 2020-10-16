<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArraySortByKeys with callback function");
		}
		  

		function TestCase_ArraySortByKeys(
			$arrayInput,
			$CallbackCompare,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArraySortByKeys");
			$arraySorted = ArraySortByKeys($arrayInput,$CallbackCompare);
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
				"a-a" => "a",
				"aa" => "x",
				"a" => "z"
				);
			$arrayExpected = array(
				"a" => "z",
				"aa" => "x",
				"a-a" => "a"
				);
			$this->TestCase_ArraySortByKeys($arrayInput,'CompareStringIgnoreHyphen',$arrayExpected);


		}
	}
	
	

		
