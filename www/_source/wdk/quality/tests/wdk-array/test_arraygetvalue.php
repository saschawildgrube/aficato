<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayGetValue");
		}
		

		function TestCase_ArrayGetValue(
			$arrayData,
			$arrayKeys,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_GetArrayValue");
	
			$this->Trace($arrayKeys);
			$this->Trace("strExpectedResult = \"$strExpectedResult\"");
	
			$strResult = ArrayGetValue($arrayData,$arrayKeys[0],$arrayKeys[1],$arrayKeys[2],$arrayKeys[3]);
	
			$this->Trace("strResult = \"$strResult\"");
	
			if ($strResult == $strExpectedResult)
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
											
			$this->Trace($arrayData);
						
			$this->TestCase_ArrayGetValue($arrayData,array("key1","","",""),"value1");
			$this->TestCase_ArrayGetValue($arrayData,array("key3","key31","",""),"value31");
			$this->TestCase_ArrayGetValue($arrayData,array("key3","key33","key333","key3331"),"value3331");
			$this->TestCase_ArrayGetValue($arrayData,array("bogus","bogus","",""),"");



			$arrayData = array(
				0 => "value1",
				1 => "value2",
				2 => array(
					"key31" => "value31",
					"key32" => "value32",
					"key33" => array(
						0 => "value331",
						1 => "value332",
						2 => array(
							"key3331" => "value3331",
							"key3332" => "value3332"
							)
						)
					)
				);
											
			$this->Trace($arrayData);
						
			$this->TestCase_ArrayGetValue($arrayData,array(0,"","",""),"value1");
			$this->TestCase_ArrayGetValue($arrayData,array(2,"key31","",""),"value31");
			$this->TestCase_ArrayGetValue($arrayData,array(2,"key33",2,"key3331"),"value3331");
			$this->TestCase_ArrayGetValue($arrayData,array("bogus","bogus","",""),"");



			$arrayData = array(
				0 => "value1",
				1 => "value2",
				2 => array(
					"key31" => "value31",
					"key32" => "value32",
					"key33" => array(
						0 => "value331",
						1 => "value332",
						2 => array(
							0 => "value3331",
							1 => "value3332"
							)
						)
					)
				);
											
			$this->Trace($arrayData);
						
			$this->TestCase_ArrayGetValue($arrayData,array(0,"","",""),"value1");
			$this->TestCase_ArrayGetValue($arrayData,array(2,"key31","",""),"value31");
			$this->TestCase_ArrayGetValue($arrayData,array(2,"key33",2,0),"value3331");
			$this->TestCase_ArrayGetValue($arrayData,array(2,"key33",2,1),"value3332");
			$this->TestCase_ArrayGetValue($arrayData,array("bogus","bogus","",""),"");




		}
		
		
	}
	
	

		
