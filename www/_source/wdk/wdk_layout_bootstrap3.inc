<?php

	abstract class CLayoutBootstrap3 extends CLayout
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
			$this->GetWebsite()->ActivateBootstrap3();
			
			return $bResult;
		}
	}	
