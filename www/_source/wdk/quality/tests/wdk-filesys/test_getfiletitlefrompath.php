<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetFileTitleFromPath");
		}
		

		function TestCase_GetFileTitleFromPath(
			$strParam,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_GetFileTitleFromPath");
	
			$this->Trace("strParam          = \"$strParam\"");
			$this->Trace("strExpectedResult = \"$strExpectedResult\"");
	
	
			// Test some function here
			$strResult = GetFileTitleFromPath($strParam);
	
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
			
			$this->TestCase_GetFileTitleFromPath(
				"/var/test.txt",
				"test");
			$this->TestCase_GetFileTitleFromPath(
				"/var/123//test.txt",
				"test");
			$this->TestCase_GetFileTitleFromPath(
				"/file",
				"file");
			$this->TestCase_GetFileTitleFromPath(
				"",
				"");
			$this->TestCase_GetFileTitleFromPath(
				"/",
				"");
			$this->TestCase_GetFileTitleFromPath(
				".",
				".");
			$this->TestCase_GetFileTitleFromPath(
				".ext",
				".ext");
			$this->TestCase_GetFileTitleFromPath(
				"file",
				"file");
			$this->TestCase_GetFileTitleFromPath(
				"test.xhtml",
				"test");
			$this->TestCase_GetFileTitleFromPath(
				"dir.with.dots/file.ext",
				"file");
			$this->TestCase_GetFileTitleFromPath(
				".htaccess",
				".htaccess");
			
		}
		
		
	}
	
	

		
