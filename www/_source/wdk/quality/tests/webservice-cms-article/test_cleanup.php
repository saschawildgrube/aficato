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
			parent::__construct("Web service cms/article: Cleanup based on article name",$arrayConfig);
		}
		
		function OnInit()
		{
			$this->RequireWebservice($this->m_strWebservice);
			$this->SetVerbose(true);

			// Test remains inactive as long as the cleanup command is implemented
			$this->SetActive(false);	
			
			return parent::OnInit();	
		}	
		
		function Test_Cleanup($arrayArticles,$strName,$strLanguage,$strCountry,$strType,$strThresholdDateTime,$strThresholdVersions)
		{
			$this->Trace("Test_Cleanup name=\"$strName\"");
			if (!is_array($arrayArticles))
			{
				$this->Trace("WARNING: arrayArticles is not an array!");
				return;
			}
			$consumer = new CWebServiceConsumerWebApplication($this);
			foreach ($arrayArticles as $arrayArticle)
			{
				if (!is_array($arrayArticle))
				{
					$this->Trace("WARNING: arrayArticles contains at least on non-array member!");
					return;
				}
				$this->Trace("SET ARTICLE VERSION");
				$arrayParams = ArrayFilterByKeys(
					$arrayArticle,
					array("name","title","language","country","type","publication_start_datetime","publication_end_datetime","status"),
					true);
				$arrayParams["trace"] = false;
				$arrayParams["command"] = "set";
				$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
				if ($consumer->GetError() != "")	
				{
					$this->Trace("Error: \"".$consumer->GetError()."\"");		
					$this->SetResult(false);
					return;
				}
				$strArticleVersionID = $consumer->GetResultValue("NEW_ARTICLEVERSION_ID");
				if ($strArticleVersionID == "")
				{
					$this->Trace("Error: Article Version ID is empty");		
					$this->SetResult(false);
					return;
				}
				$this->m_arrayArticleVersionIDs[] = $strArticleVersionID;
				$this->Trace("New article version ID: ".$strArticleVersionID);
				$this->Trace("");
			}
			
	
			
			
			$this->Trace("CLEANUP");
			$arrayParams = array();
			$arrayParams["trace"] = $this->GetVerbose();
			if ($strName != "")
			{
				$arrayParams["name"] = $strName;
			}
			if ($strLanguage != "")
			{
				$arrayParams["language"] = $strLanguage;
			}
			if ($strCountry != "")
			{
				$arrayParams["country"] = $strCountry;
			}
			if ($strThresholdDateTime != "")
			{
				$arrayParams["threshold_datetime"] = $strThresholdDateTime;
			}
			if ($strThresholdVersions != "")
			{
				$arrayParams["threshold_versions"] = $strThresholdVersions;
			}
			$arrayParams["command"] = "cleanup";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Unexpected error code: \"".$consumer->GetError()."\"");	
				$this->SetResult(false);
				return;	
			}
			
			
			foreach ($arrayArticles as $arrayArticle)
			{
				$this->Trace("GET ARTICLE VERSION");
				$arrayParams = ArrayFilterByKeys($arrayArticle,array("name","language","country","type"),true);
				$arrayParams["trace"] = false;
				$arrayParams["command"] = "get";
				$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
				
				$bExpectCleanUp = ArrayGetValue($arrayArticle,"expectcleanup");
				if ($bExpectCleanUp == true)
				{
					if ($consumer->GetError() != "ITEM_NOT_FOUND")	
					{
						$this->Trace("Unexpected Error: \"".$consumer->GetError()."\"");
						$this->SetResult(false);
						return;
					}
				}
				else
				{
					if ($consumer->GetError() != "")	
					{
						$this->Trace("Error: \"".$consumer->GetError()."\"");		
						$this->SetResult(false);
						return;
					}
					
				}
				$this->Trace("");
			}			
			
			
			return;			
		}
		

		
		function OnTest()
		{
			parent::OnTest();

			$this->SetResult(true);

			$consumer = new CWebServiceConsumerWebApplication($this);
			
			
			$strName = "testtestarticlecleanup1";
			$strLanguage = "EN";
			$strThresholdDateTime = RenderDateTimeNow();
			$strThresholdVersions = "0";
			$arrayArticles = array(
				array(
					"name" => $strName,
					"language" => $strLanguage,
					"status" => "ACTIVE",
					"expectcleanup" => 1
					),
				array(
					"name" => $strName,
					"language" => $strLanguage,
					"status" => "ACTIVE",
					"expectcleanup" => 0
					)
				);
			
			$this->Test_Cleanup(
				$arrayArticles,
				$strName,
				$strLanguage,
				"",
				"STATIC",
				$strThresholdDateTime,
				$strThresholdVersions);
				
				
				
				
			$strName = "testtestarticlecleanup2";
			$strLanguage = "EN";
			$strThresholdVersions = "2";
			$arrayArticles = array(
				array(
					"name" => $strName,
					"language" => $strLanguage,
					"status" => "ACTIVE",
					"expectcleanup" => 1
					),
				array(
					"name" => $strName,
					"language" => $strLanguage,
					"status" => "ACTIVE",
					"expectcleanup" => 1
					),
				array(
					"name" => $strName,
					"language" => $strLanguage,
					"status" => "ACTIVE",
					"expectcleanup" => 0
					),
				array(
					"name" => $strName,
					"language" => $strLanguage,
					"status" => "ACTIVE",
					"expectcleanup" => 0
					),
				array(
					"name" => $strName,
					"language" => $strLanguage,
					"status" => "ACTIVE",
					"expectcleanup" => 0
					)
				);
			
			$this->Test_Cleanup(
				$arrayArticles,
				$strName,
				$strLanguage,
				"",
				"STATIC",
				$strThresholdDateTime,
				$strThresholdVersions);
				
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
	
	

		
