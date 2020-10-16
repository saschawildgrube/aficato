<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");
	require_once(GetWDKDir()."wdk_entity_job.inc");

	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		private $m_arrayIDs;
		private $m_consumer;
		
		function __construct()
		{
			$this->m_strWebservice = "system/scheduler";
			$this->m_arrayIDs = array();
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct(
				"Web service ".$this->m_strWebservice.": Parameter Check",
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->RequireWebservice($this->m_strWebservice);

			$this->m_consumer = new CWebServiceConsumerWebApplication($this);
			
			$this->SetVerbose(false);
			$this->SetResult(true);
			return parent::CallbackInit();	
		}	
		 
		
		function TestCase_IncorrectParameters($strTestcaseTitle, $arrayParams)
		{
			$this->Trace("TESTCASE: ".$strTestcaseTitle);
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace("Error: \"".$this->m_consumer->GetError()."\"");		
			if ($this->m_consumer->GetError() == "")	
			{
				$this->Trace("An error was expected. Test case failed!");
				$strNewID = $this->m_consumer->GetResultValue("NEW_JOB_ID");
				if ($strNewID == "")
				{
					$this->Trace("No error was reported but no new id has been provided! This is even worse!");
				}
				else
				{
					$this->m_arrayIDs[] = $strNewID;
				}
				$this->SetResult(false);
			}
			if ($this->m_consumer->GetError() != "PARAMETER_CHECK_FAILED")	
			{
				$this->Trace("PARAMETER_CHECK_FAILED was expected. Test case failed!");
				$this->SetResult(false);
			}
			$this->Trace("");
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();

			
			
			
			// Set a correct Parameter array
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["job_name"] = "Test Job";
			$arrayParams["job_active"] = "1"; 
			$arrayParams["job_url"] = "http://www.example.com"; 
			$arrayParams["schedule_minute"] = "*";
			$arrayParams["schedule_hour"] = "*";
			$arrayParams["schedule_dayofweek"] = "*";
			$arrayParams["schedule_month"] = "*";
			$arrayParams["schedule_dayofmonth"] = "*";
			$arrayParams["timeout_seconds"] = "30";
			$arrayParams["command"] = "add";

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["job_name"] = ""; 
			$this->TestCase_IncorrectParameters("Check empty job name",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["job_url"] = ""; 
			$this->TestCase_IncorrectParameters("Check empty job url",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["schedule_minute"] = "";
			$this->TestCase_IncorrectParameters("Check empty schedule_minute",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["schedule_minute"] = "abc";
			$this->TestCase_IncorrectParameters("Check incorrect schedule_minute",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["schedule_hour"] = "";
			$this->TestCase_IncorrectParameters("Check empty schedule_hour",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["timeout_seconds"] = "181";
			$this->TestCase_IncorrectParameters("Check incorrect timeout_seconds",$arrayParamsDamaged);

/*
			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["timeout_seconds"] = "179";
			$this->TestCase_IncorrectParameters("Check incorrect timeout_seconds",$arrayParamsDamaged);
*/			

		
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			
			$this->Trace("DELETE JOBS");
			$this->m_consumer = new CWebServiceConsumerWebApplication($this);
			$this->Trace("Going to delete ".ArrayCount($this->m_arrayIDs)." items.");
			foreach ($this->m_arrayIDs as $strID)
			{
				$arrayParams = array();
				$arrayParams["trace"] = "0";
				$arrayParams["job_id"] = $strID;
				$arrayParams["command"] = "delete";
				$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
				$this->Trace("");
			}
			
			return true;
		}
		
		
	}
	
	

		
