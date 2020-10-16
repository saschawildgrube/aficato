<?php

	require_once(GetWDKDir()."wdk_webservice_database_entity.inc");
	require_once(GetWDKDir()."wdk_entity_session.inc");
	require_once(GetWDKDir()."wdk_ip.inc");
		
   class CSessionWebService extends CDatabaseEntityWebService
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

			$arrayEntityDefinition = array(
				"session" => GetEntityDefinitionSession()
				);
			
			parent::__construct(
				"system/session",
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$arrayEntityDefinition);
		}

		function CallbackInitParameterDefinition(&$paramDef)
		{
			$paramDef->AddParameterDefinitionInteger("duration",1,intval($this->GetConfig("duration_max")));
			$paramDef->AddParameterDefinitionUsername("user_name");
			$paramDef->AddParameterDefinitionString("authenticationpayload",1,255,CHARSET_TEXT);
			parent::CallbackInitParameterDefinition($paramDef);
		}
		
		function CallbackInitCommandDefinition(&$commandDef)
		{
			parent::CallbackInitCommandDefinition($commandDef);
			$commandDef->AddCommandDefinition("login",array("session_id","owner_id","user_id","user_name","authenticationpayload"));
			$commandDef->AddCommandDefinition("logout",array("session_id","owner_id"));	
			$commandDef->AddCommandDefinition("cleanup",array());	
		}


		function CallbackInit()
		{
			if ($this->DatabaseConnect() != true)
			{
				return false;
			}			
			return parent::CallbackInit();
		}

		function CallbackCommand($strCommand)
		{
			switch ($strCommand)
			{
			case "create":
				$this->CommandCreate();
				return;

			case "write":
				$this->CommandWrite();
				return;

			case "read":
				$this->CommandRead();
				return;

			case "login":
				$this->CommandLogin();
				return;

			case "logout":
				$this->CommandLogout();
				return;

			case "destroy":
				$this->CommandDestroy();
				return;
				
			case "cleanup":
				$this->CommandCleanUp();
				return;

			default:
				parent::CallbackCommand($strCommand);
				return;
			}
		}

	
		function CallbackCheckParameter($strCommand,$strParamName,$strParamValue)
		{
			switch ($strParamName)
			{
			case "user_ip":
				$strUserIP = $strParamValue;
				if ($strUserIP != "")
				{
					if (IsValidIPv4($strUserIP) != true)
					{
						$this->SetError("PARAMETER_USER_IP_INVALID");
						return false;	
					}
				}
				return true;

			default:
				return parent::CallbackCheckParameter($strCommand,$strParamName,$strParamValue);
			}
		}


		function MakeNewSessionID($strOwnerID)
		{
			return sha1(uniqid(rand()).GetTimeNow().rand().$strOwnerID);	
		}

		function UpdateLastAccess($strSessionID)
		{
			$arrayFields = array();
			$arrayFields["LASTACCESS_DATETIME"] = RenderDateTimeNow();
			$strSQL = MakeSQL_Update(
				$this->GetDatabaseTableName("session"),
				$arrayFields,
				"`SESSION_ID` = '$strSessionID' LIMIT 1");
			
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
		}

		function CommandInstall()
		{
			$this->Trace("CommandInstall");
			
			parent::CommandInstall();
			if ($this->GetError() != "")
			{
				return;	
			}

			// Set up clean up scheduler job
			$arrayParams = array();
			$arrayParams["command"] = "cleanup";
			$this->Install_AddScheduledJob(
				"SessionCleanup",
				$arrayParams,
				"0",
				"*",
				"*",
				"*",
				"*",
				20);
			
			return;	
		}

		
				
		function CommandCreate()
		{
			$this->Trace("CommandCreate");
			
			/*
			
			Create a new session by creating a new unique id and store the new session in the session db
			
			1. Check Parameters
			2. Create Unique identifier
			3. Add session
			4. Return session details
			5. Log

			
			*/
			
			// 1. Check Parameters
			
			$strOwnerID = $this->GetParam("owner_id");
			$nDuration = intval($this->GetParam("duration"));
			$strUserIP = $this->GetParam("user_ip");

			// 2. Create new unique identifier
			
			$strNewSessionID = $this->MakeNewSessionID($strOwnerID);
			
			$this->Trace("New Session ID: $strNewSessionID");
			
			
			// 3. Add session to database
			$strDateTimeNow = RenderDateTimeNow();
			$strDateTimeEnd = RenderDateTime(GetTimeNow() + $nDuration);
	
			$arrayFields = array();
			$arrayFields["SESSION_ID"] = $strNewSessionID;
			$arrayFields["OWNER_ID"] = $strOwnerID;
			$arrayFields["CREATION_DATETIME"] = $strDateTimeNow;
			$arrayFields["LASTACCESS_DATETIME"] = $strDateTimeNow;
			$arrayFields["END_DATETIME"] = $strDateTimeEnd;
			$arrayFields["DATA"] = "";
			$arrayFields["USER_IP"] = $strUserIP;
			
						 
			$strSQL = MakeSQL_Insert(
				$this->GetDatabaseTableName("session"),
				$arrayFields);
						 
					
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}

			// 4. Return Session details
			$this->SetResultValue($strNewSessionID,"NEW_SESSION_ID");

			// 5. Log
			$this->SystemLog("INFORMATIONAL","SESSION_CREATE",$strNewSessionID);
			
			return;	
		}
		
	
	
		function CommandWrite()
		{
			$this->Trace("CommandWrite");
			
			/*
			
			Write data to an existing session
			
			1. Check Parameters
			2. Check if session is expired
			3. Write data
						
			*/
			
			// 1. Check Parameters
	
			$strSessionID = $this->GetParam("session_id");
			$strOwnerID = $this->GetParam("owner_id");
			$strData = $this->GetParam("data");
			

			// 3. Write data
						 
			$arrayFields = array();
			$arrayFields["DATA"] = $strData;
			$arrayFields["LASTACCESS_DATETIME"] = RenderDateTimeNow();
			$strCondition = "`SESSION_ID` = '$strSessionID' AND `OWNER_ID` = '$strOwnerID' AND `END_DATETIME` > '".RenderDateTimeNow()."' LIMIT 1";
			$strSQL = MakeSQL_Update(
				$this->GetDatabaseTableName("session"),
				$arrayFields,
				$strCondition);
							
			
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			if ($this->m_database->GetAffectedRows() == 0)
			{
				$this->SetError("SESSION_NOT_FOUND");
				return;	
			}
		
			return;	
		}


		function CommandRead()
		{
			$this->Trace("CommandRead");
			
			/*
			
			Read session data
			
			1. Check Parameters
			2. Read session from database
			3. Create output
			4. Update last access timestamp
			
			*/
			
			// 1. Check Parameters
			$strSessionID = $this->GetParam("session_id");
			
			// 2. Read session from database
			$strSQL = MakeSQL_Select(
				$this->GetDatabaseTableName("session"),
				"*",
				"`SESSION_ID` = '$strSessionID' AND `END_DATETIME` > '".RenderDateTimeNow()."' LIMIT 0, 1");
			
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}

			$arrayData = $this->m_database->GetData();
			
			$nRows = ArrayCount($arrayData);
			
			if ($nRows == 0)
			{
				$this->SetError("SESSION_NOT_FOUND");
				return;
			}

			if ($nRows > 1)
			{
				$this->SetError("SESSION_AMBIGUOUS");
				return;
			}

		
			// 3. Create output
	
			$row = ArrayGetValue($arrayData,0);
			
			if (ArrayGetValue($row,"USER_ID_AUTHENTICATION") == "0")
			{
				$row["USER_ID_AUTHENTICATION"] = "";
			}

			$arraySessionTableFields = array();
			array_push($arraySessionTableFields,"SESSION_ID");
			array_push($arraySessionTableFields,"OWNER_ID");
			array_push($arraySessionTableFields,"CREATION_DATETIME");
			array_push($arraySessionTableFields,"LASTACCESS_DATETIME");
			array_push($arraySessionTableFields,"END_DATETIME");
			array_push($arraySessionTableFields,"DATA");
			array_push($arraySessionTableFields,"USER_ID_AUTHENTICATION");
			array_push($arraySessionTableFields,"USER_IP");

			$element = ArrayFilterByKeys($row,$arraySessionTableFields);
			$this->SetResultValue($element,"SESSION");
			
			// 4. Update last access timestamp

			$this->UpdateLastAccess($strSessionID);

			return;
		}



		function CommandLogin()
		{
			$this->Trace("CommandLogin");
			
			/*
			
			Check user authentication and write result into session and return to caller
			
			1. Check Parameters
			2. Authenticate user via user service
			3. Write result to session
			4. Return result to client
						
			*/
			
			// 1. Check Parameters
			
			$strSessionID = $this->GetParam("session_id");
			$strOwnerID = $this->GetParam("owner_id");
	
			
			// 2. User authentication
			
			$arrayParams = array();
			$arrayParams["user_id"] = $this->GetParam("user_id");
			$arrayParams["user_name"] = $this->GetParam("user_name");
			$arrayParams["command"] = "authenticate";
			$arrayParams["authenticationpayload"] = $this->GetParam("authenticationpayload");
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService("system/user",$arrayParams);
			
			if ($consumer->GetError() != "")
			{
				$this->SetError($consumer->GetError());
				return;
			}
			
			$this->Trace("Authentication successful.");

			$strUserID = $consumer->GetResultValue("USER","USER_ID");

			// 4. Write authentication to database

			$arrayFields = array();
			$arrayFields["USER_ID_AUTHENTICATION"] = $strUserID;
			$arrayFields["LASTACCESS_DATETIME"] = RenderDateTimeNow();
			$strSQL = MakeSQL_Update(
				$this->GetDatabaseTableName("session"),
				$arrayFields,
				"`SESSION_ID` = '$strSessionID' AND `OWNER_ID` = '$strOwnerID' AND `END_DATETIME` > '".RenderDateTimeNow()."' LIMIT 1");
										 
						 
			
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			if ($this->m_database->GetAffectedRows() == 0)
			{
				$this->SetError("SESSION_NOT_FOUND");
				return;
			}
			
			$this->CommandRead();
			
			
			return;	
		}

		function CommandLogout()
		{
			$this->Trace("CommandLogout");
			
		
			// 1. Check Parameters
			
			$strSessionID = $this->GetParam("session_id");
			$strOwnerID = $this->GetParam("owner_id");
	
		
			// 2. Write
			$arrayFields = array();
			$arrayFields["USER_ID_AUTHENTICATION"] = "";
			$arrayFields["LASTACCESS_DATETIME"] = RenderDateTimeNow();

			$strSQL = MakeSQL_Update(
				$this->GetDatabaseTableName("session"),
				$arrayFields,
				"`SESSION_ID` = '$strSessionID' AND `OWNER_ID` = '$strOwnerID' AND `END_DATETIME` > '".RenderDateTimeNow()."' LIMIT 1");				
			
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}

			if ($this->m_database->GetAffectedRows() == 0)
			{
				$this->SetError("SESSION_NOT_FOUND");
				return;
			}

		
			return;	
		}


		function CommandDestroy()
		{
			$this->Trace("CommandDestroy");
			
			/*
			
			Remove expired sessions from database
			
			1. Check Parameters
			2. Remove session from database
			3. Report to log
					
			*/
			
			// 1. Check Parameters

			$strSessionID = $this->GetParam("session_id");
			
			
			// 2. Remove sessions from database

			$strSQL = MakeSQL_Delete(
				$this->GetDatabaseTableName("session"),
				"`SESSION_ID` = '$strSessionID'");
				
			
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			if ($this->m_database->GetAffectedRows() == 0)
			{
				$this->SetError("SESSION_NOT_FOUND");
				return;	
			}
			
			// 3. Report to log
			$this->SystemLog("INFORMATIONAL","SESSION_DESTROY",$strSessionID);
			
			return;
		}

		function CommandCleanUp()
		{
			$this->Trace("CommandCleanUp");

			$strSQL = MakeSQL_Delete(
				$this->GetDatabaseTableName("session"),
				"`END_DATETIME` < '".RenderDateTimeNow()."'");

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}

/*			
			$arrayDetails = array(
				"deletedsessions" => $this->DatabaseGetAffectedRows()
				);
*/
		
			$this->SystemLog("INFORMATIONAL","SESSION_CLEANUP",$this->DatabaseGetAffectedRows(),array());
			
			return;	
		}

	
	
		
	}

