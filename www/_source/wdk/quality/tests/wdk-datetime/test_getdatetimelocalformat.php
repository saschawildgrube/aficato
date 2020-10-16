<?php
	
	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test GetDateTimeLocalFormat");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			date_default_timezone_set("UTC");
			return true;
		}

		function TestCase_GetDateTimeLocalFormat(
			$strCountryID,
			$strExpectedValid)
		{ 
			$this->Trace("TestCase_GetDateTimeLocalFormat");
		
			$this->Trace("strCountryID = \"$strCountryID\"");
		
		
			$strValue = GetDateTimeLocalFormat($strCountryID);
		
			$this->Trace("GetDateTimeLocalFormat(\"".$strCountryID."\") = \"".$strValue."\"");
		
			if ($strValue == $strExpectedValid)
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

		
		function CallbackTest()
		{
			parent::CallbackTest();
			$this->TestCase_GetDateTimeLocalFormat("","%Y-%m-%d %H:%M:%S");
			$this->TestCase_GetDateTimeLocalFormat("USA","%m/%d/%Y %I:%M:%S%P");
		}
		
		
	}
	
	

		
