<?php

	class CMyContent extends CContentSystemAdmin
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$arrayConfig = array();
			$arrayConfig["privileges"] = $this->GetWebsite()->GetConfig("privileges");
			$arrayConfig["silo_id"] = "0";
			
			$this->GetWebsite()->IncludeModule("system/usermanager");
			$usermanager = new CUserManagerModule($this->GetWebsite(),"",$arrayConfig);
			$arrayItems["MODULE_USERMANAGER"] = $usermanager->RenderModule();
		}
	}
	
