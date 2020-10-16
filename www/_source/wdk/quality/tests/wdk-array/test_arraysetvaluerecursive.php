<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArraySetValue");
		}
		  

		function TestCase_ArraySetValueRecursive(
			$arrayData,
			$value,
			$key,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArraySetValueRecursive");
	
			ArraySetValueRecursive($arrayData,$value,$key);
			
			$this->Trace($arrayData);

			if ($arrayData == $arrayExpectedResult)
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
			
			$arrayData = array(
				"key1" => "value1",
				"key2" => "value2",
				"key3" => array(
					"key1" => "value31",
					"key2" => "value32",
					"key3" => array(
						"key1" => "value331",
						"key2" => "value332",
						"key3" => array(
							"key1" => "value3331",
							"key2" => "value3332"
							)
						)
					)
				);

			$arrayExpected = array(
				"key1" => "test1",
				"key2" => "value2",
				"key3" => array(
					"key1" => "test1",
					"key2" => "value32",
					"key3" => array(
						"key1" => "test1",
						"key2" => "value332",
						"key3" => array(
							"key1" => "test1",
							"key2" => "value3332"
							)
						)
					)
				);
			$this->TestCase_ArraySetValueRecursive($arrayData,"test1","key1",$arrayExpected);


			$arrayExpected = array(
				"key1" => "value1",
				"key2" => "value2",
				"key3" => ""
				);
			$this->TestCase_ArraySetValueRecursive($arrayData,"","key3",$arrayExpected);


		}
	}
	
	

		
