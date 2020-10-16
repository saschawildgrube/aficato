<?php

/*
		https://developers.facebook.com/docs/plugins/share-button/
*/

	class CFacebookShareElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"facebookshare",
				$strStyleID,
				false,
				true);
		}
		
		function RenderFacebookShare()
		{
			$this->GetWebsite()->ActivateFacebookSDK();
			
			$strOutput = $this->GetLayoutHTML(); 
/*
			$arrayItems = array();
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
*/		
			return $strOutput;			

		}
	}

