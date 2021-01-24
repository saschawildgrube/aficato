<?php
	
	require_once(GetWDKDir()."wdk_fileio.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("FileReadToArray");
		}
		

		function TestCase_FileReadToArray(
			$strParam,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_FileReadToArray");
	
			$this->Trace("strParam = \"$strParam\"");
			$this->Trace("strExpectedResult:");
			$this->Trace(RenderValue($arrayExpectedResult));
		
			$arrayResult = FileReadToArray($strParam);
	
			$this->Trace("Result:");
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


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);
			
			$this->TestCase_FileReadToArray(
				GetWDKDir()."quality/testfiles/helloworld.txt",
				array("Hello","World"));

			$this->TestCase_FileReadToArray(
				GetWDKDir()."quality/testfiles/wrongfile.txt",
				false);

			$this->TestCase_FileReadToArray(
				GetWDKDir()."quality/testfiles/empty.txt",
				array());
			
		}
		
		
	}
	
	

		
