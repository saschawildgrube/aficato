<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule('aml/caseanalysis');
			$arrayConfig = array();
			$caseanalysis = new CCaseAnalysisModule($this->GetWebsite(),'',$arrayConfig);
			$arrayItems['MODULE_CASEANALYSIS'] = $caseanalysis->RenderModule();
		}
	}
	
