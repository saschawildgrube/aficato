<?php

	class CMyContent extends CContentDevtools
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("devtools/cookiemanager");
			$cookiemanager = new CCookieManagerModule($this->GetWebsite(),"list");
			$arrayItems["MODULE_COOKIEMANAGER"] = $cookiemanager->RenderModule();
		}
	}
	
