<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$arrayConfig = array();
			$arrayConfig["defaultprivileges"] = $this->GetWebsite()->GetConfig("defaultprivileges");
			$arrayConfig["privileges"] = $this->GetWebsite()->GetConfig("privileges");
			
			$this->GetWebsite()->IncludeModule("system/usermanager");
			$usermanager = new CUserManagerModule($this->GetWebsite(),"",$arrayConfig);
			$arrayItems["MODULE_USERMANAGER"] = $usermanager->RenderModule();
		}
		
		function CallbackGetRequiredConditions()
		{
			return array("useradmin" => "INSUFFICIENT_PRIVILEGES");
		}

	}
	
