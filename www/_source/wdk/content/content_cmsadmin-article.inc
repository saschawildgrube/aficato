<?php

	class CMyContent extends CContentCMSAdmin
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("cms/articlemanager");
			$articlemanager = new CArticleManagerModule($this->GetWebsite(),"");
			$arrayItems["MODULE_ARTICLEMANAGER"] = $articlemanager->RenderModule();
		}
		
		function CallbackGetRequiredConditions()
		{
			return array("cmsadmin" => "INSUFFICIENT_PRIVILEGES");
		}				
		
	}
	
