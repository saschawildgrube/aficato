<?php
	
	require_once(GetWDKDir()."wdk_localresources.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("IsValidTID");
		}
		

		function TestCase_IsValidTID(
			$strTID,
			$bExpectedResult)
		{ 
			$this->Trace("TestCase_IsValidTID");
	
			$this->Trace("strTID: = \"$strTID\"");
			$this->Trace("bExpectedResult: ".RenderBool($bExpectedResult));
			
			$bResult = IsValidTID($strTID);
			$this->Trace("IsValidTID() returns: ".RenderBool($bResult));
			if ($bResult == $bExpectedResult)
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
			
			$this->Trace(CHARSET_TID);

			$this->TestCase_IsValidTID(
				"TID_EXAMPLE",
				true);
			
			$this->TestCase_IsValidTID(
				"TID_1",
				true);

			$this->TestCase_IsValidTID(
				"TID_THIS_IS_A_TEST-123",
				true);

			$this->TestCase_IsValidTID(
				"",
				false);

				
			$this->TestCase_IsValidTID(
				"TID",
				false);

			$this->TestCase_IsValidTID(
				"TID_",
				false);
								
			$this->TestCase_IsValidTID(
				"TID_EXAMPLE_",
				false);

			$this->TestCase_IsValidTID(
				"TID_EXAMPLE-",
				false);
				
			$this->TestCase_IsValidTID(
				"TID_ExAMPLE",
				false);

			$this->TestCase_IsValidTID(
				"TID_EXAMPLE_%&/_INVALID",
				false);

			$this->TestCase_IsValidTID(
				"TID_EXAMPLE.PDF",
				false);

				
		}
		
		
	}
	
	

		
