<?php

	require_once('wdk_plugin.inc');


	class CWebsitePlugin extends CWebsiteSatellite
	{
	
		function __construct(
			CWebSite &$website,
			$strPluginID)
		{
			if (IsValidPluginID($strPluginID) == false)
			{
				FatalError("CPlugin has been initialized with an invalid plugin id.");
			}
			parent::__construct($website,$strPluginID);
		}
		
		function GetSatelliteType()
		{
			return 'Plugin';	
		}
	
	
		function OnInit()
		{
			$this->Trace('OnInit');
			return true;
		}
		
		function OnCreateImage(&$image,$strImageID)
		{
			$this->Trace('OnCreateImage');
			return false;
		}
		
		function OnUserChangePassword($strOldPassword,$strNewPassword)
		{
			$this->Trace('OnUserChangePassword');
			return true;
		}
		
		function OnRenderGenericOutputItem($strItem, $arrayAttributes)
		{
			$this->Trace('OnRenderGenericOutputItem');
			return '';
		}

		
	}



