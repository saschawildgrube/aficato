<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$link = new CLinkElement($this->GetWebsite(), "default");
			$arrayItems["ELEMENT_LINK_NOTEXT"] = $link->RenderLink("http://www.example.com");
			$arrayItems["ELEMENT_LINK"] = $link->RenderLink("http://www.example.com","Example");
			$arrayItems["ELEMENT_LINK_EXTERNAL"] = $link->RenderLink("http://www.example.com","Example (external)",true);
		}
	}
	
