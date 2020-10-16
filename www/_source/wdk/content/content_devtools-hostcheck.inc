<?php

	class CMyContent extends CContentDevtools
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("devtools/hostcheck");
			$arrayConfig = array();
			$hostcheck = new CHostCheckModule($this->GetWebsite(),"",$arrayConfig);
			$arrayItems["MODULE_HOSTCHECK"] = $hostcheck->RenderModule();
		}
	}
	
