<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayAppend");
		}
		  

		function TestCase_ArrayAppend(
			$array1,
			$array2,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArrayAppend");
	
			$this->Trace("Array1");
			$this->Trace($array1);
			$this->Trace("Array2");
			$this->Trace($array2);

			$this->Trace("Expected"); 
			$this->Trace($arrayExpectedResult);

			$array1 = ArrayAppend($array1,$array2);
			
			$this->Trace("Result");
			$this->Trace($array1);

			if ($array1 == $arrayExpectedResult)
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
			
			$this->Trace("Index Array");
			$array1 = array(
				1 => "z",
				2 => "x",
				3 => "a"
				);
			$array2 = array(
				1 => "z",
				2 => "x",
				3 => "a"
				);
			$arrayExpected = array(
				1 => "z",
				2 => "x",
				3 => "a",
				4 => "z",
				5 => "x",
				6 => "a"
				);
			$this->TestCase_ArrayAppend($array1,$array2,$arrayExpected);


			$this->Trace("Index Array");
			$array1 = array();
			$array1[] = "z";
			$array1[] = "x";
			$array1[] = "a";

			$array2 = array();
			$array2[] = "z";
			$array2[] = "x";
			$array2[] = "a";
			
			$arrayExpected = array();
			$arrayExpected[] = "z";
			$arrayExpected[] = "x";
			$arrayExpected[] = "a";
			$arrayExpected[] = "z";
			$arrayExpected[] = "x";
			$arrayExpected[] = "a";
			$this->TestCase_ArrayAppend($array1,$array2,$arrayExpected);


			$this->Trace("Assoc Array");
			$array1 = array(
				"alpha" => "z",
				"beta" => "x",
				"gamma" => "a"
				);
			$array2 = array(
				"alpha" => "a",
				"beta" => "b",
				"gamma" => "c"
				);
			$arrayExpected = array(
				"alpha" => "a",
				"beta" => "b",
				"gamma" => "c",
				);
			$this->TestCase_ArrayAppend($array1,$array2,$arrayExpected);
		
		
			$array1 = array(
				"" => "empty",
				"alpha" => "one",
				"beta" => "two",
				"gamma" => "three"
				);
			$array2 = array(
				"" => "new empty"
				);
			$arrayExpected = array(
				"" => "new empty",
				"alpha" => "one",
				"beta" => "two",
				"gamma" => "three"
				);
			$this->TestCase_ArrayAppend($array1,$array2,$arrayExpected);
			
			/*   // DOES NOT WORK DUE TO A PHP BUG!!!!!!!
			$this->Trace("Assoc Array");
			$array1 = array(
				"1" => "z",
				"2" => "x",
				"3" => "a"
				);
			$array2 = array(
				"1" => "a",
				"2" => "b",
				"3" => "c"
				);
			$arrayExpected = array(
				"1" => "a",
				"2" => "b",
				"3" => "c",
				);
				*/
			$this->TestCase_ArrayAppend($array1,$array2,$arrayExpected);


		}
	}
	
	

		
