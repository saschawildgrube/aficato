<?php

	require_once("wdk_ip.inc");

	function ScanMailWarnLogForMaliciousIPs($arrayMailWarnLog)
	{
		if (!is_array($arrayMailWarnLog))
		{
			return false;	
		}

		$arrayIPs = array();
		foreach ($arrayMailWarnLog as $strLogLine)
		{
			if (FindString($strLogLine,"password incorrect from") != -1)
			{
				$nPos = strrpos($strLogLine,"[");
				if ($nPos != -1)
				{
					$strIP = StringSection($strLogLine,$nPos+1,-1);
					if (IsValidIPv4($strIP))
					{
						$arrayIPs[$strIP] = 1 + ArrayGetValue($arrayIPs,$strIP);
					}
				}
			}
		}
		return $arrayIPs;
	}	
	
		
