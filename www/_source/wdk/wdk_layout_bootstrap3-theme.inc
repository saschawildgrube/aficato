<?php

	require_once(GetWDKDir()."wdk_layout_bootstrap3.inc");

	abstract class CLayoutBootstrap3Theme extends CLayoutBootstrap3
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
			
			$this->GetWebsite()->AddHeader('<meta name="viewport" content="width=device-width, initial-scale=1.0">');
			$this->GetWebsite()->AddHeader('<!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->');
			$this->GetWebsite()->AddHeader('<!--[if lt IE 9]>');
			$this->GetWebsite()->AddHeader('	<script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>');
			$this->GetWebsite()->AddHeader('	<script src="https://oss.maxcdn.com/libs/respond.js/1.3.0/respond.min.js"></script>');
			$this->GetWebsite()->AddHeader('<![endif]-->');

			// BOOTSTRAP THEME
			$this->AddStyleSheetURL('bootstrap-theme','https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css');
			// integrity="sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp" crossorigin="anonymous"
			
			$this->GetWebsite()->AddJavaScript('layout-element-navigation-horizontalpath','layout_default_element_navigation_horizontalpath');
			
			return $bResult;
		}
	}	
