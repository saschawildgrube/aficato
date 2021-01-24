<?php

	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test ArrayKeyExists");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_ArrayKeyExists(
			$arrayInput,
			$key,
			$bExpectedValue)
		{ 
			$this->Trace("TestCase_ArrayKeyExists");
		
			$bValue = ArrayKeyExists($arrayInput,$key);
		
			$this->Trace("Input Array:");
			$this->Trace($arrayInput);
		
			$this->Trace("ArrayKeyExists(...,".RenderValue($key).") = ".RenderBool($bValue));
		
			if ($bValue == $bExpectedValue)
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
			$this->TestCase_ArrayKeyExists($arrayInput,"1",false);
			$this->TestCase_ArrayKeyExists($arrayInput,"",false);
			
			$arrayInput = array(
				"apples","oranges");
			$this->TestCase_ArrayKeyExists($arrayInput,0,true);
			$this->TestCase_ArrayKeyExists($arrayInput,1,true);
			$this->TestCase_ArrayKeyExists($arrayInput,2,false);    

			$arrayInput = array(
				"first" => "apples",
				"second" => "oranges");
			$this->TestCase_ArrayKeyExists($arrayInput,"first",true);
			$this->TestCase_ArrayKeyExists($arrayInput,"second",true);
			$this->TestCase_ArrayKeyExists($arrayInput,"third",false);
			
			
		}
		
		
	}
	
	

		
