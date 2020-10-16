<?php

	require_once(GetWDKDir().'wdk_element_react_d3.inc');

	class CAMLNetworkGraphElement extends CD3ReactElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'amlnetworkgraph',
				$strStyleID,
				true,  
				false); 
		}
		
		function RenderAMLNetworkGraph($arrayData)
		{
			$this->AddJavaScript('d3_aml-networkgraph');
			$this->GetWebsite()->AddStyleSheet('d3_aml-networkgraph');			 

			$this->AddJavaScript('react_aml-networkgraph');

			$arrayProps = array();
			$arrayProps['data'] = $arrayData;
			$arrayProps['height'] = intval($this->GetLayoutDef('LAYOUT_SVG_HEIGHT'));
			$arrayProps['width'] = intval($this->GetLayoutDef('LAYOUT_SVG_WIDTH'));
			$arrayProps['hidetext'] = intval($this->GetLayoutDef('LAYOUT_HIDETEXT'));
	
			return $this->RenderReactComponent(
				'AMLNetworkGraphD3ReactComponent',
				$arrayProps);
			
		}
	}

