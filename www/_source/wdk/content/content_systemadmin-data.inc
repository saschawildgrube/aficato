<?php

	class CMyContent extends CContentSystemAdmin
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("system/datamanager");
			$datamanager = new CDataManagerModule($this->GetWebsite(),"list");
			$arrayItems["MODULE_DATAMANAGER"] = $datamanager->RenderModule();
		}
	}
	
