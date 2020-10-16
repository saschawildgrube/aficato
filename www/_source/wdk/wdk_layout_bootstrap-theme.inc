<?php

	require_once(GetWDKDir()."wdk_layout_bootstrap.inc");

	abstract class CLayoutBootstrapTheme extends CLayoutBootstrap
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
			
			$this->GetWebsite()->AddHeader('<meta name="viewport" content="width=device-width, initial-scale=1.0">');
			$this->GetWebsite()->AddHeader('<!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->');
			$this->GetWebsite()->AddHeader('<!--[if lt IE 9]>');
			$this->GetWebsite()->AddHeader('	<script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>');
			$this->GetWebsite()->AddHeader('	<script src="https://oss.maxcdn.com/libs/respond.js/1.3.0/respond.min.js"></script>');
			$this->GetWebsite()->AddHeader('<![endif]-->');

			// BOOTSTRAP THEME
			$this->GetWebsite()->AddStyleSheet('bootstrap-theme','bootstrap-theme.min');
			
			$this->GetWebsite()->AddJavaScript('layout-element-navigation-horizontalpath','layout_default_element_navigation_horizontalpath');
			
			return $bResult;
		}
	}	
