<?php

	require_once(GetWDKDir()."wdk_csv.inc");

	define("DOMAIN_STATUS_UNKNOWN",		0);
	define("DOMAIN_STATUS_FREE",			1);
	define("DOMAIN_STATUS_TAKEN",			2);
	define("DOMAIN_STATUS_FORSALE",		3);
	define("DOMAIN_STATUS_INVALID",		4);
	define("DOMAIN_STATUS_UNDEFINED",	5);
	
	function GetDomainStatusArray()
	{
		$arrayDomainStatus = array(
			DOMAIN_STATUS_UNKNOWN => "UNKNOWN",
			DOMAIN_STATUS_FREE => "FREE",
			DOMAIN_STATUS_TAKEN => "TAKEN",
			DOMAIN_STATUS_FORSALE => "FORSALE",
			DOMAIN_STATUS_INVALID => "INVALID",
			DOMAIN_STATUS_UNDEFINED => "UNDEFINED"
			);
		return $arrayDomainStatus;
	}


	function IsValidIPv4($strIP)
	{
		$strIPv4regex = "\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b";
		if (RegExpMatch($strIP,"/^$strIPv4regex$/") == false) 
		//if (preg_match("/^$strIPv4regex$/",$strIP) == false) 
		{ 
			return false; 
	  } 
		return true;
	}
	

	function GetIPFromHostName($strHostname)
	{
		require_once(GetWDKDir().'wdk_shell.inc');
		$strIP = '';
		$arrayResult = array();
		$bResult = ShellExecute(
			'nslookup -timeout=3 -retry=1 '.$strHostname,
			array(),
			array(),
			'',
			$arrayResult);
		if ($bResult == false)
		{
			return false;	
		}
		$strResult = $arrayResult['stdout'];
   	if (preg_match('/\nAddress: (.*)\n/', $strResult, $arrayMatches))
   	{
      $strIP = StringTrim(ArrayGetValue($arrayMatches,1));
    }
    if ($strIP == '')
    {
    	return false;
    }
		if ($strIP == $strHostname)
		{
			return false;
		}
		return $strIP;
	}
	
	function GetDnsRecords($strHostname)
	{
		$ea = new CErrorAmnesty();
		$ea->Start();
		$arrayRecordsRaw = dns_get_record($strHostname,DNS_ALL);
		if ($arrayRecordsRaw == false)
		{
			// For unknown reasons DNS_ALL does not work in the Digital Ocean droplet. But the following will do:
			$arrayRecordsRaw = dns_get_record($strHostname,DNS_ALL - DNS_A6);
		}
		$ea->End();


		if ($arrayRecordsRaw == false)
		{
			return false;	
		}
		if (ArrayCount($arrayRecordsRaw) == 0)
		{
			return false;	
		}
		$arrayRecords = array();
		foreach ($arrayRecordsRaw as $arrayRecordRaw)
		{
			$strType = ArrayGetValue($arrayRecordRaw,"type");
			$arrayRecordRaw = ArrayRemoveKey($arrayRecordRaw,"type");
			$arrayRecordRaw = ArrayRemoveKey($arrayRecordRaw,"host");
			$arrayRecords[$strType][] = $arrayRecordRaw;
		}
		return $arrayRecords;		 
	}
	
	function IsValidHostName($strHostName)
	{
		if ($strHostName == "")
		{
			return false;	
		}
		if (StringCheckCharSet($strHostName,CHARSET_HOSTNAME) == false)
		{
			return false;
		}
    $strHostRegex = "(([a-z0-9-.]*)\.([a-z]{2,20})|localhost)"; // Host with TLD or "localhost"
		if (preg_match("/^$strHostRegex$/", StringLowerCase($strHostName)) == false) 
		{ 
			return false; 
	  } 
	  if (StringSection($strHostName,0,1) == ".")
	  {
	  	return false;	
	  }
	  if (FindString($strHostName,"..") != -1)
	  {
	  	return false;
	  }
		
		return true;
	}
	
	function GetHostNameFromIP($strIP)
	{
		if (IsValidIPv4($strIP) == false)
		{
			return false;	
		}
		require_once(GetWDKDir().'wdk_shell.inc');
		$arrayResult = array();
		$bResult = ShellExecute(
			'host -W 2 '.$strIP,
			array(),
			array(),
			'',
			$arrayResult);
		if ($bResult == false)
		{
			return false;	
		}
		$strResult = $arrayResult['stdout'];
		
		$nPos = FindString($strResult,"domain name pointer ");
		if ($nPos == -1)
		{
			return false;	
		}
		$strHostName = StringSection($strResult,$nPos+20,-2);
		if (IsValidHostName($strHostName) == false)
		{
			return false;	
		}
		return $strHostName;
	}	
	
	function GetWhoisRecords($strHost,&$strWhoisServer = "")
	{
		$strWhoisServer = "";
		$strQuery = "";
		if (IsValidIPv4($strHost))
		{
			$strWhoisServer = "whois.ripe.net";
			$strQuery = "{HOST}\r\n";
		}
		else if (IsValidHostName($strHost))
		{
			// Based on: http://www.nirsoft.net/whois_servers_list.html
			$strWhoisServersFile = GetWDKDir()."data/whois.csv";
			if (!IsFile($strWhoisServersFile))
			{
				return false;	
			}
			$arrayServers = FileReadCSV_RFC4180($strWhoisServersFile);
			
			$arrayShortList = array();
			foreach ($arrayServers as $arrayServer)
			{
				$strTld = ".".ArrayGetValue($arrayServer,"TLD");
				$nLength = StringLength($strTld);
				if (StringSection($strHost,-$nLength) == $strTld)
				{
					$arrayShortList[$nLength] = $arrayServer;
				}
			}
			
			
			$arrayShortList = ArraySort($arrayShortList);
			$arrayShortList = ArrayReverse($arrayShortList);
			
			foreach ($arrayShortList as $arrayServer)
			{
				$strWhoisServer = ArrayGetValue($arrayServer,"SERVER");
				$strQuery = ArrayGetValue($arrayServer,"QUERY");
				if ($strQuery != "")
				{
					$strQuery .= " ";
				}
				$strQuery .= "{HOST}\r\n";
				break;
			}
		}
		
		if ($strWhoisServer == "")
		{
			return false;	
		}
			
		$strWhois = "";
		$strError = "";
		$nError = 0;
  	
  	$hSocket = @fsockopen($strWhoisServer,43,$nError,$strError,15);
  	if (!$hSocket)
  	{
  		return false;
  	}
		$strQuery = ReplaceString($strQuery,"{HOST}",$strHost);
    fputs ($hSocket,$strQuery);
    while (!feof($hSocket))
    {
      $strWhois .= fgets($hSocket,256);
    }
    fclose($hSocket);
    
    return $strWhois;
	}
	
	
	function GetDomainStatus($strDomainName,$bCheckForSale = false)
	{
		if (IsValidHostName($strDomainName) == false)
		{
			return DOMAIN_STATUS_INVALID;	
		}
		$arrayDnsRecords = GetDnsRecords($strDomainName);
		
		if ($arrayDnsRecords == false)
		{
			$strWhois = GetWhoisRecords($strDomainName);

			if ($strWhois === false)
			{
				return DOMAIN_STATUS_UNDEFINED;
			}
			$strNextNeedle = "";
			$nPos = FindStringMultipleIgnoreCase(
				$strWhois,
				array(
					"Status: clientTransferProhibited",
					"Status: connect",
					"Holder of domain name:"), // for .ch domains without DNS records
				$strNextNeedle);
			if ($nPos == -1)
			{
				return DOMAIN_STATUS_FREE;	
			}	
		}
		if ($bCheckForSale == true)
		{
			$strSOAmname =  ArrayGetValue($arrayDnsRecords,"SOA",0,"mname");
			$strNextNeedle = "";
			$nPos = FindStringMultipleIgnoreCase(
				$strSOAmname,
				array(
					".domaincontrol.com",
					".parkingcrew.net",
					".cashparking.com",
					".buydomains.com",
					".name-services.com",
					".sedoparking.com"),
				$strNextNeedle);
			if ($nPos != -1)
			{
				return DOMAIN_STATUS_FORSALE;	
			}
			
			$strResponse = HttpRequest(
				'http://'.$strDomainName,
				array(),
				"get",
				5,
				array(),
				array(),
				true);

			if ($strResponse == false)
			{
				$strResponse = HttpRequest(
					'https://'.$strDomainName,
					array(),
					"get",
					5,
					array(),
					array(),
					true);
			}
			
			if ($strResponse != false)
			{
				$strNextNeedle = "";
				$nPos = FindStringMultipleIgnoreCase(
					$strResponse,
					array(
						"sedo.com",
						"go daddy",
						"godaddy",
						"Location: http://www.buydomains.com",
						'Location: http://www.hugedomains.com',
						'location.reload(true);',
						'http://www.dsparking.com/w3c/p3p.xml',
						'check options for domain ownership',
						'check availability and options for domain ownership',
						'This domain has recently been listed in the marketplace',
						'Diese Domain wird vom Inhaber zum Verkauf angeboten',
						'Domain erwerben</a>',
						'Buy this domain</a>',
						'nnen die Domain <b>%{domainName}<\/b> kaufen!'),
					$strNextNeedle);
				if ($nPos != -1) 
				{
					return DOMAIN_STATUS_FORSALE;	
				}	
			}	
		}
		return DOMAIN_STATUS_TAKEN;
	}
	
	

