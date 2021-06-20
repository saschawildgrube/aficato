<?php

	require_once(GetWDKDir().'wdk_element_react_d3.inc');

	class CZoomableSunburstElement extends CD3ReactElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'zoomablesunburst',
				$strStyleID,
				true,  
				false); 
		}
		
		function RenderZoomableSunburst($arrayData)
		{
			$arrayProps = array();
			$arrayProps['height'] = intval($this->GetLayoutDef('LAYOUT_SVG_HEIGHT'));
			$arrayProps['width'] = intval($this->GetLayoutDef('LAYOUT_SVG_WIDTH'));
			$arrayProps['cssclassprefix'] = $this->GetLayoutDef('LAYOUT_CSSCLASSPREFIX');
			
			if (ArrayGetValue($arrayData,'label') == '')
			{
				$arrayData['label'] = '';	
			}
			
			if (ArrayCount(ArrayGetValue($arrayData,'children')) == 0)
			{
				$arrayData['children'] = [];
			}
			
			
			$arrayProps['data'] = $arrayData;
			$this->AddJavaScript('d3_zoomablesunburst');
			$this->AddJavaScript('react_zoomablesunburst');
			return $this->RenderReactComponent(
				'ZoomableSunburstReactComponent',
				$arrayProps);
			
		}
	}

