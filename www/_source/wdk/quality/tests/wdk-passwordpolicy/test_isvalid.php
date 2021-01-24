<?php

	require_once(GetWDKDir()."wdk_passwordpolicy.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("CPasswordPolicy::IsValid");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_IsValid(
			CPasswordPolicy $passwordpolicy,
			$bExpectedResult)
		{
			$this->Trace("TestCase_IsValid");
			$this->Trace("CPasswordPolicy");
			$this->Trace($passwordpolicy->GetDataArray());
			$this->Trace("Expected Result: ".RenderBool($bExpectedResult)."");
			
			$bResult = $passwordpolicy->IsValid();
			$this->Trace("CPasswordPolicy::IsValid returns: ".RenderBool($bResult)."");
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
			
		}
		
		
		function OnTest()
		{
			parent::OnTest();

			$passwordpolicy = new CPasswordPolicy();
			$this->TestCase_IsValid(
				$passwordpolicy,
				true);
			
			$passwordpolicy = new CPasswordPolicy();
			$passwordpolicy->SetMinimumLength(0);
			$this->TestCase_IsValid(
				$passwordpolicy,
				false);

			$passwordpolicy = new CPasswordPolicy();
			$passwordpolicy->SetMinimumLength(4);
			$passwordpolicy->SetRequiredCharsAlpha(true);
			$passwordpolicy->SetRequiredCharsNumeric(true);
			$this->TestCase_IsValid(
				$passwordpolicy,
				true);
				
				
				
		}

		

	}
	
	

		
