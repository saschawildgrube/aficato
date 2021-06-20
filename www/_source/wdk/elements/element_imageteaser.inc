<?php

	class CImageTeaserElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{ 
			parent::__construct(
				$website,
				"imageteaser",
				$strStyleID,
				true,
				true);
		}
		
		function RenderImageTeaser(
			$strImageURL,
			$strTitle,
			$strText,
			$strLinkURL,
			$strLinkText,
			$bExternalLink = false)
		{
			$strOutput = $this->GetLayoutHTML(); 
		
			$arrayItems["IMAGEURL"] = $strImageURL;
			$arrayItems["TITLE"] = $strTitle; 
			$arrayItems["TEXT"] = $strText;
			$arrayItems["LINKURL"] = $strLinkURL;
			$arrayItems["LINKTEXT"] = $strLinkText;
			
			if ($strLinkURL != "")
			{
				$strLinkStyle = $this->GetLayoutDef("LAYOUT_LINK_STYLE");
				if ($strLinkStyle == "")
				{
					$strLinkStyle = "default";	
				}
				
				$link = new CLinkElement($this->GetWebsite(),$strLinkStyle);
				$arrayItems["LINK"] = $link->RenderLink(
					$strLinkURL,
					$strLinkText,
					$bExternalLink);
			}
			else
			{
				$arrayItems["LINK"] = "";
			}
		
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

