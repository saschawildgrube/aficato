<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArraySort");
		}
		  

		function TestCase_ArraySort(
			$arrayInput,
			$bMaintainKeys,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArraySort");
			$arraySorted = ArraySort($arrayInput,$bMaintainKeys);
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


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);
			
			$arrayInput = array(
				1 => "z",
				2 => "x",
				3 => "a"
				);
			$arrayExpected = array(
				3 => "a",
				2 => "x",
				1 => "z"
				);
			$this->TestCase_ArraySort($arrayInput,true,$arrayExpected);

			$arrayInput = array(
				1 => "z",
				2 => "x",
				3 => "a"
				);
			$arrayExpected = array(
				0 => "a",
				1 => "x",
				2 => "z"
				);
			$this->TestCase_ArraySort($arrayInput,false,$arrayExpected);


			$arrayInput = array(
				"alpha" => "z",
				"beta" => "x",
				"gamma" => "a"
				);
			$arrayExpected = array(
				"gamma" => "a",
				"beta" => "x",
				"alpha" => "z"
				);
			$this->TestCase_ArraySort($arrayInput,true,$arrayExpected);


		}
	}
	
	

		
