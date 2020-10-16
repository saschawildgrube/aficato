<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetFolderNameFromPath");
		}
		

		function TestCase_GetFolderNameFromPath(
			$strParam,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_GetFolderNameFromPath");
	
			$this->Trace("strParam          = \"$strParam\"");
			$this->Trace("strExpectedResult = \"$strExpectedResult\"");
	
	
			$strResult = GetFolderNameFromPath($strParam);
	
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
			
			$this->TestCase_GetFolderNameFromPath(
				"/var/test.txt",
				"var");
			/*$this->TestCase_GetFolderNameFromPath(
				"/var/123//test.txt",
				"123");*/
			$this->TestCase_GetFolderNameFromPath(
				"/a/b/c/",
				"c");
			$this->TestCase_GetFolderNameFromPath(
				"/folder",
				"");
			$this->TestCase_GetFolderNameFromPath(
				"",
				"");
			$this->TestCase_GetFolderNameFromPath(
				"/",
				"");
			$this->TestCase_GetFolderNameFromPath(
				".",
				"");
			$this->TestCase_GetFolderNameFromPath(
				".ext",
				"");
			$this->TestCase_GetFolderNameFromPath(
				"folder",
				"");
			$this->TestCase_GetFolderNameFromPath(
				"test.xhtml",
				"");
			$this->TestCase_GetFolderNameFromPath(
				"dir.with.dots/folder.ext",
				"dir.with.dots");
			$this->TestCase_GetFolderNameFromPath(
				".htaccess",
				"");
			
		}
		
		
	}
	
	

		
