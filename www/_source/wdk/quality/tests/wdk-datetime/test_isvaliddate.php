<?php

	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test IsValidDate");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			date_default_timezone_set("UTC");
			return true;
		}

		function TestCase_IsValidDate(
			$strDate,
			$bExpectedValid)
		{ 
			$this->Trace("TestCase_IsValidDate");
		
			$this->Trace("strDate = \"$strDate\"");
		
		
			$bValue = IsValidDate($strDate);
		
			$this->Trace("IsValidDate(\"".$strDate."\") = ".RenderBool($bValue));
		
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

		
		function CallbackTest()
		{
			parent::CallbackTest();
			$this->TestCase_IsValidDate("2008-05-03",true);
			$this->TestCase_IsValidDate("2008-03-25",true);
			$this->TestCase_IsValidDate("1970-01-01",true);
			$this->TestCase_IsValidDate("2038-01-14",true);
			$this->TestCase_IsValidDate("2008-0325",false);
			$this->TestCase_IsValidDate("2008-03-2",false);
			$this->TestCase_IsValidDate("2008-03-234",false);
			$this->TestCase_IsValidDate("2008-13-01",false);
			
			$this->TestCase_IsValidDate("1969-12-31",true);
			$this->TestCase_IsValidDate("2038-01-20",true);
			


		}
		
		
	}
	
	

		
