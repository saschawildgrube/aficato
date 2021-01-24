<?php

	require_once(GetWDKDir()."wdk_passwordpolicy.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("CPasswordPolicy::CheckPassword");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_CheckPassword(
			CPasswordPolicy $passwordpolicy,
			$strPassword,
			$bExpectedResult,
			$arrayExpectedErrors)
		{
			$this->Trace("TestCase_CheckPassword");
			$this->Trace("strPassword = \"$strPassword\"");
			$this->Trace("CPasswordPolicy");
			$this->Trace($passwordpolicy->GetDataArray());
			$this->Trace("Expected Result: ".RenderBool($bExpectedResult)."");
			$this->Trace("Expected Errors:");
			$this->Trace($arrayExpectedErrors);
			
			$arrayErrors = array();
			$bResult = $passwordpolicy->CheckPassword($strPassword,$arrayErrors);
			$this->Trace("CPasswordPolicy::CheckPassword returns: ".RenderBool($bResult)."");
			$this->Trace("Errors:");
			$this->Trace($arrayErrors);
			if ($bResult == $bExpectedResult && $arrayErrors == $arrayExpectedErrors)
			{
				$this->Trace("Testcase PASSED!");	 
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
			}
			$this->Trace("");
			
		}
		
		
		function OnTest()
		{
			parent::OnTest();

			$passwordpolicy = new CPasswordPolicy();
			$this->TestCase_CheckPassword(
				$passwordpolicy,
				"1",
				true,
				array());
			
			
			$passwordpolicy = new CPasswordPolicy();
			$passwordpolicy->SetMinimumLength(3);
			$this->TestCase_CheckPassword(
				$passwordpolicy,
				"1",
				false,
				array(
					"PASSWORDPOLICY_MINIMUM_LENGTH")
				);

			$passwordpolicy = new CPasswordPolicy();
			$passwordpolicy->SetMinimumLength(3);
			$this->TestCase_CheckPassword(
				$passwordpolicy,
				"123",
				true,
				array()
				);
			
			
			$passwordpolicy = new CPasswordPolicy();
			$passwordpolicy->SetMinimumLength(3);
			$passwordpolicy->SetRequiredCharsAlpha(true);
			$passwordpolicy->SetRequiredCharsNumeric(true);
			$this->TestCase_CheckPassword(
				$passwordpolicy,
				"$%",
				false,
				array(
					"PASSWORDPOLICY_MINIMUM_LENGTH",
					"PASSWORDPOLICY_REQUIRED_CHARS_ALPHA",
					"PASSWORDPOLICY_REQUIRED_CHARS_NUMERIC")
				);

			$passwordpolicy = new CPasswordPolicy();
			$passwordpolicy->SetMinimumLength(3);
			$passwordpolicy->SetRequiredCharsAlpha(true);
			$passwordpolicy->SetRequiredCharsNumeric(true);
			$this->TestCase_CheckPassword(
				$passwordpolicy,
				"abc123",
				true,
				array()
				);









		}
		

	}
	
	

		
