<?php

	class CMyContent extends CContentDevtools
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("devtools/domaincheck");
			$arrayConfig = array();
			$domaincheck = new CDomainCheckModule($this->GetWebsite(),"",$arrayConfig);
			$arrayItems["MODULE_DOMAINCHECK"] = $domaincheck->RenderModule();
		}
	}
	
