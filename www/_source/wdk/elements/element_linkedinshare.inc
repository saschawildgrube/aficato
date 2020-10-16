<?php

/*
	https://developer.linkedin.com/plugins/share
*/

	class CLinkedInShareElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"linkedinshare",
				$strStyleID,
				false,
				true);
		}
		
		function RenderLinkedInShare()
		{
			$strOutput = $this->GetLayoutHTML(); 
			
			
				// lang: de_DE
				// lang: en_US <- if this is embedded into the first script tag, the w3c validation fails


/*			$arrayItems = array();
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			*/
			
			return $strOutput;			

		}
	}

