<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();			
			parent::__construct("Test DEMO web service",$arrayConfig);
		}
	
		function OnInit()
		{
			$this->RequireWebservice("demo/demo");
			$this->SetVerbose(false);
			return parent::OnInit();	
		}
	
		function OnTest()
		{
			parent::OnTest();	
	
			$strWebservice = "demo/demo";
	
	
			$this->Trace("1. INVOKE DEMO WEBSERVICE:");
			$arrayParams = array();
			$arrayParams["accesscode"] = $this->GetWebserviceAccessCode($strWebservice);	
			$arrayParams["command"] = "demo";
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService($strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
		
			$strTest1 = $consumer->GetResultValue("TEST1");
			$this->Trace("Test1 = \"$strTest1\"");
			if ($strTest1 != "Test")
			{
				$this->Trace("Test1 failed!");
				return;	
			}
				
			$strTest2 = $consumer->GetResultValueInsecure("TEST2");
			$this->Trace("Test2 = \"$strTest2\"");
			if ($strTest2 != "Text is encapsulated like this \"<![CDATA[This is a test]]>\".")
			{
				$this->Trace("Test2 failed!");
				return;	
			}
		
			$strTest3 = $consumer->GetResultValueInsecure("TEST3");
			$this->Trace("Test3 = \"$strTest3\"");
			if ($strTest3 != u("ÄÜÖäüö"))
			{
				$this->Trace("Test3 failed!");
				return;	
			}

			$strTest3 = $consumer->GetResultValue("TEST3");
			$this->Trace("Test3b (Secure) = \"$strTest3\"");
			//if ($strTest3 != "&Auml;&Uuml;&Ouml;&auml;&uuml;&ouml;")
			if ($strTest3 != "&Auml;&Uuml;&Ouml;&auml;&uuml;&ouml;")
			{
				$this->Trace("Test3b failed!");
				return;	
			}

		
		
			$strTest4 = $consumer->GetResultValueInsecure("TEST4");
			$this->Trace("Test4 = \"$strTest4\"");
			if ($strTest4 != "<>/&\"\'")
			{
				$this->Trace("Test4 failed!");
				return;	
			}

			$strTest6 = $consumer->GetResultValueInsecure("TEST6");
			$this->Trace("Test6 = \"$strTest6\"");
			if ($strTest6 != "uppercase?")
			{
				$this->Trace("Test6 failed!");
				return;	
			}		
					
			$this->SetResult(true);	
	
		}
		
		
	}
	
		
	
	
