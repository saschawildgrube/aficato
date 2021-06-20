<?php

	require_once("element_article.inc");

	class CNewsArticleElement extends CArticleElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{ 
			CElement::__construct(
				$website,
				"newsarticle",
				$strStyleID,
				true,
				true);
		}
		
		function RenderNewsArticle(
			$strTitle,
			$strContent,
			$strFormat,
			$strDateTime)
		{
			$strOutput = $this->GetLayoutHTML(); 
		
			$arrayItems["TITLE"] = $strTitle;    
			$arrayItems["TEXT"] = $this->RenderArticle($strContent,$strFormat);
			$arrayItems["CREATION_DATE"] = $this->GetWebsite()->RenderDate($strDateTime);
			$arrayItems["CREATION_TIME"] = $this->GetWebsite()->RenderTime($strDateTime);
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			
		}
	}

