<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("cms/sitemap");
			$sitemap = new CSiteMapModule($this->GetWebsite());
			$arrayItems["MODULE_SITEMAP"] = $sitemap->RenderModule();
		}
	}
	
