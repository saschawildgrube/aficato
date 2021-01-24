<?php

	require_once(GetWDKDir().'wdk_layout_bootstrap3-theme.inc'); 

	class CMyLayout extends CLayoutBootstrap3Theme
	{
		function OnInit()
		{
			$bResult = parent::OnInit();
			$this->GetWebsite()->AddJavaScript('wdk');
			$this->GetWebsite()->AddJavaScript('wdk-progressindicator-activate');
			$this->GetWebsite()->AddJavaScript('wdk-progressindicatordownload-activate');
			return $bResult;
		}
	}
	
