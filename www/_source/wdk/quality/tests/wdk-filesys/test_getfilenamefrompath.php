<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetFileNameFromPath");
		}
		

		function TestCase_GetFileNameFromPath(
			$strParam,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_GetFileNameFromPath");
	
			$this->Trace("strParam          = \"$strParam\"");
			$this->Trace("strExpectedResult = \"$strExpectedResult\"");
	
	
			// Test some function here
			$strResult = GetFileNameFromPath($strParam);
	
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


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);
			
			$this->TestCase_GetFileNameFromPath(
				"/var/test.txt",
				"test.txt");
			$this->TestCase_GetFileNameFromPath(
				"/var/123//test.txt",
				"test.txt");
			$this->TestCase_GetFileNameFromPath(
				"/file",
				"file");
			$this->TestCase_GetFileNameFromPath(
				"",
				"");
			$this->TestCase_GetFileNameFromPath(
				"/",
				"");
			$this->TestCase_GetFileNameFromPath(
				".",
				".");
			$this->TestCase_GetFileNameFromPath(
				".ext",
				".ext");
			$this->TestCase_GetFileNameFromPath(
				"file",
				"file");
			$this->TestCase_GetFileNameFromPath(
				"test.xhtml",
				"test.xhtml");
			$this->TestCase_GetFileNameFromPath(
				"dir.with.dots/file.ext",
				"file.ext");
			$this->TestCase_GetFileNameFromPath(
				".htaccess",
				".htaccess");
			
		}
		
		
	}
	
	

		
