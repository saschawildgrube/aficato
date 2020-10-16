<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$this->IncludeElement("linkicon");
			$linkicon = new CLinkIconElement($this->GetWebsite(), "default");
			$arrayItems["ELEMENT_LINKICON_NOTEXT"] = $linkicon->RenderLinkIcon("http://www.example.com","link");
			$arrayItems["ELEMENT_LINKICON"] = $linkicon->RenderLinkIcon("http://www.example.com","link","Example");
			$arrayItems["ELEMENT_LINKICON_EXTERNAL"] = $linkicon->RenderLinkIcon("http://www.example.com","linkexternal","Example (external)",true);
		}
	}
	
