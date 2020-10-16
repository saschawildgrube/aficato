<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$arrayConfig = array();
			$this->GetWebsite()->IncludeModule("system/servermanager");
			$servermanager = new CServerManagerModule($this->GetWebsite(),"dashboard",$arrayConfig);
			$arrayItems["MODULE_SERVERMANAGER"] = $servermanager->RenderModule();
		}
	}
	
