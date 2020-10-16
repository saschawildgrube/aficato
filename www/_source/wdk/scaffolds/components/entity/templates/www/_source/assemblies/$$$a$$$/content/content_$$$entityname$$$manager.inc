<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("$$$modulegroup$$$/$$$moduleid$$$");
			
			$arrayConfig = array();
			$$$$moduleid$$$ = new C$$$EntityName$$$ManagerModule($this->GetWebsite(),"",$arrayConfig);
			
			$arrayItems["MODULE_$$$MODULEID$$$"] = $$$$moduleid$$$->RenderModule();
		}
	}
	
