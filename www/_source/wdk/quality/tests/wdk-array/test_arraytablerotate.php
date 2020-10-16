<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayTableRotate");
		}
		  

		function TestCase_ArrayTableRotate(
			$arrayInput,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArrayTableRotate");
	
			$this->Trace("arrayInput");
			$this->Trace($arrayInput);
			$this->Trace("Expected"); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = ArrayTableRotate($arrayInput);
			
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
			
			$arrayInput = array(
				array("column1","column2","column3"),
				array("value11","value12","value13"),
				array("value21","value22","value23"),
				array("value31","value32","value33")
				);
			$arrayExpected = array(
				array("column1","value11","value21","value31"),
				array("column2","value12","value22","value32"),
				array("column3","value13","value23","value33")
				);
			$this->TestCase_ArrayTableRotate($arrayInput,$arrayExpected);

			$arrayInput = array();
			$arrayExpected = array();
			$this->TestCase_ArrayTableRotate($arrayInput,$arrayExpected);


			$arrayInput = array(
				"hello" => "world",
				"tag" => "value",
				"Me" => "You"
				);
			$arrayExpected = array();
			$this->TestCase_ArrayTableRotate($arrayInput,$arrayExpected);

		}
	}
	
	

		
