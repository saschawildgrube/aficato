<?php

	require_once(GetWDKDir()."wdk_xml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test StringXMLEntities");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}
		
		function TestCase_StringXMLEntities(
			$strIn,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase: TestCase_StringXMLEntities");
			$this->Trace("Input:");
			$this->Trace(">>>");
			$this->Trace($strIn);
			$this->Trace("<<<");

			$this->Trace("Expected Result:");
			$this->Trace(">>>");
			$this->Trace($strExpectedResult);
			$this->Trace("<<<");


			$strResult = StringXMLEntities(
				$strIn);
				
			$this->Trace("Result:");
			$this->Trace(">>>");
			$this->Trace($strResult);
			$this->Trace("<<<");
						
				
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
		
		
		function OnTest()
		{
			parent::OnTest();

			$this->TestCase_StringXMLEntities("Äbc","&#196;bc");
			$this->TestCase_StringXMLEntities("&uuml;","&uuml;");
			$this->TestCase_StringXMLEntities("ä","&#228;");
			$this->TestCase_StringXMLEntities("&amp;","&amp;");
			$this->TestCase_StringXMLEntities("&copy;","&copy;");
			$this->TestCase_StringXMLEntities("Hallo &amp; Welt","Hallo &amp; Welt");
			$this->TestCase_StringXMLEntities("&nbsp;","&nbsp;");
			$this->TestCase_StringXMLEntities("<html><body><textarea>highlighted</textarea><p>This is some text with a highlighted text with some entities (&copy;&amp;&nbsp;).</p></body></html>","<html><body><textarea>highlighted</textarea><p>This is some text with a highlighted text with some entities (&copy;&amp;&nbsp;).</p></body></html>");
			
			/*
			for ($nChar = 180; $nChar < 255; $nChar++)
			{
				$strInput = chr($nChar);
				$strExpected = chr($nChar);
				if ($nChar > 100 && $nChar < 105)
				{
					$strExpected = "&#".$nChar.";";
				}
				$this->TestCase_StringXMLEntities($strInput,$strExpected);
			}
			*/
			


		}
		

		
	}
	
	

		
