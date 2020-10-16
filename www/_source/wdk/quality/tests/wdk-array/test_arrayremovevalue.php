<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayRemoveValue");
		}
		  

		function TestCase_ArrayRemoveValue(
			$arraySource,
			$value,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArrayRemoveValue");
	
			$this->Trace("Source");
			$this->Trace($arraySource);
			$this->Trace("Value");
			$this->Trace($value);

			$this->Trace("Expected"); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = ArrayRemoveValue($arraySource,$value);
			
			$this->Trace("Result");
			$this->Trace($arrayResult);

			if ($arrayResult == $arrayExpectedResult)
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
			
			$arraySource = array(
				"id" 			=> 123,
				"name" 		=> "john doe",
				"address" 	=> "33 backstreet"
				);
			$value = "33 backstreet";
			$arrayExpected = array(
				"id" 			=> 123,
				"name" 		=> "john doe"
				);
			$this->TestCase_ArrayRemoveValue($arraySource,$value,$arrayExpected);

		}
	}
	
	

		
