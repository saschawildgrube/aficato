<?php

	class CBlogModule extends CModule
	{
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			$this->m_arrayErrors = array();
			parent::__construct(
				$website,
				"cms/blog",
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$this->GetWebsite()->IncludeElement("article");
			$this->GetWebsite()->IncludeElement("blogarticle");
			$this->GetWebsite()->IncludeElement("blogarticleteaser");
			
			return parent::OnInit();
		}
		
		function OnRenderModule($strState)
		{
			switch ($strState)
			{
				case "article":
				{
					return $this->StateArticle();
				}
				case "archive":
				{
					return $this->StateArchive();
				}
				case "latest":
				default:	
				{
					return $this->StateLatest();
				}
			}
		}
		
		
		function GetArticle($strArticleID)
		{
			$arrayParams = array();
			$arrayParams["articleversion_id"] = $strArticleID;
			$arrayParams["command"] = "get";
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$webservice->ConsumeWebService("cms/article",$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				return false;
			}
			$arrayResult = $webservice->GetResultArray();
			$arrayArticle = $arrayResult["ARTICLE"];
			return $arrayArticle;
		}
		
		function GetArticles($nBlocksize = 5, $nOffset = 0)
		{
			$arrayParams = array();
			
			$arrayParams["blocksize"] = $nBlocksize;
			$arrayParams["offset"] = $nOffset;

			$arrayParams["sort1"] = "publication_start_datetime";
			$arrayParams["sort1_order"] = "desc";    

			$arrayParams["filter1"] = "status";
			$arrayParams["filter1_operator"] = "=";
			$arrayParams["filter1_value"] = "ACTIVE";

			$arrayParams["filter2"] = "type";
			$arrayParams["filter2_operator"] = "=";
			$arrayParams["filter2_value"] = "BLOG";

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
				return false;
			}
		
			$list = $webservice->GetResultList();
			
			return $list;
		}
		
		
		function StateLatest()
		{
			$this->Trace("StateLatest");


			
			$nMaxArticles = 5;
			
			$arrayArticles = $this->GetArticles($nMaxArticles);
			if ($arrayArticles === false)
			{
				return "error";	
			}
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "details";
			
			$strLatest = "";
			$strTeasers = "";
			
			$article = new CBlogArticleElement($this->GetWebsite(),"default");
			$articleTeaser = new CBlogArticleTeaserElement($this->GetWebsite(),"default");
			
			$bFirst = true;
			
			foreach ($arrayArticles as $arrayArticle)
			{
				
				if ($bFirst == true)
				{
					$strLatest .= $article->RenderBlogArticle(
						$arrayArticle["TITLE"],
						$arrayArticle["CONTENT"],
						$arrayArticle["TEXTFORMAT"],
						$arrayArticle["PUBLICATION_START_DATETIME"]);
					
					$bFirst = false;
				}
				else
				{
					$arrayParams = array();
					$arrayParams[$this->GetModuleVarNameState()] = "article";
					$arrayParams[$this->GetModuleVarName("id")] = $arrayArticle["ARTICLEVERSION_ID"];
					$arrayParams[$this->GetModuleVarName("backlink")] = "latest";
					$strURL = $this->GetWebsite()->MakeURL($arrayParams);
					$strTeasers .= $articleTeaser->RenderBlogArticleTeaser(
						$arrayArticle["TITLE"],
						$arrayArticle["PUBLICATION_START_DATETIME"],
						$strURL,
						"?TID_BLOG_LINK_READ?");
				}
/*				$arrayParams[$this->GetModuleVarName("training_id")] = $arrayTraining["TRAINING_ID"];
				$strLinkURL = $this->GetWebsite()->MakeURL($arrayParams);
				
				$strSchedule .= $trainingteaser->RenderTrainingTeaser(
					$arrayTraining,
					$this->RenderDate($arrayTraining),
					$strLinkURL,
					$this->RenderPDFLink($arrayTraining));
					*/
				

			}

			$this->SetOutputItem(
				"LATEST",
				$strLatest);

			$this->SetOutputItem(
				"TEASERS",
				$strTeasers);
				
				
			$strLinkArchive = "";
			if (ArrayCount($arrayArticles) == $nMaxArticles)
			{
				$link = new CLinkElement($this->GetWebsite(),"default");
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "archive";
				$strURL = $this->GetWebsite()->MakeURL($arrayParams); 
				$strLinkArchive = $link->RenderLink($strURL,"?TID_BLOG_LINK_ARCHIVE?");
			}
				
			$this->SetOutputItem(
				"LINK_ARCHIVE",
				$strLinkArchive);

			return "latest";
		}
		
		function StateArticle()
		{
			$this->Trace("StateArticle");
			$strArticleID = $this->GetModuleVar("id");
			if ($strArticleID == "")
			{
				return $this->StateLatest();	
			}
			
			$arrayArticle = $this->GetArticle($strArticleID);
			if ($arrayArticle == false)
			{
				return $this->StateLatest();
			}
			
			$article = new CBlogArticleElement($this->GetWebsite(),"default");
			$strArticle = $article->RenderBlogArticle(
				$arrayArticle["TITLE"],
				$arrayArticle["CONTENT"],
				$arrayArticle["TEXTFORMAT"],
				$arrayArticle["PUBLICATION_START_DATETIME"]);			
			
			$this->SetOutputItem(
				"ARTICLE",
				$strArticle);					
			
			$link = new CLinkElement($this->GetWebsite(),"default");
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = $this->GetModuleVar("backlink");
			$strURL = $this->GetWebsite()->MakeURL($arrayParams);
			$strLinkBack = $link->RenderLink($strURL,"?TID_BLOG_LINK_BACK?");
				
			$this->SetOutputItem(
				"LINK_BACK",
				$strLinkBack);

			
			return "article";
		}



		function StateArchive()
		{
			$this->Trace("StateArchive");



			
			//$this->GetWebsite()->IncludeElement("trainingteaser");
			//$trainingteaser = new CTrainingTeaserElement($this->GetWebsite(),"default");
			
			$arrayArticles = $this->GetArticles(100);
			if ($arrayArticles === false)
			{
				return $this->StateLatest();	
			}
			
			$strTeasers = "";
			$articleTeaser = new CBlogArticleTeaserElement($this->GetWebsite(),"default");
			
			foreach ($arrayArticles as $arrayArticle)
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "article";
				$arrayParams[$this->GetModuleVarName("id")] = $arrayArticle["ARTICLEVERSION_ID"];
				$arrayParams[$this->GetModuleVarName("backlink")] = "latest";
				$strURL = $this->GetWebsite()->MakeURL($arrayParams);
				$strTeasers .= $articleTeaser->RenderBlogArticleTeaser(
					$arrayArticle["TITLE"],
					$arrayArticle["PUBLICATION_START_DATETIME"],
					$strURL,
					"?TID_BLOG_LINK_READ?");
			}

			$this->SetOutputItem(
				"TEASERS",
				$strTeasers);
				
				
			$link = new CLinkElement($this->GetWebsite(),"default");
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "latest";
			$strURL = $this->GetWebsite()->MakeURL($arrayParams); 
			$strLinkLatest = $link->RenderLink($strURL,"?TID_BLOG_LINK_LATEST?");
				
			$this->SetOutputItem(
				"LINK_LATEST",
				$strLinkLatest);

			return "archive";
		}



		

	}

