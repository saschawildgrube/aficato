<?php

	class CMyContent extends CContent
	{
		function CallbackRenderContent(&$arrayItems)
		{
			$strText = u("<p>Line<br/>Break</p>");
			$strToken = u("B");
			$this->IncludeElement("highlight");
			$highlight = new CHighlightElement($this->GetWebsite(),"default");
			$arrayItems["ELEMENT_HIGHLIGHT"] = $highlight->RenderHighlight($strText,$strToken);

		}
	}
	
