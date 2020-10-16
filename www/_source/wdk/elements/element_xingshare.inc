<?php

/*
	https://dev.xing.com/plugins/share_button/new
*/


	class CXingShareElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"xingshare",
				$strStyleID,
				false,
				true);
		}
		
		function RenderXingShare()
		{
			$strOutput = $this->GetLayoutHTML(); 
			
			$this->GetWebsite()->AddJavaScript('xing-plugin-share');
			
			/*
			data-shape="square"
			*/
			
/*			$arrayItems = array();
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			*/
			
			return $strOutput;			

		}
	}

