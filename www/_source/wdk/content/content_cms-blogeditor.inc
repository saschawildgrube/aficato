<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("cms/blogarticlemanager");
			$blogarticlemanager = new CBlogArticleManagerModule($this->GetWebsite(),"");
			$arrayItems["MODULE_BLOGARTICLEMANAGER"] = $blogarticlemanager->RenderModule();
		}
		
		function CallbackGetRequiredConditions()
		{
			return array("blogeditor" => "INSUFFICIENT_PRIVILEGES");
		}		
		
	}
	
