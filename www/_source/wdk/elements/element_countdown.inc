<?php

	class CCountdownElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"countdown",
				$strStyleID,
				false,
				true);

			$website->AddJavaScript('jquery','jquery-1.11.2.min');
			$website->AddJavaScript('jquery-countdown','jquery.countdown.min');
			$website->AddJavaScript('wdk-element-countdown','wdk-element-countdown');			
		}
		
		function RenderCountdown($time)
		{
			$strOutput = $this->GetLayoutHTML();


			// How to configure the countdown: http://hilios.github.io/jQuery.countdown/documentation.html -->
			//$arrayItems["CONFIG"] = RenderDateTimeFormat($time, "%Y/%m/%d %H:%M:%S"/*,$this->GetWebsite()->GetTimeZone()*/);
			$arrayItems["CONFIG"] = ''.round($time*1000);
			$arrayItems["DATETIME_COMMENT"] = 'UTC: '.RenderDateTime($time).' / '.$this->GetWebsite()->GetTimeZone().': '.RenderDateTime($time,$this->GetWebsite()->GetTimeZone());
						 
			 
			$this->SetOutputItems($arrayItems);
			$strOutput = $this->ReplaceOutputItems($strOutput);
			return $strOutput;
		}
	}
