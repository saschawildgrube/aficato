<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringGetFromEmbracingTags");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringGetFromEmbracingTags(
			$strString,
			$strStartTag,
			$strEndTag,
			$strExpectedResult)
		{
			$this->Trace("TestCase_StringGetFromEmbracingTags");
			$this->Trace("strString = \"$strString\"");
			$this->Trace("strStartTag = \"$strStartTag\", strEndTag = \"$strEndTag\"");
			if ($strExpectedResult == false)
			{
				$this->Trace("Expected Result = false");
			}
			else
			{
				$this->Trace("Expected Result = \"$strExpectedResult\"");
			}
			$strResult = StringGetFromEmbracingTags($strString,$strStartTag,$strEndTag);
			if ($strResult == false)
			{
				$this->Trace("StringGetFromEmbracingTags returns false");
			}
			else
			{
				$this->Trace("StringGetFromEmbracingTags returns: \"$strResult\"");
			}
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
		}

		function CallbackTest()
		{
			parent::CallbackTest();
			
		
			$this->TestCase_StringGetFromEmbracingTags(
				"<xml>Contents</xml>",
				"<xml>",
				"</xml>",
				"Contents");

			$this->TestCase_StringGetFromEmbracingTags(
				"<xml>Contents</xml>",
				"<xml2>",
				"</xml2>",
				false);

			$this->TestCase_StringGetFromEmbracingTags(
				"blah<!--\"Test\"-->blubb",
				"<!--\"",
				"\"-->",
				"Test");		

			$this->TestCase_StringGetFromEmbracingTags(
				"blah<!--Test-->blubb",
				"<!--\"",
				"\"-->",
				false);		
				


			$this->TestCase_StringGetFromEmbracingTags(
				"blah\"--><!--\"Test\"-->blubb",
				"<!--\"",
				"\"-->",
				"Test");		

			$this->TestCase_StringGetFromEmbracingTags(
				"<TEST attrib=\"token\">",
				"attrib=\"",
				"\"",
				"token");		

			$this->TestCase_StringGetFromEmbracingTags(
				"{URL CONTENT=\"test\"}",
				"CONTENT=\"",
				"\"",
				"test");	

			$this->TestCase_StringGetFromEmbracingTags(
				"{URL CONTENT=\"test\"}",
				"content=\"",
				"\"",
				"test");	

			$this->TestCase_StringGetFromEmbracingTags(
				"INSERT INTO `System-TestStatusHistory` SET `SITE_PATH` = 'local', `ASSEMBLY_ID` = 'scaffoldtest', `GROUP_ID` = 'webservice-test-thing', `TEST_ID` = 'basic', `STATUS` = 'PASSED', `DATETIME` = '2013-02-11 23:19:53', `RUNTIME_SECONDS` = '3.994', `HASH` = '2788f4ac392d59464be45139316b5bb014e24538' ON DUPLICATE KEY UPDATE `SITE_PATH` = 'local', `ASSEMBLY_ID` = 'scaffoldtest', `GROUP_ID` = 'webservice-test-thing', `TEST_ID` = 'basic', `STATUS` = 'PASSED', `DATETIME` = '2013-02-11 23:19:53', `RUNTIME_SECONDS` = '3.994', `HASH` = '2788f4ac392d59464be45139316b5bb014e24538';",
				"INTO `",
				"`",
				"System-TestStatusHistory");	
				
				
				
				
				
				
				
				
				

		}
		

	}
	
	

		
