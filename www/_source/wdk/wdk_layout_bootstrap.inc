<?php

	abstract class CLayoutBootstrap extends CLayout
	{
		function __construct(
			CWebSite $website,
			$strLayoutID)
		{
			parent::__construct($website, $strLayoutID);
		}
		
		function CallbackInit()
		{
			$bResult = parent::CallbackInit();
			
			// BOOTSTRAP
			$this->GetWebsite()->AddStyleSheet('bootstrap','bootstrap.min');
			$this->GetWebsite()->AddJavaScript('bootstrap','bootstrap.min');
			
			// JQUERY
			$this->GetWebsite()->ActivateJQuery();
			
			// GLYPHICONS
			//$this->GetWebsite()->AddStyleSheet('font-glyphicons-halflings','glyphicons-halflings');
			
			return $bResult;
		}
	}	
