<?php

	require_once(GetWDKDir()."wdk_csv.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("CSV");
		}
		

		function TestCase_ParseCSV(
			$arrayExpectedResult,
			$strRawData,
			$bHeaderRow = true,
			$IncludeHeadersInResult = true,
			$strSeparator = ',',
			$arrayCommentTokens = array(),
			$strQuote = '"',
			$strEscapedQuote = '""')
		{ 
			$this->Trace("TestCase_CSV");
	
			$this->Trace("strRawData:\n>>>\n$strRawData\n<<<");
			$this->Trace("Expected Result:");
			$this->Trace($arrayExpectedResult);
	
	
			$arrayData = ParseCSV(
				$strRawData,
				$bHeaderRow,
				$IncludeHeadersInResult,
				$strSeparator,
				$arrayCommentTokens,
				$strQuote,
				$strEscapedQuote);
			
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


			$arrayExpectedResult = array();
			$strRawData = "NUMBER	;TEXT	;TEXT2		;COMMENT\n";
			
			for ($nRow = 0; $nRow < 100; $nRow++)
			{
				$strRawData .= "$nRow;\tLoram ipsum $nRow ;  \"hello-world-$nRow with \"\"quoted\"\" quotes\";Some Comment  // And some real comment; with a separator\n";
				$arrayExpectedResult[] = array(
					"NUMBER" => $nRow,
					"TEXT" => "Loram ipsum $nRow",
					"TEXT2" => "hello-world-$nRow with \"quoted\" quotes",
					"COMMENT" => "Some Comment");	
			}

		
			$this->TestCase_ParseCSV(
				$arrayExpectedResult,
				$strRawData,
				true,
				false,
				';',
				array('//'),
				'"',
				'""');

				

				
		}
	
		
		
	}
	
	

		
