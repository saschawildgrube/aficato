<?php
	
	require_once(GetWDKDir()."wdk_mail.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test IsValidEmail");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}
		
		
		function TestCase_IsValidEmail($strEmail,$bExpectedResult)
		{ 
			$this->Trace("TestCase_IsValidEmail");
			$this->Trace("strEmail = \"$strEmail\", bExpectedResult = ".RenderBool($bExpectedResult));
					
			$bResult = IsValidEmail($strEmail);
				
			$this->Trace("Testcase IsValidEmail returns: ".RenderBool($bResult));
			
			if ($bExpectedResult == $bResult)
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
			
			$this->TestCase_IsValidEmail("info@example.com",true);
			$this->TestCase_IsValidEmail("info@example.info",true);
			$this->TestCase_IsValidEmail("",false);
			$this->TestCase_IsValidEmail("@",false);
			$this->TestCase_IsValidEmail("@example.com",false);
			$this->TestCase_IsValidEmail("@example.com",false);
			$this->TestCase_IsValidEmail("info@.com",false);
			$this->TestCase_IsValidEmail("info@example",false);
			$this->TestCase_IsValidEmail("i@ex.us",true);
			$this->TestCase_IsValidEmail("test@example.com,",false);
			$this->TestCase_IsValidEmail(" test@example.com",false);
			$this->TestCase_IsValidEmail("test@@example.com",false);
			$this->TestCase_IsValidEmail("common@2.4.25.js",false);
			$this->TestCase_IsValidEmail("common@2.4.25.css",false);
		}
		
	}
	
	

		
