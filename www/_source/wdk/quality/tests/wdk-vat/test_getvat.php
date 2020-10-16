<?php
	
	require_once(GetWDKDir()."wdk_vat.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetVAT");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_GetVAT($strCountry,$strArea,$strDate,$strCPC,$vExpectedResult)
		{
			$this->Trace("TestCase_GetVAT");
			$this->Trace("Country: $strCountry");
			$this->Trace("Area:    $strArea");
			$this->Trace("Date:    $strDate");
			$this->Trace("CPC:     $strCPC");
			$this->Trace("Expected Result: \"$vExpectedResult\"");
			$vResult = GetVAT($strCountry,$strArea,$strDate,$strCPC); 
			$this->Trace("GetVAT returns: ".RenderValue($vResult));
			if ($vResult === $vExpectedResult)
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

			$this->TestCase_GetVAT(
				"XXX", // invalid country
				"",
				"",
				"",
				false);

			$this->TestCase_GetVAT(
				"DEU",
				"",
				"2001-13-01", // invalid date
				"",
				false);

			$this->TestCase_GetVAT(
				"DEU",
				"Bogusland",  // area does not exist
				"", 
				"",
				false);

			
			$this->TestCase_GetVAT(
				"TEST",
				"",
				"",
				"",
				0.12);


			$this->TestCase_GetVAT(
				"TEST",
				"TEST",
				"",
				"",
				0.11);

			$this->TestCase_GetVAT(
				"TEST",
				"",
				"1999-12-31",
				"",
				0.1);


			$this->TestCase_GetVAT(
				"TEST",
				"",
				"",
				"1",
				0.13);

			$this->TestCase_GetVAT(
				"TEST",
				"",
				"",
				"1",
				0.13);

			$this->TestCase_GetVAT(
				"TEST",
				"",
				"",
				"11",
				0.13
				);

			$this->TestCase_GetVAT(
				"TEST",
				"",
				"",
				"111",
				0.13);

		
		}
		

	}
	
	

		
