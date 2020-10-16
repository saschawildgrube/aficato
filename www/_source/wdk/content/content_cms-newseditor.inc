<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("cms/newsarticlemanager");
			$newsarticlemanager = new CNewsArticleManagerModule($this->GetWebsite(),"");
			$arrayItems["MODULE_NEWSARTICLEMANAGER"] = $newsarticlemanager->RenderModule();
		}
		
		function CallbackGetRequiredConditions()
		{
			return array("newseditor" => "INSUFFICIENT_PRIVILEGES");
		}		
		
	}
	
