<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("aml/accountanalysis");
			$arrayConfig = array();
			$accountanalysis = new CAccountAnalysisModule($this->GetWebsite(),'',$arrayConfig);
			$arrayItems["MODULE_ACCOUNTANALYSIS"] = $accountanalysis->RenderModule();
		}
	}
	
