<?php

	require_once(GetWDKDir()."wdk_localresources.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("MakeTID");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_MakeTID($strString,$strExpectedResult)
		{
			$this->Trace("TestCase_MakeTID");
			$this->Trace("Input          : \"$strString\"");
			$this->Trace("Expected result: \"$strExpectedResult\"");
			$strResult = MakeTID($strString); 
			$this->Trace("MakeTID returns: \"$strResult\"");
			if ($strResult == $strExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
			}
			$this->Trace("");
			
		}


		function OnTest()
		{
			parent::OnTest(); 

			$this->TestCase_MakeTID(
				"",
				"");

			
			$this->TestCase_MakeTID(
				"dies ist ein test",
				"TID_DIES_IST_EIN_TEST");

			$this->TestCase_MakeTID(
				"/path/",
				"TID_PATH");

			$this->TestCase_MakeTID(
				"/dir1/dir2/",
				"TID_DIR1_DIR2");

			$this->TestCase_MakeTID(
				"/dir1/dir2////",
				"TID_DIR1_DIR2");
				
			$this->TestCase_MakeTID(
				"/dir1/test.txt",
				"TID_DIR1_TEST-TXT");

			$this->TestCase_MakeTID(
				"//////dir1/test.txt",
				"TID_DIR1_TEST-TXT");
				
			$this->TestCase_MakeTID(
				"TID_EXAMPLE",
				"TID_EXAMPLE");


			
		}
		

	}
	
	

		
