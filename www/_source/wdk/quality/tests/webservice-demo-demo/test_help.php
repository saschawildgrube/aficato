<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();			
			parent::__construct("Test DEMO web service HELP",$arrayConfig);
		}
	
		function OnInit()
		{
			$this->RequireWebservice("demo/demo");
			$this->SetVerbose(true);
			return parent::OnInit();	
		}	
	
		function OnTest()
		{
			parent::OnTest();	
	
			$strWebservice = "demo/demo";
	
	
			$this->Trace("1. INVOKE DEMO WEBSERVICE");
			$arrayParams = array();
			$arrayParams["command"] = "help";
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService($strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
		
			$strHelp = $consumer->GetHelp();
			$this->Trace("Help:");
			$this->Trace($strHelp);
			if ($strHelp == "")
			{
				$this->Trace("Help is empty!");
				return;	
			}
				
					
			$this->SetResult(true);	
	
		}
		
		
	}
	
		
	
	
