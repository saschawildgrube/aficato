<?php

	class CGoogleMapsElement extends CElement
	{
		function __construct(
			CWebSite $website,
			$strStyleID)
		{
			parent::__construct(
				$website,
				"googlemaps",
				$strStyleID,
				true,
				true);
		}
		
		function RenderGoogleMaps($strLocation)
		{
			$strOutput = $this->GetLayoutHTML(); 
			
			$strApiKey = $this->GetWebsite()->GetConfig("googlemaps_apikey");
			if ($strApiKey == "")
			{
				$this->GetWebsite()->Error('Google Maps api key is missing. Please add the api key to the config: "googlemaps_apikey"');
			}
			
			$strLocation = StringURLEncode($strLocation);
			
			$nWidth = GetIntegerValue($this->GetLayoutDef("LAYOUT_WIDTH"));
			if ($nWidth < 10)
			{
				$nWidth = 600;
			}
			$nHeight = GetIntegerValue($this->GetLayoutDef("LAYOUT_HEIGHT"));
			if ($nHeight < 10)
			{
				$nHeight = 450;
			}
			$nWidthXs = GetIntegerValue($this->GetLayoutDef("LAYOUT_WIDTH_XS"));
			if ($nWidthXs < 10)
			{
				$nWidthXs = 330;
			}
			$nHeightXs = GetIntegerValue($this->GetLayoutDef("LAYOUT_HEIGHT_XS"));
			if ($nHeightXs < 10)
			{
				$nHeightXs = 275; 
			}

			
			$arrayItems = array();
			$arrayItems["QUERY"] = $strLocation;
			$arrayItems["APIKEY"] = $strApiKey;
			$arrayItems["WIDTH"] = $nWidth;
			$arrayItems["HEIGHT"] = $nHeight;
			$arrayItems["WIDTH_XS"] = $nWidthXs;
			$arrayItems["HEIGHT_XS"] = $nHeightXs;
			
			$strOutput = ReplaceTagsWDK($strOutput, $arrayItems);
			
			return $strOutput;
		}
	}

