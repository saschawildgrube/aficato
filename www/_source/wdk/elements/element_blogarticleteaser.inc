<?php

	class CBlogArticleTeaserElement extends CArticleElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{ 
			CElement::__construct(
				$website,
				"blogarticleteaser",
				$strStyleID,
				true,
				true);
		}
		
		function RenderBlogArticleTeaser(
			$strTitle,
			$strDateTime,
			$strURL,
			$strLinkText)
		{
			$strOutput = $this->GetLayoutHTML(); 
			$link = new CLinkElement($this->GetWebsite(),"default");
			$arrayItems["TITLE"] = $strTitle;    
			//$arrayItems["TEXT"] = StringCutOff($this->RenderArticle($strContent,$strFormat),200,"[...]");
			$arrayItems["CREATION_DATE"] = $this->GetWebsite()->RenderDate($strDateTime);
			$arrayItems["LINK"] = $link->RenderLink($strURL,$strLinkText);
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			
		}
	}

