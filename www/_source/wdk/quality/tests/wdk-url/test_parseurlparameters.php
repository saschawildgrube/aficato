<?php

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ParseURLParameters");
		}
		

		function TestCase_ParseURLParameters(
			$strURLParameters,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ParseURLParameters");
	
			$this->Trace("URLParameter : $strURLParameters");
			if (is_array($arrayExpectedResult))
			{
				$this->Trace("Expected Result:");
				$this->Trace($arrayExpectedResult);
			}
			else
			{
				$this->Trace("Expected Result: ".RenderBool($arrayExpectedResult));
			}

			$arrayResult = ParseURLParameters($strURLParameters);
			
			if (is_array($arrayResult))
			{
				$this->Trace("Result:");
				$this->Trace($arrayResult);
			}
			else
			{
				$this->Trace("Result: ".RenderBool($arrayResult));
			}
			
		
			if ($arrayResult != $arrayExpectedResult)
			{
				$this->Trace("Testcase FAILED!");	
				$this->Trace("");
				$this->Trace("");
				$this->SetResult(false);	
				return;
			}
	
			$this->Trace("Testcase PASSED!");
			$this->Trace("");
			$this->Trace("");
		}



		function OnTest()
		{
			parent::OnTest();
			
			$this->SetResult(true);

			$strInput = "tag";
			$arrayOutput = array(
				"tag" => ""
				);
			$this->TestCase_ParseURLParameters($strInput,$arrayOutput);
			$this->TestCase_ParseURLParameters(u($strInput),$arrayOutput);

			$strInput = "hello%20world";
			$arrayOutput = array( 
				"hello world" => ""  
				);
			$this->TestCase_ParseURLParameters($strInput,$arrayOutput);
			$this->TestCase_ParseURLParameters(u($strInput),$arrayOutput);

			$strInput = "hello world";
			$arrayOutput = array( 
				"hello world" => ""  
				);
			$this->TestCase_ParseURLParameters($strInput,$arrayOutput);
			$this->TestCase_ParseURLParameters(u($strInput),$arrayOutput);


			
			$strInput = "tag=value&tag2=value2";
			$arrayOutput = array(
				"tag" => "value",
				"tag2" => "value2"
				);
			$this->TestCase_ParseURLParameters($strInput,$arrayOutput);
			$this->TestCase_ParseURLParameters(u($strInput),$arrayOutput);

			$strInput = "tag=&tag2=";
			$arrayOutput = array(
				"tag" => "",
				"tag2" => ""
				);
			$this->TestCase_ParseURLParameters($strInput,$arrayOutput);
			$this->TestCase_ParseURLParameters(u($strInput),$arrayOutput);


			$strInput = "&";
			$arrayOutput = array(
				);
			$this->TestCase_ParseURLParameters($strInput,$arrayOutput);
			$this->TestCase_ParseURLParameters(u($strInput),$arrayOutput);

			$strInput = "   ";
			$arrayOutput = array(
				);
			$this->TestCase_ParseURLParameters($strInput,$arrayOutput);
			$this->TestCase_ParseURLParameters(u($strInput),$arrayOutput);


			$strInput = "hallo=welt\nhokus=pokus";
			$arrayOutput = array(
				"hallo" => "welt",
				"hokus" => "pokus");
			$this->TestCase_ParseURLParameters($strInput,$arrayOutput);
			$this->TestCase_ParseURLParameters(u($strInput),$arrayOutput);

			$strInput = "\n\nhallo=welt\n\n\n\nhokus=pokus\n\n\n";
			$arrayOutput = array(
				"hallo" => "welt",
				"hokus" => "pokus");
			$this->TestCase_ParseURLParameters($strInput,$arrayOutput);
			$this->TestCase_ParseURLParameters(u($strInput),$arrayOutput);

				
		}
	}
	
	

		
