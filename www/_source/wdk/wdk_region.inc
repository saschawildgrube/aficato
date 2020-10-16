<?php

	
	function IsValidRegionID($strRegion)
	{
		return ArrayValueExists(GetAllRegionIDs(),$strRegion);	
	}

	

	function GetAllRegionIDs()
	{
		return array(
			"EU");
	}

