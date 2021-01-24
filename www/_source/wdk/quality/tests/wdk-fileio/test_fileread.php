<?php
	
	require_once(GetWDKDir()."wdk_fileio.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("FileRead");
		}
		

		function TestCase_FileRead(
			$strParam,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_FileRead");
	
			$this->Trace("strParam = \"$strParam\"");
			$this->Trace("strExpectedResult:");
			$this->Trace(RenderValue($strExpectedResult));
		
			$strResult = FileRead($strParam);
	
			$this->Trace("Result:");
			$this->Trace(RenderValue($strResult));
	
			if ($strResult === $strExpectedResult)
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
			
			$this->TestCase_FileRead(
				GetWDKDir()."quality/testfiles/helloworld.txt",
				"Hello\nWorld");

			$this->TestCase_FileRead(
				GetWDKDir()."quality/testfiles/wrongfile.txt",
				false);

			$this->TestCase_FileRead(
				GetWDKDir()."quality/testfiles/empty.txt",
				""); 
			
		}
		
		
	}
	
	

		
