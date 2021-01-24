<?php

	require_once(GetWDKDir()."wdk_xml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test HtmlEncode");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}
		
		function TestCase_HtmlEncode(
			$strString,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase: HtmlEncode");
			
			$this->Trace("Input: \"$strString\"");
			$this->Trace("Expected Result: \"$strExpectedResult\"");

			$strResult = HtmlEncode($strString);

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

			//$this->TestCase_HtmlEncode("Ä","&Auml;");
			$this->TestCase_HtmlEncode(u("Äbc"),"&Auml;bc");
			$this->TestCase_HtmlEncode(u("Ä"),"&Auml;");
			$this->TestCase_HtmlEncode(u("ÄÄ"),"&Auml;&Auml;");
			$this->TestCase_HtmlEncode(u("XXXXXÄ"),"XXXXX&Auml;");
			$this->TestCase_HtmlEncode(u("ÄXXXXX"),"&Auml;XXXXX");
			$this->TestCase_HtmlEncode("abc","abc");
			$this->TestCase_HtmlEncode(u("ÄÖÜ"),"&Auml;&Ouml;&Uuml;");
			$this->TestCase_HtmlEncode(u("üü"),"&uuml;&uuml;");
			$this->TestCase_HtmlEncode("&","&amp;");
			$this->TestCase_HtmlEncode("http://www.websitedevkit.com/qualitydashboard/?sessionid=5ffbcbd2e13707135c23baec1599dcbe1c6c7754&image=icon_undo","http://www.websitedevkit.com/qualitydashboard/?sessionid=5ffbcbd2e13707135c23baec1599dcbe1c6c7754&amp;image=icon_undo");
		}
	}
		
