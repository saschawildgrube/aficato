<?php

	class CTeaserElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{ 
			parent::__construct(
				$website,
				"teaser",
				$strStyleID,
				true,
				true);
		}
		
		function RenderTeaser(
			$strTitle,
			$strText,
			$strLinkURL,
			$strLinkText,
			$bExternalLink = false)
		{
			$strOutput = $this->GetLayoutHTML();   
		
			$arrayItems["TITLE"] = $strTitle;
			$arrayItems["TEXT"] = $strText;
			$arrayItems["LINKURL"] = $strLinkURL;
			$arrayItems["LINKTEXT"] = $strLinkText;

			if ($bExternalLink == true)
			{
				$strLinkStyle = $this->GetLayoutDef("LAYOUT_LINK_STYLE_EXTERNAL");
				if ($strLinkStyle == "")
				{
					$strLinkStyle = "default";	
				}
			}
			else
			{
				$strLinkStyle = $this->GetLayoutDef("LAYOUT_LINK_STYLE_INTERNAL");
				if ($strLinkStyle == "")
				{
					$strLinkStyle = "default";	
				}
			}

			$link = new CLinkElement($this->GetWebsite(),$strLinkStyle);
			$arrayItems["LINK"] = $link->RenderLink(
				$strLinkURL,
				$strLinkText,
				$bExternalLink);
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

