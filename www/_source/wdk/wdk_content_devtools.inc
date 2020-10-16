<?php

	abstract class CContentDevtools extends CContent
	{
		function __construct(
			CWebSite $website,
			$strContentID)
		{
			parent::__construct($website, $strContentID);
		}
		
		function CallbackGetRequiredConditions()
		{
			return array("devtools" => "INSUFFICIENT_PRIVILEGES");
		}
	}	
