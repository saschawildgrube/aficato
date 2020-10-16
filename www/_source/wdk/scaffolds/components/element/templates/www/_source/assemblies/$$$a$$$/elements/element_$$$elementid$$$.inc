<?php

	class C$$$ElementId$$$Element extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'$$$elementid$$$',
				$strStyleID,
				true,
				true);
		}
		
		function Render$$$ElementId$$$($strText)
		{
			$strLayoutDef = $this->GetLayoutHTML();
			
			$arrayItems = array();
			
			$strColor = $this->GetLayoutDef('LAYOUT_$$$ELEMENTID$$$_COLOR');
			if ($strColor == '')
			{
				$strColor = 'blue';
			}
			
			$arrayItems['COLOR'] = $strColor;
			$arrayItems['TEXT'] = $strText;

			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strLayoutDef);
			return $strOutput;			
		}
	}
