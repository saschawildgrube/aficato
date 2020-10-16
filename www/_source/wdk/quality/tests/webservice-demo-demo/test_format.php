<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");
	
	class CTest extends CUnitTest
	{
		function __construct() 
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();			
			parent::__construct("Test DEMO web service FORMAT",$arrayConfig);
		}
	
		function CallbackInit()
		{
			$this->RequireWebservice("demo/demo");
			$this->SetVerbose(false);
			$this->SetResult(true);
			return parent::CallbackInit();	
		}
	
		function TestCase_WebserviceFormat($strFormat)
		{
			$strWebservice = "demo/demo";

			$this->Trace("");
			$this->Trace("");
			$this->Trace("TEST CASE: $strFormat - Success TEST1");
			$arrayParams = array();
			$arrayParams["format"] = $strFormat;
			$arrayParams["command"] = "demo";
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService($strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				$this->Trace("TEST CASE: FAILED");
				$this->SetResult(false);
			}
			$strTest1 = $consumer->GetResultValue("TEST1");
			$this->Trace("Test1 = \"$strTest1\"");
			if ($strTest1 != "Test")
			{
				$this->Trace("TEST CASE: FAILED");
				$this->SetResult(false);
			}
			else
			{
				$this->Trace("TEST CASE: PASSED");
			}



			$this->Trace("");
			$this->Trace("");
			$this->Trace("TEST CASE: $strFormat - Success LIST");
			$arrayParams = array();
			$arrayParams["format"] = $strFormat;
			$arrayParams["command"] = "demo";
			if ($strFormat == "csv")
			{
				$arrayParams["csvpath"] = "LIST";	
			}
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService($strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				$this->Trace("TEST CASE: FAILED");
				$this->SetResult(false);
			}
			$arrayList = $consumer->GetResultList();
			$this->Trace($arrayList);
			if (!is_array($arrayList))
			{
				$this->Trace("consumer->GetResultList() did not return an array()!");
				$this->Trace("TEST CASE: FAILED");
				$this->SetResult(false);
			}
			else if (ArrayCount($arrayList) != 3)
			{
				$this->Trace("Resulting list does not have 3 elements!");
				$this->Trace("TEST CASE: FAILED");
				$this->SetResult(false);
			}
			else if (ArrayGetValue($arrayList,1,"TAG") != "Tag1")
			{
				$this->Trace("Expected text not found in list!");
				$this->Trace("TEST CASE: FAILED");
				$this->SetResult(false);
			}
			else
			{
				$this->Trace("TEST CASE: PASSED");
			}



			$this->Trace('');
			$this->Trace('');
			$this->Trace("TEST CASE: $strFormat - Error");
			$arrayParams = array();
			$arrayParams["format"] = $strFormat;
			$arrayParams["command"] = "demo-error";
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService($strWebservice,$arrayParams);
			$this->Trace("Error: \"".$consumer->GetError()."\"");					
			if ($consumer->GetError() != 'COMMAND_UNKNOWN')	
			{
				$this->Trace("TEST CASE: FAILED");
				$this->SetResult(false);				
			}
			else
			{
				$this->Trace("TEST CASE: PASSED");
			}

			
		}
	
	
		function CallbackTest()
		{
			parent::CallbackTest();	
			
			$this->TestCase_WebserviceFormat("xml");
			$this->TestCase_WebserviceFormat("json");
			$this->TestCase_WebserviceFormat("csvpath");
			$this->TestCase_WebserviceFormat("csv");
	
		}
		
		
	}
	
		
	
	
