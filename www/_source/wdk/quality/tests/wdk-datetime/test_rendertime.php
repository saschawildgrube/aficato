<?php
	
	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test RenderTime");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			date_default_timezone_set("UTC");
			return true;
		}

		function TestCase_RenderTime(
			$time,
			$strExpectedValue)
		{ 
			$this->Trace("TestCase_RenderTime");
			$strValue = RenderTime($time);
			$this->Trace("RenderTime(\"".$time."\") = \"".$strValue."\"");
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
			$this->TestCase_RenderTime(0,"");
			$this->TestCase_RenderTime(1,"00:00:01");
			$this->TestCase_RenderTime(mktime(8,46,0,9,11,2001),"08:46:00");
		}
		
		
	}
	
	

		
