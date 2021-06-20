<?php

	require_once(GetWDKDir().'wdk_element_react_d3.inc');

	class CNetworkElement extends CD3ReactElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'network',
				$strStyleID,
				true,  
				false); 
		}
		
		function RenderNetwork($arrayData)
		{
			$this->AddJavaScript('d3_network');
			$this->AddStyleSheet('d3-network');
			$this->AddJavaScript('react_network');

			$arrayProps = array();
			$arrayProps['height'] = intval($this->GetLayoutDef('LAYOUT_SVG_HEIGHT'));
			$arrayProps['width'] = intval($this->GetLayoutDef('LAYOUT_SVG_WIDTH'));
			$arrayProps['cssclassprefix'] = $this->GetLayoutDef('LAYOUT_CSSCLASSPREFIX');
			$arrayProps['data'] = $arrayData;
			return $this->RenderReactComponent(
				'NetworkReactComponent',
				$arrayProps);
			
		}
	}

