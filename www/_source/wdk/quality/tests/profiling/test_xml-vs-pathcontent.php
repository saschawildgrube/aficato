<?php
		
	require_once(GetWDKDir()."wdk_pathcontent.inc");
	require_once(GetWDKDir()."wdk_xml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("path/content vs. xml");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}
		
		function TestCase_FormatCompetition(
			$arrayData)
		{ 
			$bTestcase = true;
			$this->Trace("TestCase: Format Competition");
			
			$stopwatch = new CStopWatch();
			$stopwatch->Start();
			$strDataRendered = RenderPathContent($arrayData);
			$stopwatch->Stop();
			$fSecondsRenderPathContent = $stopwatch->GetSeconds();

			$stopwatch = new CStopWatch();
			$stopwatch->Start();
			$arrayDataParsed = ParsePathContent($strDataRendered);
			$stopwatch->Stop();
			$fSecondsParsePathContent = $stopwatch->GetSeconds();
			
			if ($arrayData != $arrayDataParsed)
			{
				$this->Trace("Testcase FAILED because of unexpected result of ParsePathContent()");
				$this->Trace($arrayDataParsed);
				$this->Trace("");
				$this->Trace("Source Data Array:");
				$this->Trace($arrayData);
				$this->Trace("");
				$this->Trace("Rendered Data:");
				$this->Trace($strDataRendered);
				$this->Trace("");
				
				$this->SetResult(false);
				$bTestcase = false;
			}
			


			$xml = new CXMLDocument();
			$xml->SetName("TEST");
			$xml->SetRecursiveArray($arrayData);
			$stopwatch = new CStopWatch();
			$stopwatch->Start();
			$strDataRendered = $xml->RenderDocument();
			$stopwatch->Stop();
			$fSecondsRenderXML = $stopwatch->GetSeconds();

			$xml = new CXMLDocument();
			$xml->SetRecursiveArray($arrayData);
			$stopwatch = new CStopWatch();
			$stopwatch->Start();
			$xml->Parse($strDataRendered);
			$arrayDataParsed = $xml->GetRecursiveArray();
			$stopwatch->Stop();
			$fSecondsParseXML = $stopwatch->GetSeconds();
			
			if ($arrayData != $arrayDataParsed)
			{
				$this->Trace("Testcase FAILED because of unexpected result of XML parsing:");
				$this->Trace($arrayDataParsed);
				$this->Trace("");
				$this->Trace("Source Data Array:");
				$this->Trace($arrayData);
				$this->Trace("");
				$this->Trace("Rendered Data:");
				$this->Trace($strDataRendered);
				$this->Trace("");
				$this->SetResult(false);
				$bTestcase = false;
			}

			
			
			
			
			
			
			$this->Trace("Render_PathContent: ".$fSecondsRenderPathContent);
			$this->Trace("Render_XML________: ".$fSecondsRenderXML);
			$this->Trace("");
			$this->Trace("Parse__PathContent: ".$fSecondsParsePathContent);
			$this->Trace("Parse__XML________: ".$fSecondsParseXML);
			$this->Trace("");
			$this->Trace("Total__PathContent: ".($fSecondsParsePathContent+$fSecondsRenderPathContent));
			$this->Trace("Total__XML________: ".($fSecondsParseXML+$fSecondsRenderXML));
			
			
			
			
			
			if ($bTestcase == true)
			{
				$this->Trace("Testcase PASSED!");
			} 
			$this->Trace("");
			$this->Trace("");
		}
		
		
		function OnTest()
		{
			parent::OnTest();
			

			$arrayData = array();
			$arrayData["LIST"] = array();
			for ($nIndex = 0; $nIndex < 1; $nIndex++)
			{
				array_push($arrayData["LIST"],
					array(
						"INDEX" => $nIndex,
						"TEXT" => "Loram Ipsum Dulum Manul",
						"MIXED" => "Item no. $nIndex"));
			}
			$this->TestCase_FormatCompetition($arrayData);
		
			
			$arrayData = array();
			$arrayData["LIST"] = array();
			for ($nIndex = 0; $nIndex < 10; $nIndex++)
			{
				array_push($arrayData["LIST"],
					array(
						"INDEX" => $nIndex,
						"TEXT" => "Loram Ipsum Dulum Manul",
						"MIXED" => "Item no. $nIndex"));
			}
			$this->TestCase_FormatCompetition($arrayData);
	
			$arrayData = array();
			$arrayData["LIST"] = array();
			for ($nIndex = 0; $nIndex < 1000; $nIndex++)
			{
				array_push($arrayData["LIST"],
					array(
						"INDEX" => $nIndex,
						"TEXT" => "Loram Ipsum Dulum Manul",
						"MIXED" => "Item no. $nIndex"));
			}
			$this->TestCase_FormatCompetition($arrayData);
		
		

		
		}
		

		
	}
	
	

		
