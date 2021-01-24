<?php

	class CMyContent extends CContentLogin   
	{
		function OnRenderContent(&$arrayItems)
		{
			$arrayItems["MODULE_USERSIGNOUT"] = $this->GetWebsite()->m_moduleSignOut->RenderModule("");
		}
	}
	
