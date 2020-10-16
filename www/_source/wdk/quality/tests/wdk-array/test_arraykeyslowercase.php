<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayKeysLowerCase");
		}
		  

		function TestCase_ArrayKeysLowerCase(
			$arrayInput,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArrayKeysLowerCase");
	
			$arrayResult = ArrayKeysLowerCase($arrayInput);
			
			$this->Trace("Input:");
			$this->Trace(RenderValue($arrayInput));
			$this->Trace("Expected:");
			$this->Trace(RenderValue($arrayExpectedResult));   
			$this->Trace("Result:");
			$this->Trace(RenderValue($arrayResult));

			if ($arrayResult === $arrayExpectedResult)
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
				"KEY1" => "value1",
				"KEY2" => "value2",
				"KEY3" => array(
					"key31" => "value31",
					"key32" => "value32",
					"key33" => array(
						"key331" => "value331",
						"key332" => "value332",
						"key333" => array(
							"key3331" => "value3331",
							"key3332" => "value3332"
							)
						)
					)
				);
			$arrayExpected = array(
				"key1" => "value1",
				"key2" => "value2",
				"key3" => array(
					"key31" => "value31",
					"key32" => "value32",
					"key33" => array(
						"key331" => "value331",
						"key332" => "value332",
						"key333" => array(
							"key3331" => "value3331",
							"key3332" => "value3332"
							)
						)
					)
				);
			$this->TestCase_ArrayKeysLowerCase($arrayInput,$arrayExpected);


			$arrayInput = array(
				"Apples" => 3,
				"ORANGES" => 4,
				34 => "other fruits"
				);
			$arrayExpected = array(
				"apples" => 3,
				"oranges" => 4,
				34 => "other fruits"
				);
			$this->TestCase_ArrayKeysLowerCase($arrayInput,$arrayExpected);

			$arrayInput = array(
				"A" => false
				);
			$arrayExpected = array(
				"a" => false
				);
			$this->TestCase_ArrayKeysLowerCase($arrayInput,$arrayExpected);

			$arrayInput = array();
			$arrayExpected = array();
			$this->TestCase_ArrayKeysLowerCase($arrayInput,$arrayExpected);

			$arrayInput = "Bogus";
			$arrayExpected = "Bogus";
			$this->TestCase_ArrayKeysLowerCase($arrayInput,$arrayExpected);

			$arrayInput = false;
			$arrayExpected = false;
			$this->TestCase_ArrayKeysLowerCase($arrayInput,$arrayExpected);

			$arrayInput = 1.5;
			$arrayExpected = 1.5;
			$this->TestCase_ArrayKeysLowerCase($arrayInput,$arrayExpected);
	

		}
	}
	
	

		
