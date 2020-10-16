<?php

	class CExampleElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"example",
				$strStyleID,
				false,
				true);
		}
		
		function RenderExample($strPayload)
		{
			$strOutput = $this->GetLayoutHTML(); 
			$arrayItems["PAYLOAD"] = $strPayload;
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;			
		}
	}

