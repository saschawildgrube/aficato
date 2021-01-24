<?php

	require_once(GetWDKDir()."wdk_webservice_database_entity.inc");  
	require_once(GetWDKDir()."wdk_entity_log.inc");  
	require_once(GetWDKDir()."wdk_hash.inc");  
		
   class CLogWebService extends CDatabaseEntityWebService
	{
		private $m_strInitEntry;
		private $m_arraySeverities;
		
		function __construct(
			$strSourceDir,
			$strServiceDir,
			$arrayConfig,
			$arrayParams)
		{
			$arrayMeta = array();
			$arrayMeta["version"] = WDK_VERSION; 
			$arrayMeta["author"] = "Sascha Wildgrube";

			$this->m_strInitEntry = "This entry must be the first in the log to ensure consistency.";
			
			// Severity level as defined in RFC 3164
			$this->m_arraySeverities = GetEventSeverities();

			/*
			The severity level TEST is used for
			general testing purposes.
			The severity levels TESTWHITELIST and
			TESTBLACKLIST are used to test the
			severity level filtering feature.
			TEST* severities are not defined in RFC 3164.
			*/

			$arrayEntityDefinition = array(
				"log" => GetEntityDefinitionLog()
				);
				
				
			parent::__construct(
				"system/log",
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$arrayEntityDefinition);
				
		}
		
		function OnInitParameterDefinition(&$paramDef)
		{
			$paramDef->AddParameterDefinitionBoolean("logmail");
			parent::OnInitParameterDefinition($paramDef);
			
		}
		
		function OnInitCommandDefinition(&$commandDef)
		{
			$commandDef->AddCommandDefinition("log",array("reporter_id","event_severity","event_id","event_message","event_details","event_datetime","logmail"));
			$commandDef->AddCommandDefinition("verify",array());	
			parent::OnInitCommandDefinition($commandDef);
		}
		
		function OnInit()
		{
			if ($this->DatabaseConnect() != true)
			{
				return false;
			}			
			return parent::OnInit();
		}
		
		function OnSystemLog($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails)
		{
			// No, the log web service will not trigger a log entry
		}
		
		
		function OnCommand($strCommand)
		{
			switch ($strCommand)
			{
			case "log":
				$this->CommandLog();
				return;
		
			case "verify":
				$this->CommandVerify();
				return;
				
			default:
				parent::OnCommand($strCommand);
				return;
			}
		}
		
		
		function MakeHashChain($strPreviousHashChain,$strHash)
		{
			return MakeHash($strPreviousHashChain.$strHash);
		}
		
		function IsIgnored($strEventSeverity,$strReporterID,$strEventID)
		{
			$arraySeverityBlacklist = $this->GetConfig("severity_blacklist");
			if (is_array($arraySeverityBlacklist))
			{
				if (ArrayValueExists($arraySeverityBlacklist,$strEventSeverity) == true)
				{
					$this->Trace("Ignored because severity $strEventSeverity is blacklisted.");
					return true;	
				}
			}
			$arrayReporterBlacklist = $this->GetConfig("reporterid_blacklist");
			if (is_array($arrayReporterBlacklist))
			{
				if (ArrayValueExists($arrayReporterBlacklist,$strReporterID) == true)
				{
					$this->Trace("Ignored because reporter $strReporterID is blacklisted.");
					return true;	
				}
			}
			$arrayEventBlacklist = $this->GetConfig("eventid_blacklist");
			if (is_array($arrayEventBlacklist))
			{
				if (ArrayValueExists($arrayEventBlacklist,$strEventID) == true)
				{
					$this->Trace("Ignored because eventid $strEventID is blacklisted.");
					return true;	
				}
			}	
			$arraySeverityWhitelist = $this->GetConfig("severity_whitelist");
			if (is_array($arraySeverityWhitelist))
			{
				if (ArrayCount($arraySeverityWhitelist) > 0)
				{
					if (ArrayValueExists($arraySeverityWhitelist,$strEventSeverity) == false)
					{
						$this->Trace("Ignored because severity $strEventSeverity is not on the white list.");
						return true;	
					}
				}
			}
			$arrayReporterWhitelist = $this->GetConfig("reporterid_whitelist");
			if (is_array($arrayReporterWhitelist))
			{
				if (ArrayCount($arrayReporterWhitelist) > 0)
				{
					if (ArrayValueExists($arrayReporterWhitelist,$strReporterID) == false)
					{
						$this->Trace("Ignored because reporter $strReporterID is not on the white list.");
						return true;	
					}
				}
			}
			$arrayEventWhitelist = $this->GetConfig("eventid_whitelist");
			if (is_array($arrayEventWhitelist))
			{
				if (ArrayCount($arrayEventWhitelist) > 0)
				{
					if (ArrayValueExists($arrayEventWhitelist,$strEventID) == false)
					{
						$this->Trace("Ignored because eventid $strEventID is not on the white list.");
						return true;	
					}
				}
			}								
			return false;
		}
		
		
				
		function CommandLog()
		{
			$strReporterID = $this->GetParam("reporter_id");
			if ($strReporterID == "")
			{
				$this->SetError("PARAMETER_REPORTERID_MISSING");
				return;
			}
			$strReporterID = StringUpperCase($strReporterID);
			
			$strEventID = $this->GetParam("event_id");
			if ($strEventID == "")
			{
				$this->SetError("PARAMETER_EVENTID_MISSING");
				return;
			} 
			$strEventID = StringUpperCase($strEventID);
			
			$strEventSeverity = $this->GetParam("event_severity");
			if ($strEventSeverity == "")
			{
				$this->SetError("PARAMETER_EVENTSEVERITY_MISSING");
				return;
			} 
			
			$strEventDateTime = $this->GetParam("event_datetime");
			if ($strEventDateTime == "")
			{
				$strEventDateTime = RenderDateTimeNow();
			}
			/*if (CheckParam_IsValidDateTime($strEventDateTime) != true)
			{
				$this->Trace("strEventDateTime = \"$strEventDateTime\"");
				$this->SetError("PARAMETER_DATETIME_INVALID");
				return;
			}
			*/
			
			$strEventMessage = $this->GetParam("event_message");			
			$strEventDetails = $this->GetParam("event_details");
			
			
			if ($this->IsIgnored($strEventSeverity,$strReporterID,$strEventID) == true)
			{
				$this->SetResultValue("","NEW_LOG_ID");
				$this->SetResultValue("IGNORED","STATUS");
				return;	
			}
			

			

			/*
			Process description
			
			1. Get previous hash chain value and id
			2. Build hash payload, make hash value, and hash chain value
			3. Add new log entry
			*/
			
	
			
			$strTable = $this->GetDatabaseTableName("log");
		
			$bLocked = $this->DatabaseLockTable($strTable);
			
			if (!$bLocked)
			{
				return;	
			}
			
			
			// 1. Get previous hash chain value and id
			
			$this->Trace("1. Get Previous Hash Chain value and log id");
					
			$strSQL = "SELECT `HASHCHAIN`, `LOG_ID` FROM ".MakeSQL_Helper_Table($strTable)." ORDER BY `LOG_ID` DESC LIMIT 0 , 1";
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			$this->Trace($this->m_database->GetData());
			$arrayData = $this->m_database->GetData();
			
			$strPreviousHashChain = ArrayGetValue(ArrayGetValue($arrayData,0),"HASHCHAIN");
			$strLogID = ArrayGetValue(ArrayGetValue($arrayData,0),"LOG_ID");
			
			$this->Trace("previous LOG_ID: $strLogID");
			$this->Trace("previous HASHCHAIN: $strPreviousHashChain");
			
			
			
			// 2. Make Hash and hashchain
			$this->Trace("2. Make Hash and HashChain");
			
			$strNewLogID = intval($strLogID)+1;
			$strLogDateTime = RenderDateTimeNow();

			$arrayFields = array();
			$arrayFields["LOG_ID"] = $strNewLogID;
			$arrayFields["LOG_DATETIME"] = $strLogDateTime;
			$arrayFields["REPORTER_ID"] = $strReporterID;
			$arrayFields["EVENT_ID"] = $strEventID;
			$arrayFields["EVENT_SEVERITY"] = $strEventSeverity;
			$arrayFields["EVENT_DATETIME"] = $strEventDateTime;
			$arrayFields["EVENT_MESSAGE"] = $strEventMessage;
			$arrayFields["EVENT_DETAILS"] = $strEventDetails;
			
			$strHash = MakeHash($arrayFields);
			$this->Trace("HASH: $strHash");
			
			$strHashChain = $this->MakeHashChain($strPreviousHashChain,$strHash);
			$this->Trace("HASHCHAIN: $strHashChain");



	
			// 3. Add new log entry
			$this->Trace("3. Add new log entry");

			$arrayFields["HASH"] = $strHash;
			$arrayFields["HASHCHAIN"] = $strHashChain;
			

			$strSQL = MakeSQL_Insert(
				$strTable,
				$arrayFields);
			
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			$this->DatabaseUnlockTables();
			
			$strHashsinkEmail = $this->GetConfig("hashsink_email");
			if (IsValidEmail($strHashsinkEmail))
			{
				$this->SendMail(
					$strHashsinkEmail,
					"LOG HASH ($strNewLogID)",
					"LOG_ID:    $strNewLogID\n".
					"HASH:      $strHash\n".
					"HASHCHAIN: $strHashChain");
				$this->Trace("Hash mail sent");
			}
			
			$strLogEmail = $this->GetConfig("logmail_email");
			if (	IsValidEmail($strLogEmail)
				&& ($this->GetParamBoolean("logmail") || $this->GetConfigBoolean("logmail_active")) )
			{
				$this->SendMail(
					$strLogEmail,
					"LOG $strReporterID / $strEventSeverity / $strEventID ($strNewLogID)",
					ArrayRender($arrayFields) 
					);
				$this->Trace("Log mail sent");
			}
			
			$this->SetResultValue($strNewLogID,"NEW_LOG_ID");
			$this->SetResultValue("ADDED","STATUS");

			
			return;	
		}
		
		function CommandInstall()
		{
			$this->Trace("CommandInstall");
			
			parent::CommandInstall();
			
			if ($this->GetError() == "DATABASE_INSTALL_NOT_REQUIRED")
			{
				return;	
			}
						
			$arrayFields = array();
			$arrayFields["LOG_ID"] = "1";
			$arrayFields["LOG_DATETIME"] = RenderDateTimeNow();
			$arrayFields["REPORTER_ID"] = "SYSTEM/LOG";
			$arrayFields["EVENT_SEVERITY"] = "INFORMATIONAL";
			$arrayFields["EVENT_ID"] = "INIT";
			$arrayFields["EVENT_DATETIME"] = $arrayFields["LOG_DATETIME"];
			$arrayFields["EVENT_MESSAGE"] = $this->m_strInitEntry;
			$arrayFields["EVENT_DETAILS"] = "";

			$strHash = MakeHash($arrayFields);

			$strHashChain = $this->MakeHashChain("",$strHash);

			$arrayFields["HASH"] = $strHash;
			$arrayFields["HASHCHAIN"] = $strHashChain;						
	
			$strTable = $this->GetDatabaseTableName("log");
			
			$strSQL = MakeSQL_Insert(
				$strTable,
				$arrayFields);

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			return;	
		}


		function CommandVerify()
		{
			$strTable = $this->GetDatabaseTableName("log");			
			
			/*
  			Verify if the log is consistent, complete, and unaltered.
  			
  			1. First log id must be 0
  			2. There mustbe no missing log entries
  			3. The datetime of the log must be consistent with the increment of log ids
  			4. The hash value must be correct
  			5. The hash chain must be correct
  						
			*/

			// 1. get the latest LOG_ID
			
			$strSQL = "SELECT `".$strTable."`.`LOG_ID` FROM `".$strTable."` ORDER BY `".$strTable."`.`LOG_ID` DESC LIMIT 0 , 1";
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			$this->Trace($this->m_database->GetData());
			$arrayData = $this->m_database->GetData();
			
			$strLogID = ArrayGetValue(ArrayGetValue($arrayData,0),"LOG_ID");
			
			$this->Trace("Latest LOG_ID: $strLogID");
			
			$nLatestLogID = intval($strLogID);
						
			
			// 2. Loop through all entries starting from the oldest one and load into memory
			
			$nCurrentLogID = 1;
			
			$strPreviousHashChain = "";
			
			while ($nCurrentLogID < $nLatestLogID)
			{
				$strSQLCondition = "1 ORDER BY `LOG_ID` ASC LIMIT ".($nCurrentLogID-1).", 10000";
				$strSQL = MakeSQL_Select($strTable,"*",$strSQLCondition);
				
				if ($this->DatabaseQuery($strSQL) != true)
				{
					return; 
				}
				
				$arrayData = $this->m_database->GetData();
				foreach ($arrayData as $row)
				{
					if ($nCurrentLogID != $row["LOG_ID"])
					{
						$this->Trace("Invalid LOG_ID at LOG_ID=".$row["LOG_ID"]." (should be $nCurrentLogID)");
						$this->Trace($row);
						$this->SetError("INVALID_LOG_ID");
						return;
					}
					
					
					if ($nCurrentLogID == 1)
					{
						if ($row["EVENT_MESSAGE"] != $this->m_strInitEntry)
						{
							$this->SetError("INVALID_INIT_ENTRY");
							$this->Trace($row);
							return;
						}
					}
					
					$arrayHash = $row;
					$arrayHash = ArrayRemoveKey($arrayHash,"HASH");
					$arrayHash = ArrayRemoveKey($arrayHash,"HASHCHAIN");
				
					$strHash = MakeHash($arrayHash);
						
					$strHashChain = $this->MakeHashChain($strPreviousHashChain,$strHash);
					
					if ($strHash != $row["HASH"])
					{
						$this->Trace("Invalid HASH at LOG_ID=".$row["LOG_ID"]);
						$this->Trace($row);
						$this->SetError("INVALID_HASH");
						return;
					}
					if ($strHashChain != $row["HASHCHAIN"])
					{
						$this->Trace("Invalid HASHCHAIN at LOG_ID=".$row["LOG_ID"]);
						$this->Trace($row);
						$this->SetError("INVALID_HASHCHAIN");
						return;
					}

					$strPreviousHashChain = $row["HASHCHAIN"];
					$nCurrentLogID++;
				}
				
			}
			
			return;	
		}
	}

