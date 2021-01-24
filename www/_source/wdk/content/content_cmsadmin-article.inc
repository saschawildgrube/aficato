<?php

	class CMyContent extends CContentCMSAdmin
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("cms/articlemanager");
			$articlemanager = new CArticleManagerModule($this->GetWebsite(),"");
			$arrayItems["MODULE_ARTICLEMANAGER"] = $articlemanager->RenderModule();
		}
		
		function OnGetRequiredConditions()
		{
			return array("cmsadmin" => "INSUFFICIENT_PRIVILEGES");
		}				
		
	}
	
