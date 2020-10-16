<?php
		
	require_once(GetWDKDir()."wdk_xml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("CXMLElement::RecursiveArray");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);	
			return true;
		}
		
		function TestCase_CXMLElement_RecursiveArray(
			$arrayData)
		{ 
			$this->Trace("TestCase: Set and Get Recursive Array");
			
			$this->Trace("Source Data");
			$this->Trace($arrayData);
	
			$element = new CXMLElement();
			$element->SetName("TEST");
			$element->SetRecursiveArray($arrayData);
			$this->Trace($element->GetDataArray());
			$this->Trace($element->Render());
			$arrayData2 = $element->GetRecursiveArray();
			$this->Trace("Result Data");
			$this->Trace($arrayData2);
		
			if ($arrayData == $arrayData2)
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
			
					
		
			$arrayData = array();
			$this->TestCase_CXMLElement_RecursiveArray($arrayData);
			
			$arrayData = array();
			for ($nIndex = 0; $nIndex < 5; $nIndex++)
			{
				array_push($arrayData,$nIndex);
			}
			$this->TestCase_CXMLElement_RecursiveArray($arrayData);
		
			$arrayData = array();
			$arrayData["apple"] = "red";
			$arrayData["banana"] = "yellow";
			$arrayData["salad"] = "green";
			$arrayData["orange"] = "orange";
			$this->TestCase_CXMLElement_RecursiveArray($arrayData);
		
		
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
			$this->TestCase_CXMLElement_RecursiveArray($arrayData);
		
				
			$arrayData = array(); 
			$arrayData["ELEMENT1"] = "Hydrogen";
			$arrayData["ELEMENT2"] = "Oxygen";
			$arrayData["MOLECULE"] = "Water";
			$this->TestCase_CXMLElement_RecursiveArray($arrayData);		
			
		}
		

		
	}
	
	

		
