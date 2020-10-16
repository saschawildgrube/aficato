<?php

	require_once(GetWDKDir()."wdk_passwordpolicy.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("CPasswordPolicy::GeneratePassword");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			
			$this->Trace("CPasswordPolicy::GeneratePassword() is not yet implemented");
			$this->SetActive(false);			
			
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_GeneratePassword(
			CPasswordPolicy $passwordpolicy)
		{
			$this->Trace("TestCase_GeneratePassword");
			$this->Trace("CPasswordPolicy");
			$this->Trace($passwordpolicy->GetDataArray());
			
			$strPassword = $passwordpolicy->GeneratePassword();
			$this->Trace("CPasswordPolicy::GeneratePassword returns: \"$strPassword\"");
			
			$bResult = $passwordpolicy->CheckPassword($strPassword);
			if ($bResult == true)
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
		
		
		function CallbackTest()
		{
			parent::CallbackTest();

			$passwordpolicy = new CPasswordPolicy();
			$this->TestCase_GeneratePassword($passwordpolicy);	
			
			$passwordpolicy = new CPasswordPolicy();
			$passwordpolicy->SetMinimumLength(2);
			$this->TestCase_GeneratePassword($passwordpolicy);

			$passwordpolicy = new CPasswordPolicy();
			$passwordpolicy->SetMinimumLength(4);
			$passwordpolicy->SetRequiredCharsAlpha(true);
			$passwordpolicy->SetRequiredCharsNumeric(true);
			$this->TestCase_GeneratePassword($passwordpolicy);	
				
		}

		

	}
	
	

		
