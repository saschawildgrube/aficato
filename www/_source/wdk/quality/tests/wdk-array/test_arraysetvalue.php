<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArraySetValue");
		}
		  

		function TestCase_ArraySetValue(
			$arrayData,
			$value,
			$arrayKeys,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArraySetValue");
	
			ArraySetValue($arrayData,$value,$arrayKeys[0],$arrayKeys[1],$arrayKeys[2],$arrayKeys[3]);
			
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


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);
			
			$arrayData = array(
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
				
				

			$arrayExpected = array(
				"key1" => "test1",
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
			$this->TestCase_ArraySetValue($arrayData,"test1",array("key1","","",""),$arrayExpected);

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
							"key3332" => "value3332",
							"key3333" => "value3333"
							)
						)
					)
				);
			$this->TestCase_ArraySetValue($arrayData,"value3333",array("key3","key33","key333","key3333"),$arrayExpected);


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
					),
				0 => "test0"
				);
			$this->TestCase_ArraySetValue($arrayData,"test0",array(0,null,null,null),$arrayExpected);

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
							"key3332" => "value3332",
							1 => "value3331"
							)
						)
					) 
				);
			$this->TestCase_ArraySetValue($arrayData,"value3331",array("key3","key33","key333",1),$arrayExpected);


		}
	}
	
	

		
