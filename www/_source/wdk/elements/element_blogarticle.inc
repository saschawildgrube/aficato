<?php

	class CBlogArticleElement extends CArticleElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{ 
			CElement::__construct(
				$website,
				"blogarticle",
				$strStyleID,
				true,
				true);
		}
		
		function RenderBlogArticle(
			$strTitle,
			$strContent,
			$strFormat,
			$strDateTime)
		{
			$strOutput = $this->GetLayoutHTML(); 
		
			$arrayItems["TITLE"] = $strTitle;    
			$arrayItems["TEXT"] = $this->RenderArticle($strContent,$strFormat);
			$arrayItems["CREATION_DATE"] = $this->GetWebsite()->RenderDate($strDateTime);
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			
		}
	}

