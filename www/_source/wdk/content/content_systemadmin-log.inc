<?php

	class CMyContent extends CContentSystemAdmin
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("system/logmanager");
			$logmanager = new CLogManagerModule($this->GetWebsite(),"");
			$arrayItems["MODULE_LOGMANAGER"] = $logmanager->RenderModule();
		}
	}
	
