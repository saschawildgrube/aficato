<?php
	
	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Time calculation functions");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			//
			//$this->SetActive(false);
			//
			$this->SetResult(true);
			date_default_timezone_set("UTC");
			return true;
		}

		function TestCase_TimeCalc(
			$strFunction,
			$param1,
			$param2,
			$timeExpected)
		{ 
			$this->Trace("TestCase_TimeCalc");
			
			$strEval = '$timeResult = $strFunction($param1';
			if ($param2 !== null)
			{
				$strEval .= ',$param2';	
			}
			$strEval .= ');';
			
			$this->Trace("$strFunction()");
			$this->Trace("param1 =          \"".RenderDateTime($param1)."\"");
			if ($param2 != null)
			{
				$this->Trace("param2 =          \"$param2\"");
			}
			$this->Trace("GetTimeExpected = \"".RenderDateTime($timeExpected)."\"");
			
			eval($strEval);
			
			$this->Trace("GetTimeResult =   \"".RenderDateTime($timeResult)."\"");



			if ($timeResult == $timeExpected)
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
	
			$this->TestCase_TimeCalc(
				"GetTimeAddSeconds",
				GetTime("1970-01-01 00:00:01"),
				0,
				GetTime("1970-01-01 00:00:01"));
			
			$this->TestCase_TimeCalc(
				"GetTimeAddSeconds",
				GetTime("1970-01-01 00:00:01"),
				1,
				GetTime("1970-01-01 00:00:02"));

			$this->TestCase_TimeCalc(
				"GetTimeAddSeconds",
				GetTime("2013-12-31 23:59:59"),
				1,
				GetTime("2014-01-01 00:00:00"));

			$this->TestCase_TimeCalc(
				"GetTimeAddSeconds",
				GetTime("2014-01-01 00:00:00"),
				-1,
				GetTime("2013-12-31 23:59:59"));





			$this->TestCase_TimeCalc(
				"GetTimeAddMinutes",
				GetTime("2013-12-31 23:59:59"),
				0,
				GetTime("2013-12-31 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeAddMinutes",
				GetTime("2013-12-31 23:59:59"),
				1,
				GetTime("2014-01-01 00:00:59"));

			$this->TestCase_TimeCalc(
				"GetTimeAddMinutes",
				GetTime("2014-01-01 00:00:59"),
				-1,
				GetTime("2013-12-31 23:59:59"));





			$this->TestCase_TimeCalc(
				"GetTimeAddDays",
				GetTime("2013-12-31 23:59:59"),
				0,
				GetTime("2013-12-31 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeAddDays",
				GetTime("2013-12-31 23:59:59"),
				1,
				GetTime("2014-01-01 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeAddDays",
				GetTime("2013-12-31 23:59:59"),
				-1,
				GetTime("2013-12-30 23:59:59"));





			$this->TestCase_TimeCalc(
				"GetTimeAddMonths",
				GetTime("2013-12-31 23:59:59"),
				0,
				GetTime("2013-12-31 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeAddMonths",
				GetTime("2013-12-31 23:59:59"),
				1,
				GetTime("2014-01-31 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeAddMonths",
				GetTime("2013-12-31 23:59:59"),
				-1,
				GetTime("2013-11-30 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeAddMonths",
				GetTime("2013-12-31 23:59:59"),
				-2,
				GetTime("2013-10-31 23:59:59"));


			$this->TestCase_TimeCalc(
				"GetTimeAddMonths",
				GetTime("2013-12-31 23:59:59"),
				-12,
				GetTime("2012-12-31 23:59:59"));


			$this->TestCase_TimeCalc(
				"GetTimeAddMonths",
				GetTime("2013-01-31 23:59:59"),
				13,
				GetTime("2014-02-28 23:59:59"));



			$this->TestCase_TimeCalc(
				"GetTimeAddYears",
				GetTime("2013-12-31 23:59:59"),
				0,
				GetTime("2013-12-31 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeAddYears",
				GetTime("2013-12-31 23:59:59"),
				1,
				GetTime("2014-12-31 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeAddYears",
				GetTime("2012-02-29 23:59:59"),
				1,
				GetTime("2013-02-28 23:59:59"));







			$this->TestCase_TimeCalc(
				"GetTimeWeekBegin",
				GetTime("2018-12-30 11:11:00"),
				null,
				GetTime("2018-12-24 00:00:00"));

			$this->TestCase_TimeCalc(
				"GetTimeWeekBegin",
				GetTime("2018-12-31 11:11:00"),
				null,
				GetTime("2018-12-31 00:00:00")); 

			$this->TestCase_TimeCalc(
				"GetTimeWeekBegin",
				GetTime("2019-01-01 11:11:00"),
				null,
				GetTime("2018-12-31 00:00:00"));

			$this->TestCase_TimeCalc(
				"GetTimeWeekBegin",
				GetTime("2019-01-10 11:11:00"),
				null,
				GetTime("2019-01-07 00:00:00"));

			$this->TestCase_TimeCalc(
				"GetTimeWeekBegin",
				GetTime("2016-03-01 11:11:00"),
				null,
				GetTime("2016-02-29 00:00:00")); // Leap year case







			$this->TestCase_TimeCalc(
				"GetTimeWeekEnd",
				GetTime("2018-12-31 11:11:00"),
				null,
				GetTime("2019-01-06 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeWeekEnd",
				GetTime("2018-12-24 11:11:00"),
				null,
				GetTime("2018-12-30 23:59:59"));  

			$this->TestCase_TimeCalc(
				"GetTimeWeekEnd",
				GetTime("2019-01-01 11:11:00"),
				null,
				GetTime("2019-01-06 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeWeekEnd",
				GetTime("2019-01-06 11:11:00"),
				null,
				GetTime("2019-01-06 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeWeekEnd",
				GetTime("2016-02-29 11:11:00"),
				null,
				GetTime("2016-03-06 23:59:59"));  // Leap year case







			$this->TestCase_TimeCalc(
				"GetTimeMonthBegin",
				GetTime("2013-12-01 11:11:00"),
				null,
				GetTime("2013-12-01 00:00:00"));

			$this->TestCase_TimeCalc(
				"GetTimeMonthBegin",
				GetTime("2013-09-01 11:11:00"),
				null,
				GetTime("2013-09-01 00:00:00"));  

			$this->TestCase_TimeCalc(
				"GetTimeMonthBegin",
				GetTime("2013-01-16 11:11:00"),
				null,
				GetTime("2013-01-01 00:00:00"));

			$this->TestCase_TimeCalc(
				"GetTimeMonthBegin",
				GetTime("2013-12-01 11:11:00"),
				null,
				GetTime("2013-12-01 00:00:00"));

			$this->TestCase_TimeCalc(
				"GetTimeMonthBegin",
				GetTime("2013-01-02 11:11:00"),
				null,
				GetTime("2013-01-01 00:00:00"));







			$this->TestCase_TimeCalc(
				"GetTimeMonthEnd",
				GetTime("2013-12-01 11:11:00"),
				null,
				GetTime("2013-12-31 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeMonthEnd",
				GetTime("2013-09-01 11:11:00"),
				null,
				GetTime("2013-09-30 23:59:59"));  

			$this->TestCase_TimeCalc(
				"GetTimeMonthEnd",
				GetTime("2013-01-16 11:11:00"),
				null,
				GetTime("2013-01-31 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeMonthEnd",
				GetTime("2013-12-01 11:11:00"),
				null,
				GetTime("2013-12-31 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeMonthEnd",
				GetTime("2013-01-02 11:11:00"),
				null,
				GetTime("2013-01-31 23:59:59"));





			$this->TestCase_TimeCalc(
				"GetTimeQuarterBegin",
				GetTime("2013-12-01 11:11:00"),
				null,
				GetTime("2013-10-01 00:00:00"));

			$this->TestCase_TimeCalc(
				"GetTimeQuarterBegin",
				GetTime("2013-09-01 11:11:00"),
				null,
				GetTime("2013-07-01 00:00:00"));

			$this->TestCase_TimeCalc(
				"GetTimeQuarterBegin",
				GetTime("2013-10-31 11:11:00"),
				null,
				GetTime("2013-10-01 00:00:00"));

			$this->TestCase_TimeCalc(
				"GetTimeQuarterBegin",
				GetTime("2013-01-16 11:11:00"),
				null,
				GetTime("2013-01-01 00:00:00"));

			$this->TestCase_TimeCalc(
				"GetTimeQuarterBegin",
				GetTime("2013-12-01 11:11:00"),
				null,
				GetTime("2013-10-01 00:00:00"));

			$this->TestCase_TimeCalc(
				"GetTimeQuarterBegin",
				GetTime("2013-01-02 11:11:00"),
				null,
				GetTime("2013-01-01 00:00:00"));







			$this->TestCase_TimeCalc(
				"GetTimeQuarterEnd",
				GetTime("2013-12-01 11:11:00"),
				null,
				GetTime("2013-12-31 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeQuarterEnd",
				GetTime("2013-09-01 11:11:00"),
				null,
				GetTime("2013-09-30 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeQuarterEnd",
				GetTime("2013-10-31 11:11:00"),
				null,
				GetTime("2013-12-31 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeQuarterEnd",
				GetTime("2013-01-16 11:11:00"),
				null,
				GetTime("2013-03-31 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeQuarterEnd",
				GetTime("2013-12-01 11:11:00"),
				null,
				GetTime("2013-12-31 23:59:59"));

			$this->TestCase_TimeCalc(
				"GetTimeQuarterEnd",
				GetTime("2013-01-02 11:11:00"),
				null,
				GetTime("2013-03-31 23:59:59"));



		}
		
		
	}
	
	

		
