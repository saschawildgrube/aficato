<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$strText = "A highlighted text with a line<br/>break";
			$strToken = "high";
			$this->IncludeElement("highlight");
			$highlight = new CHighlightElement($this->GetWebsite(),"default");
			$arrayItems["ELEMENT_HIGHLIGHT"] = $highlight->RenderHighlight($strText,$strToken);

		}
	}
	
