<?php

	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test IsValidDateTime");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			date_default_timezone_set("UTC");
			return true;
		}

		function TestCase_IsValidDateTime(
			$strDateTime,
			$bExpectedValid)
		{ 
			$this->Trace("TestCase_IsValidDateTime");
		
			$this->Trace("strDateTime = \"$strDateTime\"");
		
		
			$bValue = IsValidDateTime($strDateTime);
		
			$this->Trace("IsValidDateTime(\"".$strDateTime."\") = ".(($bValue)?("true"):("false")));
		
			if ($bValue == $bExpectedValid)
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
			$this->TestCase_IsValidDateTime("2008-05-03 23:55:03",true);
			$this->TestCase_IsValidDateTime("2008-03-25 12:34:00",true);
			$this->TestCase_IsValidDateTime("2008-0325 12:34:00",false);
			$this->TestCase_IsValidDateTime("2008-03-2 12:34:00",false);
			$this->TestCase_IsValidDateTime("2008-03-2 12:3:00",false);
			$this->TestCase_IsValidDateTime("2008-13-01 23:55:03",false);
			$this->TestCase_IsValidDateTime("2008-05-03 24:55:03",false);
			$this->TestCase_IsValidDateTime("2008-05-03 00:66:03",false);
			$this->TestCase_IsValidDateTime("2008-05-03 00:01:60",false);
			
			$this->TestCase_IsValidDateTime("1969-12-31 23:59:59",true); // the second before the unix epoch
			$this->TestCase_IsValidDateTime("2038-01-19 03:14:08",true); // the second after 2^16


		}
		
		
	}
	
	

		
