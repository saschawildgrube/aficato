<?php
	
	require_once(GetWDKDir().'wdk_datetime.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Test RenderDateTime');
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			date_default_timezone_set('UTC');
			return true;
		}

		function TestCase_RenderDateTime(
			$time,
			$strExpectedValue)
		{ 
			$this->Trace('TestCase_RenderDateTime');
			$strValue = RenderDateTime($time);
			$this->Trace('Expected value: "'.$strExpectedValue.'"');
			$this->Trace('RenderDateTime("'.$time.'") = "'.$strValue.'"');
			if ($strValue == $strExpectedValue)
			{
				$this->Trace('Testcase PASSED!');
			}
			else
			{
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);	
			}
			$this->Trace('');
			$this->Trace('');
		}

		
		function CallbackTest()
		{
			parent::CallbackTest();
			$this->TestCase_RenderDateTime(0,'');
			$this->TestCase_RenderDateTime(1,'1970-01-01 00:00:01');
			$this->TestCase_RenderDateTime(-1,'1969-12-31 23:59:59');
			if (PHP_INT_SIZE == 8)
			{
				$this->Trace('64 bit: 0xFFFFFFFF is in the future!');
				$this->TestCase_RenderDateTime(0xFFFFFFFF,'2106-02-07 06:28:15');
			}
			else
			{
				$this->Trace('32 bit: 0xFFFFFFFF is in the past!');
				$this->TestCase_RenderDateTime(0xFFFFFFFF,'1969-12-31 23:59:59');
			}
			$this->TestCase_RenderDateTime(mktime(8,46,0,9,11,2001),'2001-09-11 08:46:00');
			$this->TestCase_RenderDateTime(-2147483647,'1901-12-13 20:45:53'); 
			$this->TestCase_RenderDateTime(2147483647,'2038-01-19 03:14:07');
			
			$this->TestCase_RenderDateTime('1977-07-27 12:00:00','1977-07-27 12:00:00'); 
			$this->TestCase_RenderDateTime('1977-07-27 12:00','1977-07-27 12:00:00'); 
			$this->TestCase_RenderDateTime('1977-07-27','1977-07-27 00:00:00'); 

			$this->TestCase_RenderDateTime('bogus',''); 

			 
		}
		
		
	}
	
	

		
