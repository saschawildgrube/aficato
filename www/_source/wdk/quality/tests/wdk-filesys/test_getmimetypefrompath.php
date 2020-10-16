<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetMimeTypeFromPath");
		}
		

		function TestCase_GetMimeTypeFromPath(
			$strParam,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_GetMimeTypeFromPath");
	
			$this->Trace("strParam          = \"$strParam\"");
			$this->Trace("strExpectedResult = \"$strExpectedResult\"");

			$strResult = GetMimeTypeFromPath($strParam);
	
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
			
			$this->TestCase_GetMimeTypeFromPath(
				"/var/test.txt",
				"text/plain");

			$this->TestCase_GetMimeTypeFromPath(
				".txt",
				"text/plain");

			$this->TestCase_GetMimeTypeFromPath(
				"test.pdf",
				"application/pdf");

			$this->TestCase_GetMimeTypeFromPath(
				"/",
				false);


		}
		
		
	}
	
	

		
