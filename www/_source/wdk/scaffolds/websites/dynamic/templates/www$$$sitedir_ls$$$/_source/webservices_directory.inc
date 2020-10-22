<?php
	
	function GetWebservicesDirectory($arrayWebservices = array())
	{
		if (is_array($arrayWebservices) == false)
		{
			$arrayWebservices = array();
		}
		
		$strWebservicesRootURL			= 'https://'.GetWebservicesURL();
		
		$arrayWebservices['system/server']['url']  			= $strWebservicesRootURL . 'system/server/';
		$arrayWebservices['system/log']['url']  				= $strWebservicesRootURL . 'system/log/';
		$arrayWebservices['system/test']['url'] 				= $strWebservicesRootURL . 'system/test/';
		$arrayWebservices['system/data']['url'] 				= $strWebservicesRootURL . 'system/data/';
		$arrayWebservices['system/session']['url'] 			= $strWebservicesRootURL . 'system/session/';
		$arrayWebservices['system/user']['url'] 				= $strWebservicesRootURL . 'system/user/';
		$arrayWebservices['system/entitlement']['url']	= $strWebservicesRootURL . 'system/entitlement/';
		$arrayWebservices['system/scheduler']['url'] 		= $strWebservicesRootURL . 'system/scheduler/';
		
		return $arrayWebservices;
	}
	



