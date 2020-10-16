<?php

	require_once(GetWDKDir()."wdk_xml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test StringHTMLAddEmbracingTags");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			//$this->SetActive(false);
			$this->SetResult(true);	
			return true;
		}
		
		function TestCase_StringHTMLAddEmbracingTags(
			$strStringXML,
			$strToken,
			$strTagStart,
			$strTagEnd,
			$bIgnoreCase,
			$arrayTagWhitelist,
			$arrayTagBlacklist,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase: TestCase_StringHTMLAddEmbracingTags");
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


			$strResult = StringHTMLAddEmbracingTags(
				$strStringXML,
				$strToken,
				$strTagStart,
				$strTagEnd,
				$bIgnoreCase,
				$arrayTagWhitelist,
				$arrayTagBlacklist);

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
		
		
		function CallbackTest()
		{
			parent::CallbackTest();



$strIn =
"<p class=\"class1\">
	This paragraph is of class1.
</p>
";
$strOut =
"<p class=\"class1\">
	This paragraph is of <strong>class</strong>1.
</p>
";
			$this->TestCase_StringHTMLAddEmbracingTags(
				$strIn,
				"class",
				"<strong>",
				"</strong>",
				true,
				array(),
				array(),
				$strOut
				);
				
		

$strIn =
"<XML>
	<TEST attrib=\"token\">
		Here is the token!
	</TEST>
	<DONT>
		token
	</DONT>
</XML>
";
$strOut =
"<XML>
	<TEST attrib=\"token\">
		Here is the <tagged>token</tagged>!
	</TEST>
	<DONT>
		token
	</DONT>
</XML>
";
			$this->TestCase_StringHTMLAddEmbracingTags(
				$strIn,
				"token",
				"<tagged>",
				"</tagged>",
				true,
				array(),
				array("DONT"),
				$strOut
				);

		
			$strIn = "Smith &amp; Wesson";
			$strOut = "Smith <tagged>&amp;</tagged> Wesson";
			$this->TestCase_StringHTMLAddEmbracingTags(
				$strIn,
				"&amp;",
				"<tagged>",
				"</tagged>",
				true,
				array(),
				array(),
				$strOut
				);




			$strIn = "Smith &amp; Wesson";
			$strOut = "Smith <tagged>&amp;</tagged> Wesson";
			$this->TestCase_StringHTMLAddEmbracingTags(
				$strIn,
				"&amp;",
				"<tagged>",
				"</tagged>",
				true,
				array(),
				array(),
				$strOut
				);

			$strIn = "Line<br/>Break";
			$strOut = "Lin<tagged>e</tagged><br/>Br<tagged>e</tagged>ak";
			$this->TestCase_StringHTMLAddEmbracingTags(
				$strIn,
				"e",
				"<tagged>",
				"</tagged>",
				true,
				array(),
				array(),
				$strOut
				);


			$strIn = "Some Umlauts: ÄÖÜäöü";
			$strOut = "Some Umlauts: <tagged>&#196;</tagged>&#214;&#220;&#228;&#246;&#252;";
			$this->TestCase_StringHTMLAddEmbracingTags(
				$strIn,
				"Ä",
				"<tagged>",
				"</tagged>",
				true,
				array(),
				array(),
				$strOut
				);
	
			$strIn =  u("<html><body><textarea>highlighted</textarea><p>This is some text with a highlighted text with some entities (&copy;&amp;&nbsp;).</p></body></html>");
			$strOut = u("<html><body><textarea>highlighted</textarea><p>This is some text with a <strong>highlighted</strong> text with some entities (&#169;&amp;&#160;).</p></body></html>");
			$this->TestCase_StringHTMLAddEmbracingTags(
				$strIn,
				"highlighted",
				"<strong>",
				"</strong>",
				true,
				array(),
				array("textarea"),
				$strOut
				);


			$strIn =  "<html><body><textarea>highlighted</textarea><p>This is some text with a highlighted text with some entities (&copy;&amp;&nbsp;).</p></body></html>";
			$strOut = "<html><body><textarea>highlighted</textarea><p>This is some text with a <strong>highlighted</strong> text with some entities (&#169;&amp;&#160;).</p></body></html>";
			$this->TestCase_StringHTMLAddEmbracingTags(
				$strIn,
				"highlighted",
				"<strong>",
				"</strong>",
				true,
				array(),
				array("textarea"),
				$strOut
				);   

 
		}
		

		
	}
	
	

		
