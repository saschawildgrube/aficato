<?php

	require_once(GetWDKDir()."wdk_list.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ParseDateTime");
		}
		

		function TestCase_ParseDate(
			$strValue,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ParseDate");
	
			$this->Trace("Date/Time: $strValue");
			if (is_array($arrayExpectedResult))
			{
				$this->Trace("Expected Result:");
				$this->Trace($arrayExpectedResult);
			}
			else
			{
				$this->Trace("Expected Result: ".RenderBool($arrayExpectedResult));
			}

			$arrayResult = ParseDate($strValue);
			
			if (is_array($arrayResult))
			{
				$this->Trace("Result:");
				$this->Trace($arrayResult);
			}
			else
			{
				$this->Trace("Result: ".RenderBool($arrayResult));
			}
			
		
			if ($arrayResult != $arrayExpectedResult)
			{
				$this->Trace("Testcase FAILED!");	
				$this->Trace("");
				$this->Trace("");
				$this->SetResult(false);	
				return;
			}
	
			$this->Trace("Testcase PASSED!");
			$this->Trace("");
			$this->Trace("");
			
		}



		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->SetResult(true);
			
			$strValue = "2013-12-20";
			$arrayExpected = array(
				"YEAR" => 2013,
				"MONTH" => 12,
				"DAY" => 20,
				"HOUR" => 0,
				"MINUTE" => 0,
				"SECOND" => 0,
				"WEEKDAY" => 5,
				"DAYOFYEAR" => 354/*,  
				"DST" => false,
				"MINUTEOFDAY" => 1300*/
				);
			$this->TestCase_ParseDate($strValue,$arrayExpected);

			$strValue = "2013-01-01";
			$arrayExpected = array(
				"YEAR" => 2013,
				"MONTH" => 1,
				"DAY" => 1,
				"HOUR" => 0,
				"MINUTE" => 0,
				"SECOND" => 0,
				"WEEKDAY" => 2,
				"DAYOFYEAR" => 1
				/*"DST" => false,
				"MINUTEOFDAY" => 1300*/
				);
			$this->TestCase_ParseDate($strValue,$arrayExpected);

			$strValue = "2013-07-01";
			$arrayExpected = array(
				"YEAR" => 2013,
				"MONTH" => 7,
				"DAY" => 1,
				"HOUR" => 0,
				"MINUTE" => 0,
				"SECOND" => 0,
				"WEEKDAY" => 1,
				"DAYOFYEAR" => 182/*,  
				"DST" => true,
				"MINUTEOFDAY" => 1300*/
				);
			$this->TestCase_ParseDate($strValue,$arrayExpected);


			$strValue = "";
			$arrayExpected = false;
			$this->TestCase_ParseDate($strValue,$arrayExpected);

			$strValue = false;
			$arrayExpected = false;
			$this->TestCase_ParseDate($strValue,$arrayExpected);

			$strValue = "1.1.2011";
			$arrayExpected = false;
			$this->TestCase_ParseDate($strValue,$arrayExpected);

			$strValue = "2013-45-01 11:11:11";
			$arrayExpected = false;
			$this->TestCase_ParseDate($strValue,$arrayExpected);

			$strValue = "2011-11-11 11:11:00";
			$arrayExpected = false;
			$this->TestCase_ParseDate($strValue,$arrayExpected);


		}
	}
	
	

		
