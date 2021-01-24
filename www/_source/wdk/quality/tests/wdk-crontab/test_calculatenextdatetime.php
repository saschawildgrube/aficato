<?php

	require_once(GetWDKDir()."wdk_crontab.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test CrontabCalculateNextDateTime");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			date_default_timezone_set("UTC");
			return true;
		}

		function TestCase_CrontabCalculateNextDateTime(
			$strPivotDatetime,
			$strMinute,
			$strHour,
			$strDayOfMonth,
			$strMonth,
			$strDayOfWeek,
			$strExpectedResult,
			$strExpectedError)
		{ 
			$this->Trace("TestCase_CrontabCalculateNextDateTime");
	
			$this->Trace("strPivotDatetime  = \"$strPivotDatetime\"");
			$this->Trace("strMinute         = \"$strMinute\"");
			$this->Trace("strHour           = \"$strHour\"");
			$this->Trace("strDayOfMonth     = \"$strDayOfMonth\"");
			$this->Trace("strMonth          = \"$strMonth\"");
			$this->Trace("strDayOfWeek      = \"$strDayOfWeek\"");
			$this->Trace("strExpectedResult = \"$strExpectedResult\"");
			$this->Trace("strExpectedError  = \"$strExpectedError\"");
			
			$strError = "";
					
			$strResult = CrontabCalculateNextDateTime(
				$strPivotDatetime,
				$strMinute,
				$strHour,
				$strDayOfMonth,
				$strMonth,
				$strDayOfWeek,
				$strError);
				
			$this->Trace("Testcase CrontabCalculateNextDateTime returns:");
			$this->Trace("strResult = \"$strResult\"");
			$this->Trace("strError  = \"$strError\"");
			
			if (($strResult == $strExpectedResult) && ($strError == $strExpectedError))
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
	
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-05-03 23:55:03",
				"*/5", // strMinute,
				"*", // strHour,
				"*", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-05-04 00:00:00",
				"");
		
		
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-05-02 22:27:32",
				"0-10,20-40/5,50-59/2", // strMinute,
				"*", // strHour,
				"*", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-05-02 22:30:00",
				"");
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-05-02 22:27:32",
				"0", // strMinute,
				"*/4", // strHour,
				"*/2", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-05-03 00:00:00",
				"");
		
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-05-02 22:34:50",
				"0", // strMinute,
				"*/4", // strHour,
				"*/2", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-05-03 00:00:00",
				"");	
		
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-05-03 05:34:50",
				"0", // strMinute,
				"*/4", // strHour,
				"*/2", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-05-03 08:00:00",
				"");	
		
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-05-02 05:34:50",
				"0", // strMinute,
				"*/4", // strHour,
				"*/2", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-05-03 00:00:00",
				"");	
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-05-02 22:00:10",
				"*/15", // strMinute,
				"*", // strHour,
				"*", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-05-02 22:15:00",
				"");		
		
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-04-28 22:00:00",
				"30", // strMinute,
				"1-23/4", // strHour,
				"*", // strDayOfMonth,
				"*", // strMonth,
				"3", // strDayOfWeek,
				"2008-04-30 01:30:00",
				"");		
			
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-04-28 22:00:00",
				"0", // strMinute,
				"*", // strHour,
				"*", // strDayOfMonth,
				"*", // strMonth,
				"3", // strDayOfWeek,
				"2008-04-30 00:00:00",
				"");
			
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-01-01 00:00:00",
				"0", // strMinute,
				"*", // strHour,
				"*", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-01-01 01:00:00",
				"");
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-01-01 23:59:59",
				"0", // strMinute,
				"*", // strHour,
				"*", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-01-02 00:00:00",
				"");
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-01-01 00:00:00",
				"*", // strMinute,
				"abc", // strHour,
				"*", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				false,
				"CRONTAB_DEFINITION_HOUR");
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-01-01 00:00:00",
				"*", // strMinute,
				"*", // strHour,
				"*", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-01-01 00:01:00",
				"");
		
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-01-01 00:05:00",
				"*/5", // strMinute,
				"*", // strHour,
				"*", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-01-01 00:10:00",
				"");
		
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-01-01 23:57:10",
				"*/5", // strMinute,
				"*", // strHour,
				"*", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-01-02 00:00:00",
				"");
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-01-01 23:29:59",
				"30", // strMinute,
				"*", // strHour,
				"*", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-01-01 23:30:00",
				"");
		
			$this->TestCase_CrontabCalculateNextDateTime(
				"2008-01-01 23:30:59",
				"0-10,30-59/2", // strMinute,
				"*", // strHour,
				"*", // strDayOfMonth,
				"*", // strMonth,
				"*", // strDayOfWeek,
				"2008-01-01 23:32:00",
				"");

		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			return true;
		}
		
		
	}
	
	

		
