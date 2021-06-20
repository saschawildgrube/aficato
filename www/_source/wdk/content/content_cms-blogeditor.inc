<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("cms/blogarticlemanager");
			$blogarticlemanager = new CBlogArticleManagerModule($this->GetWebsite(),"");
			$arrayItems["MODULE_BLOGARTICLEMANAGER"] = $blogarticlemanager->RenderModule();
		}
		
		function OnGetRequiredConditions()
		{
			return array("blogeditor" => "INSUFFICIENT_PRIVILEGES");
		}		
		
	}
	
