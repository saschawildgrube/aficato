<?php

	require_once(GetWDKDir()."wdk_layout_bootstrap-theme.inc"); 

	class CMyLayout extends CLayoutBootstrapTheme
	{
		function CallbackInit()
		{
			$bResult = parent::CallbackInit();
			$this->GetWebsite()->AddStyleSheetURL('fontawesome','https://maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.min.css');
			$this->GetWebsite()->AddJavaScript('wdk');
			$this->GetWebsite()->AddJavaScript('wdk-progressindicator-activate');
			$this->GetWebsite()->AddJavaScript('wdk-progressindicatordownload-activate');
			return $bResult;
		}
	}
	
