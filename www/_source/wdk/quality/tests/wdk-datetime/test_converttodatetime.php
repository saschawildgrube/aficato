<?php

	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test ConvertToDateTime");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			date_default_timezone_set("UTC");
			return true;
		}

		function TestCase_ConvertToDateTime(
			$strInputValue,
			$strExpectedValue)
		{ 
			$this->Trace("TestCase_ConvertToDateTime");
			$strValue = ConvertToDateTime($strInputValue);
			$this->Trace("ConvertToDateTime(\"".$strInputValue."\") = \"".$strValue."\"");
			if ($strValue == $strExpectedValue)
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
			$this->TestCase_ConvertToDateTime("12.01.2010","2010-01-12 00:00:00");
			$this->TestCase_ConvertToDateTime("01.12.2010","2010-12-01 00:00:00");
			$this->TestCase_ConvertToDateTime("27.7.77","1977-07-27 00:00:00");
			$this->TestCase_ConvertToDateTime("9.11.01","2001-11-09 00:00:00");
			$this->TestCase_ConvertToDateTime("01/08/01","2001-01-08 00:00:00");
			$this->TestCase_ConvertToDateTime("19.1.2038","2038-01-19 00:00:00");

			$this->TestCase_ConvertToDateTime("bogus","");
			$this->TestCase_ConvertToDateTime("","");
			$this->TestCase_ConvertToDateTime("44.1.1985","");
			
			//$this->TestCase_ConvertToDateTime("1.1.1900","1900-01-01 00:00:00");
			//$this->TestCase_ConvertToDateTime("1.1.2039","2039-01-01 00:00:00");

			
//			2038-01-19 03:14:07
		}
		
		
	}
	
	

		
