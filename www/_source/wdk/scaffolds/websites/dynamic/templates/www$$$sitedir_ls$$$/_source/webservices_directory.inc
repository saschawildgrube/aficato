<?php
	
	function GetWebservicesDirectory($arrayWebservices = array())
	{
		if (is_array($arrayWebservices) == false)
		{
			$arrayWebservices = array();
		}
		
		$strWebservicesRootURL			= "https://".GetWebservicesURL();
		
		$arrayWebservices["system/server"]["url"]  				= $strWebservicesRootURL . "system/server/";
		$arrayWebservices["system/server"]["accesscode"]	= "$$$system-server-accesscode$$$";
		
		$arrayWebservices["system/log"]["url"]  				= $strWebservicesRootURL . "system/log/";
		$arrayWebservices["system/log"]["accesscode"]		= "$$$system-log-accesscode$$$";
		
		$arrayWebservices["system/test"]["url"] 				= $strWebservicesRootURL . "system/test/";
		$arrayWebservices["system/test"]["accesscode"]	= "$$$system-test-accesscode$$$";
		
		$arrayWebservices["system/data"]["url"] 				= $strWebservicesRootURL . "system/data/";
		$arrayWebservices["system/data"]["accesscode"]	= "$$$system-data-accesscode$$$";
		
		$arrayWebservices["system/session"]["url"] 				= $strWebservicesRootURL . "system/session/";
		$arrayWebservices["system/session"]["accesscode"]	= "$$$system-session-accesscode$$$";
		
		$arrayWebservices["system/user"]["url"] 				= $strWebservicesRootURL . "system/user/";
		$arrayWebservices["system/user"]["accesscode"]	= "$$$system-user-accesscode$$$";
		
		$arrayWebservices["system/entitlement"]["url"]				= $strWebservicesRootURL . "system/entitlement/";
		$arrayWebservices["system/entitlement"]["accesscode"]	= "$$$system-entitlement-accesscode$$$";
		
		$arrayWebservices["system/scheduler"]["url"] 				= $strWebservicesRootURL . "system/scheduler/";
		$arrayWebservices["system/scheduler"]["accesscode"] = "$$$system-scheduler-accesscode$$$";
		
		return $arrayWebservices;
	}
	



