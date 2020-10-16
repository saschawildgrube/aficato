<?php

	abstract class CContentAdmin extends CContent
	{
		function __construct(
			CWebSite $website,
			$strContentID)
		{
			parent::__construct($website, $strContentID);
		}
		
		function CallbackGetRequiredConditions()
		{
			return array("admin" => "INSUFFICIENT_PRIVILEGES");
		}
	}	
