<?php

	error_reporting (E_ALL);
	ini_set('display_errors', 1);

	require_once("wdk_webapplication.inc");
	require_once("wdk_webserviceconsumer_webapplication.inc");

	class CInstaller extends CWebApplication
	{
		function __construct()
		{
			header('Content-type: text/plain');
			header('X-Content-Type-Options: nosniff');
			header("Content-Disposition: attachment;filename=\"install_log.txt\"");
 			LockParameterSources();
 
			$arrayConfig = array();
 			$arrayConfig["webservices"] = GetWebservicesDirectory();
 	
			parent::__construct(
				array(),
				$arrayConfig,
				array());
			
			$this->SetTraceActive(true);
			
			$this->Trace("INSTALL");
			$this->Trace("");
			
			if (CheckEnvironment() == false)
			{
				$this->Trace("CheckEnvironment returned false. Check out env.inv if all environemnt functions have been defined!");
				return;
			}
					
			$bResult = $this->OnInstall();
			if ($bResult == false)
			{
				$this->Trace("INSTALLATION FAILED");
				$this->Rollback();
				return;
			}
			$this->Trace("INSTALLATION COMPLETED");
		}
		
		final
		function Rollback()
		{
			$this->Trace("ROLLBACK BEGIN");
			$bResult = $this->OnRollback();
			if ($bResult == false)
			{
				$this->Trace("ERROR: Rollback failed!");
			}
			$this->Trace("ROLLBACK END");
		}
		
		function __destruct()
    	{
			print($this->GetTraceLog());
		}

		
		function OnInstall()
		{
			$this->Trace("OnInstall is not implemented!");
			return true;
		}
		function OnRollback()
		{
			$this->Trace("OnRollback is not implemented!");
			return true;
		}
		
		function InstallWebservice($strWebservice)
		{
			$this->Trace("Install $strWebservice");
			$consumer = new CWebServiceConsumerWebApplication($this);

			$arrayParams = array();
			$arrayParams["command"] = "selfcheck";
			$consumer->ConsumeWebService($strWebservice,$arrayParams);
			if ($consumer->GetError() != "")
			{
				$this->Trace("$strWebservice self check failed.");	
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return false;	
			}
			$this->Trace("");	

			
			$arrayParams = array();
			$arrayParams["command"] = "install";
			$consumer->ConsumeWebService($strWebservice,$arrayParams);
			if (($consumer->GetError() != "") && ($consumer->GetError() != "INSTALL_NOT_REQUIRED"))
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return false;	
			}
			else
			{
				$this->Trace("$strWebservice installed successfully.");	
			}
			$this->Trace("");	
			return true;
		}
		
		function AddUser($strUserName,$strPassword,$arrayPrivileges)
		{
			$this->Trace("Add user \"$strUserName\"");
			$consumer = new CWebServiceConsumerWebApplication($this);
			$arrayParams = array();
			$arrayParams["command"] = "add";
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["password"] = $strPassword;
			$arrayParams["ignoreblacklist"] = "1";
			$consumer->ConsumeWebService("system/user",$arrayParams);
			if (($consumer->GetError() != ""))
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return false;	
			}
			$strUserID = $consumer->GetResultValue("NEW_USER_ID");
			if ($strUserID == "")
			{
				$this->Trace("Error: New user ID is empty.");		
				return false;	
			}
			
			
			if (is_array($arrayPrivileges))
			{
				foreach ($arrayPrivileges as $strPrivilege)
				{
					$consumer = new CWebServiceConsumerWebApplication($this);
					$arrayParams = array();
					$arrayParams["command"] = "set";
					$arrayParams["user_id"] = $strUserID;
					$arrayParams["privilege_id"] = $strPrivilege;
					$consumer->ConsumeWebService("system/entitlement",$arrayParams);
					if ($consumer->GetError() != "")
					{
						$this->Trace("Error: \"".$consumer->GetError()."\"");		
						return false;	
					}
				}
			}
			
			$this->Trace("");	
			return true;
		}


		
		function AddScheduledJob(
			$strJobName,
			$strJobURL,
			$arrayJobParams,
			$strMinute,
			$strHour,
			$strDayOfMonth,
			$strMonth,
			$strDayOfWeek,
			$nTimeoutSeconds = 0)
		{
			$this->Trace("Add Scheduled Job \"$strJobName\"");
			
			$strJobPostParams = MakeURLParameters($arrayJobParams,"\n");

			// Set up scheduler job
			$arrayParams = array();
			$arrayParams["job_name"] = $strJobName;
			$arrayParams["job_url"] = $strJobURL;
			$arrayParams["job_postparams"] = $strJobPostParams;
			$arrayParams["job_active"] = "1";
			$arrayParams["schedule_minute"] = $strMinute;
			$arrayParams["schedule_hour"] = $strHour;
			$arrayParams["schedule_dayofmonth"] = $strDayOfMonth;
			$arrayParams["schedule_month"] = $strMonth;
			$arrayParams["schedule_dayofweek"] = $strDayOfWeek;
			$arrayParams["timeout_seconds"] = $nTimeoutSeconds;
			
			$arrayParams["command"] = "add";
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService("system/scheduler",$arrayParams);
			if ($consumer->GetError() != "" && $consumer->GetError() != "JOB_NAMEOCCUPIED")
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");
				return;
			}
	
			$this->Trace("");	
		}			

		
		function Test()
		{
			$this->Trace("Execute all local tests");
			$this->SetTraceActive(false);
			$consumer = new CWebServiceConsumerWebApplication($this);
			$arrayParams = array();
			$arrayParams["command"] = "testlocal";
			$consumer->ConsumeWebService("system/test",$arrayParams,600);
			$this->SetTraceActive(true);
			if (($consumer->GetError() != ""))
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return false;	
			}
			$nFailed = intval($consumer->GetResultValue("STATISTICS","FAILED"));
			$this->Trace("Failed Tests: $nFailed");
			if ($nFailed > 0)
			{
				$this->Trace("Tests have failed.");		
				return false;	
			}
			$nRequestError = intval($consumer->GetResultValue("STATISTICS","REQUESTERROR"));
			$this->Trace("Tests reporting request errors: $nRequestError");
			if ($nRequestError > 0)
			{
				$this->Trace("Tests have reported request errors.");		
				return false;	
			}		
			$nUndefined = intval($consumer->GetResultValue("STATISTICS","UNDEFINED"));
			$this->Trace("Undefined Tests: $nUndefined");
			if ($nUndefined > 0)
			{
				$this->Trace("Tests are undefined.");		
				return false;	
			}
			$this->Trace("");
			return true;
		}

	}

