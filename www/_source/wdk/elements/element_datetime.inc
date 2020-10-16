<?php

	class CDateTimeElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"datetime",
				$strStyleID,
				false,
				true);
		}
		
		function RenderDateTime($time)
		{
			$strOutput = $this->GetLayoutHTML(); 
			$arrayItems["DATE"] = $this->GetWebsite()->RenderDate($time);
			$arrayItems["TIME"] = $this->GetWebsite()->RenderTime($time);
			$arrayItems["TIMEZONE"] = $this->GetWebsite()->GetTimeZone();
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;
		}
	}

