<?php

	class CSeparatorElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"separator",
				$strStyleID,
				false,
				true);
		}
		
		function RenderSeparator()
		{
			$strOutput = $this->GetLayoutHTML();
			return $strOutput;
		}
	}

