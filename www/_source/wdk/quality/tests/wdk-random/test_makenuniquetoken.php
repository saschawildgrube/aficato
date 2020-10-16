<?php
	
	require_once(GetWDKDir()."wdk_parameter.inc");
	require_once(GetWDKDir()."wdk_random.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("MakeUniqueToken");
			
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);	
			return true;
		} 

		function TestCase_MakeUniqueToken($strHaystack, $arrayTokens)
		{
			$this->Trace("TestCase_MakeUniqueToken");
			$this->Trace("strHaystack = \"$strHaystack\"");
			$this->Trace("arrayTokens:");
			$this->Trace($arrayTokens);
			$strUniqueToken = MakeUniqueToken($strHaystack,$arrayTokens);
			$this->Trace("MakeUniqueToken() returns \"$strUniqueToken\"");
			
			if (StringLength($strUniqueToken) != 10)
			{
				$this->Trace("Unexpected length!");
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
				return;
			}
			if (StringCheckCharset($strUniqueToken,"1234567890") != true)  
			{
				$this->Trace("CheckCharset failed");
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
				return;
			}

			$this->Trace("Testcase PASSED!");			
			$this->Trace("");
		}


		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->TestCase_MakeUniqueToken(
				"ABC 123 ABC",
				array("B","23"));

			$this->TestCase_MakeUniqueToken(
				"",
				array("1","2"));
		}
		
		
	}
	
	

		
