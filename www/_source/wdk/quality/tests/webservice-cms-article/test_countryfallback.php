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
			parent::__construct("Web service cms/article: Country Fallback",$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->RequireWebservice($this->m_strWebservice);
			$this->SetVerbose(true);
			return parent::CallbackInit();	
		}	
		
		function CallbackTest()
		{
			parent::CallbackTest();

			$consumer = new CWebServiceConsumerWebApplication($this);
			
			
			
			$strName1 = "testtestarticle";
			$strTitle1 = "Test Test Article";		
			$strContent1 = "This is country independend content.";
			$strContent2 = "This is content for Great Britain.";		
			$strContent3 = "This is content for the USA.";		
			$strLanguage1 = "EN";	 
			$strCountry1 = "";	
			$strCountry2 = "GBR";	
			$strCountry3 = "USA";	

			/*
			1. Add article without country specification 
			2. Get country independent article by title
			3. Add article with same title for Great Britain 
			4. Get article for Great Britain by title
			5. Get article for the USA by title (the independent article should be returned)
			6. Delete article by title
			7. Get independent article by title (there should be no result)
			8. Get article for Great Britain by title (there should be no result)
			9. Get article for USA by title (there should be no result)
			*/
			
			

			$this->Trace("1. SET ARTICLE BY TITLE WITHOUT COUNTRY SPECIFICATION");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
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

	
		
		
			$this->Trace("2. GET ARTICLE BY TITLE");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["title"] = $strTitle1;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strStatus = $consumer->GetResultValue("ARTICLE","STATUS");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$strTitle = $consumer->GetResultValue("ARTICLE","TITLE");
			$strContent = $consumer->GetResultValue("ARTICLE","CONTENT");
			$strActiveStart = $consumer->GetResultValue("ARTICLE","PUBLICATION_START_DATETIME");
			$strActiveEnd = $consumer->GetResultValue("ARTICLE","PUBLICATION_END_DATETIME");
			$strLanguage = $consumer->GetResultValue("ARTICLE","LANGUAGE");
			$strCountry = $consumer->GetResultValue("ARTICLE","COUNTRY");
			$this->Trace("NAME = \"$strName\"");
			$this->Trace("TITLE = \"$strTitle\"");
			$this->Trace("CONTENT = \"$strContent\"");
			$this->Trace("STATUS = \"$strStatus\"");
			$this->Trace("PUBLICATION_START_DATETIME = \"$strActiveStart\"");
			$this->Trace("PUBLICATION_END_DATETIME = \"$strActiveEnd\"");
			$this->Trace("LANGUAGE = \"$strLanguage\"");
			$this->Trace("COUNTRY = \"$strCountry\"");
			if ($strStatus != "ACTIVE")
			{
				$this->Trace("Error: article status mismatch");		
				return;
			}
			if ($strName != $strName1)
			{
				$this->Trace("Error: article name mismatch");		
				return;
			}
			if ($strTitle != $strTitle1)
			{
				$this->Trace("Error: article title mismatch");		
				return;
			}
			if ($strContent != $strContent1)
			{
				$this->Trace("Error: article content mismatch");		
				return;
			}
			if ($strActiveStart != "")
			{
				$this->Trace("Error: active start mismatch");		
				return;
			}
			if ($strActiveEnd != "")
			{
				$this->Trace("Error: active end mismatch");		
				return;
			}
			if ($strLanguage != $strLanguage1)
			{
				$this->Trace("Error: article language mismatch");		
				return;
			}
			if ($strCountry != $strCountry1)
			{
				$this->Trace("Error: article country mismatch");		
				return;
			}
			$this->Trace("");
			


			$this->Trace("3. SET ARTICLE BY TITLE FOR GREAT BRITAIN");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["title"] = $strTitle1;
			$arrayParams["content"] = $strContent2;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["country"] = $strCountry2;
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


			
			
				
		
			$this->Trace("4. GET ARTICLE FOR GREAT BRITAIN BY TITLE");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["title"] = $strTitle1;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["country"] = $strCountry2;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strStatus = $consumer->GetResultValue("ARTICLE","STATUS");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$strTitle = $consumer->GetResultValue("ARTICLE","TITLE");
			$strContent = $consumer->GetResultValue("ARTICLE","CONTENT");
			$strActiveStart = $consumer->GetResultValue("ARTICLE","PUBLICATION_START_DATETIME");
			$strActiveEnd = $consumer->GetResultValue("ARTICLE","PUBLICATION_END_DATETIME");
			$strLanguage = $consumer->GetResultValue("ARTICLE","LANGUAGE");
			$strCountry = $consumer->GetResultValue("ARTICLE","COUNTRY");
			$this->Trace("NAME = \"$strName\"");
			$this->Trace("TITLE = \"$strTitle\"");
			$this->Trace("CONTENT = \"$strContent\"");
			$this->Trace("STATUS = \"$strStatus\"");
			$this->Trace("PUBLICATION_START_DATETIME = \"$strActiveStart\"");
			$this->Trace("PUBLICATION_END_DATETIME = \"$strActiveEnd\"");
			$this->Trace("LANGUAGE = \"$strLanguage\"");
			$this->Trace("COUNTRY = \"$strCountry\"");
			if ($strStatus != "ACTIVE")
			{
				$this->Trace("Error: article status mismatch");		
				return;
			}
			if ($strName != $strName1)
			{
				$this->Trace("Error: article name mismatch");		
				return;
			}
			if ($strTitle != $strTitle1)
			{
				$this->Trace("Error: article title mismatch");		
				return;
			}
			if ($strContent != $strContent2)
			{
				$this->Trace("Error: article content mismatch");		
				return;
			}
			if ($strActiveStart != "")
			{
				$this->Trace("Error: active start mismatch");		
				return;
			}
			if ($strActiveEnd != "")
			{
				$this->Trace("Error: active end mismatch");		
				return;
			}
			if ($strLanguage != $strLanguage1)
			{
				$this->Trace("Error: article language mismatch");		
				return;
			}
			if ($strCountry != $strCountry2)
			{
				$this->Trace("Error: article country mismatch");		
				return;
			}
			$this->Trace("");
			
			
			
			$this->Trace("5. GET ARTICLE FOR USA BY TITLE");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["title"] = $strTitle1;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["country"] = $strCountry3;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strStatus = $consumer->GetResultValue("ARTICLE","STATUS");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$strTitle = $consumer->GetResultValue("ARTICLE","TITLE");
			$strContent = $consumer->GetResultValue("ARTICLE","CONTENT");
			$strActiveStart = $consumer->GetResultValue("ARTICLE","PUBLICATION_START_DATETIME");
			$strActiveEnd = $consumer->GetResultValue("ARTICLE","PUBLICATION_END_DATETIME");
			$strLanguage = $consumer->GetResultValue("ARTICLE","LANGUAGE");
			$strCountry = $consumer->GetResultValue("ARTICLE","COUNTRY");
			$this->Trace("NAME = \"$strName\"");
			$this->Trace("TITLE = \"$strTitle\"");
			$this->Trace("CONTENT = \"$strContent\"");
			$this->Trace("STATUS = \"$strStatus\"");
			$this->Trace("PUBLICATION_START_DATETIME = \"$strActiveStart\"");
			$this->Trace("PUBLICATION_END_DATETIME = \"$strActiveEnd\"");
			$this->Trace("LANGUAGE = \"$strLanguage\"");
			$this->Trace("COUNTRY = \"$strCountry\"");
			if ($strStatus != "ACTIVE")
			{
				$this->Trace("Error: article status mismatch");		
				return;
			}
			if ($strName != $strName1)
			{
				$this->Trace("Error: article name mismatch");		
				return;
			}
			if ($strTitle != $strTitle1)
			{
				$this->Trace("Error: article title mismatch");		
				return;
			}
			if ($strContent != $strContent1)
			{
				$this->Trace("Error: article content mismatch");		
				return;
			}
			if ($strActiveStart != "")
			{
				$this->Trace("Error: active start mismatch");		
				return;
			}
			if ($strActiveEnd != "")
			{
				$this->Trace("Error: active end mismatch");		
				return;
			}
			if ($strLanguage != $strLanguage1)
			{
				$this->Trace("Error: article language mismatch");		
				return;
			}
			if ($strCountry != $strCountry1)
			{
				$this->Trace("Error: article country mismatch");		
				return;
			}
			$this->Trace("");
			
			
			
				
			
			$this->Trace("6. DELETE ARTICLE BY TITLE");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["title"] = $strTitle1;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Unexpected error code: \"".$consumer->GetError()."\"");		
				return;	
			}			
			
			
			$this->Trace("7. GET ARTICLE BY TITLE (there should be no result!)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["title"] = $strTitle1;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Unexpected error code: \"".$consumer->GetError()."\"");		
				return;	
			}			

			$this->Trace("8. GET ARTICLE FOR GREAT BRITAIN BY TITLE (there should be no result!)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["title"] = $strTitle1;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["country"] = $strCountry2;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Unexpected error code: \"".$consumer->GetError()."\"");		
				return;	
			}			

			$this->Trace("9. GET ARTICLE FOR USA BY TITLE (there should be no result!)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["title"] = $strTitle1;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["country"] = $strCountry3;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Unexpected error code: \"".$consumer->GetError()."\"");		
				return;	
			}			
		

			$this->SetResult(true);
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			
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
	
	

		
