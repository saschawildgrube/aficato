<?php

	class CMyContent extends CContentSystemAdmin
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("system/testmanager");
			$testmanager = new CTestManagerModule($this->GetWebsite(),"");
			$arrayItems["MODULE_TESTMANAGER"] = $testmanager->RenderModule();
		}
	}
	
