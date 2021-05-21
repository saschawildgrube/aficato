<?php

	require_once(GetWDKDir().'wdk_element_react.inc');

	class CAMLRedFlagsLegendElement extends CReactElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				'amlredflagslegend',
				$strStyleID,
				false,
				false);
		}
		
		function RenderAMLRedFlagsLegend($arraySupportedTypologyIDs, $arrayDetectedTypologyIDs)
		{
			$this->AddJavaScript('react_iconlegend');
			$this->AddJavaScript('react_aml-redflagslegend');
			
			if (ArrayCount($arraySupportedTypologyIDs) == 0)
			{
				$arraySupportedTypologyIDs = array();
			}
			if (ArrayCount($arrayDetectedTypologyIDs) == 0)
			{
				$arrayDetectedTypologyIDs = array();
			}

			
			$arrayProps = array();
			$arrayProps['supportedtypologyids'] = array_values($arraySupportedTypologyIDs);
			$arrayProps['detectedtypologyids'] = array_values($arrayDetectedTypologyIDs);
			
			return $this->RenderReactComponent(
				'AMLRedFlagsLegendReactComponent',
				$arrayProps);
		}
	}

