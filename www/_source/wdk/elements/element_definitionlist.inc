<?php

	class CDefinitionListElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"definitionlist",
				$strStyleID,
				true,
				false);
		}
		
		function RenderDefinitionList($arrayList)
		{
			$strOutput = "";
			if (!is_array($arrayList))
			{
				$this->Error("RenderList: a44ayList is not an array.");	
				return;
			}
			if (ArrayCount($arrayList) == 0)
			{
				return "";
			}

			$strOutput .= $this->GetLayoutDef("LAYOUT_LIST_BEGIN") . "\n";
			foreach ($arrayList as $strTerm => $arrayDefinitions)
			{
				$strOutput .= $this->GetLayoutDef("LAYOUT_LIST_TERM_BEGIN");
				$strOutput .= $strTerm;
				$strOutput .= $this->GetLayoutDef("LAYOUT_LIST_TERM_END") . "\n";
				
				if (is_array($arrayDefinitions))
				{
					foreach ($arrayDefinitions as $strDefinition)
					{
						$strOutput .= $this->GetLayoutDef("LAYOUT_LIST_DEFINITION_BEGIN");
						$strOutput .= $strDefinition;
						$strOutput .= $this->GetLayoutDef("LAYOUT_LIST_DEFINITION_END") . "\n";						
					}	
				}
				else
				{
					$strOutput .= $this->GetLayoutDef("LAYOUT_LIST_DEFINITION_BEGIN");
					$strOutput .= $arrayDefinitions;
					$strOutput .= $this->GetLayoutDef("LAYOUT_LIST_DEFINITION_END") . "\n";						
					
				}
			}
			$strOutput .= $this->GetLayoutDef("LAYOUT_LIST_END") . "\n";
			return $strOutput;
		}
	}

