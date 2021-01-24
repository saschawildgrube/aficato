<?php

	require_once(GetWDKDir()."wdk_webservice_database_entity.inc");
	require_once(GetWDKDir()."wdk_crontab.inc");
	require_once(GetWDKDir()."wdk_entity_job.inc"); 
		
   class CSchedulerWebService extends CDatabaseEntityWebService
	{
		private $m_nMaxTimeout;
		
		function __construct(
			$strSourceDir,
			$strServiceDir,
			$arrayConfig,
			$arrayParams)
		{
			$arrayMeta = array();
			$arrayMeta["version"] = WDK_VERSION; 
			$arrayMeta["author"] = "Sascha Wildgrube";
			
			$arrayEntityDefinition = array(
				"job" => GetEntityDefinitionJob()
				);


			$this->m_nMaxTimeout = intval(ArrayGetValue($arrayConfig,"max_timeout"));
			if ($this->m_nMaxTimeout <= 0 || $this->m_nMaxTimeout >= 180)
			{
				$this->m_nMaxTimeout = 180;
			}


			parent::__construct(
				"system/scheduler",
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$arrayEntityDefinition);
		}
		
		function OnInitParameterDefinition(&$paramDef)
		{
			$paramDef->AddParameterDefinitionBoolean("forceexecute");
			parent::OnInitParameterDefinition($paramDef);
		}
		
		function OnCheckParameter($strCommand,$strParamName,$strParamValue)
		{
			if ($strParamName == "timeout_seconds")
			{
				$nTimeoutSeconds = intval($strParamValue);
				if ($nTimeoutSeconds > $this->GetMaxTimeout())
				{
					return false;
				}
			}	
			return parent::OnCheckParameter($strCommand,$strParamName,$strParamValue);
		}
		
		function OnInitCommandDefinition(&$commandDef)
		{
			$commandDef->AddCommandDefinition("execute",array("job_id","forceexecute"));
			$commandDef->AddCommandDefinition("crontab",array());
			$commandDef->AddCommandDefinition("status",array());
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
		
		function OnCommand($strCommand)
		{
			switch ($strCommand)
			{
			case "install":
				$this->CommandInstall();
				return;

			case "execute":
				$this->CommandExecute();
				return;

			case "crontab":
				$this->CommandCrontab();
				return;

			case "status":
				$this->CommandStatus();
				return;

			default:
				parent::OnCommand($strCommand);
				return;
			}
		}

		function GetMaxTimeout()
		{
			return $this->m_nMaxTimeout;	
		}
		
		function CheckParamsAndMakeSQLCondition_JobID()
		{
			$strSQLCondition = "";
			if ($this->GetParam("job_id") == "")
			{
				return "";	
			}
			$strSQLCondition = "`JOB_ID` = ".MakeSQL_Helper_Value($this->GetParam("job_id"));
			return $strSQLCondition;
		}
		
		function CalculateNextExecutionDateTime(
			$strPivotDatetime,
			$strMinute,
			$strHour,
			$strDayOfMonth,
			$strMonth,
			$strDayOfWeek)
		{
			$this->Trace("CalculateNextExecutionDateTime BEGIN");
			
			$strNextDateTime = CrontabCalculateNextDateTime(
				$strPivotDatetime,
				$strMinute,
				$strHour,
				$strDayOfMonth,
				$strMonth,
				$strDayOfWeek,
				$strError);
			
			$this->Trace("CalculateNextExecutionDateTime: \"$strNextDateTime\" (Error = \"$strError\")");
						
			$this->Trace("CalculateNextExecutionDateTime END");
		
			if ($strNextDateTime == "" || $strError != "")
			{
				return RenderDateTimeNow();
			}
			
			
			return $strNextDateTime;
		}

		function CommandAdd($strEntity)
		{
			parent::CommandAdd($strEntity);
			if ($strEntity == "job")
			{
				if ($this->m_database->GetErrorCode() == 1062)
				{
					$this->SetError("JOB_NAMEOCCUPIED",true);
				}
			}
		}

		function OnCommandAddPrepare($strEntity,&$arrayFields)
		{
			if ($strEntity == "job")
			{
				$strDateTimeNow = RenderDateTimeNow();
				$arrayFields["CREATION_DATETIME"] = $strDateTimeNow;
				$arrayFields["LASTCHANGE_DATETIME"] = $arrayFields["CREATION_DATETIME"];
				$arrayFields["NEXTEXECUTION_DATETIME"] = $this->CalculateNextExecutionDateTime(
					$strDateTimeNow,
					$arrayFields["SCHEDULE_MINUTE"],
					$arrayFields["SCHEDULE_HOUR"],
					$arrayFields["SCHEDULE_DAYOFMONTH"],
					$arrayFields["SCHEDULE_MONTH"],
					$arrayFields["SCHEDULE_DAYOFWEEK"]);
			}
			return parent::OnCommandAddPrepare($strEntity,$arrayFields);				
		}
		function OnCommandAddComplete($strEntity,$arrayItem,$strNewItemID)
		{
			if ($strEntity == "job")
			{
				$this->SystemLog(
					"INFORMATIONAL",
					"JOB_ADD",
					"A new job has been added: ".$arrayItem["JOB_NAME"],
					array("jobid"=>$strNewItemID,"jobname"=>$arrayItem["JOB_NAME"]));
			}
			return parent::OnCommandAddComplete($strEntity,$arrayItem,$strNewItemID);
		}		
		
		function CommandSet($strEntity)
		{
			parent::CommandSet($strEntity);
			if ($strEntity == "job")
			{
				if ($this->m_database->GetErrorCode() == 1062)
				{
					$this->SetError("JOB_NAMEOCCUPIED",true);
				}
			}
		}		

		function OnCommandSetPrepare($strEntity,&$arrayFields)
		{
			if ($strEntity == "job")
			{
				$strDateTimeNow = RenderDateTimeNow();
				$arrayFields["LASTCHANGE_DATETIME"] = RenderDateTimeNow();
				$arrayFields["NEXTEXECUTION_DATETIME"] = $this->CalculateNextExecutionDateTime(
					$strDateTimeNow,
					$arrayFields["SCHEDULE_MINUTE"],
					$arrayFields["SCHEDULE_HOUR"],
					$arrayFields["SCHEDULE_DAYOFMONTH"],
					$arrayFields["SCHEDULE_MONTH"],
					$arrayFields["SCHEDULE_DAYOFWEEK"]);			
			}
			return parent::OnCommandSetPrepare($strEntity,$arrayFields);
		}
		
		/*
		
		
			if ($this->DatabaseQuery($strSQL) != true)
			{
				if ($this->m_database->GetErrorCode() == 1062)
				{
					$this->SetError("JOB_MODIFY_NAMEOCCUPIED",true);
				}
				return;
			}

		
		
		*/


	


		function CommandExecute()
		{
			$this->Trace("CommandExecute");
			
			$strSQLCondition = $this->CheckParamsAndMakeSQLCondition_JobID();
			if ($strSQLCondition == "")
			{
				return;	
			}
			
			$strTable = $this->GetDatabaseTableName("job");
			
			$strSQL = MakeSQL_Select(
				$strTable,
				"*",
				$strSQLCondition);

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			$arrayData = $this->m_database->GetData();
			$nRows = ArrayCount($arrayData);
			if ($nRows == 0)
			{
				$this->SetError("JOB_NOT_FOUND");
				return;
			}
			if ($nRows > 1)
			{
				$this->SetError("JOB_AMBIGUOUS");
				return;
			}
			$row = ArrayGetValue($arrayData,0);
			
			$bActive = $row["JOB_ACTIVE"];
			$bForceExecution = $this->GetParamBoolean("forceexecute");
			
			if (($bActive != true) && ($bForceExecution == false))
			{
				$this->SetError("JOB_INACTIVE");
				return;	
			}
			
			$strJobURL = $row["JOB_URL"];
			$strJobPostParams = $row["JOB_POSTPARAMS"];
			$strJobID = $row["JOB_ID"];
			$strJobName = $row["JOB_NAME"];
			$nTimeoutSeconds = $row["TIMEOUT_SECONDS"];
			
			$strDateTimeNow = RenderDateTimeNow();

			$arrayFields = array();			
			$arrayFields["LASTEXECUTION_DATETIME"] = $strDateTimeNow;
			$arrayFields["NEXTEXECUTION_DATETIME"] = $this->CalculateNextExecutionDateTime(
				$strDateTimeNow,
				$row["SCHEDULE_MINUTE"],
				$row["SCHEDULE_HOUR"],
				$row["SCHEDULE_DAYOFMONTH"],
				$row["SCHEDULE_MONTH"],
				$row["SCHEDULE_DAYOFWEEK"]);

			
			$strSQL = MakeSQL_Update(
				$strTable,
				$arrayFields,
				$strSQLCondition);
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}

			
			if ($this->ExecuteJob($strJobURL,$strJobPostParams,$strJobID,$strJobName,$row["TIMEOUT_SECONDS"]) == false)
			{
				return;	
			}
			
	
			return;
		}


		function SetCrontabHeartBeat()
		{
			$bCrontabHeartBeat = $this->GetConfigBoolean("crontab_heartbeat");
			if ($bCrontabHeartBeat == false)
			{
				return;	
			}
			$strWebserviceURL = $this->GetWebserviceURL("system/data");
			if ($strWebserviceURL == "")
			{
				return;
			}
			$arrayParam = array();
			$arrayParam["command"] = "set";
			$arrayParam["data_name"] = "crontab_heartbeat";
			$arrayParam["data_content"] = RenderDateTimeNow();  
			$consumer = new CWebServiceConsumerWebApplication($this);    
			$consumer->ConsumeWebService("system/data",$arrayParam);
			return;
		}
		
		function GetCrontabHeartBeat()
		{
			$bCrontabHeartBeat = $this->GetConfigBoolean("crontab_heartbeat");
			if ($bCrontabHeartBeat == true)
			{
				$strWebserviceURL = $this->GetWebserviceURL("system/data");
				if ($strWebserviceURL != "")
				{
					$arrayParam = array();
					$arrayParam["command"] = "get";
					$arrayParam["data_name"] = "crontab_heartbeat";
					$consumer = new CWebServiceConsumerWebApplication($this);    
					$consumer->ConsumeWebService("system/data",$arrayParam);
					if ($this->GetError() == "")
					{
						$strCrontabHeartBeat = $consumer->GetResultValue("DATA","DATA_CONTENT");	
						if ($strCrontabHeartBeat != "")
						{
							return $strCrontabHeartBeat;
						}
					}
				}
			}
			return false;			
		}
		


		function CommandCrontab()
		{
			$this->Trace("CommandCrontab");
			
			$this->SetCrontabHeartBeat();
			
			$strTable = $this->GetDatabaseTableName("job");
			
			$nCount = 0;
			while(1)
			{
				$strDateTimeNow = RenderDateTimeNow();
				
				$strSQLCondition = "`NEXTEXECUTION_DATETIME` < '$strDateTimeNow' AND `JOB_ACTIVE` <> '0' LIMIT 1";
			
				$strSQL = MakeSQL_Select(
					$strTable,
					"*",
					$strSQLCondition);
				if ($this->DatabaseQuery($strSQL) != true)
				{
					return;
				}
			
				$arrayData = $this->m_database->GetData();
			
				$nRows = ArrayCount($arrayData);
						
				if ($nRows == 0)
				{
					$this->Trace("No more jobs to be executed.");
					$this->SystemLog("INFORMATIONAL","CRONTAB","$nCount jobs executed");
					return;
				}
				if ($nRows > 1)
				{
					$this->SetError("SQL");
					$this->Trace("SQL statement was not interpreted properly. More than one rows were returned for a SELECT statement with \"LIMIT 1\".");	
					$this->SendErrorMailToAdmin();
					return;
				}
				
				$nCount++;
				
				$row = $arrayData[0];
				
				$this->Trace("");
				$this->Trace("Crontab: Execute Job BEGIN: ".$row["JOB_NAME"]);
				
				$strJobURL = $row["JOB_URL"];
				$strJobPostParams = $row["JOB_POSTPARAMS"];
				$strJobID = $row["JOB_ID"];
				
				$arrayFields = array();			
				$arrayFields["NEXTEXECUTION_DATETIME"] = $this->CalculateNextExecutionDateTime(
					$strDateTimeNow,
					$row["SCHEDULE_MINUTE"],
					$row["SCHEDULE_HOUR"],
					$row["SCHEDULE_DAYOFMONTH"],
					$row["SCHEDULE_MONTH"],
					$row["SCHEDULE_DAYOFWEEK"]);
				$arrayFields["LASTEXECUTION_DATETIME"] = $strDateTimeNow;
				$strSQL = MakeSQL_Update(
					$strTable,
					$arrayFields,
					"`JOB_ID` = ".MakeSQL_Helper_Value($strJobID));
				if ($this->DatabaseQuery($strSQL) != true)
				{
					$this->SendErrorMailToAdmin();
					return;
				}
				if ($this->ExecuteJob($strJobURL,$strJobPostParams,$strJobID,$row["JOB_NAME"],$row["TIMEOUT_SECONDS"]) == false)
				{
					// if there was an error, schedule the job for re-execution
					$arrayFields["NEXTEXECUTION_DATETIME"] = $strDateTimeNow;
					$arrayFields["LASTEXECUTION_DATETIME"] = $strDateTimeNow;
					$strSQL = MakeSQL_Update(
						$strTable,
						$arrayFields,
						"`JOB_ID` = ".MakeSQL_Helper_Value($strJobID));
					if ($this->DatabaseQuery($strSQL) != true)
					{
						$this->SendErrorMailToAdmin();
						return;
					}
				}

				$this->Trace("Crontab: Execute Job END: ".$row["JOB_NAME"]);
				$this->Trace("");
			}
	
			return;
		}
		
		function CommandStatus()
		{
			$strCrontabHeartBeat = $this->GetCrontabHeartBeat();
			if ($strCrontabHeartBeat != "")
			{
				$this->SetResultValue($strCrontabHeartBeat,"CRONTAB_HEARTBEAT_DATETIME");	
			}
		}


		function ExecuteJob($strJobURL,$strJobPostParams,$strJobID,$strJobName,$nTimeoutSeconds)
		{
			$this->Trace("");
			$this->Trace("EXECUTE JOB BEGIN: \"$strJobName\" (ID=$strJobID)");
			/*$strAccessCode = $this->GetWebserviceAccessCode($strJobURL);
			*if (IsValidURL($strJobURL) == false)
			{
				$strJobURL = $this->GetWebserviceURL($strJobURL);
			}
			*/
			$bIsWebservice = false;
			if (IsValidURL($strJobURL) == false)
			{
				$strJobURL2 = $this->GetWebserviceURL($strJobURL);
				if (IsValidURL($strJobURL2) == false)
				{
					$this->SetError("JOB_EXECUTION");
					return false;
				}
				else
				{
					$bIsWebservice = true;		
				}
			}

			$arrayParams = ParseURLParameters($strJobPostParams);
			$strMode = (ArrayCount($arrayParams))?("post"):("get");
			
			/*
			if ($strMode == "post")
			{
				if ($strAccessCode != "")
				{
					if (ArrayKeyExists($arrayParams,"accesscode") == false)
					{
						$arrayParams["accesscode"] = $strAccessCode;
					}
				}
			}
			*/
			
			
			$this->Trace("Job request timeout    : ".(($nTimeoutSeconds == 0)?("not configured"):($nTimeoutSeconds)));
			$this->Trace("Maximum request timeout: ".$this->GetMaxTimeout());
			if ($nTimeoutSeconds > 0)
			{
				$nTimeoutSeconds = min($nTimeoutSeconds,$this->GetMaxTimeout());	
			}
			else
			{
				$nTimeoutSeconds = $this->GetMaxTimeout();
			}
			$this->Trace("Actual request timeout : ".$nTimeoutSeconds);
			$stopwatch = new CStopWatch();
			$stopwatch->Start();
			$strJobOutput = false;
			if ($bIsWebservice == false)
			{
				$strJobOutput = HttpRequest($strJobURL,$arrayParams,$strMode,$nTimeoutSeconds);
				if ($strJobOutput == false)
				{
					$this->SetError("JOB_EXECUTION");	
				}
			}
			else
			{
				$consumer = new CWebserviceConsumerWebApplication($this);
				$consumer->ConsumeWebService($strJobURL,$arrayParams,$nTimeoutSeconds);
				if ($consumer->GetError() != "")
				{
					if ($consumer->GetError() == "CAPACITY_CONSTRAINT")
					{
						// A CAPACITY_CONSTRAINT is an error, but we don't treat it as such. Retrying would make it worse!
					}
					else
					{
						$strJobOutput = false;
						$this->SetError("JOB_EXECUTION_WEBSERVICE");	
					}
					
					
					//$this->SetResult("WEBSERVICE_ERROR",$consumer->GetError());
				}
				else
				{
					$strJobOutput = $consumer->GetServiceOutput();	
				}
			}
			$stopwatch->Stop();
			$this->Trace("Seconds elapsed in http request: ".$stopwatch->GetSeconds());
			if ($strJobOutput == false)
			{
				if (ArrayKeyExists($arrayParams,"accesscode") == true)
				{
					$arrayParams["accesscode"] = "*";
				}
				$this->SystemEvent(
					"WARNING",
					"JOB_EXECUTION_FAILED",
					"$strJobName ($strJobID)",
					array(
						"jobid" => $strJobID,
						"jobname" => $strJobName,
						"joburl" => $strJobURL,
						"timeout" => $nTimeoutSeconds,
						"postparams" => MakeURLParameters($arrayParams,", ")
						)
					);				
			}
			else
			{
				$this->Trace("JOB OUTPUT BEGIN:");
				$this->Trace($strJobOutput);
				$this->Trace("JOB OUTPUT END");
				$this->SystemLog(
					"INFORMATIONAL",
					"JOB_EXECUTED",
					"$strJobName ($strJobID)",
					array(
						"jobid" => $strJobID,
						"joburl" => $strJobURL
					));
			}
			$this->Trace("EXECUTE JOB END: \"$strJobName\" ($strJobID)");
			$this->Trace("");
			return true;
		}
		

	}
	

