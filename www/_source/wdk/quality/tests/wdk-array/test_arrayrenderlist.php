<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayRenderList");
		}
		

		function TestCase_ArrayRenderList(
			$arrayInput,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_GetArrayValue");
	
			$this->Trace("Input:");
			$this->Trace($arrayInput);
			$this->Trace("Expected result:");
			$this->Trace($strExpectedResult);
	
			$strResult = ArrayRenderList($arrayInput);
	
			$this->Trace("Result:");
			$this->Trace($strResult);
	
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


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);
			
			
			$this->TestCase_ArrayRenderList("","");
			$this->TestCase_ArrayRenderList(false,"");
			$this->TestCase_ArrayRenderList(array(),"");
			$this->TestCase_ArrayRenderList("blubb","");

			$arrayInput = array(
				"value1");
			$this->TestCase_ArrayRenderList($arrayInput,"\"value1\"");

			$arrayInput = array(
				"value1",
				"");
			$this->TestCase_ArrayRenderList($arrayInput,"\"value1\", \"\"");

			
			$arrayInput = array(
				"value1",
				"value2",
				"value3");
			$this->TestCase_ArrayRenderList($arrayInput,"\"value1\", \"value2\", \"value3\"");

			$arrayInput = array(
				"value1",
				"",
				"value2",
				"value3");
			$this->TestCase_ArrayRenderList($arrayInput,"\"value1\", \"\", \"value2\", \"value3\"");

			$arrayInput = array(
				"value1",
				array(1,2,3),
				"value2",
				"value3");
			$this->TestCase_ArrayRenderList($arrayInput,"\"value1\", array, \"value2\", \"value3\"");




		}
		
		
	}
	
	

		
