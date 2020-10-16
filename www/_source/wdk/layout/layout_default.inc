<?php

	require_once(GetWDKDir().'wdk_layout_bootstrap-theme.inc'); 

	class CMyLayout extends CLayoutBootstrapTheme
	{
		function CallbackInit()
		{
			$bResult = parent::CallbackInit();
			$this->GetWebsite()->AddJavaScript('wdk');
			$this->GetWebsite()->AddJavaScript('wdk-progressindicator-activate');
			$this->GetWebsite()->AddJavaScript('wdk-progressindicatordownload-activate');
			return $bResult;
		}
	}
	
