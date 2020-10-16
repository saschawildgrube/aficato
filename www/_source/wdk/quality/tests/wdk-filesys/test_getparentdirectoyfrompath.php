<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetParentDirectoryFromPath");
		}
		

		function TestCase_GetParentDirectoryFromPath(
			$strParam,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_GetParentDirectoryFromPath");
	
			$this->Trace("strParam          = \"$strParam\"");
			$this->Trace("strExpectedResult = \"$strExpectedResult\"");
	
	
			// Test some function here
			$strResult = GetParentDirectoryFromPath($strParam);
	
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
			
			$this->TestCase_GetParentDirectoryFromPath(
				"/var/test.txt",
				"/");
			$this->TestCase_GetParentDirectoryFromPath(
				"/var/123//test.txt",
				"/var/123/");
			$this->TestCase_GetParentDirectoryFromPath(
				"/var/",
				"/");
			$this->TestCase_GetParentDirectoryFromPath(
				"/dir1/dir2/dir3/",
				"/dir1/dir2/");
			$this->TestCase_GetParentDirectoryFromPath(
				"/",
				false);
			$this->TestCase_GetParentDirectoryFromPath(
				"test.txt",
				false);
			
		}
		
		
	}
	
	

		
