<?php

	abstract class CContentCMSAdmin extends CContent
	{
		function __construct(
			CWebSite $website,
			$strContentID)
		{
			parent::__construct($website, $strContentID);
		}
		
		function CallbackGetRequiredConditions()
		{
			return array("cmsadmin" => "INSUFFICIENT_PRIVILEGES");
		}
	}	
