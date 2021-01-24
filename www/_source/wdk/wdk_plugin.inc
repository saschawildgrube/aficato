<?php

	function IsValidPluginID($strPluginID)
	{
		if ($strPluginID == '')	
		{
			return false;	
		}
		if (StringCheckCharSet($strPluginID,CHARSET_PLUGINID) != true)
		{
			return false;	
		}
		if (StringCheckCharSet(StringSection($strPluginID,0,1),CHARSET_ALPHA_LOWERCASE) != true)
		{
			return false;
		}
		return true;
	}


