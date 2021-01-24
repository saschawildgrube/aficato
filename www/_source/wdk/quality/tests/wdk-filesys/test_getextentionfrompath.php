<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetExtentionFromPath");
		}
		

		function TestCase_GetExtentionFromPath(
			$strParam,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_GetExtentionFromPath");
	
			$this->Trace("strParam          = \"$strParam\"");
			$this->Trace("strExpectedResult = \"$strExpectedResult\"");
	
	
			// Test some function here
			$strResult = GetExtentionFromPath($strParam);
	
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
			
			$this->TestCase_GetExtentionFromPath(
				"/var/test.txt",
				"txt");
			$this->TestCase_GetExtentionFromPath(
				"/var/123//test.txt",
				"txt");
			$this->TestCase_GetExtentionFromPath(
				"/file",
				"");
			$this->TestCase_GetExtentionFromPath(
				"",
				"");
			$this->TestCase_GetExtentionFromPath(
				"/",
				"");
			$this->TestCase_GetExtentionFromPath(
				".",
				"");
			$this->TestCase_GetExtentionFromPath(
				".ext",
				"ext");
			$this->TestCase_GetExtentionFromPath(
				"file",
				"");
			$this->TestCase_GetExtentionFromPath(
				"test.xhtml",
				"xhtml");
			$this->TestCase_GetExtentionFromPath(
				"dir.with.dots/file.ext",
				"ext");
			$this->TestCase_GetExtentionFromPath(
				".htaccess",
				"htaccess");
			
		}
		
		
	}
	
	

		
