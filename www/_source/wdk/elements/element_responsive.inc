<?php

	class CResponsiveElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'responsive',
				$strStyleID,
				false,
				true);
		}
		
		function RenderResponsive($strXS, $strSM, $strMD, $strLG)
		{
			$strLayoutDef = $this->GetLayoutHTML();
			
			$arrayItems = array();
			$arrayItems['XS'] = $strXS;
			$arrayItems['SM'] = $strSM;
			$arrayItems['MD'] = $strMD;
			$arrayItems['LG'] = $strLG;
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strLayoutDef);
			return $strOutput;			
		}
	}
