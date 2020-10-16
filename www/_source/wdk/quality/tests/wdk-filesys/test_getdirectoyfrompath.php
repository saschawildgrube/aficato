<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetDirectoryFromPath");
		}
		

		function TestCase_GetDirectoryFromPath(
			$strParam,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_GetDirectoryFromPath");
	
			$this->Trace("strParam          = \"$strParam\"");
			$this->Trace("strExpectedResult = \"$strExpectedResult\"");
	
	
			// Test some function here
			$strResult = GetDirectoryFromPath($strParam);
	
			$this->Trace("strResult = \"$strResult\"");
	
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
			$this->Trace("");
		}


		function CallbackTest()
		{
			parent::CallbackTest();
					
			$this->SetResult(true);
			
			$this->TestCase_GetDirectoryFromPath(
				"/var/test.txt",
				"/var/");
			$this->TestCase_GetDirectoryFromPath(
				"/var/123//test.txt",
				"/var/123//");
			$this->TestCase_GetDirectoryFromPath(
				"/",
				"/");
			$this->TestCase_GetDirectoryFromPath(
				"test.txt",
				"");
			
		}
		
		
	}
	
	

		
