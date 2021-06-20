<?php

	class CNumberedListElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"numberedlist",
				$strStyleID,
				true,
				false);
		}
		
		function RenderNumberedList($arrayListContent, $nStart = 1)
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

			
			$strStart = "".intval($nStart);
			
			$strOutput .= ReplaceString(
				$this->GetLayoutDef("LAYOUT_LIST_BEGIN"),
				"{START}",
				$strStart)
				."\n";
		
						
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

