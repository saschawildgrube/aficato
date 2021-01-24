<?php

	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test ArrayMaxStringLength");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_ArrayMaxStringLength(
			$arrayInput,
			$nExpectedValue)
		{ 
			$this->Trace("TestCase_ArrayMaxStringLength");
		
			$nValue = ArrayMaxStringLength($arrayInput);
		
			$this->Trace("Input:");
			$this->Trace(RenderValue($arrayInput));
		
			$this->Trace("ArrayMaxStringLength = ".RenderValue($nValue));
		
			if ($nValue === $nExpectedValue)
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
			
			$arrayInput = array();
			$this->TestCase_ArrayMaxStringLength($arrayInput,false);

			$arrayInput = array("");
			$this->TestCase_ArrayMaxStringLength($arrayInput,0);

			$arrayInput = array(
				"apples",
				"oranges");
			$this->TestCase_ArrayMaxStringLength($arrayInput,7);

			$arrayInput = array(
				"first" => "apples",
				"second" => "oranges");
			$this->TestCase_ArrayMaxStringLength($arrayInput,7);

			$arrayInput = array(
				23,
				123);
			$this->TestCase_ArrayMaxStringLength($arrayInput,3);

			$arrayInput = array(
				23.45);
			$this->TestCase_ArrayMaxStringLength($arrayInput,5);

			$arrayInput = array(
				23.45, 
				1000.01);
			$this->TestCase_ArrayMaxStringLength($arrayInput,7);


			$arrayInput = "";
			$this->TestCase_ArrayMaxStringLength($arrayInput,false);

			$arrayInput = 23.45;
			$this->TestCase_ArrayMaxStringLength($arrayInput,false);
			
			
		}
		
		
	}
	
	

		
