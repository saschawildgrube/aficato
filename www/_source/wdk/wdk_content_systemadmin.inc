<?php

	abstract class CContentSystemAdmin extends CContent
	{
		function __construct(
			CWebSite $website,
			$strContentID)
		{
			parent::__construct($website, $strContentID);
		}
		
		function OnGetRequiredConditions()
		{
			return array("systemadmin" => "INSUFFICIENT_PRIVILEGES");
		}
	}	
