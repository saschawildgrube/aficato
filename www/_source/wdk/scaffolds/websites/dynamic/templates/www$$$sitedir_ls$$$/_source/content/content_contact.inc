<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->GetWebsite()->IncludeModule("crm/contact");
			$contact = new CContactModule($this->GetWebsite(),"");
			$arrayItems["MODULE_CONTACT"] = $contact->RenderModule();
		}
	}
	
