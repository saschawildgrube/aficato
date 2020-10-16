<?php

	require_once(GetWDKDir()."wdk_csv.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Make CSV");
		}
		

		function TestCase_MakeCSV(
			$arrayData,
			$strFormat,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_MakeCSV");
	
			$this->Trace("Input:");
			$this->Trace($arrayData);
			$this->Trace("Expected Result:");
			$this->Trace($strExpectedResult);
	
	
			$strResult = MakeCSV_Format(
				$strFormat,
				$arrayData);
			
			$this->Trace("Result:");
			$this->Trace($strResult);
			
			$this->Trace(RenderHex($strExpectedResult));
			$this->Trace(RenderHex($strResult));
	
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

			$arrayData = array(
				array(
					"COLUMN1" => "some quoted text with a semi-colon;",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => "more text",
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
			$strExpected = "\"COLUMN1\";\"COLUMN2\";\"COLUMN3\"\r\n\"some quoted text with a semi-colon;\";\"23\";\"line one\"\r\n\"more text\";\"0.23\";\"line two\"";
			$this->TestCase_MakeCSV(
				$arrayData,
				"wdk",
				$strExpected);



			$arrayData = array(
				array(
					"COLUMN1" => "some quoted text with a semi-colon; and \"quoted\" text.",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => "more text",
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
			$strExpected = "\"COLUMN1\";\"COLUMN2\";\"COLUMN3\"\r\n\"some quoted text with a semi-colon; and \"\"quoted\"\" text.\";\"23\";\"line one\"\r\n\"more text\";\"0.23\";\"line two\"";
			$this->TestCase_MakeCSV(
				$arrayData,
				"wdk",
				$strExpected);



			$arrayData = array(
				array(
					"COLUMN1" => "some quoted text with a semi-colon; and \"quoted\" text.",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => "more text with new\nline",
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
			$strExpected = 
				"\"COLUMN1\";\"COLUMN2\";\"COLUMN3\"\r\n".
				"\"some quoted text with a semi-colon; and \"\"quoted\"\" text.\";\"23\";\"line one\"\r\n".
				"\"more text with new\r\nline\";\"0.23\";\"line two\"";
			$this->TestCase_MakeCSV(
				$arrayData,
				"wdk",
				$strExpected);


			$arrayData = array(
				array(
					"COLUMN1" => "some quoted text with a semi-colon; and \"quoted\" text.",
					"COLUMN2" => "23",
					"COLUMN3" => "line one"),
				array(
					"COLUMN1" => "more text with new\r\nline",
					"COLUMN2" => "0.23",
					"COLUMN3" => "line two")
				);
			$strExpected = "\"COLUMN1\",\"COLUMN2\",\"COLUMN3\"\r\n\"some quoted text with a semi-colon; and \"\"quoted\"\" text.\",\"23\",\"line one\"\r\n\"more text with new\r\nline\",\"0.23\",\"line two\"";
			$this->TestCase_MakeCSV(
				$arrayData,
				"rfc4180",
				$strExpected);


			$arrayData = array();
			$strExpected = "";
			$this->TestCase_MakeCSV(
				$arrayData,
				"wdk",
				$strExpected);

  
				
		}
	
		
		
	}
	
	

		
