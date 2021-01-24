<?php

	class CMyContent extends CContentSystemAdmin
	{
		function OnRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("system/schedulermanager");
			$manager = new CSchedulerManagerModule($this->GetWebsite(),"");
			$arrayItems["MODULE_SCHEDULERMANAGER"] = $manager->RenderModule();
		}
	}
	
