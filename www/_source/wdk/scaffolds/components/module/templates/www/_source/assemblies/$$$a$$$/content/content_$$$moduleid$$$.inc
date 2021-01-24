<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule('$$$modulegroup$$$/$$$moduleid$$$');
			$arrayConfig = array();
			$arrayConfig['hello'] = 'world';
			$$$$moduleid$$$ = new C$$$ModuleID$$$Module($this->GetWebsite(),'',$arrayConfig);
			$arrayItems['MODULE_$$$MODULEID$$$'] = $$$$moduleid$$$->RenderModule();
		}
	}
	
