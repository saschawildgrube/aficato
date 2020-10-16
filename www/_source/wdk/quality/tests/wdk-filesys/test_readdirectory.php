<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("ReadDirectory");
		}
		
		function CallbackInit()
		{
			$bMissingDir = false;
			$strDir = GetWDKDir()."quality/testdir/dir3/";
			if (IsDirectory($strDir) == false)
			{
				$this->Trace("The following directory does not exist:\n$strDir");
				$bMissingDir = true;
			}
			$strDir = GetWDKDir()."quality/testdir/dir2/dir21/";
			if (IsDirectory($strDir) == false)
			{
				$this->Trace("The following directory does not exist:\n$strDir");
				$bMissingDir = true;
			}
			if ($bMissingDir == true)
			{
				$this->Trace("This will cause the test to fail. A possible reason for that is that zip archives do not store empty directories. Create the directories to make the test work!");
			}
			return parent::CallbackInit();	 
		}

		function TestCase_ReadDirectory(
			$strDirPath,
			$bRecursive,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ReadDirectory");
	
			$this->Trace("strDirPath = \"$strDirPath\"");
			$this->Trace("bRecursive = ".RenderBool($bRecursive));
			$this->Trace("arrayExpectedResult:");
			$this->Trace(RenderValue($arrayExpectedResult));
	
	
			// Test some function here
			$arrayResult = ReadDirectory($strDirPath,$bRecursive);
	
			$this->Trace("arrayResult:");
			$this->Trace(RenderValue($arrayResult));
	
			if ($arrayResult == $arrayExpectedResult)
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
			
			$this->TestCase_ReadDirectory(
				GetWDKDir()."quality/testdir/",
				false,
				array(
					"dir1/",
					"dir2/",
					"a.ext",
					"file1.ext",
					"file2.ext"));
		
		
			$this->TestCase_ReadDirectory(
				GetWDKDir()."quality/testdir/",
				true,
				array(
					"dir1/",
					"dir1/dir11/",
					"dir1/dir11/file111.ext",
					"dir1/dir11/file112.ext",
					"dir2/",
					"dir2/file21.ext",
					"dir2/file22",
					"dir2/file23.extext",
					"a.ext",
					"file1.ext",
					"file2.ext"));					

			$this->TestCase_ReadDirectory(
				"/",
				true,
				false);					
					
		}
		
		
	}
	
	

		
