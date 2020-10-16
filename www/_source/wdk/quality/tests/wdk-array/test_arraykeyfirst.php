<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayKeyFirst");
		}
		  

		function TestCase_ArrayKeyFirst(
			$arrayInput,
			$vKey,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArrayKeyFirst");
	
			$this->Trace("Input:");
			$this->Trace($arrayInput);
			$this->Trace("vKey: ".RenderValue($vKey));

			$this->Trace("ExpectedResult:");
			$this->Trace($arrayExpectedResult);

	
			$arrayResult = ArrayKeyFirst($arrayInput,$vKey);
			
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
				0 => "a",
				1 => "z",
				2 => "x" 
				);
			$this->TestCase_ArrayKeyFirst($arrayInput,3,$arrayExpected);
			$this->TestCase_ArrayKeyFirst($arrayInput,69,$arrayInput);

			$arrayInput = array(
				"alpha" => "z",
				"beta" => "x",
				"gamma" => "a"
				);
			$arrayExpected = array(
				"gamma" => "a",
				"alpha" => "z",
				"beta" => "x"
				);
			$this->TestCase_ArrayKeyFirst($arrayInput,"gamma",$arrayExpected);
			$this->TestCase_ArrayKeyFirst($arrayInput,"bogus",$arrayInput);


		}
	}
	
	

		
