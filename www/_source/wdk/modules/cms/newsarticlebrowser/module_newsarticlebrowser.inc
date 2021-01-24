<?php

	class CNewsArticleBrowserModule extends CModule
	{
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			$this->m_arrayErrors = array();
			parent::__construct(
				$website,
				"cms/newsarticlebrowser",
				$strInstanceID,
				$arrayConfig);
		}
		
		
		function OnRenderModule($strState)
		{
			switch ($strState)
			{
				/*
				case "details":
				{
					return $this->StateDetails();
				}
				*/
				case "overview":
				default:	
				{
					return $this->StateOverview();
				}
			}
		}
		
		
		function GetArticle($strArticleID)
		{
			$arrayParams = array();
			$arrayParams["articleversion_id"] = $strTrainingID;
			$arrayParams["command"] = "get";
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$webservice->ConsumeWebService("cms/article",$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				return false;
			}
			$arrayResult = $webservice->GetResultArray();
			$arrayArticle = $arrayArticle["TRAINING"];
			return $arrayArticle;
			
		}
		
		
		function StateOverview()
		{
			$this->Trace("StateOverview");


			$arrayParams = array();
			
			$arrayParams["blocksize"] = 100;
			$arrayParams["offset"] = 0;
			$arrayParams["sort1"] = "creation_datetime";
			$arrayParams["sort1_order"] = "desc";    

			$arrayParams["filter1"] = "status";
			$arrayParams["filter1_operator"] = "=";
			$arrayParams["filter1_value"] = "ACTIVE";

			$arrayParams["filter2"] = "type";
			$arrayParams["filter2_operator"] = "=";
			$arrayParams["filter2_value"] = "NEWS";

			$strDateTimeNow = RenderDateTimeNow();

			$arrayParams["filter3"] = "publication_start_datetime";
			$arrayParams["filter3_operator"] = "<="; 
			$arrayParams["filter3_value"] = $strDateTimeNow;

			$arrayParams["filter4"] = "publication_end_datetime";
			$arrayParams["filter4_operator"] = ">=";
			$arrayParams["filter4_value"] = $strDateTimeNow;

 
			$arrayParams["command"] = "list";
						
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$webservice->ConsumeWebService("cms/article",$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
				return "error";
			}
			
			
			$list = $webservice->GetResultList();
			
			//$this->GetWebsite()->IncludeElement("trainingteaser");
			//$trainingteaser = new CTrainingTeaserElement($this->GetWebsite(),"default");
			
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "details";
			
			$strOverview = "";
			
			$this->IncludeElement("newsarticle");
			$newsarticle = new CNewsArticleElement($this->GetWebsite(),"default");
			
			foreach ($list as $arrayArticle)
			{
/*				$arrayParams[$this->GetModuleVarName("training_id")] = $arrayTraining["TRAINING_ID"];
				$strLinkURL = $this->GetWebsite()->MakeURL($arrayParams);
				
				$strSchedule .= $trainingteaser->RenderTrainingTeaser(
					$arrayTraining,
					$this->RenderDate($arrayTraining),
					$strLinkURL,
					$this->RenderPDFLink($arrayTraining));
					*/
				
				$strOverview .= $newsarticle->RenderNewsArticle(
					$arrayArticle["TITLE"],
					$arrayArticle["CONTENT"],
					$arrayArticle["TEXTFORMAT"],
					$arrayArticle["CREATION_DATETIME"]);
					
			}

			$this->SetOutputItem(
				"OVERVIEW",
				$strOverview);

	
			return "overview";
		}

	}

