<?php

	class CMyContent extends CContentSystemAdmin
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("system/schedulermanager");
			$manager = new CSchedulerManagerModule($this->GetWebsite(),"");
			$arrayItems["MODULE_SCHEDULERMANAGER"] = $manager->RenderModule();
		}
	}
	
