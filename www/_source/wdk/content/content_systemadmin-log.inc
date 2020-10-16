<?php

	class CMyContent extends CContentSystemAdmin
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("system/logmanager");
			$logmanager = new CLogManagerModule($this->GetWebsite(),"");
			$arrayItems["MODULE_LOGMANAGER"] = $logmanager->RenderModule();
		}
	}
	
