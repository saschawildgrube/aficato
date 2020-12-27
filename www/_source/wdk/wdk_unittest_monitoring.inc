<?php

	require_once('wdk_unittest.inc');
	require_once('wdk_webserviceconsumer_webapplication.inc');
	require_once('wdk_ip.inc');
	require_once('wdk_w3c.inc');

	define('WEBSERVICE_SYSTEM_SERVER_TIMEOUT',	12);
	define('LOG_MAILWARN_FILESIZE_THRESHOLD', 	(4*1024*1024));
	define('LOG_PANEL_FILESIZE_THRESHOLD', 			100);

	abstract class CMonitoringUnitTest extends CUnitTest
	{
		function __construct($strName,$arrayConfig = array())
		{
			parent::__construct(
				$strName,
				$arrayConfig);
		}
		
		function TestCase_Domain(
			$strDomain,
			$strIP,
			$bCheckWWW = true,
			$bMailDomain = false,
			$strMainDomain = "")
		{
			$bResult = true;
			
			$this->Trace("");			
			$this->Trace(		"Checking ".$strDomain);
			$this->Trace(		"Expected IP : ".$strIP);
			$this->Trace(		"Mail Domain : ".RenderBool($bMailDomain));
			if ($strMainDomain != "")
			{ 
				$this->Trace(	"Main Domain : ".$strMainDomain);
			}


			$strResolvedIP = GetIPFromHostname($strDomain);
			$this->Trace(		"Resolved IP : ".$strResolvedIP);
			
			if ($strResolvedIP == '' && $strIP != '')
			{
				$bResult = false;
				$this->Trace('No IP could not be resolved!');
			}

			if ($strResolvedIP != $strIP)
			{
				$this->Trace('Resolved IP does not match!');
				$bResult = false;
			}
			

			
			$arrayDnsRecords = GetDnsRecords($strDomain);
			if ($arrayDnsRecords == false)
			{
				$this->Trace("DNS Records could not be retrieved!");
				$bResult = false;
			}

			$arrayMX = ArrayAppend(ArrayGetValue($arrayDnsRecords,"MX"),array());			
			$arrayTXT = ArrayAppend(ArrayGetValue($arrayDnsRecords,'TXT'),array());
			$bSPF = false;
			$strSPF = '';
			foreach ($arrayTXT as $arrayRecord)
			{
				$strTxt = ArrayGetValue($arrayRecord,'txt');
				if (FindString($strTxt,'v=spf1') != -1)
				{
					$bSPF = true;
					$strSPF = $strTxt;
				}
			}
			
			if ($bMailDomain == true)
			{
				if (ArrayCount($arrayMX) == 0)
				{
					$this->Trace('No MX records found.');
					$bResult = false;
				}
				if ($bSPF == false)
				{
					$this->Trace('No SPF record found.');
					$bResult = false;
				}
				else
				{
					if (FindString($strSPF,$strIP) == -1)
					{
						$this->Trace('SPF record does not refer to the IP of the domain:');
						$this->Trace('"'.$strSPF.'"');
						$bResult = false;
					}
				}
			}
			else
			{
				if (ArrayCount($arrayMX) != 0)
				{
					$this->Trace('Unexpected MX record(s) found.');
					$bResult = false;
				}
				if ($bSPF == true)
				{
					$this->Trace('Unexpected SPF record(s) found.');
					$bResult = false;
				}
			}

			if ($strMainDomain != "")
			{
				if ($strMainDomain != $strDomain)
				{
					$strResponse = HttpRequest(
						'http://'.$strDomain, 
						array(),
						"get",
						10,
						array(),
						array(),
						true);
					
					if ($strResponse == false)
					{
						$this->Trace("Host does not respond!");
						$bResult = false;
					}
					else
					{					
						$arrayResponse = ParseHttpResponseEx($strResponse);				
						if (!is_array($arrayResponse))
						{
							$this->Trace("Parsing the response failed!");
							$bResult = false;
						}
						else
						{
							$strHttpStatusCode = ArrayGetValue($arrayResponse,"statuscode");
							if ($strHttpStatusCode != "301")
							{
								$this->Trace("Host does not respond with HTTP 301!");
								$bResult = false;
							}
							else
							{
								$strRedirectLocation = ArrayGetValue($arrayResponse,"redirect-location");
								if (FindString($strRedirectLocation,"//".$strMainDomain) == -1)
								{
									$this->Trace("Host does not redirect to $strMainDomain!");
									$bResult = false;
								}
								else
								{
									$this->Trace("Redirection : $strMainDomain");
								}
							}
						}
					}
				}	
			}

			$strDomainWWW = "";
			if ($bCheckWWW == true)
			{
				$strDomainWWW = "www.".$strDomain;
				$this->Trace(		"Checking ".$strDomainWWW);
				$strResolvedIP = GetIPFromHostname($strDomainWWW);
				$this->Trace(		"Resolved IP : ".$strResolvedIP);
				
				if ($strResolvedIP == "" && $strIP != "")
				{
					$bResult = false;
					$this->Trace("No IP could not be resolved!");
				}
	
				if ($strResolvedIP != $strIP)
				{
					$this->Trace("Resolved IP does not match!");
					$bResult = false;
				}

				if ($strMainDomain != "")
				{
					if ($strMainDomain != $strDomainWWW)
					{
						$strResponse = HttpRequest(
							'http://'.$strDomainWWW, 
							array(),
							"get",
							10,
							array(),
							array(),
							true);
						
						if ($strResponse == false)
						{
							$this->Trace("Host does not respond!");
							$bResult = false;
						}
						else
						{					
							$arrayResponse = ParseHttpResponseEx($strResponse);				
							if (!is_array($arrayResponse))
							{
								$this->Trace("Parsing the response failed!");
								$bResult = false;
							}
							else
							{
								$strHttpStatusCode = ArrayGetValue($arrayResponse,"statuscode");
								if ($strHttpStatusCode != "301")
								{
									$this->Trace("Host does not respond with HTTP 301!");
									$bResult = false;
								}
								else
								{
									$strRedirectLocation = ArrayGetValue($arrayResponse,"redirect-location");
									if (FindString($strRedirectLocation,"//".$strMainDomain) == -1)
									{
										$this->Trace("Host does not redirect to $strMainDomain!");
										$bResult = false;
									}
									else 
									{ 
										$this->Trace("Redirection : $strMainDomain");
									}
								}
							}
						}
					}	
				}
			}

			if ($bResult == true)
			{
				$this->Trace("TESTCASE PASSED");
			}
			else
			{
				$this->SetResult(false);
				$this->Trace("TESTCASE FAILED");
			}
		}

		function TestCase_DomainStatus(
			$strDomain,
			$strExpectedStatus)
		{
			$this->Trace("");			
			$this->Trace("Hostname       : ".$strDomain);
			$this->Trace("Expected status: ".$strExpectedStatus);
			$strStatus = ArrayGetValue(GetDomainStatusArray(),GetDomainStatus($strDomain,false));
			$this->Trace("GetDomainStatus: ".$strStatus);

			if ($strStatus == $strExpectedStatus)
			{
				$this->Trace("TESTCASE PASSED");
			}			
			else
			{
				$this->SetResult(false);
				$this->Trace("TESTCASE FAILED");
			}
		}
		
		
		function TestCase_Server(
			$strURL,
			$strAccesscode,
			$bCheckReverseLookup = true)
		{
			$this->Trace("");			
			$this->Trace("Server URL: ".$strURL);
			$bResult = true;

			$webservice = new CWebServiceConsumerWebApplication($this);
			$webservice->SetTrace(false);

			$arrayParams = array();
			if ($strAccesscode != "")
			{
				$arrayParams["accesscode"] = $strAccesscode;
			}


			$arrayParams["command"] = "status";
			$webservice->ConsumeWebService($strURL,$arrayParams,WEBSERVICE_SYSTEM_SERVER_TIMEOUT);
			if ($webservice->GetError() != '')	
			{
				$this->Trace("Error: Server status: \"".$webservice->GetError()."\"");
				$bResult = false;					
			}
			else
			{
				$this->Trace("Checking disk space");
				$nDiskFreePercent = $webservice->GetResultValue("DISK_FREE_PERCENT");
				if ($nDiskFreePercent < 10)
				{
					$this->Trace("FAILED: Free space on server is below 10%");
					$bResult = false;					
				}
	
				$this->Trace("Checking average load");
				$fAvgLoad15 = $webservice->GetResultValue("LOAD_AVERAGE_15MINUTES");
				if ($fAvgLoad15 > 10)  
				{
					$this->Trace("FAILED: 15 minutes average load was ".$fAvgLoad15);
					$bResult = false;					
				}
				
				$this->Trace("Checking mail.warn file");
				$fLogMailWarnFileSize = $webservice->GetResultValue("MAIL_MAILWARN_FILESIZE");
				if ($fLogMailWarnFileSize > LOG_MAILWARN_FILESIZE_THRESHOLD)  
				{
					$this->Trace("FAILED: mail.warn file size is ".$fLogMailWarnFileSize.". This may indicate malicious spam activity on the server.");
					$bResult = false;										
				}
				
				
				$this->Trace("Checking mail queue");
				$nMailQueueSize = $webservice->GetResultValue("MAIL_QUEUE_SIZE");
				if ($nMailQueueSize > 100)
				{
					$this->Trace("FAILED: $nMailQueueSize mails are in the mail queue. This may indicate malicious spam activity on the server.");
					$bResult = false;										
				}


				$this->Trace("Checking core dump files");
				$nCoreDumpFiles = $webservice->GetResultValue("CORE_DUMP_FILE_COUNT");
				if ($nCoreDumpFiles > 0)
				{
					$this->Trace("FAILED: Number of core dump files on the server: $nCoreDumpFiles");
					$bResult = false;										
				}


				$this->Trace("Checking Plesk's panel.log file");
				$fLogPanelLogFileSize = $webservice->GetResultValue("PLESK_PANELLOG_FILESIZE");
				if ($fLogPanelLogFileSize > LOG_PANEL_FILESIZE_THRESHOLD)  
				{
					$this->Trace('FAILED: panel.log file size is '.$fLogPanelLogFileSize.'. This may indicate problems with Plesk configuration or Lets Encrypt SSL certificate renewals.'); 
					$bResult = false;										
				}




			}



			$arrayParams["command"] = "info";
			$webservice->ConsumeWebService($strURL,$arrayParams,WEBSERVICE_SYSTEM_SERVER_TIMEOUT);
			if ($webservice->GetError() != '')	
			{
				$this->Trace("Error: Server info: \"".$webservice->GetError()."\"");
				$bResult = false;					
			}
			else
			{
				$this->Trace("Checking OpenSSL");
				
				$strOpenSSLBuildDate = $webservice->GetResultValue("OPENSSL","BUILD_DATE");
				if ($strOpenSSLBuildDate == '')
				{
					$this->Trace("WARNING: OpenSSL build date could not be determined.");
				}
				else
				{
					$this->Trace('OpenSSL build date: '.$strOpenSSLBuildDate);
					if ($strOpenSSLBuildDate < "2014-03-07")
					{
						$this->Trace("FAILED: OpenSSL builds prior to 2014-04-07 are vulnerable to the SSL heartbleed bug.");
						$bResult = false;					
					}
					if ($strOpenSSLBuildDate < "2014-06-02")
					{
						$this->Trace("FAILED: OpenSSL builds prior to 2014-06-02 are vulnerable to several security issues.");
						$bResult = false;					
					}
					if ($strOpenSSLBuildDate < "2015-03-19")
					{
						$this->Trace("FAILED: OpenSSL builds prior to 2015-03-19 are vulnerable to the FREAK attack.");
						$bResult = false;					
					}
					if ($strOpenSSLBuildDate < "2015-06-11")
					{
						$this->Trace("FAILED: OpenSSL builds prior to 2015-06-11 are vulnerable to the LOGJAM attack.");
						$this->Trace("For more information refer to: https://www.openssl.org/news/secadv_20150611.txt");
						$bResult = false;					
					}
					if ($strOpenSSLBuildDate < "2015-07-11")
					{
						$this->Trace("FAILED: OpenSSL builds prior to 2015-07-11 are vulnerable to CVE-2015-1793.");
						$this->Trace("For more information refer to: https://www.openssl.org/news/secadv_20150709.txt");
						$bResult = false;					
					}
					if ($strOpenSSLBuildDate < "2016-05-02") 
					{
						$this->Trace("FAILED: OpenSSL build date is before May 2nd 2016.\nIt may still be vulnerable to CVE-2016-2107.");	 
						$bResult = false;
					}
					if ($strOpenSSLBuildDate < "2016-09-22") 
					{
						$this->Trace("FAILED: OpenSSL build date is before September 22nd 2016.\nIt may still be vulnerable to CVE-2016-6304.");	 
						$bResult = false;
					}
					if ($strOpenSSLBuildDate < "2016-09-26") 
					{
						$this->Trace("FAILED: OpenSSL build date is before September 26th 2016.\nIt may still be vulnerable to CVE-2016-6309.");
						$bResult = false;
					}

				}
	
	
				
				$this->Trace("Checking PHP");
				$strPHPVersion = $webservice->GetResultValue("PHP","VERSION");
				$arrayPHPVersion = ParseVersion($strPHPVersion);
				if ($arrayPHPVersion["MAJOR"] == 7)
				{
					if ($arrayPHPVersion["MINOR"] == 2)   
					{
						if ($arrayPHPVersion["PATCH"] < 22)
						{
							$this->Trace("FAILED: PHP patch version is below 22.");   
							$bResult = false;
						}
					}
					else
					{
						$this->Trace("FAILED: PHP minor version is not 2.");
						$bResult = false;
					}
				}
				else
				{
					$this->Trace("FAILED: PHP major version is not 7.");
					$bResult = false;
				}
				
				
				$this->Trace("Checking WGET");
				$strWgetVersion = $webservice->GetResultValue("WGET","VERSION");
				if (CompareVersion($strWgetVersion,"1.16") == -1)
				{
					if ($strWgetVersion == "1.13.4")
					{
						$strWgetVerboseVersion = $webservice->GetResultValue("WGET","VERBOSE");
						if (		$strWgetVerboseVersion != "1.13.4-2ubuntu1.2"
								&&	$strWgetVerboseVersion != "1.13.4-2ubuntu1.3"
								&&	$strWgetVerboseVersion != "1.13.4-2ubuntu1.4")
						{
							$this->Trace("FAILED: WGET version is below 1.13.4\nbut package version is not 1.13.4-2ubuntu1.2, 1.13.4-2ubuntu1.3, or 1.13.4-2ubuntu1.4"); 
							$bResult = false;
						}
					}
					else
					{
						$this->Trace("FAILED: WGET version is below 1.16"); 
						$bResult = false;
					}
				}
				
				
				
				$this->Trace("Checking GLIBC");
				$strGlibcVersion = $webservice->GetResultValue("GLIBC","VERSION");
				if ($strGlibcVersion == "")
				{
					$this->Trace("FAILED: GLIBC version could not be determined");
					$bResult = false;
				}
				else
				{
					if (CompareVersion($strGlibcVersion,"2.23") == -1)
					{
						$strGlibcVerboseVersion = $webservice->GetResultValue("GLIBC","VERBOSE");
						if (($strGlibcVerboseVersion != "2.15-0ubuntu10.15") && ($strGlibcVerboseVersion != "2.15-0ubuntu10.18"))
						{
								$this->Trace("FAILED: GLIBC version is below 2.23\nand package version is neither 2.15-0ubuntu10.15 nor 2.15-0ubuntu10.18\nThis server is vulnerable to CVE-2015-7547"); 
								$bResult = false;
						}
					}				
				}				
				
				
			}			

			
		


			if ($bCheckReverseLookup == true)
			{
				$this->Trace("Checking reverse lookup");
				
				$arrayURL = ParseURL($strURL);
				$strHostname = ArrayGetValue($arrayURL,"HOST");
				$strIP = GetIPFromHostName($strHostname);
				$strPTR = GetHostNameFromIP($strIP);
				
				if ($strHostname != $strPTR)
				{
					$this->Trace("PTR record: $strPTR");
					$this->Trace("FAILED: PTR record does not yield the same hostname.");
					$bResult = false;
				}
			}


			if ($bResult == true)
			{
				$this->Trace("TESTCASE PASSED");
			}
			else
			{
				$this->SetResult(false);
				$this->Trace("TESTCASE FAILED");
			}
		} 
		
		
		
		function TestCase_Scheduler(
			$strSchedulerWebserviceURL,
			$nMaxMinutes,
			$strAccesscode)
		{
			$this->Trace("");			
			$this->Trace("Scheduler webservice URL: ".$strSchedulerWebserviceURL);
			$bResult = true;

			$webservice = new CWebServiceConsumerWebApplication($this);

			$arrayParams = array();
			$arrayParams["command"] = "status";
			if ($strAccesscode != "")
			{
				$arrayParams["accesscode"] = $strAccesscode;
			}
			$webservice->ConsumeWebService($strSchedulerWebserviceURL,$arrayParams);
			if ($webservice->GetError() != "")	
			{
				$this->Trace("Error: \"".$webservice->GetError()."\"");
				$bResult = false;					
			}
			else
			{
				$this->Trace("Checking crontab heartbeat");
				$strCrontabHeartBeat = $webservice->GetResultValue("CRONTAB_HEARTBEAT_DATETIME");
				if ($strCrontabHeartBeat == "")
				{
					$this->Trace("No crontab heartbeat was set.");
					$bResult = false;					
				}
				else
				{
					$time = GetTime($strCrontabHeartBeat);
					$timeNow = GetTimeNow();
					$nMinutesDiff = ($timeNow - $time) / 60;
					$nMaxMinutes = intval($nMaxMinutes);
					if ($nMinutesDiff > $nMaxMinutes)
					{
						$this->Trace("The set crontab heart beat is too old. A reason may be that the crontab for the scheduler does not work.");
						$bResult = false;					
					}
				}
			}

			if ($bResult == true)
			{
				$this->Trace("TESTCASE PASSED");
			}
			else
			{
				$this->SetResult(false);
				$this->Trace("TESTCASE FAILED");
			}
		}
		
		
		
		function TestCase_MonitorURL(
			$strURL,
			$nTimeoutSeconds = 10,
			$arrayExpectedPatternWhitelist = array(),
			$arrayExpectedPatternBlacklist = array(),
			$arrayRegExpPattern = array(),
			$arrayParams = array(),
			$arrayCookies = array(),
			$arrayHeaders = array(),
			$strMode = "get")
		{
			$strResponse = $this->TestCase_CheckURL(
				$strURL,
				$arrayExpectedPatternWhitelist,
				$arrayExpectedPatternBlacklist,
				$arrayRegExpPattern,
				$arrayParams,
				$arrayCookies,
				$arrayHeaders,
				$strMode,				
				$nTimeoutSeconds,
				false,
				false,
				14);
		}

		
		
		function TestCase_Backup(
			$strURL,
			$strAccesscode,
			$arrayBackupFileRules)
		{
			$this->Trace("");			
			$this->Trace("Backup Server URL: ".$strURL);
			
			$this->Trace("Backup File Rules:");
			$this->Trace($arrayBackupFileRules);
			
			$bResult = true;

			$webservice = new CWebServiceConsumerWebApplication($this);
			$webservice->SetTrace(false);

			$arrayParams = array();
			if ($strAccesscode != "")
			{
				$arrayParams["accesscode"] = $strAccesscode;
			}

			$arrayParams["command"] = "status";
			$webservice->ConsumeWebService($strURL,$arrayParams);
			if ($webservice->GetError() != "")	
			{
				$this->Trace("Error: \"".$webservice->GetError()."\"");
				$bResult = false;					
			}
			else
			{
				$arrayBackupFiles = $webservice->GetResultValue("BACKUP_FILES");
				if (is_array($arrayBackupFiles))
				{
					$this->Trace("Backup files on server:");
					$this->Trace($arrayBackupFiles);
	
					if (ArrayCount($arrayBackupFileRules) == 0)
					{
						$this->Trace("No backup file rules are defined. Stopping test case.");
						return;
					}
					foreach ($arrayBackupFileRules as $strRuleBackupFileName => $arrayRule)
					{
						$this->Trace("Checking backup file rule: $strRuleBackupFileName");
						$bFound = false;
						foreach ($arrayBackupFiles as $arrayBackupFile)
						{
							if (ArrayGetValue($arrayBackupFile,"NAME") == $strRuleBackupFileName)
							{
								$bFound = true;
								
								$nSize = intval(ArrayGetValue($arrayBackupFile,"SIZE"));
								$nSizeMin = intval(ArrayGetValue($arrayRule,"SIZE_MIN"));
								if ($nSizeMin > 0)
								{
									if ($nSize < $nSizeMin)	
									{
										$this->Trace("Backup file is smaller than expected: $nSize < $nSizeMin");
										$this->Trace("TESTCASE FAILED");
										$bResult = false;
									}
								}
	
								$strLastChange = ArrayGetValue($arrayBackupFile,"LASTCHANGE");
								$nLastChangeMinutes = intval(ArrayGetValue($arrayRule,"LASTCHANGE_MINUTES"));
								if ($nLastChangeMinutes > 0)
								{
									$timeAcceptableAge = GetTimeAddMinutes($strLastChange,$nLastChangeMinutes);
									$timeNow = GetTimeNow();
									if ($timeAcceptableAge < $timeNow)	
									{
										$this->Trace("Backup file is older than expected.");
										$this->Trace("TESTCASE FAILED");
										$bResult = false;
									}
								}
								continue;
							}
						}
						if ($bFound == false)
						{
							$this->Trace("Backup file is not on server!");
							$this->Trace("TESTCASE FAILED");
							$bResult = false;							
						}
					}					
				}
			}

			if ($bResult == true)
			{
				$this->Trace("TESTCASE PASSED");
			}
			else
			{
				$this->SetResult(false);
			}
		}		
		
		
		function TestCase_CheckSSL(
			$strHost,
			$nSSLCertValidityThresholdDays = 14)
		{			
			$this->Trace('Test SSL certificates: '.$strHost);
	
			$bResult = true;
			
			if ($nSSLCertValidityThresholdDays < 1)
			{
				$nSSLCertValidityThresholdDays = 1;
			}

			if (IsValidURL($strHost))
			{				
				$arrayURL = ParseURL($strHost);
				$strHost = ArrayGetValue($arrayURL,'HOST');
			}
			$arrayCerts = GetSSLCertificatesFromHost($strHost);
			$arrayCert = ArrayGetValue($arrayCerts,0);
			if (!is_array($arrayCert))
			{
				$this->Trace('Could not retrieve SSL certificate.');	
				$bResult = false;
			}
			else
			{
				$timeValidTo = intval(ArrayGetValue($arrayCert,'validTo_time_t'));
				$strDateTimeValidTo = RenderDateTime($timeValidTo);
				$nSecondsRemainingThreshold = $nSSLCertValidityThresholdDays * 24 * 60 * 60;
				if ($timeValidTo < time() + $nSecondsRemainingThreshold)
				{
					if ($timeValidTo < time())
					{
						$this->Trace('SSL certificate expired as of '.$strDateTimeValidTo.'!');	
					}
					else
					{
						$this->Trace('SSL certificate will expire as of '.$strDateTimeValidTo.'!');									
					}
					$bResult = false;
				}
				else
				{
					$this->Trace('SSL certificate is OK.');	
				}
			}
			
			if ($bResult != true)
			{
				$this->SetResult(false);
				$this->Trace('TESTCASE FAILED');
			}
		}

		function TestCase_w3cValidateURL(
			$strURL,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase_w3cValidateURL");
	
			$this->Trace("strURL = \"$strURL\"");
			$this->Trace("strExpectedResult = \"$strExpectedResult\"");

			$strValidatorURL = w3cGetValidatorURL($strURL);
			$this->Trace("Validator URL: \"$strValidatorURL\"");
			
			$strResult = w3cValidateURL($strURL);
			$this->Trace("Testcase w3cValidateURL returns: \"$strResult\"");
			
			if ($strResult == $strExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else if ($strResult == "UNDEFINED")
			{
				$this->Trace("Testcase UNDEFINED! This is because the w3c validator did not yield any defined result.");
				$this->SetActive(false);	
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);	
			}
			$this->Trace("");
			$this->Trace(""); 
		}		

	}

