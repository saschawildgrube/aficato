<?php

	class CMyContent extends CContentLogin
	{
		function OnRenderContent(&$arrayItems)
		{
		
			$this->GetWebsite()->IncludeModule("user/usersettings");
			$usersettings = new CUserSettingsModule($this->GetWebsite());
			$arrayItems["MODULE_USERSETTINGS"] = $usersettings->RenderModule("");

			$passwordpolicy = new CPasswordPolicy();
			$passwordpolicy->SetPolicy($this->GetConfig("passwordpolicy"));

			$this->GetWebsite()->IncludeModule("user/userpassword");
			$userpassword = new CUserPasswordModule($this->GetWebsite(),$passwordpolicy);
			$arrayItems["MODULE_USERPASSWORD"] = $userpassword->RenderModule("");

			$arrayItems["MODULE_USEREMAIL"] = $this->GetWebsite()->m_moduleUserEmail->RenderModule("");
			
			$arrayItems["MODULE_USERSIGNOUT"] = $this->GetWebsite()->m_moduleSignOut->RenderModule("");
		}
	}
	
