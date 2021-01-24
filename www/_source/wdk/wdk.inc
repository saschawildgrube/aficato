<?php
	
	define('WDK_VERSION', '0.0.0.78');
	define('WDK_VERSION_DATE', '2021-01-17');
	define('WDK_COPYRIGHT_SINCE', '2005');
	
	function GetVersionInfoWDK()
	{
		$arrayVersion = array();
		$arrayVersion['NAME'] = 'WDK';
		$arrayVersion['VERSION'] = WDK_VERSION;
		$arrayVersion['VERSION_DATE'] = WDK_VERSION_DATE;
		$arrayVersion['BUILD_DATE'] = WDK_VERSION_DATE;
		$arrayVersion['VERBOSE'] = 'Website Development Kit (WDK) '.WDK_VERSION.((WDK_VERSION_DATE=='')?(' WORK IN PROGRESS'):(' as of '.WDK_VERSION_DATE));
		return $arrayVersion;
	}

