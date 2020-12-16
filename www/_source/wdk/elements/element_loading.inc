<?php

	class CLoadingElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'loading',
				$strStyleID,
				false,
				true);
				
			$this->GetWebsite()->ActivateFontAwesome4();
		}
		
		function RenderLoading($strText = '')
		{
			$strOutput = $this->GetLayoutHTML(); 

			$arrayItems = array();
		
			if ($strText == "")
			{
				$strText = '?TID_INFO_LOADING?';
			}
			$arrayItems['TEXT'] = $strText;
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}
