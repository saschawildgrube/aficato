<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule('cms/newsarticlemanager');
			$newsarticlemanager = new CNewsArticleManagerModule($this->GetWebsite(),'');
			$arrayItems['MODULE_NEWSARTICLEMANAGER'] = $newsarticlemanager->RenderModule();
		}
		
		function OnGetRequiredConditions()
		{
			return array('newseditor' => 'INSUFFICIENT_PRIVILEGES');
		}		
		
	}
	
