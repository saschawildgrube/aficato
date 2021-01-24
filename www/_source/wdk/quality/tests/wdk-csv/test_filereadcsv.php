<?php

	require_once(GetWDKDir()."wdk_csv.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("FileReadCSV");
		}
		

		function TestCase_FileReadCSV_Format(
			$arrayExpectedResult,
			$strFormat,
			$strFile,
			$bHeaderRow,
			$IncludeHeadersInResult)
		{ 
			$this->Trace("TestCase_FileReadCSV_Format");
	
			$this->Trace("File: $strFile");
			$this->Trace("Expected Result:");
			$this->Trace($arrayExpectedResult);
	
	
			$arrayData = FileReadCSV_Format(
				$strFormat,
				$strFile,
				$bHeaderRow,
				$IncludeHeadersInResult);
			
			$this->Trace("Result:");
			$this->Trace($arrayData);
	
			if ($arrayData == $arrayExpectedResult)
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
			

			$arrayExpectedResult = array(
				array(
					"LEVEL" => "0",
					"CONTEXT" => "{W3CLINK}",
					"LABEL" => "?TID_NAVIGATION_VALIDATOR?",
					"CONDITIONS" => "")
				);
					
			$this->TestCase_FileReadCSV_Format(
				$arrayExpectedResult,
				"wdk",
				GetWDKDir() . "quality/testfiles/navigation_footer_en.txt",
				true,
				false);

			$arrayExpectedResult = array();
					
			$this->TestCase_FileReadCSV_Format(
				$arrayExpectedResult,
				"wdk",
				GetWDKDir() . "quality/testfiles/navigation_footer2_en.txt",
				true,
				false);

				
		}
	
		
		
	}
	
	

		
