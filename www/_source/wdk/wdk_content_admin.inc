<?php

	abstract class CContentAdmin extends CContent
	{
		function __construct(
			CWebSite $website,
			$strContentID)
		{
			parent::__construct($website, $strContentID);
		}
		
		function OnGetRequiredConditions()
		{
			return array("admin" => "INSUFFICIENT_PRIVILEGES");
		}
	}	
