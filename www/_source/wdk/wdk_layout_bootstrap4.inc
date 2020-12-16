<?php

	abstract class CLayoutBootstrap4 extends CLayout
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
			$this->GetWebsite()->ActivateBootstrap4();
			
			return $bResult;
		}
	}	
