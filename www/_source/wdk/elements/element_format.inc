<?php

	class CFormatElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"format",
				$strStyleID,
				false,
				true);
		}
		
		function RenderFormat($strText)
		{
			$strOutput = $this->GetLayoutHTML(); 
			$arrayItems["TEXT"] = $strText;
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;
		}
	}

