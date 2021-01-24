<?php

	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test RenderDate");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			date_default_timezone_set("UTC");
			return true;
		}

		function TestCase_RenderDate(
			$time,
			$strExpectedValue)
		{ 
			$this->Trace("TestCase_RenderDate");
			$strValue = RenderDate($time);
			$this->Trace("RenderDate(\"".$time."\") = \"".$strValue."\"");
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
			$this->TestCase_RenderDate(0,"");
			$this->TestCase_RenderDate(1,"1970-01-01");
			$this->TestCase_RenderDate(mktime(8,46,0,9,11,2001),"2001-09-11");
		}
		
		
	}
	
	

		
