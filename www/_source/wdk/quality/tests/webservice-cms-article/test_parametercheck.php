<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");
	require_once(GetWDKDir()."wdk_entity_article.inc");

	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		private $m_arrayIDs;
		private $m_consumer;
		
		function __construct()
		{
			$this->m_strWebservice = "cms/article";
			$this->m_arrayIDs = array();
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service ".$this->m_strWebservice.": Parameter Check",
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->RequireWebservice($this->m_strWebservice);

			$this->m_consumer = new CWebServiceConsumerWebApplication($this);
			
			$this->SetVerbose(true);
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
				$strNewID = $this->m_consumer->GetResultValue("NEW_ARTICLEVERSION_ID");
				if ($strNewID == "")
				{
					$this->Trace("No error was reported, but this also means that a new id should have been created - but it wasn't! This is even worse!");
				}
				else
				{
					$this->m_arrayIDs[] = $strNewID;
				}
				$this->SetResult(false);
			}
			if ($this->m_consumer->GetError() == "FORMAT_INVALID")
			{
				$this->Trace("Format invalid is not expected!");
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
			$arrayParams["title"] = "This is a test";
			$arrayParams["content"] = "This is test content."; 
			$arrayParams["language"] = "EN"; 
			$arrayParams["country"] = "";
			$arrayParams["status"] = "ACTIVE";
			$arrayParams["type"] = "STATIC";
			$arrayParams["author_user_id"] = "1";  
			$arrayParams["publication_start_datetime"] = "2011-01-01 00:00:00";
			$arrayParams["publication_end_datetime"] = "2012-01-01 00:00:00";
			$arrayParams["command"] = "set";

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["language"] = ""; 
			$this->TestCase_IncorrectParameters("Check empty language",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["language"] = "xx";
			$this->TestCase_IncorrectParameters("Check incorrect language",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["title"] = ""; 
			$this->TestCase_IncorrectParameters("Check empty title",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["title"] = StringRepeat("Test!",26);
			$this->TestCase_IncorrectParameters("Check too long title",$arrayParamsDamaged);


			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["status"] = "BOGUS";
			$this->TestCase_IncorrectParameters("Check invalid status",$arrayParamsDamaged);


			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["publication_start_datetime"] = "2011-11-11 55:55:55";
			$this->TestCase_IncorrectParameters("Check incorrect start date time (1)",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["publication_start_datetime"] = "2011-1-1 55:55:55";
			$this->TestCase_IncorrectParameters("Check incorrect start date time (2)",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["publication_end_datetime"] = "2011-11-11 55:55:55";
			$this->TestCase_IncorrectParameters("Check incorrect end date time (1)",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["publication_end_datetime"] = "2011-1-1 55:55:55";
			$this->TestCase_IncorrectParameters("Check incorrect end date time (2)",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["publication_start_datetime"] = "2011-01-01 00:00:00";
			$arrayParamsDamaged["publication_end_datetime"] = "2010-01-01 00:00:00";
			$this->TestCase_IncorrectParameters("Check end date time BEFORE start date time",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["country"] = "US";
			$this->TestCase_IncorrectParameters("Check incorrect country (1)",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["country"] = "Germany";
			$this->TestCase_IncorrectParameters("Check incorrect country (2)",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["country"] = "XXX";
			$this->TestCase_IncorrectParameters("Check incorrect country (3)",$arrayParamsDamaged);

/*
			$arrayParamsDamaged = $arrayParams;
			unset($arrayParamsDamaged["author_user_id"]); 
			$this->TestCase_IncorrectParameters("Check empty userid",$arrayParamsDamaged);
*/

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["author_user_id"] = "admin";
			$this->TestCase_IncorrectParameters("Check incorrect author_user_id",$arrayParamsDamaged);

			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["type"] = "xxx";
			$this->TestCase_IncorrectParameters("Check incorrect type",$arrayParamsDamaged);


		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			
			$this->Trace("DELETE ARTICLES");
			$this->m_consumer = new CWebServiceConsumerWebApplication($this);
			$this->Trace("Going to delete ".ArrayCount($this->m_arrayIDs)." items.");
			foreach ($this->m_arrayIDs as $strID)
			{
				$arrayParams = array();
				$arrayParams["trace"] = "0";
				$arrayParams["articleversion_id"] = $strID;
				$arrayParams["command"] = "delete";
				$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
				$this->Trace("");
			}
			
			return true;
		}
		
		
	}
	
	

		
