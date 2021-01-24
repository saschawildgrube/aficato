<?php

	class CMyContent extends CContent
	{
		function OnRenderContent(&$arrayItems)
		{
			$strText = "<textarea>highlighted</textarea><p>This is some text with a highlighted text with some entities (&copy;&amp;&nbsp;).</p>";
			$strToken = "highlighted";
			$this->IncludeElement("highlight");
			$highlight = new CHighlightElement($this->GetWebsite(),"default");
			$arrayItems["ELEMENT_HIGHLIGHT"] = $highlight->RenderHighlight($strText,$strToken);

		}
	}
	
