<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("IsDirectoryReadWriteAccess");
		}
		

		function TestCase_IsDirectoryReadWriteAccess(
			$strDirPath,
			$bExpectedResult)
		{ 
			$this->Trace("TestCase_IsDirectoryReadWriteAccess");
	
			$this->Trace("strDirPath = \"$strDirPath\"");
			$this->Trace("bExpectedResult: ".RenderBool($bExpectedResult));
			$bResult = IsDirectoryReadWriteAccess($strDirPath);
	
			$this->Trace("IsDirectory() returns: ".RenderBool(IsDirectory($strDirPath)));
			$this->Trace("IsDirectoryReadWriteAccess() returns: ".RenderBool($bResult));
	
			if ($bResult == $bExpectedResult)
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
			$this->TestCase_IsDirectoryReadWriteAccess(
				"/",
				false);
			$this->TestCase_IsDirectoryReadWriteAccess(
				GetTempDir(),
				IsDirectory(GetTempDir()));
			$this->Trace("");
		}
	}
	
	

		
