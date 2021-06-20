<?php


	class CMyContent extends CContentSystemAdmin
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("system/sessionmanager");
			$sessionmanager = new CSessionManagerModule($this->GetWebsite(),"");
			$arrayItems["MODULE_SESSIONMANAGER"] = $sessionmanager->RenderModule();
		}
	}
	
