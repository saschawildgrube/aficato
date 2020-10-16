<?php

	require_once(GetWDKDir().'wdk_element_react_d3.inc');

	class CDivergingStackedBarChartElement extends CD3ReactElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'divergingstackedbarchart',
				$strStyleID,
				true,  
				false); 
		}
		
		function RenderDivergingStackedBarChart($arrayData,$arrayLegends)
		{
			$this->AddJavaScript('d3_divergingstackedbarchart');
			$this->AddStyleSheet('d3-divergingstackedbarchart');
			$this->AddJavaScript('react_divergingstackedbarchart');

			$arrayProps = array();
			$arrayProps['height'] = intval($this->GetLayoutDef('LAYOUT_SVG_HEIGHT'));
			$arrayProps['width'] = intval($this->GetLayoutDef('LAYOUT_SVG_WIDTH'));
			$arrayProps['data'] = $arrayData;
			$arrayProps['legends'] = $arrayLegends;
			return $this->RenderReactComponent(
				'DivergingStackedBarChartReactComponent',
				$arrayProps);
			
		}
	}

