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
			parent::__construct("Web service cms/article: article translation",$arrayConfig);
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

			/*
			1. Add article in English
			2. Get article in English
			3. Add translated German version
			4. Get translated German version (and check if the version is marked as up-to-date)
			5. Set new article version in English
			6. Get new article version in English
			7. Get translated German version (and check if the version is marked as outdated)
			8. Delete article and all it's versions in one go
			9. Get original article in English (and check if it was deleted)
			10. Get translated article in German (and check if it was deleted)
			11. Get new article in English (and check if it was deleted)
			*/

			$consumer = new CWebServiceConsumerWebApplication($this);
		
			$strName1 = "testtestarticletranslation";
			$strTitle1en = "A Test Article to be Translated";		
			$strTitle1de = u("Ein Übersetzter Test-Artikel");
			$strContent1en = "This is test content that is going to be translated.";		
			$strContent1de = u("Dies ist Test-Text, der übersetzt werden wird.");
			$strContent2en = "This is updated test content that is NOT going to be translated.";		

	
		
			$this->Trace("1. SET ENGLISH ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["title"] = $strTitle1en;
			$arrayParams["content"] = $strContent1en;
			$arrayParams["language"] = "EN";
			$arrayParams["status"] = "ACTIVE";
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;
			}
			$strArticleVersionID1en = $consumer->GetResultValue("NEW_ARTICLEVERSION_ID");
			if ($strArticleVersionID1en == "")
			{
				$this->Trace("Error: Article Version ID is empty");		
				return;
			}
			$this->m_arrayArticleVersionIDs[] = $strArticleVersionID1en;
			$this->Trace("New article version ID: ".$strArticleVersionID1en);
			$this->Trace("");

	
		
		
			$this->Trace("2. GET ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["language"] = "EN";
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strArticleVersionID = $consumer->GetResultValue("ARTICLE","ARTICLEVERSION_ID");
			$strStatus = $consumer->GetResultValue("ARTICLE","STATUS");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$strTitle = $consumer->GetResultValueInsecure("ARTICLE","TITLE");
			$strContent = $consumer->GetResultValueInsecure("ARTICLE","CONTENT");
			$strLanguage = $consumer->GetResultValue("ARTICLE","LANGUAGE");
			$strCountry = $consumer->GetResultValue("ARTICLE","COUNTRY");
			$this->Trace("ARTICLEVERSION_ID = \"$strArticleVersionID\"");
			$this->Trace("NAME = \"$strName\"");
			$this->Trace("TITLE = \"$strTitle\"");
			$this->Trace("CONTENT = \"$strContent\"");
			$this->Trace("STATUS = \"$strStatus\"");
			$this->Trace("LANGUAGE = \"$strLanguage\"");
			$this->Trace("COUNTRY = \"$strCountry\"");
			if ($strArticleVersionID != $strArticleVersionID1en)
			{
				$this->Trace("Error: article version id mismatch");		
				return;
			}
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
			if ($strTitle != $strTitle1en)
			{
				$this->Trace("Error: article title mismatch");		
				return;
			}
			if ($strContent != $strContent1en)
			{
				$this->Trace("Error: article content mismatch");		
				return;
			}
			if ($strLanguage != "EN")
			{
				$this->Trace("Error: article language mismatch");		
				return;
			}
			$this->Trace("");
			
			
			
			$this->Trace("3. SET TRANSLATED ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["title"] = $strTitle1de;
			$arrayParams["name"] = $strName1;
			$arrayParams["content"] = $strContent1de;
			$arrayParams["language"] = "DE";
			$arrayParams["reference_articleversion_id"] = $strArticleVersionID1en;
			$arrayParams["status"] = "ACTIVE";
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;
			}
			$strArticleVersionID1de = $consumer->GetResultValue("NEW_ARTICLEVERSION_ID");
			if ($strArticleVersionID1de == "")
			{
				$this->Trace("Error: Article Version ID is empty");		
				return;
			}
			$this->m_arrayArticleVersionIDs[] = $strArticleVersionID1de;
			$this->Trace("New article version ID: ".$strArticleVersionID1de);
			$this->Trace("");

	
		
		
			$this->Trace("4. GET TRANSLATED ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["language"] = "DE";
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strArticleVersionID = $consumer->GetResultValue("ARTICLE","ARTICLEVERSION_ID");
			$strStatus = $consumer->GetResultValue("ARTICLE","STATUS");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$strTitle = $consumer->GetResultValueInsecure("ARTICLE","TITLE");
			$strContent = $consumer->GetResultValueInsecure("ARTICLE","CONTENT");
			$strLanguage = $consumer->GetResultValue("ARTICLE","LANGUAGE");
			$strCountry = $consumer->GetResultValue("ARTICLE","COUNTRY");
			$this->Trace("ARTICLEVERSION_ID = \"$strArticleVersionID\"");
			$this->Trace("NAME = \"$strName\"");
			$this->Trace("TITLE = \"$strTitle\"");
			$this->Trace("CONTENT = \"$strContent\"");
			$this->Trace("STATUS = \"$strStatus\"");
			$this->Trace("LANGUAGE = \"$strLanguage\"");
			$this->Trace("COUNTRY = \"$strCountry\"");
			if ($strArticleVersionID != $strArticleVersionID1de)
			{
				$this->Trace("Error: article version id mismatch");		
				return;
			}
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
			if ($strTitle != $strTitle1de)
			{
				$this->Trace("Error: article title mismatch: should be \"".$strTitle1de."\"");		
				return;
			}
			if ($strContent != $strContent1de)
			{
				$this->Trace("Error: article content mismatch");		
				return;
			}
			if ($strLanguage != "DE")
			{
				$this->Trace("Error: article language mismatch");		
				return;
			}
			$this->Trace("");
			
			
			
			
			$this->Trace("5. SET UPDATED ENGLISH ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["title"] = $strTitle1en;
			$arrayParams["content"] = $strContent2en;
			$arrayParams["language"] = "EN";
			$arrayParams["status"] = "ACTIVE";
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;
			}
			$strArticleVersionID2en = $consumer->GetResultValue("NEW_ARTICLEVERSION_ID");
			if ($strArticleVersionID2en == "")
			{
				$this->Trace("Error: Article Version ID is empty");		
				return;
			}
			$this->m_arrayArticleVersionIDs[] = $strArticleVersionID2en;
			$this->Trace("New article version ID: ".$strArticleVersionID2en);
			$this->Trace("");
			
			
			

			$this->Trace("6. GET UPDATED ENGLISH ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["language"] = "EN";
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strArticleVersionID = $consumer->GetResultValue("ARTICLE","ARTICLEVERSION_ID");
			$strStatus = $consumer->GetResultValue("ARTICLE","STATUS");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$strTitle = $consumer->GetResultValueInsecure("ARTICLE","TITLE");
			$strContent = $consumer->GetResultValueInsecure("ARTICLE","CONTENT");
			$strLanguage = $consumer->GetResultValue("ARTICLE","LANGUAGE");
			$strCountry = $consumer->GetResultValue("ARTICLE","COUNTRY");
			$this->Trace("ARTICLEVERSION_ID = \"$strArticleVersionID\"");
			$this->Trace("NAME = \"$strName\"");
			$this->Trace("TITLE = \"$strTitle\"");
			$this->Trace("CONTENT = \"$strContent\"");
			$this->Trace("STATUS = \"$strStatus\"");
			$this->Trace("LANGUAGE = \"$strLanguage\"");
			$this->Trace("COUNTRY = \"$strCountry\"");
			if ($strArticleVersionID != $strArticleVersionID2en)
			{
				$this->Trace("Error: article version id mismatch");		
				return;
			}
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
			if ($strTitle != $strTitle1en)
			{
				$this->Trace("Error: article title mismatch");		
				return;
			}
			if ($strContent != $strContent2en)
			{
				$this->Trace("Error: article content mismatch");		
				return;
			}
			if ($strLanguage != "EN")
			{
				$this->Trace("Error: article language mismatch");		
				return;
			}
			$this->Trace("");			



			$this->Trace("7. GET TRANSLATED ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["language"] = "DE";
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strArticleVersionID = $consumer->GetResultValue("ARTICLE","ARTICLEVERSION_ID");
			$strStatus = $consumer->GetResultValue("ARTICLE","STATUS");
			$strName = $consumer->GetResultValue("ARTICLE","NAME");
			$strTitle = $consumer->GetResultValueInsecure("ARTICLE","TITLE");
			$strContent = $consumer->GetResultValueInsecure("ARTICLE","CONTENT");
			$strLanguage = $consumer->GetResultValue("ARTICLE","LANGUAGE");
			$strCountry = $consumer->GetResultValue("ARTICLE","COUNTRY");
			$this->Trace("ARTICLEVERSION_ID = \"$strArticleVersionID\"");
			$this->Trace("NAME = \"$strName\"");
			$this->Trace("TITLE = \"$strTitle\"");
			$this->Trace("CONTENT = \"$strContent\"");
			$this->Trace("STATUS = \"$strStatus\"");
			$this->Trace("LANGUAGE = \"$strLanguage\"");
			$this->Trace("COUNTRY = \"$strCountry\"");
			if ($strArticleVersionID != $strArticleVersionID1de)
			{
				$this->Trace("Error: article version id mismatch");		
				return;
			}
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
			if ($strTitle != $strTitle1de)
			{
				$this->Trace("Error: article title mismatch: should be \"".$strTitle1de."\"");		
				return;
			}
			if ($strContent != $strContent1de)
			{
				$this->Trace("Error: article content mismatch");		
				return;
			}
			if ($strLanguage != "DE")
			{
				$this->Trace("Error: article language mismatch");		
				return;
			}
			$this->Trace("");



			$this->Trace("8. DELETE ARTICLE BY NAME");
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
			
			 

		
			$this->Trace("9. GET ORIGINAL ENGLISH ARTICLE BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID1en;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Unexpected Result: \"".$consumer->GetError()."\"");
				return;	
			}
			$this->Trace("");		


			$this->Trace("10. GET ORIGINAL ENGLISH ARTICLE BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID1de;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Unexpected Result: \"".$consumer->GetError()."\"");
				return;	
			}
			$this->Trace("");		
						

			$this->Trace("11. GET ORIGINAL ENGLISH ARTICLE BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["articleversion_id"] = $strArticleVersionID2en;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Unexpected Result: \"".$consumer->GetError()."\"");
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
	
	

		
