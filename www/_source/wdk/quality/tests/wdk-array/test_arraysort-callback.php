<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArraySort with callback function");
		}
		  

		function TestCase_ArraySort(
			$arrayInput,
			$bMaintainKeys,
			$CallbackFunction,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArraySort");
			$arraySorted = ArraySort($arrayInput,$bMaintainKeys,$CallbackFunction);
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
				1 => "a-a",
				2 => "aa",
				3 => "a"
				);
			$arrayExpected = array(
				3 => "a",
				2 => "aa",
				1 => "a-a"
				);
			$this->TestCase_ArraySort($arrayInput,true,'CompareStringIgnoreHyphen',$arrayExpected);

			$arrayInput = array(
				1 => "hallo-welt",
				2 => "hallowelt",
				3 => "h-allo-welt"
				);
			$arrayExpected = array(
				0 => "hallowelt",
				1 => "hallo-welt",
				2 => "h-allo-welt"
				);
			$this->TestCase_ArraySort($arrayInput,false,'CompareStringIgnoreHyphen',$arrayExpected);

			$arrayInput = array(
				"alpha" => "a-a",
				"beta" =>  "aa",
				"gamma" => "a"
				);
			$arrayExpected = array(
				"gamma" => 	"a",
				"beta" => 	"aa",
				"alpha" => 	"a-a"
				);
			$this->TestCase_ArraySort($arrayInput,true,'CompareStringIgnoreHyphen',$arrayExpected);


		}
	}
	
	

		
