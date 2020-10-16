<?php

	require_once(GetWDKDir().'wdk_element_react.inc');

	class CAMLAccountChartLegendElement extends CReactElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"amlaccountchartlegend",
				$strStyleID,
				false,
				false);
		}
		
		function RenderAMLAccountChartLegend()
		{
			$this->AddJavaScript('react_colorlegend');
			$this->AddJavaScript('react_aml-accountchartlegend');
			
			$arrayTransactionTypes = AML_GetDefaultTransactionTypes();
			foreach ($arrayTransactionTypes as &$strType)
			{
				if ($strType == '*')
				{
					$strTYPE = 'OTHER';	
				}	
			}
			
			$arrayProps = array();
			$arrayProps['transactiontypes'] = $arrayTransactionTypes;

			return $this->RenderReactComponent(
				'AMLAccountChartLegendReactComponent',
				$arrayProps);
		}
	}

