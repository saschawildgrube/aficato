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
			parent::__construct("Web service cms/article: Rename",$arrayConfig);
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

			/*
			
			
			
			
			*
			
			1. Add article 1 in English
			2. Get article 1 by name
			3. Add article 2 in English
			4. Get article 2 by name
			5. Add article 3 in German
			6. Get article 3 by name
			7. Rename all articles by name
			8. Rename all articles by name (again, to see it fail)
			9. Get article 2 by old name
			10. Get article 3 by old name
			11. Get article 2 by new name
			12. Get article 3 by new name
			
			*/

			$consumer = new CWebServiceConsumerWebApplication($this);
		
			$strName1 = "testtestarticlerename1";
			$strName2 = "testtestarticlerename2";
			$strTitle1en = "Test Test Article Rename";		
			$strTitle3de = u("Test Test Artikel Umbenennung");
			$strContent1en = "This is test content!";		
			$strContent2en = "This is updated test content!";		
			$strContent3de = u("Dies ist Test-Text.");

		
			$this->Trace("1. SET ARTICLE 1 BY NAME");
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

	
		
		
			$this->Trace("2. GET ARTICLE 1 BY NAME");
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
			
			
			
			$this->Trace("3. SET ARTICLE 2 BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["title"] = $strTitle1en;
			$arrayParams["name"] = $strName1;
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

	
		
		
			$this->Trace("4. GET ARTICLE 2 BY NAME");
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
				$this->Trace("Error: article title mismatch: should be \"".$strTitle1en."\"");		
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
			
			
			
						
			$this->Trace("5. SET ARTICLE 3 BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["title"] = $strTitle3de;
			$arrayParams["name"] = $strName1;
			$arrayParams["content"] = $strContent3de;
			$arrayParams["language"] = "DE";
			$arrayParams["status"] = "ACTIVE";
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;
			}
			$strArticleVersionID3de = $consumer->GetResultValue("NEW_ARTICLEVERSION_ID");
			if ($strArticleVersionID3de == "")
			{
				$this->Trace("Error: Article Version ID is empty");		
				return;
			}
			$this->m_arrayArticleVersionIDs[] = $strArticleVersionID3de;
			$this->Trace("New article version ID: ".$strArticleVersionID3de);
			$this->Trace("");

	
		
		
			$this->Trace("6. GET ARTICLE 3 BY NAME");
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
			if ($strArticleVersionID != $strArticleVersionID3de)
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
			if ($strTitle != $strTitle3de)
			{
				$this->Trace("Error: article title mismatch: should be \"".$strTitle3de."\"");		
				return;
			}
			if ($strContent != $strContent3de)
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
			
			

			$this->Trace("7. RENAME ARTICLE BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["new_name"] = $strName2;
			$arrayParams["command"] = "rename";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Unexpected error code: \"".$consumer->GetError()."\"");		
				return;	
			}	
			
			$this->Trace("8. RENAME ARTICLE BY NAME AGAIN");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["new_name"] = $strName2;
			$arrayParams["command"] = "rename";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Unexpected error code: \"".$consumer->GetError()."\"");		
				return;	
			}	
			
			
			$this->Trace("9. GET ARTICLE 2 BY OLD NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["language"] = "EN";
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Unexpected Result: \"".$consumer->GetError()."\"");
				return;	
			}
			$this->Trace("");		

			$this->Trace("10. GET ARTICLE 3 BY OLD NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName1;
			$arrayParams["language"] = "DE";
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Unexpected Result: \"".$consumer->GetError()."\"");
				return;	
			}
			$this->Trace("");		



	
		
			$this->Trace("11. GET ARTICLE 2 BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName2;
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
			if ($strName != $strName2)
			{
				$this->Trace("Error: article name mismatch");		
				return;
			}
			if ($strTitle != $strTitle1en)
			{
				$this->Trace("Error: article title mismatch: should be \"".$strTitle1en."\"");		
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
			

			$this->Trace("12. GET ARTICLE 3 BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			$arrayParams["name"] = $strName2;
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
			if ($strArticleVersionID != $strArticleVersionID3de)
			{
				$this->Trace("Error: article version id mismatch");		
				return;
			}
			if ($strStatus != "ACTIVE")
			{
				$this->Trace("Error: article status mismatch");		
				return;
			}
			if ($strName != $strName2)
			{
				$this->Trace("Error: article name mismatch");		
				return;
			}
			if ($strTitle != $strTitle3de)
			{
				$this->Trace("Error: article title mismatch: should be \"".$strTitle3de."\"");		
				return;
			}
			if ($strContent != $strContent3de)
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
	
	

		
