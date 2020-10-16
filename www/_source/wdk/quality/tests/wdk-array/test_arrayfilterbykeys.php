<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayFilterByKeys");
		}
		  

		function TestCase_ArrayFilterByKeys(
			$arraySource,
			$arrayKeys,
			$bIgnoreEmpty,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArrayFilterByKeys");
	
			$this->Trace("Source");
			$this->Trace($arraySource);
			$this->Trace("Keys");
			$this->Trace($arrayKeys);
			$this->Trace("IgnoreEmpty");
			$this->Trace(RenderBool($bIgnoreEmpty));
			

			$this->Trace("Expected"); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = ArrayFilterByKeys($arraySource,$arrayKeys,$bIgnoreEmpty);
			
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
			$arrayKeys = array("id","name");
			$arrayExpected = array(
				"id" 			=> 123,
				"name" 		=> "john doe"
				);
			$this->TestCase_ArrayFilterByKeys($arraySource,$arrayKeys,false,$arrayExpected);

			$arraySource = array(
				"id" 			=> 123,
				"name" 		=> "john doe",
				"address" 	=> "33 backstreet"
				);
			$arrayKeys = null;
			$arrayExpected = array();
			$this->TestCase_ArrayFilterByKeys($arraySource,$arrayKeys,false,$arrayExpected);


			$arraySource = array(
				"id" 			=> 123,
				"name" 		=> "",
				"address" 	=> "33 backstreet"
				);
			$arrayKeys = array("id","name");
			$arrayExpected = array(
				"id" 			=> 123,
				"name" 		=> ""
				);
			$this->TestCase_ArrayFilterByKeys($arraySource,$arrayKeys,false,$arrayExpected);


			$arraySource = array(
				"id" 			=> 123,
				"name" 		=> "",
				"address" 	=> "33 backstreet"
				);
			$arrayKeys = array("id","name");
			$arrayExpected = array(
				"id" 			=> 123,
				);
			$this->TestCase_ArrayFilterByKeys($arraySource,$arrayKeys,true,$arrayExpected);


		}
	}
	
	

		
