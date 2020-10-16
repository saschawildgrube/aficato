<?php

	class CMyContent extends CContentDevtools
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule('devtools/sslcheck');
			$arrayConfig = array();
			$sslcheck = new CSSLCheckModule($this->GetWebsite(),'',$arrayConfig);
			$arrayItems['MODULE_SSLCHECK'] = $sslcheck->RenderModule();
		}
	}
	
