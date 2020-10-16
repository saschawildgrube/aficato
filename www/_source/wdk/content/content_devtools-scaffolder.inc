<?php

	class CMyContent extends CContentDevtools
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("devtools/scaffolder");
			$scaffolder = new CScaffolderModule($this->GetWebsite());
			$arrayItems["SCAFFOLDER"] = $scaffolder->RenderModule();
		}
	}
	
