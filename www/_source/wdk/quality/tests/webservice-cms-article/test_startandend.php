<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		private $m_arrayArticleVersionIDs;
		
		function __construct()
		{
			$this->m_strWebservice = "cms/article";
			$this->m_arrayArticleVersionIDs = array();
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service cms/article: Start and end of a publication period",$arrayConfig);
		}
		
		function OnInit()
		{
			$this->RequireWebservice($this->m_strWebservice);
			$this->SetVerbose(true);
			return parent::OnInit();	
		}	
		
		function OnTest()
		{
			parent::OnTest();

			$consumer = new CWebServiceConsumerWebApplication($this);
			
			
			
			$strName1 = "testtestarticle4";
			$strTitle1 = "Test Test Article 4";		
			$strContent1 = "This is timed test content.";		
			$strContent2 = "This is updated timed test content.";
			$strLanguage1 = "EN";	 
			$strCountry1 = "";	

			/*
			
			1. Set article version 1 active
 			2. Set article version 2 active
 			3. Get article by name (and get version 2)
 			4. Set article version 2 start time after current time
 			5. Get article by name (and get version 1)
 			6. Set article version 1 end time before current time
 			7. Get article by name (and get nothing)
 			8. Set article version 1 end time after current time
 			9. Get article by name (and get version 1)
			10. Set article version 2 start time before, and end time after current time
			11. Get article by name (and get version 2)
			12. Delete by name

			*/
			
			
			$this->Trace("1. SET ARTICLE VERSION 1 BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["title"] = $strTitle1;
			$arrayParams["content"] = $strContent1;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["status"] = "ACTIVE";
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;
			}
			$strArticleVersionID1 = $consumer->GetResultValue("NEW_ARTICLEVERSION_ID");
			if ($strArticleVersionID1 == "")
			{
				$this->Trace("Error: Article Version ID is empty");		
				return;
			}
			$this->m_arrayArticleVersionIDs[] = $strArticleVersionID1;
			$this->Trace("New article version ID: ".$strArticleVersionID1);
			$this->Trace("");

	
			$this->Trace("1B. GET ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1; 
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strArticleVersionID = $consumer->GetResultValue("ARTICLE","ARTICLEVERSION_ID");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$this->Trace("ARTICLEVERSION_ID = \"$strArticleVersionID\"");
			$this->Trace("NAME = \"$strName\"");
			if ($strArticleVersionID != $strArticleVersionID1)
			{
				$this->Trace("Error: article version id mismatch");		
				return;
			}		
			if ($strName != $strName1)
			{
				$this->Trace("Error: article name mismatch");		
				return;
			}
			$this->Trace("");
		
		
			$this->Trace("2. SET ARTICLE VERSION 2 BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["title"] = $strTitle1;
			$arrayParams["content"] = $strContent2;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["status"] = "ACTIVE";
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;
			}
			$strArticleVersionID2 = $consumer->GetResultValue("NEW_ARTICLEVERSION_ID");
			if ($strArticleVersionID2 == "")
			{
				$this->Trace("Error: Article Version ID is empty");		
				return;
			}
			$this->m_arrayArticleVersionIDs[] = $strArticleVersionID2;
			$this->Trace("New article version ID: ".$strArticleVersionID2);
			$this->Trace("");
		
		
		
			$this->Trace("3. GET ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1; 
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strArticleVersionID = $consumer->GetResultValue("ARTICLE","ARTICLEVERSION_ID");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$this->Trace("ARTICLEVERSION_ID = \"$strArticleVersionID\"");
			$this->Trace("NAME = \"$strName\"");
			if ($strArticleVersionID != $strArticleVersionID2)
			{
				$this->Trace("Error: article version id mismatch");		
				return;
			}		
			if ($strName != $strName1)
			{
				$this->Trace("Error: article name mismatch");		
				return;
			}
			$this->Trace("");
			
			
			$this->Trace("4. SET ARTICLE VERSION 2 BY ID WITH START TIME AFTER CURRENT TIME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID2;
			$arrayParams["publication_start_datetime"] = RenderDateTime(GetTimeAddDays(GetTimeNow(),1));
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;
			}
			$this->Trace("");
			
			
			
			
			$this->Trace("5. GET ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strArticleVersionID = $consumer->GetResultValue("ARTICLE","ARTICLEVERSION_ID");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$this->Trace("ARTICLEVERSION_ID = \"$strArticleVersionID\"");
			$this->Trace("NAME = \"$strName\"");
			if ($strArticleVersionID != $strArticleVersionID1)
			{
				$this->Trace("Error: article version id mismatch");		
				return;
			}		
			if ($strName != $strName1)
			{
				$this->Trace("Error: article name mismatch");		
				return;
			}
			$this->Trace("");



			$this->Trace("6. SET ARTICLE VERSION 1 BY ID WITH END TIME BEFORE CURRENT TIME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID1;  
			$arrayParams["publication_end_datetime"] = RenderDateTime(GetTimeAddDays(GetTimeNow(),-1));
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;
			}
			$this->Trace("");


			$this->Trace("7. GET ARTICLE BY NAME (there should be no result!)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Unexpected error code: \"".$consumer->GetError()."\"");		
				return;	
			}		



			$this->Trace("8. SET ARTICLE VERSION 1 BY ID WITH END TIME AFTER CURRENT TIME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID1;
			$arrayParams["publication_end_datetime"] = RenderDateTime(GetTimeAddDays(GetTimeNow(),1));
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;
			}
			$this->Trace("");


		
			$this->Trace("9. GET ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strArticleVersionID = $consumer->GetResultValue("ARTICLE","ARTICLEVERSION_ID");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$this->Trace("ARTICLEVERSION_ID = \"$strArticleVersionID\"");
			$this->Trace("NAME = \"$strName\"");
			if ($strArticleVersionID != $strArticleVersionID1)
			{
				$this->Trace("Error: article version id mismatch");		
				return;
			}		
			if ($strName != $strName1)
			{
				$this->Trace("Error: article name mismatch");		
				return;
			}
			$this->Trace("");


			$this->Trace("10. SET ARTICLE VERSION 2 BY ID WITH START TIME BEFORE AND END TIME AFTER CURRENT TIME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID2;
			$arrayParams["publication_start_datetime"] = RenderDateTime(GetTimeAddDays(GetTimeNow(),-1));
			$arrayParams["publication_end_datetime"] = RenderDateTime(GetTimeAddDays(GetTimeNow(),1));
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;
			}
			$this->Trace("");


			$this->Trace("11. GET ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strArticleVersionID = $consumer->GetResultValue("ARTICLE","ARTICLEVERSION_ID");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$this->Trace("ARTICLEVERSION_ID = \"$strArticleVersionID\"");
			$this->Trace("NAME = \"$strName\"");
			if ($strArticleVersionID != $strArticleVersionID2)
			{
				$this->Trace("Error: article version id mismatch");		
				return;
			}		
			if ($strName != $strName1)
			{
				$this->Trace("Error: article name mismatch");		
				return;
			}
			$this->Trace("");

			$this->Trace("12. DELETE ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Unexpected error code: \"".$consumer->GetError()."\"");		
				return;	
			}		

			$this->SetResult(true);
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			

			$this->Trace("DELETE ARTICLES");
			$consumer = new CWebServiceConsumerWebApplication($this);
			$this->Trace("Going to delete ".ArrayCount($this->m_arrayArticleVersionIDs)." article versions.");
			foreach ($this->m_arrayArticleVersionIDs as $strArticleVersionID)
			{
				$arrayParams = array();
				$arrayParams["trace"] = "0";
				$arrayParams["articleversion_id"] = $strArticleVersionID;
				$arrayParams["command"] = "delete";
				$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
				$this->Trace("");
			}
			
			return true;
		}
		
		
	}
	
	

		
