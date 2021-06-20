<?php
	
	/*
		Severity levels as defined in RFC 3164 (except TEST* and UNKNOWN)

		The severity level TEST is used for
		general testing purposes.
		
		The severity levels TESTWHITELIST and
		TESTBLACKLIST are used to test the
		severity level filtering feature.
		
		TEST* severities are not defined in RFC 3164.
		
		Refer to:
		http://www.ietf.org/rfc/rfc3164.txt
	*/
	
	define("SEVERITY_EMERGENCY",			0);
	define("SEVERITY_ALERT",					1);
	define("SEVERITY_CRITICAL",				2);
	define("SEVERITY_ERROR",					3);
	define("SEVERITY_WARNING",				4);
	define("SEVERITY_NOTICE",					5);
	define("SEVERITY_INFORMATIONAL",	6);
	define("SEVERITY_DEBUG",					7);
	define("SEVERITY_TEST",						100);
	define("SEVERITY_TESTWHITELIST",	101);
	define("SEVERITY_TESTBLACKLIST",	102);
	define("SEVERITY_UNKNOWN",				1000);
	
	function GetEventSeverities($bAddTestSeverities = true,$bAddUnknown = false)
	{
		$arraySeverities = array(
			SEVERITY_EMERGENCY 			=> "EMERGENCY",
			SEVERITY_ALERT 					=> "ALERT",
			SEVERITY_CRITICAL 			=> "CRITICAL",
			SEVERITY_ERROR 					=> "ERROR",
			SEVERITY_WARNING 				=> "WARNING",
			SEVERITY_NOTICE 				=> "NOTICE",
			SEVERITY_INFORMATIONAL 	=> "INFORMATIONAL",
			SEVERITY_DEBUG					=> "DEBUG"
			);
		if ($bAddTestSeverities == true)
		{
			$arraySeverities[SEVERITY_TEST]						= "TEST";
			$arraySeverities[SEVERITY_TESTWHITELIST]	= "TESTWHITELIST";
			$arraySeverities[SEVERITY_TESTBLACKLIST]	= "TESTBLACKLIST";
		}
		$arraySeverities[SEVERITY_UNKNOWN] = "UNKNOWN";
		return $arraySeverities;
	}
	
	function GetEventSeverityNumeric($strEventSeverity)
	{
		$arraySeverities = GetEventSeverities(true,false);
		foreach ($arraySeverities as $key => $value)
		{
			if ($strEventSeverity == $value)
			{
				return $key;	
			}	
		}
		return SEVERITY_UNKNOWN;
	}
	
	
