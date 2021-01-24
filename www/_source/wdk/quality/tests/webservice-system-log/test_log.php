<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service system/log",$arrayConfig);
		}
		
		function OnInit()
		{
			parent::OnInit();
			
			$this->m_strWebservice = "system/log";
					
			$this->RequireWebservice("system/log");
					
			$this->SetVerbose(true);
			//$this->SetActive(false);
			
			return true;
		}
		
		function OnTest()
		{
			parent::OnTest();
			$consumer = new CWebServiceConsumerWebApplication($this);




		
			$arrayParams = array();
			$arrayParams["command"] = "log";
			$arrayParams["reporter_id"] = "TEST";
			$arrayParams["event_id"] = "TEST";
			$arrayParams["event_severity"] = "TEST";
			$arrayParams["event_details"] = "This is a test";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetResultValue("STATUS") == "IGNORED") 
			{
				
				$this->Trace("STATUS is IGNORED. This does not indicate that it doesn't work but that the log webservice is not configured to support this test. As a result the test is set to inactive.");
				$this->SetActive(false);
				return;	
			}			
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Unexpected error");
				return;	
			}
			$strNewLogID = $consumer->GetResultValue("NEW_LOG_ID");
			if ($strNewLogID == "")
			{
				$this->Trace("NEW_LOG_ID should not be empty!");
				return;	
			}
			if ($consumer->GetResultValue("STATUS") != "ADDED")
			{
				$this->Trace("STATUS should be ADDED!");
				return;	
			}			
			$arrayData = $consumer->GetResultArray();
			$this->Trace($arrayData);
			
			
			
			
			
			
			
			$arrayParams = array();
			$arrayParams["command"] = "list";
			$arrayParams["offset"] = "0";
			$arrayParams["blocksize"] = "100";
			$arrayParams["filter1"] = "log_id";
			$arrayParams["filter1_value"] = $strNewLogID;
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Unexpected error");
				return;	
			}
			$arrayList = $consumer->GetResultList();
			$this->Trace($arrayList);
			if (ArrayCount($arrayList) != 1)
			{
				$this->Trace("Unexpected list length (should be 1)");
				return;
			}
			if ($arrayList[0]["LOG_ID"] != $strNewLogID)
			{
				$this->Trace("Unexpected result for LOG_ID");
				return;
			}
			if ($arrayList[0]["EVENT_SEVERITY"] != "TEST")
			{
				$this->Trace("Unexpected result for EVENT_SEVERITY");
				return;
			}
			$this->Trace("");			
			
			
			
			
			
			
			
			
			$this->SetResult(true);
		}
		

		
		
	}
	
	

		
