<?php

	class CDownloadLinkElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"downloadlink",
				$strStyleID,
				false,
				true);
		}
		
		function RenderDownloadLink($strFileURL, $strLinkText, $strFileType)
		{
			$strOutput = $this->GetLayoutHTML(); 
			
			$arrayItems = array();
			
			if (strstr($strFileURL,"&amp;") == FALSE )
			{
				$strFileURL = HtmlEncode($strFileURL);
			}
			
			$strIconURL = $this->GetWebsite()->GetLayoutImageURL("icon_".StringLowerCase($strFileType),"png");
			$strIconURL = HtmlEncode($strIconURL);
			
			$arrayItems["FILEURL"] = $strFileURL;
			$arrayItems["ICONURL"] = $strIconURL;
			$arrayItems["LINKTEXT"] = $strLinkText;
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

