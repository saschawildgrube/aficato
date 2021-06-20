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
		
		function SetOutputItem($strItem, $strContents)
		{
			$this->GetWebsite()->SetOutputItem($strItem, $strContents);	
		}
		function SetOutputItems($arrayItems)
		{
			$this->GetWebsite()->SetOutputItems($arrayItems);	
		}
		
		function OnInit()
		{
			$this->Trace('OnInit');
			return true;
		}
		
		function OnGetWebsiteTitle()
		{
			$this->Trace('OnGetWebsiteTitle');
			return false;
		}	
		
		function OnGetWebsiteDescription()
		{
			$this->Trace('OnGetWebsiteDescription');
			return false;
		}
		
		function OnGetOpenGraphImageURL()
		{
			$this->Trace('OnGetOpenGraphImageURL');
			return false;
		}
		
		function OnCreateImage(&$image,$strImageID)
		{
			$this->Trace('OnCreateImage');
			return false;
		}
		
		function OnUserDelete($strUserID)
		{
			$this->Trace('OnUserDelete');
			return true;
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



