<?php

	abstract class CLayoutBootstrap4 extends CLayout
	{
		function __construct(
			CWebSite $website,
			$strLayoutID)
		{
			parent::__construct($website, $strLayoutID);
		}
		
		function OnInit()
		{
			$bResult = parent::OnInit();
			
			// BOOTSTRAP
			$this->GetWebsite()->ActivateBootstrap4();
			
			return $bResult;
		}
	}	
