<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	require_once(GetWDKDir()."wdk_utf8.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("IsFileUTF8BOM");
		}
		

		function TestCase_IsFileUTF8BOM(
			$strFilePath,
			$bExpectedResult)
		{ 
			$this->Trace("TestCase_IsFileUTF8BOM");
	
			$this->Trace("strFilePath = \"$strFilePath\"");
			$this->Trace("bExpectedResult: ".RenderBool($bExpectedResult));
			$bResult = IsFileUTF8BOM($strFilePath);
	
			$this->Trace("IsFileUTF8BOM() returns: ".RenderBool($bResult));
	
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

			$this->TestCase_IsFileUTF8BOM(
				GetWDKDir()."wdk.txt",
				false);
			
			$this->TestCase_IsFileUTF8BOM(
				GetWDKDir()."quality/testfiles/helloworld.txt",
				false);

			$this->TestCase_IsFileUTF8BOM(
				GetWDKDir()."quality/testfiles/bom.txt",
				true);

			$this->TestCase_IsFileUTF8BOM(
				GetWDKDir()."quality/testfiles/doesnotexist.txt",
				false);
			
			
		}
		
		
	}
	
	

		
