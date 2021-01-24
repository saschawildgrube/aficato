<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$arrayItems["MODULE_SIGNIN"] = $this->GetWebsite()->m_moduleSignin->RenderModule("");
			$arrayItems["MODULE_SIGNINONESTEP"] = $this->GetWebsite()->m_moduleSigninOneStep->RenderModule("");

		}
	}
	
