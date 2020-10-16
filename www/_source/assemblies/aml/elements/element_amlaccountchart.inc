<?php

	require_once(GetWDKDir().'wdk_element_react_d3.inc');

	class CAMLAccountChartElement extends CD3ReactElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'amlaccountchart',
				$strStyleID,
				true,  
				false); 
		}
		
		function RenderAMLAccountChart($arrayData)
		{
			$this->AddJavaScript('d3_aml-accountchart');
			$this->GetWebsite()->AddStyleSheet('d3_aml-accountchart');			 

			$this->AddJavaScript('react_aml-accountchart');

			$arrayProps = array();
			$arrayProps['data'] = $arrayData;
			$arrayProps['height'] = intval($this->GetLayoutDef('LAYOUT_SVG_HEIGHT'));
			$arrayProps['width'] = intval($this->GetLayoutDef('LAYOUT_SVG_WIDTH'));
	
			return $this->RenderReactComponent(
				'AMLAccountChartD3ReactComponent',
				$arrayProps);
			
		}
	}

