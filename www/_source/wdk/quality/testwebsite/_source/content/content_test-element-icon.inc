<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->IncludeElement("icon");
			$icon = new CIconElement($this->GetWebsite(), "default");
			$arrayItems["ELEMENT_ICON"] = $icon->RenderIcon("link","An icon without a link");
		}
	}
	
