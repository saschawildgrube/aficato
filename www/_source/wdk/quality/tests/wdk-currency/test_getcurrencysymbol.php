<?php
	
	require_once(GetWDKDir()."wdk_currency.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetCurrencySymbol");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_GetCurrencySymbol($strCurrencyID,$strExpectedResult)
		{
			$this->Trace("TestCase_GetCurrencySymbol");
			$this->Trace("Currency ID    : \"$strCurrencyID\"");
			$this->Trace("Expected Result: \"$strExpectedResult\"");
			$strResult = GetCurrencySymbol($strCurrencyID); 
			$this->Trace("GetCurrencySymbol returns: \"$strResult\"");
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
			
		}


		function OnTest()
		{
			parent::OnTest(); 
			
		
			$this->TestCase_GetCurrencySymbol(
				'USD',
				'$');

			$this->TestCase_GetCurrencySymbol(
				'EUR',
				'€');

			$this->TestCase_GetCurrencySymbol(
				'',
				'¤');


		}
		

	}
	
	

		
