<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK Quality: ParseTestPath");
		}
		  

		function TestCase_ParseTestPath(
			$strInput,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ParseTestPath");
	
			$this->Trace("Expected:");
			$this->Trace($arrayExpectedResult);
	
			$arrayResult = ParseTestPath($strInput);
			
			$this->Trace("Result:");
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

			$strInput = "local:assembly/group/test"; 			
			$arrayExpected = array(
				"site_path" => "local",
				"assembly_id" => "assembly",
				"group_id" => "group",
				"test_id" => "test");
			$this->TestCase_ParseTestPath($strInput,$arrayExpected);



			$strInput = "site/local:assembly/group/test"; 			
			$arrayExpected = array(
				"site_path" => "site/local",
				"assembly_id" => "assembly",
				"group_id" => "group",
				"test_id" => "test");
			$this->TestCase_ParseTestPath($strInput,$arrayExpected);

			$strInput = "bogus"; 			
			$arrayExpected = false;
			$this->TestCase_ParseTestPath($strInput,$arrayExpected);




		}
	}
	
	

		
