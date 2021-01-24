<?php

	require_once(GetWDKDir()."wdk_xml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test StringHTMLtoXMLEntities");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}
		
		function TestCase_StringHTMLtoXMLEntities(
			$strString,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase: StringHTMLtoXMLEntities");
			
			$this->Trace("Input: \"$strString\"");
			$this->Trace("Expected Result: \"$strExpectedResult\"");

			$strResult = StringHTMLtoXMLEntities($strString);

			$this->Trace("Result: \"$strResult\"");
					
				
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

			$this->TestCase_StringHTMLtoXMLEntities("Äbc","&#196;bc");
			$this->TestCase_StringHTMLtoXMLEntities(u("Ä"),"&#196;");
			$this->TestCase_StringHTMLtoXMLEntities("Ä","&#196;");
			$this->TestCase_StringHTMLtoXMLEntities("ÄÄ","&#196;&#196;");  //
			$this->TestCase_StringHTMLtoXMLEntities("XXXXXÄ","XXXXX&#196;");
			$this->TestCase_StringHTMLtoXMLEntities(u("MÄXXXXX"),"M&#196;XXXXX"); //
			$this->TestCase_StringHTMLtoXMLEntities("abc","abc"); 
			$this->TestCase_StringHTMLtoXMLEntities(u("ÄÖÜ"),"&#196;&#214;&#220;"); //
			$this->TestCase_StringHTMLtoXMLEntities(u("üü"),"&#252;&#252;");
			$this->TestCase_StringHTMLtoXMLEntities("&","&");
			$this->TestCase_StringHTMLtoXMLEntities("&amp;","&amp;");
			$this->TestCase_StringHTMLtoXMLEntities("&uuml;","&#252;");
			//$this->TestCase_XMLEncode("http://www.websitedevkit.com/qualitydashboard/?sessionid=5ffbcbd2e13707135c23baec1599dcbe1c6c7754&image=icon_undo","http://www.websitedevkit.com/qualitydashboard/?sessionid=5ffbcbd2e13707135c23baec1599dcbe1c6c7754&amp;image=icon_undo");

		}
		

		
	}
	
	

		
