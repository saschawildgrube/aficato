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
			parent::__construct(
				"Web service ".$this->m_strWebservice.": Parameter Check",
				$arrayConfig);
		}
		
		function OnInit()
		{
			$this->RequireWebservice($this->m_strWebservice);

			$this->m_consumer = new CWebServiceConsumerWebApplication($this);
			
			// Because for unknown reasons THIS script causes a HTTP 500 although the script runs until its final end in the index.php file. WHAT THE FUCK???
			$this->SetActive(false);
			
			$this->SetVerbose(true);
			$this->SetResult(true);
			return parent::OnInit();	
		}	
		 
		
		function TestCase_IncorrectParameters($strTestcaseTitle, $arrayParams)
		{
			$this->Trace('TESTCASE: '.$strTestcaseTitle);
			$this->m_consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace('Error: "'.$this->m_consumer->GetError().'"');		
			if ($this->m_consumer->GetError() == '')	
			{
				$this->Trace('An error was expected. Test case failed!');
				$strNewID = $this->m_consumer->GetResultValue('NEW_ARTICLEVERSION_ID');
				if ($strNewID == "")
				{
					$this->Trace('No error was reported, but this also means that a new id should have been created - but it was not! This is even worse!');
				}
				else
				{
					$this->m_arrayIDs[] = $strNewID;
				}
				$this->SetResult(false);
			}
			if ($this->m_consumer->GetError() == 'FORMAT_INVALID')
			{
				$this->Trace('Format invalid is not expected!');
				$this->SetResult(false);
			}
			$this->Trace('');
		}
		
		function OnTest()
		{
			parent::OnTest();

			
			
			
			// Set a correct parameter array
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["title"] = "This is a test";
			$arrayParams["content"] = "This is test content."; 
			$arrayParams["language"] = "EN"; 
			$arrayParams["country"] = "";
			$arrayParams["status"] = "ACTIVE";
			$arrayParams["user_id"] = "1";  
			$arrayParams["publication_start_datetime"] = "2011-01-01 00:00:00";
			$arrayParams["publication_end_datetime"] = "2012-01-01 00:00:00";
			$arrayParams["command"] = "set";

		
			$arrayParamsDamaged = $arrayParams;
			$arrayParamsDamaged["content"] = StringRepeat("Test!",(ARTICLE_MAXLEN/5) + 1);

			$this->TestCase_IncorrectParameters("Check too long content",$arrayParamsDamaged);

		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
									
			$this->Trace("DELETE ARTICLES");
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
	
	

		
