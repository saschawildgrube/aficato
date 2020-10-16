<?php

	class CDownloadLinkElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'downloadlink',
				$strStyleID,
				false,
				true);
		}
		
		function RenderDownloadLink($strFileURL, $strLinkText, $strFileType)
		{
			$strOutput = $this->GetLayoutHTML(); 
			
			$arrayItems = array();
			
			if (strstr($strFileURL,'&amp;') == false )
			{
				$strFileURL = HtmlEncode($strFileURL);
			}
			
			$icon = new CIconElement($this->GetWebsite(),'default');
			$strIcon = $icon->RenderIcon(GetIconIDByFileCategory(GetFileCategory('bogus.'.$strFileType)));
			
			$arrayItems['URL'] = $strFileURL;
			$arrayItems['ICON'] = $strIcon;
			$arrayItems['TEXT'] = $strLinkText;
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

