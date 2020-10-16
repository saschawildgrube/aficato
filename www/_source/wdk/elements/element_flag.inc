<?php

	class CFlagElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'flag',
				$strStyleID,
				false,
				true);
		}
		
		function RenderFlag($strCountryID)
		{
			$strCountryID = StringUpperCase($strCountryID);
			if (!IsValidCountryID($strCountryID))
			{
				return '';	
			}
			
			$strOutput = $this->GetLayoutHTML();  
			
			$arrayItems = array();
			
			$strImageURL = $this->GetWebsite()->GetLayoutImageURL('icon_country-'.StringLowerCase($strCountryID),'png'); 
			
			$arrayItems['IMAGEURL'] = HtmlEncode($strImageURL);
			$arrayItems['TOOLTIP'] = '?TID_COUNTRY_'.$strCountryID.'?';
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;						
		}
	}

