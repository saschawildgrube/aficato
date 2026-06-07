<?php

	class CIconTeaserElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{ 
			parent::__construct(
				$website,
				'iconteaser',
				$strStyleID,
				true,
				true);
		}
		
		function RenderIconTeaser(
			$strIconId,
			$strTitle,
			$strText,
			$strLinkURL,
			$strLinkText,
			$bExternalLink = false)
		{
			$strOutput = $this->GetLayoutHTML(); 
		
		
			$this->IncludeElement('icon');

			$icon = new CIconElement($this->GetWebsite(), 'large');
		
			$arrayItems['ICONID'] = $strIconId;
			$arrayItems['ICON'] = $icon->RenderIcon($strIconId);
			$arrayItems['TITLE'] = $strTitle; 
			$arrayItems['TEXT'] = $strText;
			$arrayItems['LINKURL'] = $strLinkURL;
			$arrayItems['LINKTEXT'] = $strLinkText;
			$arrayItems['LINKTARGET'] = ($bExternalLink == true)?('target="_blank"'):('');
			
			if ($strLinkURL != '')
			{
				$strLinkStyle = $this->GetLayoutDef('LAYOUT_LINK_STYLE');
				if ($strLinkStyle == '')
				{
					$strLinkStyle = 'default';	
				}
				
				$link = new CLinkElement($this->GetWebsite(),$strLinkStyle);
				$arrayItems['LINK'] = $link->RenderLink(
					$strLinkURL,
					$strLinkText,
					$bExternalLink);
			}
			else
			{
				$arrayItems['LINK'] = '';
			}
		
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

