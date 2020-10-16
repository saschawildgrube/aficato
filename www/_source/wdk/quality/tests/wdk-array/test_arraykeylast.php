<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayKeyLast");
		}
		  

		function TestCase_ArrayKeyLast(
			$arrayInput,
			$vKey,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArrayKeyLast");
	
			$this->Trace("Input:");
			$this->Trace($arrayInput);
			$this->Trace("vKey: ".RenderValue($vKey));

			$this->Trace("ExpectedResult:");
			$this->Trace($arrayExpectedResult);

	
			$arrayResult = ArrayKeyLast($arrayInput,$vKey);
			
			$this->Trace("Result:");
			$this->Trace($arrayResult);

			if (ArrayStrictCompare($arrayResult,$arrayExpectedResult) == true)
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
				0 => "z",
				1 => "a",
				2 => "x"
				);
			$this->TestCase_ArrayKeyLast($arrayInput,2,$arrayExpected);
			$this->TestCase_ArrayKeyLast($arrayInput,69,$arrayInput);

			$arrayInput = array(
				"alpha" => "z",
				"beta" => "x",
				"gamma" => "a"
				);
			$arrayExpected = array(
				"alpha" => "z",
				"gamma" => "a",
				"beta" => "x"
				);
			$this->TestCase_ArrayKeyLast($arrayInput,"beta",$arrayExpected);
			$this->TestCase_ArrayKeyLast($arrayInput,"bogus",$arrayInput);


		}
	}
	
	

		
