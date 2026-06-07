<?php

	require_once(GetWDKDir().'wdk_datetime.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Test GetTimeZoneOffset');
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_GetTimeZoneOffset(
			$strTimeZone,
			$time,
			$nExpectedValue)
		{ 
			$this->Trace('TestCase_GetTimeZoneOffset');
			$nResult = GetTimeZoneOffset($strTimeZone,$time);
			$this->Trace('GetTimeZoneOffset("'.$strTimeZone.'","'.RenderDateTime($time).'") = '.$nResult.'');
			if ($nResult == $nExpectedValue)
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

		
		function OnTest()
		{
			parent::OnTest();
			$this->TestCase_GetTimeZoneOffset('UTC',GetTime('2021-01-01'),0);
			$this->TestCase_GetTimeZoneOffset('UTC',GetTime('2021-07-01'),0);

			$this->TestCase_GetTimeZoneOffset('Europe/London',GetTime('2021-01-01'),0);
			$this->TestCase_GetTimeZoneOffset('Europe/London',GetTime('2021-07-01'),3600);

			$this->TestCase_GetTimeZoneOffset('Europe/Berlin',GetTime('2021-01-01'),3600);
			$this->TestCase_GetTimeZoneOffset('Europe/Berlin',GetTime('2021-07-01'),7200);

			$this->TestCase_GetTimeZoneOffset('America/New_York',GetTime('2021-01-01'),-18000);
			$this->TestCase_GetTimeZoneOffset('America/New_York',GetTime('2021-07-01'),-14400);
		}
		
		
	}
	
	

		
