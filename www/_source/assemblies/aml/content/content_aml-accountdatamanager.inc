<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("aml/accountdatamanager");
			$arrayConfig = array();
			$accountdatamanager = new CAccountDataManagerModule($this->GetWebsite(),'',$arrayConfig);
			$arrayItems['MODULE_ACCOUNTDATAMANAGER'] = $accountdatamanager->RenderModule();
		}
	}
	
