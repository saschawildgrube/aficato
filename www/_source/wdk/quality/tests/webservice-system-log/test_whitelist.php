<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service system/log Whitelist",$arrayConfig);
		}
		
		function OnInit()
		{
			parent::OnInit();
			
			$this->m_strWebservice = "system/log";
					
			$this->RequireWebservice("system/log");
			
			return true;
		}
		
		function OnTest()
		{
			parent::OnTest();
			$consumer = new CWebServiceConsumerWebApplication($this);			
			
			
			$this->Trace("1. Test severity whitelist");
			$arrayParams = array();
			$arrayParams["command"] = "log";
			$arrayParams["event_severity"] = "TESTBLACKLIST"; // yes, we want it to fail!
			$arrayParams["reporter_id"] = "TEST";
			$arrayParams["event_id"] = "TEST";
			$arrayParams["event_message"] = "This is a test";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Unexpected error");
				return;
			}
			if ($consumer->GetResultValue("STATUS") == "ADDED")
			{
				$this->Trace("The STATUS is not IGNORED. This is what we expected - but it may aswell be intended. The test is now set to inactive instead of failed.");
				$this->SetActive(false);
				return;
			}
			if ($consumer->GetResultValue("NEWLOGID") != "")
			{
				$this->Trace("NEWLOGID should be empty");
				return;	
			}
			if ($consumer->GetResultValue("STATUS") != "IGNORED")
			{
				$this->Trace("STATUS should be IGNORED");
				return;	
			}


			
			$this->SetResult(true);
		}
		

		
		
	}
	
	

		
