<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();			
			parent::__construct("Test DEMO web service META",$arrayConfig);
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
	
	
			$this->Trace("1. INVOKE DEMO WEB SERVICE");
			$arrayParams = array();
			$arrayParams["command"] = "help";
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService($strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
		 
			$strMetaVersion = $consumer->GetMetaValue("VERSION");
			$this->Trace("strMetaVersion = \"$strMetaVersion\"");
			if ($strMetaVersion != WDK_VERSION)  
			{
				$this->Trace("Unexpected result!");
				return;	
			}
	
					
			$this->SetResult(true);	
	
		}
		
		
	}
	
		
	
	
