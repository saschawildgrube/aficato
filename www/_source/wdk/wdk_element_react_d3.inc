<?php

	require_once(GetWDKDir().'wdk_element_react.inc');

	abstract class CD3ReactElement extends CReactElement
	{
		function __construct(
			CWebSite &$website,
			$strElementID,
			$strStyleID,
			$bLayoutCFG,
			$bLayoutHTML)
		{
			parent::__construct(
				$website,
				$strElementID,
				$strStyleID,
				$bLayoutCFG,
				$bLayoutHTML);
				
			$this->GetWebsite()->ActivateD3v5();
			$this->AddJavaScript('wdk-react-d3'); 
		}
	}

