<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("$$$modulegroup$$$/$$$moduleid$$$");
			
			$arrayConfig = array();
			$$$$moduleid$$$ = new C$$$EntityName$$$ManagerModule($this->GetWebsite(),"",$arrayConfig);
			
			$arrayItems["MODULE_$$$MODULEID$$$"] = $$$$moduleid$$$->RenderModule();
		}
	}
	
