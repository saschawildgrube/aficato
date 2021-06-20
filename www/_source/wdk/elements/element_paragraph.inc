<?php

	class CParagraphElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"paragraph",
				$strStyleID,
				false,
				true);
		}
		
		function RenderParagraph($strParagraph)
		{
			$strOutput = $this->GetLayoutHTML(); 
			$arrayItems["PARAGRAPH"] = $strParagraph;
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			
		}
	}

