<?php

	class CMyContent extends CContentLogin   
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$arrayItems["MODULE_USERSIGNOUT"] = $this->GetWebsite()->m_moduleSignOut->RenderModule("");
		}
	}
	
