<?php

	class CIconElement extends CElement
	{
		var $m_arrayAlias;
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'icon',
				$strStyleID,
				false,
				true);
				
			$this->GetWebsite()->ActivateFontAwesome4();
			
			$this->m_arrayAlias = [];
			$strAliasFile = GetWDKDir()."data/icon-alias-fontawesome.csv";
			if (IsFile($strAliasFile))
			{
				$this->m_arrayAlias = FileReadCSV_RFC4180($strAliasFile);
			}
		}

		function GetIconIdFromAlias($strIconAliasId)
		{
			foreach ($this->m_arrayAlias as $arrayAlias)
			{
				if ($strIconAliasId == ArrayGetValue($arrayAlias,'ALIAS'))
				{
					return ArrayGetValue($arrayAlias,'FONTAWESOME');
				}
			}			
		}
		
		function RenderIcon($strIconId)
		{
			$strLayoutDef = $this->GetLayoutHTML();
			
			$arrayItems = array();
			
			$strIconIdFromAlias = $this->GetIconIdFromAlias($strIconId);
			if ($strIconIdFromAlias != '')
			{
				$strIconId = $strIconIdFromAlias;
			}
			
			$arrayItems['ICONID'] = StringLowerCase($strIconId);

			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strLayoutDef);
			return $strOutput;			
		}
	}
		
	function GetIconIdByFileCategory($strFileCategory)
	{
		if ($strFileCategory == 'other')
		{
			return 'file-o';	
		}
		return 'file-'.$strFileCategory.'-o';
	}

