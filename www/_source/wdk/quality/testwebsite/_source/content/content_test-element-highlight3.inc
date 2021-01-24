<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$strText = u("Here are some German umlauts: ÄÖÜäöü");
			$strToken = u("Ä");
			$this->IncludeElement("highlight");
			$highlight = new CHighlightElement($this->GetWebsite(),"default");
			$arrayItems["ELEMENT_HIGHLIGHT"] = $highlight->RenderHighlight($strText,$strToken);

		}
	}
	
