<?php

	require_once(GetWDKDir()."wdk_xml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test StringXMLAddEmbracingTags");
		}
		
		function OnInit()
		{
			parent::OnInit();
			//$this->SetActive(false);
			$this->SetResult(true);	
			return true;
		}
		
		function TestCase_StringXMLAddEmbracingTags(
			$strStringXML,
			$strToken,
			$strTagStart,
			$strTagEnd,
			$bIgnoreCase,
			$arrayTagWhitelist,
			$arrayTagBlacklist,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase: TestCase_StringXMLAddEmbracingTags");
			$this->Trace("Input:");
			$this->Trace(">>>");
			$this->Trace($strStringXML);
			$this->Trace("<<<");

			$this->Trace("Token:");
			$this->Trace(">>>");
			$this->Trace($strToken);
			$this->Trace("<<<");


			$this->Trace("Expected Result:");
			$this->Trace(">>>");
			$this->Trace($strExpectedResult);
			$this->Trace("<<<");


			$strResult = StringXMLAddEmbracingTags(
				$strStringXML,
				$strToken,
				$strTagStart,
				$strTagEnd,
				$bIgnoreCase,
				$arrayTagWhitelist,
				$arrayTagBlacklist);
				
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
						
				
			if (StringRemoveControlChars($strResult) == StringRemoveControlChars($strExpectedResult))
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



$strXML_in =
"<TEST attrib=\"token\">
	Here is the token!
</TEST>
";
$strXML_out =
"<TEST attrib=\"token\">
	Here is the <tagged>token</tagged>!
</TEST>
";
			$this->TestCase_StringXMLAddEmbracingTags(
				$strXML_in,
				"token",
				"<tagged>",
				"</tagged>",
				true,
				array(),
				array(),
				$strXML_out
				);
				
				

$strXML_in =
"<XML>
	<TEST attrib=\"token\">
		Here is the token!
	</TEST>
	<DONT>
		token
	</DONT>
</XML>
";
$strXML_out =
"<XML>
	<TEST attrib=\"token\">
		Here is the <tagged>token</tagged>!
	</TEST>
	<DONT>
		token
	</DONT>
</XML>
";
			$this->TestCase_StringXMLAddEmbracingTags(
				$strXML_in,
				"token",
				"<tagged>",
				"</tagged>",
				true,
				array(),
				array("DONT"),
				$strXML_out
				);

		
			$strXML_in = "Smith &amp; Wesson";
			$strXML_out = "Smith <tagged>&amp;</tagged> Wesson";
			$this->TestCase_StringXMLAddEmbracingTags(
				$strXML_in,
				"&amp;",
				"<tagged>",
				"</tagged>",
				true,
				array(),
				array(),
				$strXML_out
				);

			$strXML_in = "Smith & Wesson";
			$strXML_out = "Smith <tagged>&</tagged> Wesson"; // not really :-)
			$this->TestCase_StringXMLAddEmbracingTags(
				$strXML_in,
				"&",
				"<tagged>",
				"</tagged>",
				true,
				array(),
				array(),
				false
				);


		

			$strXML_in = "Some Umlauts: ÄÖÜäöü";
			$strXML_out = "Some Umlauts: <tagged>Ä</tagged>ÖÜäöü";
/*
			$this->TestCase_StringXMLAddEmbracingTags(
				$strXML_in,
				"Ä",
				"<tagged>",
				"</tagged>",
				true,
				array(),
				array(),
				$strXML_out
				);
				*/
		
			$this->TestCase_StringXMLAddEmbracingTags(
				StringHTMLtoXMLEntities($strXML_in),
				StringHTMLtoXMLEntities("Ä"),
				"<tagged>",
				"</tagged>",
				true,
				array(),
				array(),
				StringHTMLtoXMLEntities($strXML_out)
				);
					
	
			$strXML_in =  u("<html><body><textarea>highlighted</textarea><p>This is some text with a highlighted text with some entities (&copy;&amp;&nbsp;).</p></body></html>");
			$strXML_out = u("<html><body><textarea>highlighted</textarea><p>This is some text with a <strong>highlighted</strong> text with some entities (&copy;&amp;&nbsp;).</p></body></html>");
			$this->TestCase_StringXMLAddEmbracingTags(
				StringHTMLtoXMLEntities($strXML_in),
				"highlighted",
				"<strong>",
				"</strong>",
				true,
				array(),
				array("textarea"),
				StringHTMLtoXMLEntities($strXML_out)
				);


		}
		

		
	}
	
	

		
