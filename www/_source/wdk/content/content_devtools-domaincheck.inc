<?php

	class CMyContent extends CContentDevtools
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("devtools/domaincheck");
			$arrayConfig = array();
			$domaincheck = new CDomainCheckModule($this->GetWebsite(),"",$arrayConfig);
			$arrayItems["MODULE_DOMAINCHECK"] = $domaincheck->RenderModule();
		}
	}
	
