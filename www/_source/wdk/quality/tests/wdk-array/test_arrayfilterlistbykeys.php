<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayFilterListByKeys");
		}
		  

		function TestCase_ArrayFilterListByKeys(
			$arraySource,
			$arrayKeys,
			$bIgnoreEmpty,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArrayFilterListByKeys");
	
			$this->Trace("Source");
			$this->Trace($arraySource);
			$this->Trace("Keys");
			$this->Trace($arrayKeys);
			$this->Trace("IgnoreEmpty");
			$this->Trace(RenderBool($bIgnoreEmpty));
			

			$this->Trace("Expected"); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = ArrayFilterListByKeys($arraySource,$arrayKeys,$bIgnoreEmpty);
			
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


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);
			
			$arraySource = array(
				array(
					"id" 			=> 123,
					"name" 		=> "john doe",
					"address" 	=> "33 backstreet"
					),
				array(
					"id" 			=> 1234,
					"name" 		=> "john smith",
					"address" 	=> "34 backstreet"
					)
					
				);
			$arrayKeys = array("id","name");
			$arrayExpected = array(
				array(
					"id" 			=> 123,
					"name" 		=> "john doe"
					),
				array(
					"id" 			=> 1234,
					"name" 		=> "john smith"
					)
				);
			$this->TestCase_ArrayFilterListByKeys($arraySource,$arrayKeys,false,$arrayExpected);

			$arraySource = array(
				array(
					"id" 			=> 123,
					"name" 		=> "john doe",
					"address" 	=> "33 backstreet"
					)
				);
			$arrayKeys = null;
			$arrayExpected = array();
			$this->TestCase_ArrayFilterListByKeys($arraySource,$arrayKeys,false,$arrayExpected);




		}
	}
	
	

		
