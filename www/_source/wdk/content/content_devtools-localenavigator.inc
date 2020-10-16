<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule('devtools/localenavigator');
			$arrayConfig = array();
			$arrayConfig['hello'] = 'world';
			$localenavigator = new CLocaleNavigatorModule($this->GetWebsite(),'',$arrayConfig);
			$arrayItems['MODULE_LOCALENAVIGATOR'] = $localenavigator->RenderModule();
		}
	}
	
