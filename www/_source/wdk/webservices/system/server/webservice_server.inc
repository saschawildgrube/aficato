<?php

	require_once(GetWDKDir()."wdk_webservice.inc");
	require_once(GetWDKDir()."wdk_system.inc");
	require_once(GetWDKDir()."wdk_openssl.inc");
	require_once(GetWDKDir()."wdk_php.inc");
	require_once(GetWDKDir()."wdk_apache.inc");
	require_once(GetWDKDir()."wdk_wget.inc");
	require_once(GetWDKDir()."wdk_glibc.inc");
	
   class CServerWebService extends CWebService
	{
		function __construct(
			$strSourceDir,
			$strServiceDir,
			$arrayConfig,
			$arrayParams)
		{
			$arrayMeta = array();
			$arrayMeta["version"] = WDK_VERSION; 
			$arrayMeta["author"] = "Sascha Wildgrube";
			
			parent::__construct(
				"system/server",
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				array()
				);
		}
		
		function CallbackInitParameterDefinition(&$paramDef)
		{
			parent::CallbackInitParameterDefinition($paramDef);
		}
		
		function CallbackInitCommandDefinition(&$commandDef)
		{
			//$commandDef->AddCommandDefinition("restart",array());
			$commandDef->AddCommandDefinition("info",array());
			$commandDef->AddCommandDefinition("status",array());
			$commandDef->AddCommandDefinition("performance",array());
			$commandDef->AddCommandDefinition("protect",array());
			parent::CallbackInitCommandDefinition($commandDef);
		}
		
		function CallbackInit()
		{
			$this->SetResultValue($this->GetServerIP(),"IP");
			$this->SetResultValue(GetSystemHostname(),"HOSTNAME");
		
			return parent::CallbackInit();
		}
		
		
		function CallbackCommand($strCommand)
		{
			switch ($strCommand)
			{
			case "info":
				$this->CommandInfo();
				return;
				

			case "status":
				$this->CommandStatus();
				return;
				
			case "performance":
				$this->CommandPerformance();
				return;				
/*
			case "restart":
				$this->CommandRestart();
				return;
*/

			case "protect":
				$this->CommandProtect();
				return;

			default:
				parent::CallbackCommand($strCommand);
				return;
			}
		}


		function CommandInfo()
		{
			$this->Trace("System");
			$this->SetResultValue(PHP_INT_SIZE,"SYSTEM","INTEGER_SIZE");
			$this->SetResultValue(GetSystemUser(),"SYSTEM","USER");

			$this->Trace("Apache");
			$arrayVersion = GetVersionInfoApache();
			$this->SetResultValue($arrayVersion,"APACHE");
			
			$this->Trace("PHP");
			$arrayVersion = GetVersionInfoPHP();
			$this->SetResultValue($arrayVersion,"PHP");
						
			$this->Trace("OpenSSL");
			$arrayVersion = GetVersionInfoOpenSSL();
			$this->SetResultValue($arrayVersion,"OPENSSL");

			$this->Trace("wget");
			$arrayVersion = GetVersionInfoWget();
			$this->SetResultValue($arrayVersion,"WGET");
	
			$this->Trace("glibc");
			$arrayVersion = GetVersionInfoGlibc();
			$this->SetResultValue($arrayVersion,"GLIBC");
		}


		
		function CommandStatus()
		{
	
			$this->Trace("");	
			$this->Trace("Check Disk Space");
			
			$nDiskTotalSpace = GetDiskSpaceTotal(GetSourceDir());
			$nDiskFreeSpace = GetDiskSpaceFree(GetSourceDir());
			$nDiskUsedSpace = $nDiskTotalSpace - $nDiskFreeSpace;
			$this->SetResultValue($nDiskTotalSpace,"DISK_TOTAL_BYTES");
			$this->SetResultValue($nDiskFreeSpace,"DISK_FREE_BYTES");
			$this->SetResultValue($nDiskUsedSpace,"DISK_USED_BYTES");
			if ($nDiskTotalSpace > 0)
			{
				$this->SetResultValue(RenderNumber($nDiskFreeSpace/$nDiskTotalSpace*100,3),"DISK_FREE_PERCENT");
				$this->SetResultValue(RenderNumber($nDiskUsedSpace/$nDiskTotalSpace*100,3),"DISK_USED_PERCENT");
			}



			$this->Trace("");	
			$this->Trace("Check Load");	
					
			$arrayAverageLoad = array();
			$bResult = GetSystemAverageLoad($arrayAverageLoad);
			if ($bResult == false)
			{
				$this->SetError("AVERAGE_LOAD_COMMAND_FAILED");
				return;
			}
			$this->SetResultValue(RenderNumber($arrayAverageLoad["1minute"],2),"LOAD_AVERAGE_1MINUTE");
			$this->SetResultValue(RenderNumber($arrayAverageLoad["5minutes"],2),"LOAD_AVERAGE_5MINUTES");
			$this->SetResultValue(RenderNumber($arrayAverageLoad["15minutes"],2),"LOAD_AVERAGE_15MINUTES");
			
			
			
			
			$this->Trace("");			
			$this->Trace("Check Plesk");		
				
			$strURL = "https://".GetSystemHostname().":8443/login_up.php";
			$strResponse = HttpRequest(
				$strURL,
				array(),
				"get",
				10,
				array(),
				array(),
				true);
			$arrayResponse = ParseHttpResponse($strResponse);
			$bPleskActive = false;
			if (ArrayGetValue($arrayResponse,"statuscode") == "200")
			{
				$bPleskActive = true;
			}
			$this->SetResultValue(RenderInteger($bPleskActive),"PLESK_ACTIVE");	
			$this->SetResultValue(GetFileSize('/var/log/plesk/panel.log',true),'PLESK_PANELLOG_FILESIZE');
			
			$this->Trace("");				
			$this->Trace("Check Mail Server");	
			
			$this->SetResultValue(GetFileSize("/var/log/mail.warn",true),"MAIL_MAILWARN_FILESIZE");
			$this->SetResultValue(GetFileSize("/var/log/mail.err",true),"MAIL_MAILERR_FILESIZE");
			$this->SetResultValue(GetFileSize("/var/log/mail.info",true),"MAIL_MAILINFO_FILESIZE");
			
			$nMailQueueSize = GetMailQueueSize();
			if ($nMailQueueSize !== false)
			{
				$this->SetResultValue(RenderInteger($nMailQueueSize),"MAIL_QUEUE_SIZE");
			}
			else
			{
				$this->SetResultValue("","MAIL_QUEUE_SIZE");			
			}
			
			
			/*
			$this->Trace("Check Ressource Consumption");
			$arrayRessources = array();
			$bResult = GetSystemRessourceConsumption($arrayRessources);
			if ($bResult == false)
			{
				$this->SetError("RESSOURCE_CONSUMPTION_CHECK_FAILED");
				return;
			}
			$this->Trace($arrayRessources);			
			*/
			
			
			
			
			$this->Trace("");				
			$this->Trace("Check Backup Files");
			
			$strBackupDir = GetDocumentRootDir()."../../backup/";
			$this->Trace("Backup Dir: ".$strBackupDir);
			
			$arrayBackupFiles = ReadDirectory($strBackupDir);
			$arrayFileList = array();
			if (is_array($arrayBackupFiles))
			{
				foreach ($arrayBackupFiles as $strBackupFileName)
				{
					$strBackupFile = $strBackupDir.$strBackupFileName;
					if (IsFile($strBackupFile))
					{
						$nFileSize = GetFileSize($strBackupFile);  
						$nFileLastChangeTime = GetFileLastChangeTime($strBackupFile);
						$arrayFileList[] = array(
							"NAME" => $strBackupFileName,
							"SIZE" => RenderInteger($nFileSize,0),
							"LASTCHANGE" => RenderDateTime($nFileLastChangeTime));	
					}
					
				}	
			}
			if (ArrayCount($arrayFileList) > 0)
			{
				$this->SetResultValue($arrayFileList,"BACKUP_FILES");
			}
			$this->Trace($arrayBackupFiles); 
			
			
			
			$this->Trace("");	
			$this->Trace("Check Core Dump Files");	
			
			$arrayCoreDumpFiles = GetCoreDumpFiles();
			if ($arrayCoreDumpFiles === false)
			{
				$arrayCoreDumpFiles = array();
			}
			$nCoreDumpFiles = intval(ArrayCount($arrayCoreDumpFiles));
			$this->SetResultValue("".$nCoreDumpFiles,"CORE_DUMP_FILE_COUNT");
			
		}


		function CommandPerformance()
		{
			$this->Trace("Check Counting Speed");
			
			$stopwatch = new CStopWatch();
			$stopwatch->Start();
			for ($nCounter = 0; $nCounter < 1000000; $nCounter++);
			$stopwatch->Stop();
					
			$this->SetResultValue(RenderNumber($stopwatch->GetSeconds(),3),"COUNTING_SPEED_SECONDS");
		}


	
	/*
		function CommandRestart()
		{
			$this->Trace("Restarting Server...");

			// root is required to do that
							
			$arrayResult = array();
			$bResult = ShellExecute(
				"init",
				array("6"),
				array(),
				"",
				$arrayResult);
				
			$this->Trace("bResult: $bResult");
			$this->Trace($arrayResult);
		}
	*/
		
	
	
	
		function CommandProtect()
		{
			$nProtectOptions = 0;
			if ($this->GetConfigBoolean("protect","mailserver","active") == true)
			{
				$nProtectOptions++; 
				
				$this->ProtectMailServer();
			}
			
			if ($nProtectOptions == 0)
			{
				$this->Trace("No protection options are activated via config. No action was taken.");	
			}
			
		}
		
		
		function ProtectMailServer()
		{
			$this->Trace("Protect mail server:");
						
			$this->Trace("Check for numerous unsuccessful (and hence potentially malicious) login attempts in the mail warn log and block originating IPs:");
			$arrayBlockedIPs = array();
			
			require_once(GetWDKDir()."wdk_iptables.inc");
			$bResult = IPTablesIsExecutable();
			$this->Trace("IPTablesIsExecutable returned ".RenderBool($bResult));
			
			if ($bResult == false)
			{
				$this->Trace("No action taken, since iptables cannot be run. Check sudo configuration!");
				$this->SystemEvent(
					"ERROR",
					"SERVER_PROTECT_MAILSERVER_MAILWARN_IPTABLES_NOT_EXECUTABLE",
					"iptables could not be executed.",
					array());					
			}
			else
			{
				$strMailWarnFilePath = $this->GetConfig("protect","mailserver","mailwarnfilepath");
				if ($strMailWarnFilePath == "")
				{
					$strMailWarnFilePath = "/var/log/mail.warn";
				}

				$bIsFile = IsFile($strMailWarnFilePath,true);
				
				if ($bIsFile == false)
				{
					$this->Trace("File $strMailWarnFilePath does not exist.");					
					if ($this->GetConfigBoolean('protect','mailserver','ignore_missing_mail_warn_file') == true)
					{
						$this->SystemEvent(
							'NOTICE',
							'SERVER_PROTECT_MAILSERVER',
							'No action taken due to missing log file.');	 
					}
					else
					{
						$this->SystemEvent(
							"ERROR",
							"SERVER_PROTECT_MAILSERVER_MAILWARN_FILE_NOT_FOUND",
							"File $strMailWarnFilePath does not exist",
							array());
					}
					
				}
				else
				{
				
					$this->Trace("Reading the mail.warn file: $strMailWarnFilePath");
					$arrayMailWarnLog = FileReadToArray($strMailWarnFilePath);
					if ($arrayMailWarnLog == false)
					{
						$this->Trace("Could not read mail warn file! Check file path and access rights. Also check open basedir restrictions.");
						$this->SystemEvent(
							"ERROR",
							"SERVER_PROTECT_MAILSERVER_MAILWARN_FILE_NOT_ACCESSIBLE",
							"File $strMailWarnFilePath is not accessible",
							array());												
					}
					else
					{
						$this->Trace("Scanning the mail.warn file...");
						
						require_once(GetWDKDir()."wdk_mailserver.inc");
						$arrayIPs = ScanMailWarnLogForMaliciousIPs($arrayMailWarnLog);
						
						$this->Trace("The following IPs have been identified as sources for unsuccessful login attempts:");
						$this->Trace($arrayIPs);
						
						$nThreshold = (int)$this->GetConfig("protect","mailserver","threshold");
						if ($nThreshold <= 0)
						{
							$nThreshold = 25;
						}
						
						$this->Trace("Blocking all IPs with more than $nThreshold attempts...");
	
						foreach ($arrayIPs as $strIP => $nCount)
						{
							if ($nCount > $nThreshold)
							{
								$this->Trace("Blocking IP $strIP because of $nCount unsuccessful attempts...");
								
								$bResult = IPTablesIsIPBlocked($strIP);
								if ($bResult == true)
								{
									$this->Trace("IP $strIP is already blocked. No further action required.");
								}
								else
								{
									$bResult = IPTablesBlockIP($strIP);
									if ($bResult == false)
									{
										$this->Trace("IPTablesBlockIP($strIP) returned false");
									}
									else
									{
										$arrayBlockedIPs[$strIP] = $nCount;
									}
								}
							}	
						}
						$nBlockedIPs = ArrayCount($arrayBlockedIPs);
						
						$this->Trace("$nBlockedIPs new IPs have been blocked!");
						$this->SetResultValue($nBlockedIPs,"PROTECT_MAILSERVER_BLOCKEDIPS");
						if ($nBlockedIPs > 0)
						{
							$this->SystemEvent(
								"NOTICE",
								"SERVER_PROTECT_MAILSERVER",
								"$nBlockedIPs new IPs have been blocked!",
								$arrayBlockedIPs);	 
						}
					}
				}
			}
			
		}
		
	
		
	}
