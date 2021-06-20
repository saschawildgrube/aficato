<?php

	class CHeaderElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"header",
				$strStyleID,
				true,
				false);
		}
		
		function RenderHeader($strHeader,$nLevel)
		{
			$nLevel = intval($nLevel);
			if ($nLevel < 1)
			{
				$nLevel = 1;	
			}
			if ($nLevel > 6)
			{
				$nLevel = 6;	
			}
			$strLayoutDef = $this->GetLayoutDef("LAYOUT_HEADER_".$nLevel);
			if ($strLayoutDef == "")
			{
				return $strHeader;	
			}
			$strOutput = ReplaceString($strLayoutDef,"{HEADER}",$strHeader);
			return $strOutput;
		}
	}

