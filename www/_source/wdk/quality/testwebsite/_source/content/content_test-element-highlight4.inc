<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$strText = u("Smith &amp; Wesson");
			$strToken = u("&amp;");
			$this->IncludeElement("highlight");
			$highlight = new CHighlightElement($this->GetWebsite(),"default");
			$arrayItems["ELEMENT_HIGHLIGHT"] = $highlight->RenderHighlight($strText,$strToken);

		}
	}
	
