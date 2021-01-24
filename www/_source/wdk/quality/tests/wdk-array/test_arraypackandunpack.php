<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayPack and ArrayUnpack");
		}
		  

		function TestCase_ArrayPack_ArrayUnpack(
			$arrayInput,
			$strExpectedResult)
		{ 
			$this->Trace("");
			$this->Trace("");
			$this->Trace("TestCase_ArrayPack_ArrayUnpack");
	
			$this->Trace("arrayInput");
			$this->Trace($arrayInput);
			$this->Trace("Expected:"); 
			$this->Trace("\"$strExpectedResult\"");

			$strResult = ArrayPack($arrayInput);

			$this->Trace("Result:");
			$this->Trace("\"$strResult\"");
			$this->Trace("StringLength = ".StringLength($strResult)."");
			
			$this->Trace("");
			
			if ($strExpectedResult != "")  
			{
				if ($strResult != $strExpectedResult)
				{
					$this->Trace("Testcase FAILED!");	
					$this->SetResult(false);
					return;
				}
			}
			
			$arrayResult = ArrayUnpack($strResult);
			if ($arrayResult != $arrayInput)
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
				return;
			}
			

			$this->Trace("Testcase PASSED!");
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
			$this->TestCase_ArrayPack_ArrayUnpack(
				$arrayInput,
				"");
				
			$arrayInput = array(
				array("Keys","Values"),
				array(1,"a"),
				array(2,"b"),
				array(3,"c")
				);
			$this->TestCase_ArrayPack_ArrayUnpack(
				$arrayInput,
				"");


			$arrayInput = array(
				10 => "a",
				20 => "b",
				30 => "c"
				);
			$this->TestCase_ArrayPack_ArrayUnpack(
				$arrayInput,
				"");
				
			$arrayInput = array(
				array("Keys","Values"),
				array(10,"a"),
				array(20,"b"),
				array(30,"c")
				);
			$this->TestCase_ArrayPack_ArrayUnpack(
				$arrayInput,
				"");



		}
	}
	
	

		
