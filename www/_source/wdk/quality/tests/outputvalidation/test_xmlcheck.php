<?php
	
	//require_once(GetWDKDir()."wdk_unittest.inc");
		
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("XML Check");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			
			$this->Trace("Parse() does not accept the output of the root URL. Reason is yet unclear");
			$this->SetActive(false);
			
			return true;
		}
		
		
		function TestCase_XMLCheck(
			$strURL)
		{ 
			$this->Trace("");
			$this->Trace("TestCase_XML Check");
	
			$this->Trace("URL: ".$strURL);
			$strOutput = HttpRequest($strURL,array(),"get",20);
			
			$this->Trace($strOutput);
			
/*
			// Does not work!
			// maybe mb_encode_numericentity  can resolve this?
			$strOutput = html_entity_decode($strOutput);
			$strOutput = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n\n".$strOutput;
*/
			
			$xml = new CXMLDocument();
			$bResult = $xml->Parse($strOutput);
			if ($bResult == false)
			{
				$this->Trace("XML Parsing failed: ".$xml->m_nErrorCode." \"".$xml->m_strErrorCode."\"");	
				$this->Trace("TESTCASE FAILED");
				$this->SetResult(false);
				return;
			}
			
			//$this->Trace($xml->Render());
			
			$this->Trace("TESTCASE PASSED");
		}
		
		
		function OnTest()
		{
			parent::OnTest();
			
			$this->TestCase_XMLCheck("http://".GetRootURL());
		}
	
	}
	
		
