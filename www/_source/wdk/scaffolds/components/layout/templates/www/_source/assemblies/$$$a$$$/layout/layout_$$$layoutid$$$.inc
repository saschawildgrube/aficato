<?php

	require_once(GetWDKDir()."wdk_layout_bootstrap3.inc");

	class CMyLayout extends CLayoutBootstrap3
	{
		function OnInit()
		{
			$bResult = parent::OnInit();
			
			// You can add addional headers
			$this->GetWebsite()->AddHeader('<meta name="viewport" content="width=device-width, initial-scale=1.0,maximum-scale=1">');

			// Add stylesheets via URL from external sources...
			$this->GetWebsite()->AddStyleSheetURL('fontawesome','https://maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.min.css');
			
			// Add stylesheet files
			$this->GetWebsite()->AddStyleSheet('$$$layoutid$$$','3rdparty');
	
				// Or use one of the CWebsite::Activate*() functions
			$this->GetWebsite()->ActivateFontawesome4();

			
			// Load config items			
			$strColor1 = $this->GetWebsite()->GetLayoutConfigItem("LAYOUT_COLOR1");

			// Add JavaScript files...
			$this->GetWebsite()->AddJavaScript('wdk');
			$this->GetWebsite()->AddJavaScript('wdk-progressindicator-activate');  
			$this->GetWebsite()->AddJavaScript('wdk-progressindicatordownload-activate');
			// Remove these if you do NOT want to see the progress indicator after clicking a form button
			
			
			return $bResult;
		}
	}
	
