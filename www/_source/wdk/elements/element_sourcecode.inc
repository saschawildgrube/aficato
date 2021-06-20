<?php

	class CSourceCodeElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"sourcecode",
				$strStyleID,
				false,
				true);
		}
		
		function RenderSourceCode($strSourceCode, $strSourceCodeLanguage = "")
		{
			$strOutput = $this->GetLayoutHTML(); 
		
			$arrayItems["SOURCECODE"] = $strSourceCode;
			
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			

		}
	}

