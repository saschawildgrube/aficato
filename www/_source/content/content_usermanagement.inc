<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$arrayConfig = array();
			$arrayConfig["defaultprivileges"] = $this->GetWebsite()->GetConfig("defaultprivileges");
			$arrayConfig["privileges"] = $this->GetWebsite()->GetConfig("privileges");
			
			$this->GetWebsite()->IncludeModule("system/usermanager");
			$usermanager = new CUserManagerModule($this->GetWebsite(),"",$arrayConfig);
			$arrayItems["MODULE_USERMANAGER"] = $usermanager->RenderModule();
		}
		
		function OnGetRequiredConditions()
		{
			return array("useradmin" => "INSUFFICIENT_PRIVILEGES");
		}

	}
	
