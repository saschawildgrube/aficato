<?php

	require_once("wdk_webservice.inc");
	require_once("wdk_mysql.inc");

	abstract class CDatabaseWebService extends CWebService
	{
		protected $m_database;
		protected $m_fSecondsSQLQueryAlertThreshold;
		protected $m_stopwatchDatabaseQueries;
		
		function __construct(
			$strName,
			$strSourceDir,
			$strServiceDir,
			$arrayMeta,
			$arrayConfig,
			$arrayParams,
			$arrayObfuscationItems
			)
		{
			$this->m_database = new CDatabase();
			$this->m_stopwatchDatabaseQueries = new CStopWatch();

			parent::__construct(
				$strName,
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$arrayObfuscationItems);
		}
		
		function CallbackInit()
		{
			$bResult = parent::CallbackInit();
			if ($bResult == false)
			{
				return false;
			}
			return true;
		}
		

		function CallbackInitCommandDefinition(&$commandDef)
		{
			parent::CallbackInitCommandDefinition($commandDef);
		}		
		
		function CallbackCommand($strCommand)
		{
			parent::CallbackCommand($strCommand);	
		}
		
		function CommandSelfCheck()
		{
			parent::CommandSelfCheck();
			if ($this->GetError() != "")
			{
				return;	
			}
			$this->Trace("CDatabaseWebservice::CommandSelfCheck()");

			// SQL Safe Mode
			$this->Trace("Checking SQL Safe Mode PHP setting...");
			$bSqlSafeMode = GetBoolValue(ini_get("sql.safe_mode"));
			if ($bSqlSafeMode == true)
			{
				$this->Trace("PHP setting sql-safe_mode must not be turned on!");
				$this->SetError("DATABASE_CONFIG");
				return;
			}
	
		}
		
		function CommandInstall()
		{
			parent::CommandInstall();
			if ($this->GetError() != "")
			{
				return;	
			}
			
			
			$strInstallFile = $this->GetSourceDir()."install.sql";
			if (!IsFile($strInstallFile))
			{
				$this->SetError("DATABASE_INSTALL_FILE_NOT_FOUND");
				return;
			}
			$strInstallSQL = FileRead($strInstallFile);
			if ($strInstallSQL === false)
			{
				$this->SetError("DATABASE_INSTALL_FILE_READ_FAILED");
				return;
			}
			
			if (chop($strInstallSQL) == "")
			{
				$this->Trace("SQL install file is empty. No further action required.");
				return;	
			}
			
			$arraySQL = array();
			
			$strEOL = StringGetEndOfLineToken($strInstallSQL);
			$this->Trace("EOL: ".$strEOL);
			if ($strEOL == "")
			{
				$arraySQL = array($strInstallSQL);	
			}
			else
			{
				$arraySQL = explode(";".$strEOL,$strInstallSQL);	
			}
			
			foreach ($arraySQL as $strSQL)
			{
				$strSQL = chop($strSQL);
				if ($strSQL != "")
				{
					$this->Trace("SQL: $strSQL");
					if ($this->m_database->Query($strSQL) != true)
					{
						$this->TraceDatabaseError();
						if ($this->m_database->GetErrorCode() == 1050)
						{
							$this->SetError("INSTALL_NOT_REQUIRED");
						}
						else
						{
							$this->SetError("DATABASE_QUERY_FAILED");
							return;
						}
					}
				}
			}			
		}			
		
		function DatabaseConnect()
		{
			$arrayConfigErrors = array();
			if ($this->DatabaseCheckConfig($this->GetConfigArray(),$arrayConfigErrors) == false)
			{
				$this->Trace("DatabaseCheckConfig() returned errors:");
				$this->Trace($arrayConfigErrors);
				$this->Trace("Maybe the config could not be retrieved from a config location.");
				$this->SetError("DATABASE_CONFIG");
				return false;  
			}
			
			if ($this->m_database->Connect(
				$this->GetConfig('database_host'),
				$this->GetConfig('database_user'),
				$this->GetConfig('database_password'),
				$this->GetConfig('database_name'),
				$this->GetConfig('database_tablenameprefix'),
				$this->GetConfig('database_port')) != TRUE)
			{
				$this->SetError("DATABASE_CONNECT_FAILED");
				return false;
			}

			return true;
	
		}
		
		function DatabaseCheckConfig($arrayConfig,&$arrayConfigErrors)
		{
			$bResult = true;
			if (!is_array($arrayConfig))
			{
				$arrayConfigErrors[] = "Config array is not an array.";
				$bResult = false;
			}
			$strHost = ArrayGetValue($arrayConfig,"database_host");
			if ($strHost == "")
			{
				$arrayConfigErrors[] = "Database host is not specified.";
				$bResult = false;
			}
			$strPort = ArrayGetValue($arrayConfig,"database_port");
			if ($strPort != "")
			{
				if (StringCheckCharSet($strPort,CHARSET_NUMERIC) != true)
				{
					$arrayConfigErrors[] = "Database port must be numeric.";
					$bResult = false;
				}
			}
			$strUsername = ArrayGetValue($arrayConfig,"database_user");
			if ($strUsername == "")
			{
				$arrayConfigErrors[] = "Database user is not specified.";
				$bResult = false;
			}
			$strPassword = ArrayGetValue($arrayConfig,"database_password");
			if ($strPassword == "")
			{
				$arrayConfigErrors[] = "Database user password is not specified.";
				$bResult = false;
			}
			$strDatabase = ArrayGetValue($arrayConfig,"database_name");
			if ($strDatabase == "")
			{
				$arrayConfigErrors[] = "Logical database name is not specified.";
				$bResult = false;
			}
			return $bResult;
		}
		
		function DatabaseQuery($strSQL)
		{
			$fSecondsQueryAlertThreshold = floatval($this->GetConfig("query_alert_threshold_seconds"));
			if ($fSecondsQueryAlertThreshold == 0)
			{
				$fSecondsQueryAlertThreshold = 30;   
			}
		
			$this->Trace("SQL: ".$strSQL);
			$this->m_stopwatchDatabaseQueries->Start();
			$bResult = $this->m_database->Query($strSQL);
			if ($bResult != true)
			{
				if ($this->m_database->GetErrorCode() == 2006) // Server has gone away
				{
					if ($this->DatabaseConnect()==true)
					{
						$bResult = $this->m_database->Query($strSQL);
					}
				}
			}
			$this->m_stopwatchDatabaseQueries->Stop();

			$this->SetResultEnvelopeValue(RenderNumber($this->m_stopwatchDatabaseQueries->GetTotalSeconds(),3),
				"PROFILING","DATABASEQUERIES_SECONDS");

			$this->Trace("SQL: Query took ".RenderNumber($this->m_stopwatchDatabaseQueries->GetSeconds(),3)." seconds.");
			$this->TraceDatabaseError();
			if ($bResult != true)
			{
				$nErrorCode = $this->m_database->GetErrorCode();
				if ($nErrorCode == 1146)
				{
					// Table does not exist.
					$this->Trace("A missing table is an indicator for a not installed application system.");
					$this->SetError("DATABASE_INSTALL_REQUIRED");
					$this->SendErrorMailToAdmin();
					return false;
				}
				else if ($nErrorCode == 1062)
				{
					// duplicate entry: this should not be a reason for an error mail.
					$this->SetError("DATABASE_DUPLICATE_ENTRY");
					return false;
				}
				$this->SetError("DATABASE_QUERY_FAILED");
				$this->SendErrorMailToAdmin();
				return false;
			}
			$this->Trace("SQL: Affected rows: ".$this->DatabaseGetAffectedRows());
			if ($this->m_stopwatchDatabaseQueries->GetSeconds() > $fSecondsQueryAlertThreshold)
			{
				$this->SendMailToAdmin(
					"WARNING: SQL query took longer than ".RenderNumber($fSecondsQueryAlertThreshold,3)." seconds",
					"SQL: Query took ".RenderNumber($this->m_stopwatchDatabaseQueries->GetSeconds(),3)." seconds.\n\n".
					"SQL: ".$strSQL."\n\n\n".
					$this->GetTraceLog());
			}
			return true;
		}

		function DatabaseLockTable($strTable)
		{
			$this->Trace("SQL: Lock Table (WRITE): ".$strTable);
			if ($this->m_database->LockTable($strTable) != true)
			{
				if ($this->m_database->GetErrorCode() == 1146)
				{
					// Table does not exist.
					$this->Trace("A missing table is an indicator for a not installed application system.");
					$this->SetError("DATABASE_INSTALL_REQUIRED");
					$this->SendErrorMailToAdmin();
					return false;
				}				
				$this->TraceDatabaseError();
				$this->SetError("DATABASE_QUERY_FAILED");
				$this->SendErrorMailToAdmin();
				return false;
			}
			return true;
		}

		function DatabaseUnlockTables()
		{
			$this->Trace("SQL: Unlock Tables");
			if ($this->m_database->UnlockTables() != true)
			{
				$this->TraceDatabaseError();
				$this->SetError("DATABASE_QUERY_FAILED");
				$this->SendErrorMailToAdmin();
				return false;
			}
			return true;
		}

		function DatabaseGetDataFromLastQuery()
		{
			return $this->m_database->GetData();	
		}

		function DatabaseGetAffectedRows()
		{
			return $this->m_database->GetAffectedRows();	
		}


		function TraceDatabaseError()
		{
			$nCode = $this->m_database->GetErrorCode();
			if ($nCode == 0)
			{
				$this->Trace("SQL: query successful.");
			}
			else
			{
				$this->Trace("SQL: error: ".$nCode.": ".$this->m_database->GetErrorText());
			}
		}

	}
	
