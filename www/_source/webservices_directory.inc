<?php
	
	function GetWebservicesDirectory($arrayWebservices = array())
	{
		if (is_array($arrayWebservices) == false)
		{
			$arrayWebservices = array();
		}
		
		$strWebservicesRootURL			= "https://".GetWebservicesURL();
		
		$arrayWebservices["system/server"]["url"]  				= $strWebservicesRootURL . "system/server/";
		$arrayWebservices["system/server"]["accesscode"]	= "5514242671132204218200142678499520626336";
		
		$arrayWebservices["system/log"]["url"]  				= $strWebservicesRootURL . "system/log/";
		$arrayWebservices["system/log"]["accesscode"]		= "1161525110174091253577571676966221631357";
		
		$arrayWebservices["system/test"]["url"] 				= $strWebservicesRootURL . "system/test/";
		$arrayWebservices["system/test"]["accesscode"]	= "3560754516056192751372870414741847704961";
		
		$arrayWebservices["system/data"]["url"] 				= $strWebservicesRootURL . "system/data/";
		$arrayWebservices["system/data"]["accesscode"]	= "5836247910156125459043670222099352628747";
		
		$arrayWebservices["system/session"]["url"] 				= $strWebservicesRootURL . "system/session/";
		$arrayWebservices["system/session"]["accesscode"]	= "1261632660312606442505483398612552478594";
		
		$arrayWebservices["system/user"]["url"] 				= $strWebservicesRootURL . "system/user/";
		$arrayWebservices["system/user"]["accesscode"]	= "1362634595665162901062042353554624646661";
		
		$arrayWebservices["system/entitlement"]["url"]				= $strWebservicesRootURL . "system/entitlement/";
		$arrayWebservices["system/entitlement"]["accesscode"]	= "8474431611419480899588592516753701533314";
		
		$arrayWebservices["system/scheduler"]["url"] 				= $strWebservicesRootURL . "system/scheduler/";
		$arrayWebservices["system/scheduler"]["accesscode"] = "5431600539456317985503259395686301575552";
		
		return $arrayWebservices;
	}
	



