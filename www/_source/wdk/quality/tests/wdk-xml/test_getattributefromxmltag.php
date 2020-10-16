<?php

	require_once(GetWDKDir()."wdk_xml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test GetAttributeFromXMLTag");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			//$this->SetActive(false);
			$this->SetResult(true);	
			return true;
		}
		
		function TestCase_GetAttributeFromXMLTag(
			$strTag,
			$strAttrID,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase: TestCase_GetAttributeFromXMLTag");
			$this->Trace("Input:");
			$this->Trace(">>>");
			$this->Trace($strTag);
			$this->Trace("<<<");

			$this->Trace("Attribute ID:");
			$this->Trace(">>>");
			$this->Trace($strAttrID);
			$this->Trace("<<<");


			$this->Trace("Expected Result:");
			$this->Trace(">>>");
			$this->Trace($strExpectedResult);
			$this->Trace("<<<");


			$strResult = GetAttributeFromXMLTag(
				$strTag,
				$strAttrID);
				
			if ($strResult == false && $strExpectedResult == false)
			{
				$this->Trace("Testcase PASSED!");
				$this->Trace("");
				return;
			}

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
		
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->TestCase_GetAttributeFromXMLTag(
				"<TEST attrib=\"token\">",
				"attrib",
				"token");
				
			$this->TestCase_GetAttributeFromXMLTag(
				"<TEST attrib2=\"token\">",
				"attrib",
				"");

			$this->TestCase_GetAttributeFromXMLTag(
				"<TEST attrib2=\"token\" hello=\"world\">",
				"attrib2",
				"token");

			$this->TestCase_GetAttributeFromXMLTag(
				"<TEST context=\"css\" ext=\"png\">",
				"ext",
				"png");

			$this->TestCase_GetAttributeFromXMLTag(
				"<TEST context=\"css\" ext=\"png\">",
				"context",
				"css");


			$this->TestCase_GetAttributeFromXMLTag(
				"{URL CONTENT=\"test\"}",
				"CONTENT",
				"test");
		
			$this->TestCase_GetAttributeFromXMLTag(
				"{URL CONTENT=\"test\"}",
				"content",
				"test");

			$this->TestCase_GetAttributeFromXMLTag(
				"{URL\nCONTENT=\"test\"}",
				"content",
				"test");

			$this->TestCase_GetAttributeFromXMLTag(
				"{URL\tCONTENT=\"test\"}",
				"content",
				"test");

				
		}
		

		
	}
	
	

		
