<?php

	require_once(GetWDKDir().'wdk_element_react_d3.inc');

	class CLineChartElement extends CD3ReactElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'linechart',
				$strStyleID,
				true,  
				false); 
		}
		
		function RenderLineChart($arrayData, $arrayConfig = array())
		{
			$this->AddJavaScript('d3_linechart');
			$this->AddStyleSheet('d3-linechart');			 

			$this->AddJavaScript('react_linechart');

			$arrayProps = array();
			$arrayProps['data'] = $arrayData;
			$arrayProps['config'] = $arrayConfig;
			$arrayProps['height'] = intval($this->GetLayoutDef('LAYOUT_SVG_HEIGHT'));
			$arrayProps['width'] = intval($this->GetLayoutDef('LAYOUT_SVG_WIDTH'));
			$arrayProps['cssclassprefix'] = $this->GetLayoutDef('LAYOUT_CSSCLASSPREFIX');
	
			return $this->RenderReactComponent(
				'LineChartD3ReactComponent',
				$arrayProps);
			
		}
	}

