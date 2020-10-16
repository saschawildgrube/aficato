<?php

	class CMyContent extends CContentSystemAdmin
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("cms/staticfilebrowser");
			$arrayConfig = array();
			$arrayConfig["rootdir"] = GetDocumentRootDir();
			//$arrayConfig["hidesize"] = true;
			//$arrayConfig["hidelastchange"] = true;
			//$arrayConfig["hideicon"] = true;
			$arrayConfig["hideinfo"] = true;
			$staticfilebrowser = new CStaticFileBrowserModule($this->GetWebsite(),"documentroot",$arrayConfig);
			$arrayItems["MODULE_STATICFILEBROWSER"] = $staticfilebrowser->RenderModule();
		}
	}
	
