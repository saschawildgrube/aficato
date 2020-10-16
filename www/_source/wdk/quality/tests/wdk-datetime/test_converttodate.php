<?php

	require_once(GetWDKDir().'wdk_datetime.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Test ConvertToDate');
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			date_default_timezone_set('UTC');
			return true;
		}

		function TestCase_ConvertToDate(
			$strInputValue,
			$strExpectedValue)
		{ 
			$this->Trace('TestCase_ConvertToDate');
			$strValue = ConvertToDate($strInputValue);
			$this->Trace('ConvertToDate(\''.$strInputValue.'\') = \''.$strValue.'\'');
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
			$this->TestCase_ConvertToDate('12.01.2010','2010-01-12');
			$this->TestCase_ConvertToDate('01.12.2010','2010-12-01');
			$this->TestCase_ConvertToDate('27.7.77','1977-07-27');
			$this->TestCase_ConvertToDate('07.07.77','1977-07-07');
			$this->TestCase_ConvertToDate('19.07.11','2011-07-19');
			$this->TestCase_ConvertToDate('9.11.01','2001-11-09');
			$this->TestCase_ConvertToDate('01/08/01','2001-01-08');
			$this->TestCase_ConvertToDate('19.1.2038','2038-01-19');

			$this->TestCase_ConvertToDate('bogus','');
			$this->TestCase_ConvertToDate('','');
			$this->TestCase_ConvertToDate('44.1.1985','');

		}
		
		
	}
	
	

		
