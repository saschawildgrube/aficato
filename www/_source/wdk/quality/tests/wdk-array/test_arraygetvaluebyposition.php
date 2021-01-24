<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayGetValueByPosition");
		}
		

		function TestCase_ArrayGetValueByPosition(
			$arrayData,
			$nPosition,
			$vExpectedResult)
		{ 
			$this->Trace("TestCase_GetArrayValueByPosition");
	
			$this->Trace("nPosition = ".$nPosition);

			$vResult = ArrayGetValueByPosition($arrayData,$nPosition);
	
			$this->Trace("Result:");
			if (is_array($vResult))
			{
				$this->Trace($vResult);
			}
			else
			{
				$this->Trace("\"$vResult\"");	
			}
	
			if ($vResult == $vExpectedResult)
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
			
			$this->TestCase_ArrayGetValueByPosition(array(),0,false);
			$this->TestCase_ArrayGetValueByPosition(array(),0,false);
			$this->TestCase_ArrayGetValueByPosition(null,1,false);
			$this->TestCase_ArrayGetValueByPosition(array(),1,false);
			
			$arrayData = array(
				"key1" => "value1",
				"key2" => "value2",
				"key3" => array(
					"key31" => "value31",
					"key32" => "value32",
					"key33" => "value33"
					)
				);
			$this->Trace($arrayData);
						
			$this->TestCase_ArrayGetValueByPosition($arrayData,0,"value1");
			$this->TestCase_ArrayGetValueByPosition($arrayData,1,"value2");			
			$this->TestCase_ArrayGetValueByPosition($arrayData,2,array(
					"key31" => "value31",
					"key32" => "value32",
					"key33" => "value33"
					));
					

			$arrayData = array(
				0 => "value1",
				1 => "value2",
				2 => array(
					"key31" => "value31",
					"key32" => "value32",
					"key33" => "value33"
					)
				);
			$this->Trace($arrayData);
			$this->TestCase_ArrayGetValueByPosition($arrayData,0,"value1");
			$this->TestCase_ArrayGetValueByPosition($arrayData,1,"value2");			
			$this->TestCase_ArrayGetValueByPosition($arrayData,2,array(
					"key31" => "value31",
					"key32" => "value32",
					"key33" => "value33"
					));

			$arrayData = array(
				10 => "value1",
				20 => "value2",
				30 => array(
					"key31" => "value31",
					"key32" => "value32",
					"key33" => "value33"
					)
				);
			$this->Trace($arrayData);
			$this->TestCase_ArrayGetValueByPosition($arrayData,0,"value1");
			$this->TestCase_ArrayGetValueByPosition($arrayData,1,"value2");			
			$this->TestCase_ArrayGetValueByPosition($arrayData,2,array(
					"key31" => "value31",
					"key32" => "value32",
					"key33" => "value33"
					));



		}
		
		
	}
	
	

		
