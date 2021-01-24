<?php

	class CMyContent extends CContentDevtools
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("devtools/httprequest");
			$arrayConfig = array();
			$httprequest = new CHttpRequestModule($this->GetWebsite(),"",$arrayConfig);
			$arrayItems["MODULE_HTTPREQUEST"] = $httprequest->RenderModule();
		}
	}
	
