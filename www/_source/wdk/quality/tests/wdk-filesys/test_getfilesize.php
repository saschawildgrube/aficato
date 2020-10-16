<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetFileSize");
		}
		

		function TestCase_GetFileSize(
			$strFilePath,
			$nExpectedResult)
		{ 
			$this->Trace("TestCase_GetFileSize");
	
			$this->Trace("strFilePath = \"$strFilePath\"");
			$this->Trace("nExpectedResult: ".RenderValue($nExpectedResult));
			$nResult = GetFileSize($strFilePath);
	
			$this->Trace("GetFileSize() returns: ".RenderValue($nResult));
			
			if (	(($nResult === false) && ($nExpectedResult === false))
				||  ($nResult == $nExpectedResult) )
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

			$this->TestCase_GetFileSize(
				GetWDKDir()."quality/testfiles/helloworld.txt",
				12);

			$this->TestCase_GetFileSize(
				GetWDKDir()."quality/testfiles/empty.txt",
				0);

			$this->TestCase_GetFileSize(
				GetWDKDir()."quality/testfiles/doesnotexist.txt",
				false);

		
		}
		
		
	}
	
	

		
