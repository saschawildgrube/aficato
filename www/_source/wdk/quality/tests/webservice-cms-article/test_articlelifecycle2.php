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
			parent::__construct("Web service cms/article: Article Lifecylce Extended",$arrayConfig);
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
			
			
			
			$strName1 = "testtestarticle2";
			$strTitle1 = "Test Test Article 2";		
			$strContent1 = "This is more test content.";		
			$strContent2 = "This is NEW test content.";		
			$strLanguage1 = "EN";	 
			$strCountry1 = "";	

			/*
			
			1. set article1 by title (status is active)
			2. get article1 by title (check if there IS a result, as article is active)
			3. set a newer version of article1 (status is active)
			4. get article1 by name (check if the contents matches the newer version)
			5. set the newer version of article1 inactive by id
			6. get article1 by title (check if the contents matches the older version)
			7. delete article1 by name
			8. get old version of article1 by id (check if it does not exist)
			9. get new version of article1 by id (check if it does not exist)
			
			*/
			
			
			$this->Trace("1. SET ARTICLE BY TITLE");
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
			


			$this->Trace("3. SET ARTICLE BY TITLE (NEW VERSION)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
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


			
				
			$this->Trace("4. GET ARTICLE BY TITLE");
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
			$strArticleVersionID = $consumer->GetResultValue("ARTICLE","ARTICLEVERSION_ID");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$strContent = $consumer->GetResultValue("ARTICLE","CONTENT");
			$this->Trace("ARTICLEVERSION_ID = \"$strArticleVersionID\"");
			$this->Trace("NAME = \"$strName\"");
			$this->Trace("CONTENT = \"$strContent\"");
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
			if ($strContent != $strContent2)
			{
				$this->Trace("Error: article content mismatch");
				return;
			}
			$this->Trace("");
			

			
			
			
			$this->Trace("5. SET NEWER ARTICLE VERSION INACTIVE BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID2;
			$arrayParams["status"] = "INACTIVE";
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;
			}
			$this->Trace("");

			
				
			$this->Trace("6. GET ARTICLE BY TITLE");
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
			$strArticleVersionID = $consumer->GetResultValue("ARTICLE","ARTICLEVERSION_ID");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$strContent = $consumer->GetResultValue("ARTICLE","CONTENT");
			$this->Trace("ARTICLEVERSION_ID = \"$strArticleVersionID\"");
			$this->Trace("NAME = \"$strName\"");
			$this->Trace("CONTENT = \"$strContent\"");
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
			if ($strContent != $strContent1)
			{
				$this->Trace("Error: article content mismatch");
				return;
			}
			$this->Trace("");
						
			
			
			$this->Trace("7. DELETE ARTICLE BY TITLE");
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
			
			
			$this->Trace("8. GET OLD ARTICLE VERSION BY ID (there should be no result!)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID1;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Unexpected Error Code: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");		


			$this->Trace("9. GET OLD ARTICLE VERSION BY ID (there should be no result!)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID2;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Unexpected Error Code: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");		
						
			
			
			
			
			/*
			
			$this->Trace("3. GET ARTICLE BY NAME (there should be no result!)");
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
			
			
	
	
			$this->Trace("4. SET ARTICLE BY ID TO ACTIVE");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID;
			$arrayParams["status"] = "ACTIVE";
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;
			}
			$strArticleVersionID_New = $consumer->GetResultValue("NEW_ARTICLEVERSION_ID");
			if ($strArticleVersionID_New != "")
			{
				$this->Trace("Error: NEW_ARTICLEVERSION_ID should not be present.");		
				return;
			}
			$this->Trace("");

	


	
		
			$this->Trace("5. GET ARTICLE BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strStatus = $consumer->GetResultValue("ARTICLE","STATUS");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$this->Trace("NAME = \"$strName\"");
			$this->Trace("STATUS = \"$strStatus\"");
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
			$this->Trace("");		
			
			
			
			
			$this->Trace("6. GET ARTICLE BY NAME (there should be a result!)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["language"] = $strLanguage1;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Unexpected error: \"".$consumer->GetError()."\"");
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
			
			
			
			
			
			$this->Trace("7. SET ARTICLE BY ID TO INACTIVE");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID;
			$arrayParams["status"] = "INACTIVE";
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;
			}
			$strArticleVersionID_New = $consumer->GetResultValue("NEW_ARTICLEVERSION_ID");
			if ($strArticleVersionID_New != "")
			{
				$this->Trace("Error: NEW_ARTICLEVERSION_ID should not be present.");		
				return;
			}
			$this->Trace("");

	
			
		
		
			$this->Trace("8. GET ARTICLE BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID;
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
			if ($strStatus != "INACTIVE")
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
			
			
	
	
	
				
			$this->Trace("9. GET ARTICLE BY NAME (there should be no result!)");
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
			
			
			
			$this->Trace("10. DELETE ARTICLE BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Unexpected error code: \"".$consumer->GetError()."\"");		
				return;	
			}			
			
			
			
			
			$this->Trace("11. GET ARTICLE BY ID (there should be no result!)");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Unexpected Error Code: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");		
			
			
	
	
	
				
			$this->Trace("12. GET ARTICLE BY NAME (there should be no result!)");
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
			
			
			*/
		

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
	
	

		
