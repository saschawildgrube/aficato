<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Write access to source files");
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
				GetSourceDir(),
				false);

			$this->TestCase_IsDirectoryReadWriteAccess(
				GetWDKDir(),
				false);
				
			$this->Trace("Please note: If the php process has write access to the source code files of the application this may imply a number of security risks. Please check the PHP handler settings of your hosting environment to make sure that the PHP handler process has no write access to the application source code files! Hint: The apache module handler usually does not have write access.");
			$this->Trace("");
				
		}
		
		
	}
	
	

		
