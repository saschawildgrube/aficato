<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Check PHP configuration");
		}
		
		function TestCase_CheckFunction($strFunctionName)
		{
			$this->Trace("\n\nCheck function: $strFunctionName");
			if (function_exists($strFunctionName) == false)
			{
				$this->SetResult(false);
				$this->Trace("TESTCASE FAILED");
				return false;
			}
			
			$this->Trace("TESTCASE PASSED");
			return true;
				
		}

		function TestCase_CheckPhpIniEqual($strIniOption,$vExpected)
		{
			$this->Trace("\n\nCheck: Option $strIniOption should be equal to ".RenderValue($vExpected));
			$vActual = ini_get($strIniOption);
			$this->Trace("Actual: ".RenderValue($vActual));
					
			if ($vExpected != $vActual)
			{
				$this->SetResult(false);
				$this->Trace("TESTCASE FAILED");
				return false;
			}
			
			$this->Trace("TESTCASE PASSED");
			return true;
		}
		
		function TestCase_CheckPhpIniGreaterOrEqual($strIniOption,$vExpected)
		{
			$this->Trace("\n\nCheck: Option $strIniOption should be greater than or equal to ".RenderValue($vExpected));
			$vActual = ini_get($strIniOption);
			$this->Trace("Actual: ".RenderValue($vActual));
					
			if ($vActual < $vExpected)
			{
				$this->SetResult(false);
				$this->Trace("TESTCASE FAILED");
				return false;
			}
			
			$this->Trace("TESTCASE PASSED");
			return true;
		}



		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);
			
			if (CheckEnvironment() == false)
			{
				$this->Trace("CheckEnvironment() returned false");
				$this->SetResult(false);
			}


			$this->TestCase_CheckPhpIniEqual("safe_mode",false);
			$this->TestCase_CheckPhpIniGreaterOrEqual("max_execution_time",120);

			$this->TestCase_CheckFunction("memory_get_usage");
			
		}
		
			
	}
	
	

		
