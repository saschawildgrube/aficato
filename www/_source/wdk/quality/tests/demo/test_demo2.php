<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("DEMO 2");
		}
		

		function TestCase_Demo(
			$strParam,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_Demo");
	
			$this->Trace("strParam          = \"$strParam\"");
			$this->Trace("strExpectedResult = \"$strExpectedResult\"");
	
	
			// Test some function here
			$strResult = StringUpperCase($strParam);
	
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
			
			$this->TestCase_Demo("abc","ABC");
			$this->TestCase_Demo("ABC","ABC");
		}
		
		
	}
	
	

		
