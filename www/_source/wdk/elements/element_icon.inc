<?php

	class CIconElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'icon',
				$strStyleID,
				true,
				true);
				
			$this->GetWebsite()->ActivateFontAwesome4();
		}
		
		function RenderIcon($strIconID)
		{
			$strLayoutDef = $this->GetLayoutHTML();
			
			$arrayItems = array();
			
			$strAliasIconID = $this->GetLayoutDef('LAYOUT_ICON_'.StringUpperCase($strIconID));
			if ($strAliasIconID != '')
			{
				$strIconID = $strAliasIconID;
			}
			
			$arrayItems['ICONID'] = StringLowerCase($strIconID);

			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strLayoutDef);
			return $strOutput;			
		}
	}
		
	function GetIconIDByFileCategory($strFileCategory)
	{
		if ($strFileCategory == 'other')
		{
			return 'file-o';	
		}
		return 'file-'.$strFileCategory.'-o';
	}

