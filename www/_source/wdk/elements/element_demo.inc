<?php

	class CDemoElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"demo",
				$strStyleID,
				true,
				true);
		}
		
		function RenderDemo($strText)
		{
			$strOutput = $this->GetLayoutHTML(); 
			
			$arrayItems = array();
			$arrayItems["TEXT"] = $strText;
			$arrayItems["BEGIN"] = $this->GetLayoutDef("LAYOUT_BEGIN");
			$arrayItems["END"] = $this->GetLayoutDef("LAYOUT_END");
			
			$strOutput = ReplaceTagsWDK($strOutput, $arrayItems);
			
			return $strOutput;
		}
	}

