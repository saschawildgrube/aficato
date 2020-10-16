<?php

	require_once(GetWDKDir().'wdk_element_react_d3.inc');

	class CParallelSetsElement extends CD3ReactElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'parallelsets',
				$strStyleID,
				true,  
				false); 
		}
		
		function RenderParallelSets($arrayDimensions, $arrayData, $strValueAttribute = '', $strSymbol = '', $nPrecision = 0)
		{
			$this->AddJavaScript('d3_parallelsets'); 
			$this->AddStyleSheet('d3-parallelsets');
			$this->AddJavaScript('react_parallelsets');

			$arrayProps = array();
			$arrayProps['height'] = intval($this->GetLayoutDef('LAYOUT_SVG_HEIGHT'));
			$arrayProps['width'] = intval($this->GetLayoutDef('LAYOUT_SVG_WIDTH'));
			$arrayProps['dimensions'] = $arrayDimensions;  
			$arrayProps['data'] = $arrayData;
			$arrayProps['valueattribute'] = $strValueAttribute;
			$arrayProps['symbol'] = $strSymbol;
			$arrayProps['precision'] = $nPrecision;
			$arrayProps['tension'] = floatval($this->GetLayoutDef('LAYOUT_TENSION'));
			return $this->RenderReactComponent(
				'ParallelSetsReactComponent',
				$arrayProps);
			
		}
	}

