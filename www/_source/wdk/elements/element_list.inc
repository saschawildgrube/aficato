<?php

	class CListElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"list",
				$strStyleID,
				true,
				false);
		}
		
		function RenderList($arrayListContent)
		{
			$strOutput = "";
			if (!is_array($arrayListContent))
			{
				$this->Error("RenderList: content array is not an array.");	
				return;
			}
			if (ArrayCount($arrayListContent) == 0)
			{
				return "";
			}

			
			$strOutput .= $this->GetLayoutDef("LAYOUT_LIST_BEGIN") . "\n";
		
						
			$nListElements = ArrayCount($arrayListContent);
			
			for ($nRowIndex = 0; $nRowIndex < $nListElements; $nRowIndex++)
			{
				$strOutput .= $this->GetLayoutDef("LAYOUT_LIST_ELEMENT_BEGIN");
				$strOutput .= $arrayListContent[$nRowIndex];
				$strOutput .= $this->GetLayoutDef("LAYOUT_LIST_ELEMENT_END") . "\n";
			}
			$strOutput .= $this->GetLayoutDef("LAYOUT_LIST_END") . "\n";
			return $strOutput;
		}
	}

