<?php

	class CLegendElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'legend',
				$strStyleID,
				true,
				false);
		}
		
		function RenderLegend($arrayItems)
		{
			$strOutput = '';
			if (!is_array($arrayItems))
			{
				$arrayItems = array();
			}
			if (ArrayCount($arrayItems) == 0)
			{
				return '';
			}
			
			$strOutput .= $this->GetLayoutDef('LAYOUT_LEGEND_BEGIN')."\n";
			foreach ($arrayItems as $arrayItem)
			{
				$strOutputItem = $this->GetLayoutDef('LAYOUT_LEGEND_ITEM');
				
				$strIconID = ArrayGetValue($arrayItem,'icon');
				if ($strIconID == '')
				{
					$strIconID = 'square';
				}
				
				$strColor = ArrayGetValue($arrayItem,'color');
				if ($strColor == '')
				{
					$strColor = 'black';
				}
				
				$arrayOutputItems = array();
				$arrayOutputItems['ICONID'] = $strIconID;
				$arrayOutputItems['COLOR'] = $strColor;
				$arrayOutputItems['LABEL'] = ArrayGetValue($arrayItem,'label');

				$strOutputItem = ReplaceTagsWDK(
					$strOutputItem,
					$arrayOutputItems);
				
				$strOutput .= $strOutputItem;
			}
			$strOutput .= $this->GetLayoutDef('LAYOUT_LEGEND_END')."\n";
			return $strOutput;
		}
	}

