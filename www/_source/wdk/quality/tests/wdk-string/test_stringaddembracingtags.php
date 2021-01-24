<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringAddEmbracingTags");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringAddEmbracingTags($strString,$strToken,$strStartTag,$strEndTag,$strExpectedResult)
		{
			$this->Trace("TestCase_StringAddEmbracingTags");
			$this->Trace("strString = \"$strString\"");
			$this->Trace("strToken = \"$strToken\", strStartTag = \"$strStartTag\", strEndTag = \"$strEndTag\"");
			$this->Trace("Expected Result = \"$strExpectedResult\"");
			$strResult = StringAddEmbracingTags($strString,$strToken,$strStartTag,$strEndTag);
			$this->Trace("StringAddEmbracingTags returns: \"$strResult\"");
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

		function TestCase_StringAddEmbracingTagsIgnoreCase($strString,$strToken,$strStartTag,$strEndTag,$strExpectedResult)
		{
			$this->Trace("TestCase_StringAddEmbracingTagsIgnoreCase");
			$this->Trace("strString = \"$strString\"");
			$this->Trace("strToken = \"$strToken\", strStartTag = \"$strStartTag\", strEndTag = \"$strEndTag\"");
			$this->Trace("Expected Result = \"$strExpectedResult\"");
			$strResult = StringAddEmbracingTagsIgnoreCase($strString,$strToken,$strStartTag,$strEndTag);
			$this->Trace("StringAddEmbracingTagsIgnoreCase returns: \"$strResult\"");
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
		
		function OnTest()
		{
			parent::OnTest();
			
		
			$this->TestCase_StringAddEmbracingTags(
				"This is a text with a highlight.",
				"highlight",
				"<strong>",
				"</strong>",
				"This is a text with a <strong>highlight</strong>.");

			$this->TestCase_StringAddEmbracingTags(
				"A text where indivdual words need some wiki style links",
				"links",
				"[",
				"]",
				"A text where indivdual words need some wiki style [links]");

			$this->TestCase_StringAddEmbracingTags(
				"Start is the key word",
				"Start",
				"=",
				"=\n",
				"=Start=\n is the key word");

			$this->TestCase_StringAddEmbracingTags(
				"A text containing a key word where the key word is repeated.",
				"key word",
				"<a href=\"#key\">",
				"</a>",
				"A text containing a <a href=\"#key\">key word</a> where the <a href=\"#key\">key word</a> is repeated.");



			$this->TestCase_StringAddEmbracingTagsIgnoreCase(
				"This is a text with a HIGHLIGHT.",
				"highlight",
				"<strong>",
				"</strong>",
				"This is a text with a <strong>HIGHLIGHT</strong>.");

			$this->TestCase_StringAddEmbracingTagsIgnoreCase(
				"A text where indivdual words need some wiki style links",
				"LINKS",
				"[",
				"]",
				"A text where indivdual words need some wiki style [links]");

		
			$this->TestCase_StringAddEmbracingTagsIgnoreCase(
				"Start is the key word",
				"start",
				"=",
				"=\n",
				"=Start=\n is the key word");

			$this->TestCase_StringAddEmbracingTagsIgnoreCase(
				"A text containing a key Word where the Key word is repeated.",
				"Key word",
				"<a href=\"#key\">",
				"</a>",
				"A text containing a <a href=\"#key\">key Word</a> where the <a href=\"#key\">Key word</a> is repeated.");
		

		}
		

	}
	
	

		
