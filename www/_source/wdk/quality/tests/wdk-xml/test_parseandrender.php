<?php
		
	require_once(GetWDKDir()."wdk_xml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test CXMLElement Parse and Render");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);	
			return true;
		}
		
		function TestCase_CXMLElement(
			$strXML)
		{ 
			$this->Trace("TestCase: Do some operations on a xml string");
			
			$this->Trace("Test data:");
			$this->Trace($strXML);
			
			$xml1 = new CXMLElement();
			$xml1->Parse($strXML);
			$this->Trace("xml1:");
			$this->Trace($xml1->GetDataArray());
			$strXML1 = $xml1->Render();
			$this->Trace("xml1->Render():");
			$this->Trace($strXML1);

			$xml2 = new CXMLElement();
			$xml2->Parse($strXML1);
			$this->Trace("xml2:");
			$this->Trace($xml2->GetDataArray());
			$strXML2 = $xml2->Render();
			$this->Trace("xml2->Render():");
			$this->Trace($strXML2);
			
			if ($strXML1 != $strXML2)
			{
				$this->Trace("Testcase FAILED!");
				$this->Trace("Render output is inconsistent.");
				$this->SetResult(false);
			}
			else if ($xml1->GetDataArray() != $xml2->GetDataArray())
			{
				$this->Trace("Testcase FAILED!");
				$this->Trace("xml object contents is inconsistent.");
				$this->SetResult(false);				
			}
			else
			{
				$this->Trace("Testcase PASSED!");
			}
			$this->Trace("");
			$this->Trace("");
		}
		
		
		function CallbackTest()
		{
			parent::CallbackTest();
	
	
	
			$strXML = "
<BOGUS/>
";					
			$this->TestCase_CXMLElement($strXML);
			
			$strXML = "
<TEST>DATA</TEST>
";					
			$this->TestCase_CXMLElement($strXML);
			
			
			$strXML = "
<TEST>
	<SUB1>blah</SUB1>
	<SUB2>blubb</SUB2>
</TEST>
";					
			$this->TestCase_CXMLElement($strXML);

			$strXML = "
<html>
	<body>
		<p>Some text</p>
		<div class=\"someclass\">A div</div>
	</body>
</html>
";		

			
			$this->TestCase_CXMLElement($strXML);		
			
			
			$strXML = "Some Text before the xml block.
<html>
	<body>
		<p>Some text</p>
		<div class=\"someclass\">A div</div>
	</body>
</html>
Some text after the xml block.
";		

			
			$this->TestCase_CXMLElement($strXML);	
			
			
			
			$arrayData = array();
			$arrayIndex = array();
			$xml_parser = xml_parser_create(); 
			$bXMLParseResult = xml_parse_into_struct(
				$xml_parser,
				$strXML,
				$arrayData,
				$arrayIndex);
			xml_parser_free($xml_parser);
			
			$this->Trace($arrayData);
			
			
		}
	}
	
	

		
