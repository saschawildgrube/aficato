<?php

	require_once(GetWDKDir()."wdk_xml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test CXMLElement Parse and Render");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}
		
	
		
		function OnTest()
		{
			parent::OnTest();
	
	
$strXML="<TEST>
	<TAG1>some text</TAG1>
	<small>other text</small>
	<TAG1>
		<SUB1/>
		<SUB2>SUB2</SUB2>
		<SUB3 attrib=\"a3\"/>
	</TAG1>
</TEST>
"; // yes, the render function adds a final newline

			$this->Trace("Reference XML:");
			$this->Trace($strXML);
		
			$xml = new CXMLElement();
			
			$xml->SetName("TEST");

			$xml_sub3 = new CXMLElement();
			$xml_sub3->SetName("SUB3");
			$xml_sub3->SetAttribute("attrib","a3");

			$xml_sub2 = new CXMLElement();
			$xml_sub2->SetName("SUB2");
			$xml_sub2->SetData("SUB2");
			
			$xml_sub1 = new CXMLElement();
			$xml_sub1->SetName("SUB1");
			
			$xml_tag1b = new CXMLElement();
			$xml_tag1b->SetName("TAG1");
			$xml_tag1b->AddChild($xml_sub1);
			$xml_tag1b->AddChild($xml_sub2);
			$xml_tag1b->AddChild($xml_sub3);
			
			$xml_small = new CXMLElement();
			$xml_small->SetName("small");
			$xml_small->SetData("other text");
			
			$xml_tag1a = new CXMLElement();
			$xml_tag1a->SetName("TAG1");
			$xml_tag1a->SetData("some text");
			
			$xml->AddChild($xml_tag1a);
			$xml->AddChild($xml_small);
			$xml->AddChild($xml_tag1b);
			
			
			//$this->Trace($xml->GetDataArray());
			
			$this->Trace("Render Output");
			$strXML2 = $xml->Render(true,"\r\n"); // Render with raw data
			$this->Trace($strXML2);
			
			if ($strXML == $strXML2)
			{
				$this->Trace("PASSED!");	
			}
			else
			{
				$this->Trace("FAILED");
				$this->SetResult(false);
			}
			 
		
		
		
		
		}
		

		
	}
	
	

		
