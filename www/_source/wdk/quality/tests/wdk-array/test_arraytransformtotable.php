<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayTransformToTable");
		}
		  

		function TestCase_ArrayTransformToTable(
			$arrayInput,
			$strHeaderKeys,
			$strHeaderValues,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArrayTransformToTable");
	
			$this->Trace("arrayInput");
			$this->Trace($arrayInput);
			$this->Trace("strHeaderKeys: \"".$strHeaderKeys."\"");
			$this->Trace("strHeaderValues: \"".$strHeaderValues."\"");

			$this->Trace("Expected"); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = ArrayTransformToTable($arrayInput,$strHeaderKeys,$strHeaderValues);
			
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
			
			$arrayInput = array(
				1 => "a",
				2 => "b",
				3 => "c"
				);
			$arrayExpected = array(
				array("Keys","Values"),
				array(1,"a"),
				array(2,"b"),
				array(3,"c")
				);
			$this->TestCase_ArrayTransformToTable($arrayInput,"Keys","Values",$arrayExpected);

			$arrayInput = array(
				10 => "a",
				20 => "b",
				30 => "c"
				);
			$arrayExpected = array(
				array("Keys","Values"),
				array(10,"a"),
				array(20,"b"),
				array(30,"c")
				);
			$this->TestCase_ArrayTransformToTable($arrayInput,"Keys","Values",$arrayExpected);


			$arrayInput = array(
				"hello" => "world",
				"tag" => "value",
				"Me" => "You"
				);
			$arrayExpected = array(
				array("Keys","Values"),
				array("hello","world"),
				array("tag","value"),
				array("Me","You")
				);
			$this->TestCase_ArrayTransformToTable($arrayInput,"Keys","Values",$arrayExpected);

			$arrayInput = array();
			$arrayExpected = array(
				array("Keys","Values")
				);
			$this->TestCase_ArrayTransformToTable($arrayInput,"Keys","Values",$arrayExpected);


		}
	}
	
	

		
