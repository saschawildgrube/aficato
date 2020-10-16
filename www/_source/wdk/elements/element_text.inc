<?php

	class CTextElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"text",
				$strStyleID,
				false,
				true);
		}
		
		function RenderText($strHeadline, $strText)
		{
			$strOutput = $this->GetLayoutHTML(); 
		
			$arrayItems["HEADLINE"] = $strHeadline;
			$arrayItems["TEXT"] = $strText;
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

