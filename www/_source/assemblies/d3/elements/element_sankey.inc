<?php

	require_once(GetWDKDir().'wdk_element_react_d3.inc');

	class CSankeyElement extends CD3ReactElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'sankey',
				$strStyleID,
				true,  
				false); 
		}
		
		function RenderSankey($arrayData, $strSymbol = '',$nPrecision = 0)
		{
			$this->AddJavaScript('d3_sankey'); 
			$this->AddStyleSheet('d3-sankey');
			$this->AddJavaScript('react_sankey');

			$arrayProps = array();
			$arrayProps['height'] = intval($this->GetLayoutDef('LAYOUT_SVG_HEIGHT'));
			$arrayProps['width'] = intval($this->GetLayoutDef('LAYOUT_SVG_WIDTH'));
			$arrayProps['cssclassprefix'] = $this->GetLayoutDef('LAYOUT_CSSCLASSPREFIX');
			$arrayProps['data'] = $arrayData;
			$arrayProps['symbol'] = $strSymbol;
			$arrayProps['precision'] = $nPrecision;
			return $this->RenderReactComponent(
				'SankeyReactComponent',
				$arrayProps);
		}
	}

