<?php

	class CHighlightElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"highlight",
				$strStyleID,
				true,
				false);
		}
		
		function RenderHighlight($strText,$strToken)
		{
			$strOutput = StringHTMLAddEmbracingTags(
				$strText,
				$strToken,
				$this->GetLayoutDef("LAYOUT_HIGHLIGHT_TAG_START"),
				$this->GetLayoutDef("LAYOUT_HIGHLIGHT_TAG_END"),
				true,
				array(),
				array("input","textarea","meta")
				);
				
				$this->Trace($strOutput);
				
			return $strOutput;
		}
	}

