<?php
		
	require_once(GetWDKDir()."wdk_pathcontent.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("RenderPathContent() and ParsePathContent()");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}
		
		function TestCase_RenderAndParsePathContent(
			$arrayData,
			$strExpectedData)
		{ 
			$bTestcase = true;
			$this->Trace("TestCase: RenderPathContent and ParsePathContent");
			
			$strExpectedData = ReplaceString($strExpectedData,"\r","");
			
			$this->Trace("Source Data Array:");
			$this->Trace($arrayData);
			$this->Trace("");
			
			$this->Trace("Expected Data:");
			$this->Trace($strExpectedData);
			$this->Trace("");
			
			$strData = RenderPathContent($arrayData);
			
			$this->Trace("Rendered path/content data:");
			$this->Trace($strData);

			if ($strData != $strExpectedData)
			{
				$this->Trace("Testcase FAILED because of unexpected result of RenderPathContent()");
				$this->SetResult(false);
				$bTestcase = false;
			}
			
			$arrayData2 = ParsePathContent($strData);
			
			$this->Trace("Result Data");
			$this->Trace($arrayData2);
		
			if ($arrayData != $arrayData2)
			{
				$this->Trace("Testcase FAILED because of unexpected result of ParsePathContent()");
				$this->SetResult(false);
				$bTestcase = false;
			}
			
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
			$strData = "";
			$this->TestCase_RenderAndParsePathContent($arrayData,$strData);
			
			$arrayData = array();
			for ($nIndex = 0; $nIndex < 5; $nIndex++)
			{
				array_push($arrayData,$nIndex);
			}
			$strData =
"#0,0
#1,1
#2,2
#3,3
#4,4";
			$this->TestCase_RenderAndParsePathContent($arrayData,$strData);
		
		
		
			$arrayData = array();
			for ($nIndex = 0; $nIndex < 2; $nIndex++)
			{
				array_push($arrayData,array(
					"tag1" => $nIndex,
					"tag2" => "prefix".$nIndex,
					"tag3" => $nIndex."postfix"));
			}
			$strData =
"#0/tag1,0
#0/tag2,prefix0
#0/tag3,0postfix
#1/tag1,1
#1/tag2,prefix1
#1/tag3,1postfix";
			$this->TestCase_RenderAndParsePathContent($arrayData,$strData);
		
		
		
			$arrayData = array();
			$arrayData["apple"] = "red";
			$arrayData["banana"] = "yellow";
			$arrayData["salad"] = "green";
			$arrayData["orange"] = "orange";
			$strData =
"apple,red
banana,yellow
salad,green
orange,orange";
			$this->TestCase_RenderAndParsePathContent($arrayData,$strData);
		
		
			$arrayData = array();
			$arrayData["key1"] = "value1";
			$arrayData["key2"] = "value2";
			$arrayData["key3"] = array("sub1","sub2","sub3");
			$arrayData["key4"] = "value4";
			$arrayData["key5"] = array(
				"subkey1" => array("subsubkey1" => "subsubvalue1","subsubkey2" => "subsubvalue2"),
				"subkey2" => "subvalue2",
				"subkey3" => "subvalue3");
			$arrayData["key6"] = "value6";
			$strData =
"key1,value1
key2,value2
key3/#0,sub1
key3/#1,sub2
key3/#2,sub3
key4,value4
key5/subkey1/subsubkey1,subsubvalue1
key5/subkey1/subsubkey2,subsubvalue2
key5/subkey2,subvalue2
key5/subkey3,subvalue3
key6,value6";
			$this->TestCase_RenderAndParsePathContent($arrayData,$strData);
		
				
			$arrayData = array(); 
			$arrayData["ELEMENT1"] = "Hydrogen";
			$arrayData["ELEMENT2"] = "Oxygen";
			$arrayData["MOLECULE"] = "Water";
			$strData =
"ELEMENT1,Hydrogen
ELEMENT2,Oxygen
MOLECULE,Water";
			$this->TestCase_RenderAndParsePathContent($arrayData,$strData);
			
 
			$arrayData = array();
			$arrayData["umlauts"] = u("ÄÖÜ");
			$arrayData["punctuation"] = ",.;\"";
			$arrayData["multiline"] = "Some Text\nIn multiple lines...";
			$arrayData["slashes/in/keyname"] = "The key name and the value contains / slashes!";
			$arrayData["!0"] = "Index syntax in key name";
			$strData =
"umlauts,%C3%84%C3%96%C3%9C
punctuation,%2C.%3B%22
multiline,Some+Text%0AIn+multiple+lines...
slashes%2Fin%2Fkeyname,The+key+name+and+the+value+contains+%2F+slashes%21
%210,Index+syntax+in+key+name";
			$this->TestCase_RenderAndParsePathContent($arrayData,$strData);

		
		}
		

		
	}
	
	

		
