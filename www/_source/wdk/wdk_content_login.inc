<?php

	abstract class CContentLogin extends CContent
	{
		function __construct(
			CWebSite $website,
			$strContentID)
		{
			parent::__construct($website, $strContentID);
		}

		function OnGetRequiredConditions()
		{
			return array("login" => "LOGIN_REQUIRED");
		}

	}	
