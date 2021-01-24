<?php
	
	require_once(GetWDKDir()."wdk_parameter.inc");
	require_once(GetWDKDir()."wdk_random.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("MakeNumericToken");
			
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		} 

		function TestCase_MakeNumericToken($nLength)
		{
			$this->Trace("TestCase_MakeNumericToken");
			$this->Trace("nLength = $nLength");
			$strToken = MakeNumericToken($nLength);
			$this->Trace("MakeNumericToken($nLength) returns \"$strToken\"");
			if (StringLength($strToken) != min(max(0,$nLength),40))
			{
				$this->Trace("Unexpected length!");
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
				return;
			}  
			if (StringCheckCharSet($strToken,"1234567890") != true)  
			{
				$this->Trace("StringCheckCharSet failed");
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
				return;
			}
			$this->Trace("Testcase PASSED!");			
			$this->Trace("");
		}


		function OnTest()
		{
			parent::OnTest();
			
			for ($nIndex = -2; $nIndex <= 45; $nIndex++)
			{
				$this->TestCase_MakeNumericToken($nIndex);
			}
			
		}
		
		
	}
	
	

		
