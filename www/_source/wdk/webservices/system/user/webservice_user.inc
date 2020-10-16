<?php

	require_once(GetWDKDir()."wdk_webservice_database_entity.inc");
	require_once(GetWDKDir()."wdk_entity_user.inc");
	require_once(GetWDKDir()."wdk_hash.inc");
	require_once(GetWDKDir()."wdk_language.inc");
	require_once(GetWDKDir()."wdk_country.inc");
		
   class CUserWebService extends CDatabaseEntityWebService
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
			

			$arrayEntityDefinitions = array(
				"user" => GetEntityDefinitionUser()
				);

			
			parent::__construct(
				"system/user",
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$arrayEntityDefinitions);
		}

		function CallbackInitParameterDefinition(&$paramDef)
		{
			$paramDef->AddParameterDefinitionBoolean("ignoreblacklist");
			$paramDef->AddParameterDefinitionUsername("new_user_name");

			$paramDef->AddParameterDefinition_authenticationpayload();

			$paramDef->AddParameterDefinitionBoolean("user_email_validation");
			$paramDef->AddParameterDefinitionInteger("user_email_validation_duration",1,60*60*24*5,true);
			//$paramDef->AddParameterDefinitionString("user_email_validation_token",10,10,CHARSET_INTEGER);
			
			$paramDef->AddParameterDefinitionString("data_name",1,40,CHARSET_NAME);
			$paramDef->AddParameterDefinitionString("data_content",0,65535,CHARSET_TEXT);
		
			parent::CallbackInitParameterDefinition($paramDef);	
		}
		
		function CallbackInitCommandDefinition(&$commandDef)
		{
			$commandDef->AddCommandDefinition("checkusername",array(
				"silo_id",
				"user_name"));
			$commandDef->AddCommandDefinition("rename",array(
				"silo_id",
				"user_name",
				"user_id",
				"new_user_name",
				"ignoreblacklist")); 
			$commandDef->AddCommandDefinition("changeemail",array(
				"silo_id",
				"user_name",
				"user_id",
				"user_email",				
				"user_email_validation",
				"user_email_validation_duration"));
			$commandDef->AddCommandDefinition("validateemail",array(
				"user_email_validation_token")); 				
			$commandDef->AddCommandDefinition("authenticate",array(
				"silo_id",
				"user_name",
				"user_id",
				"authenticationpayload")); 
			$commandDef->AddCommandDefinition("setdata",array(
				"silo_id",
				"user_name",
				"user_id",
				"data_name",
				"data_content"));
			$commandDef->AddCommandDefinition("getdata",array(
				"silo_id",
				"user_name",
				"user_id",
				"data_name"));
			$commandDef->AddCommandDefinition("deletedata",array(
				"silo_id",
				"user_name",
				"user_id",
				"data_name"));
			$commandDef->AddCommandDefinition("listdata",array(
				"silo_id",
				"user_name",
				"user_id",
				"blocksize",
				"offset"));
			$commandDef->AddCommandDefinition("cleanup",array());			
			parent::CallbackInitCommandDefinition($commandDef);
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
			case "add":
				$this->CommandAdd("user");
				return;

			case "checkusername":
				$this->CommandCheckUserName();
				return;

			case "get":
				$this->CommandGet("user");
				return;

			case "set":
				$this->CommandSet("user");
				return;

			case "rename":
				$this->CommandRename();
				return;
				
			case "authenticate":
				$this->CommandAuthenticate();
				return;
				
			case "changeemail":
				$this->CommandChangeEmail();
				return;			
				
			case "validateemail":
				$this->CommandValidateEmail();
				return;						

			case "setdata":
				$this->CommandSetData();
				return;

			case "getdata":
				$this->CommandGetData();
				return;

			case "deletedata":
				$this->CommandDeleteData();
				return;

			case "listdata":
				$this->CommandListData();
				return;
				
			case "cleanup":
				$this->CommandCleanup();
				return;
							
			default:
				parent::CallbackCommand($strCommand);
				return;
			}
		}


		function MakeIndexSQLCondition($strEntity)
		{
			if ($strEntity != "user")
			{
				return parent::MakeIndexSQLCondition($strEntity);
			}
			$strSQLCondition = "";
			$nSiloID = intval($this->GetParam("silo_id"));
			$strUserID = $this->GetParam("user_id");
			$strUserName = $this->GetParam("user_name");
			
			if (($strUserID == "") && ($strUserName ==""))
			{
				$this->SetError("USER_IDENTIFIER_MISSING");
				return "";	
			}
			
			if (($strUserID != "") && ($strUserName !=""))
			{
				$this->SetError("USER_IDENTIFIER_AMBIGUOUS");
				return "";	
			}
			
			if ($strUserID != "")
			{
				$strSQLCondition = $this->MakeSQL_Helper_Field("USER_ID")." = ".MakeSQL_Helper_Value($strUserID);
			}
			if ($strUserName != "")
			{
				$strUserName = StringLowerCase($strUserName);
				$strSQLCondition = 
					$this->MakeSQL_Helper_Field("USER_NAME")." = ".MakeSQL_Helper_Value($strUserName)." AND "
					.$this->MakeSQL_Helper_Field("SILO_ID")." = ".MakeSQL_Helper_Value($nSiloID);
			}
			return $strSQLCondition;
		}


		function GetUserData(&$arrayUserData)
		{
			$arrayUserData = array();
			$this->m_bLegacyHack = true;
			$strSQLCondition = $this->MakeIndexSQLCondition("user");
			if ($strSQLCondition == "")
			{
				return false;	
			}

			$strSQL = MakeSQL_Select(
				$this->GetDatabaseTableName("user"),
				"*",
				$strSQLCondition);

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return false;
			}
			
			$arrayData = $this->m_database->GetData();
			
			$nRows = ArrayCount($arrayData);
			
			if ($nRows == 0)
			{
				$this->SetError("USER_NOT_FOUND");
				return false;
			}

			if ($nRows > 1)
			{
				$this->SetError("USER_AMBIGUOUS");
				return false;
			}

			$arrayUserData = ArrayGetValue($arrayData,0);
			return true;
		}


		function CommandInstall()
		{
			$this->Trace("CommandInstall");
			
			parent::CommandInstall();
			if ($this->GetError() != "")
			{
				return;	
			}
			
			$arrayParams = array();
			$arrayParams["command"] = "cleanup";
			$this->Install_AddScheduledJob(
				"UserCleanup",
				$arrayParams,
				"0",
				"*",
				"*",
				"*",
				"*",
				20);
			
			return true;	
		}


		function GenerateEmailValidationToken()
		{
			$strError = $this->GetError();
			while (1)
			{
				$strToken = MakeNumericToken(10);
				$strSQL = MakeSQL_Select(
					$this->GetDatabaseTableName("user"),
					"USER_ID",
					"`USER_EMAIL_VALIDATION_TOKEN` = '".$strToken."'");
				if ($this->DatabaseQuery($strSQL) != true)
				{
					return false;	
				}
				$arrayData = $this->m_database->GetData();
				if (ArrayCount($arrayData) == 0)
				{
					$this->SetError($strError);					
					return $strToken;	
				}
			}
		}
		
		function CheckUserNameBlacklist($strUserName)
		{
			$strBlacklistFile = $this->GetConfig("blacklist");
			//$this->Trace("Blacklist file: \"$strBlacklistFile\"");
			if (!IsFile($strBlacklistFile))
			{
				return true;	
			}
			$arrayBlacklist = file($strBlacklistFile);
			//$this->Trace($arrayBlacklist);
			foreach ($arrayBlacklist as $strBlacklistedName)
			{
				$strBlacklistedNameChopped = chop($strBlacklistedName);
				if (strncasecmp(
					$strBlacklistedNameChopped,
					$strUserName,
					min(StringLength($strUserName),StringLength($strBlacklistedNameChopped)))
					== 0)
				{
					return false;
				}	
			}
			return true;
		}


		function CallbackCommandListPrepareSQL($strEntity,&$arraySQL)
		{
			if ($strEntity == "user")
			{
				$nSiloID = intval($this->GetParam("silo_id"));
				$arraySQL["where"]["and"][] = array(
					"left" => array("field" => "SILO_ID"),
					"operator" => "=",
					"right" => "$nSiloID");
			}			
			parent::CallbackCommandListPrepareSQL($strEntity,$arraySQL);
		}
		
				
		function CommandAdd($strEntity)
		{
			if ($strEntity != "user")
			{
				return parent::CommandAdd($strEntity);
			}			
			$this->Trace("CommandAdd");
	
			$strSiloID = StringLowerCase($this->GetParam("silo_id"));
			$strUserName = StringLowerCase($this->GetParam("user_name"));

			$bActive = true;
			if ($this->IsParam("user_active"))
			{
				$bActive = $this->GetParam("user_active");
			}
			
			$strPassword = $this->GetParam("password");
			$strPasswordHash = $this->GetParam("passwordhash");
			$strPasswordHashSeed = $this->GetParam("passwordhash_seed");
			if (($strPassword == "") && ($strPasswordHash ==""))
			{
				$this->SetError("AUTHENTICATION_PAYLOAD_MISSING");
				return;	
			}
			
			if (($strPassword != "") && ($strPasswordHash != ""))
			{
				$this->SetError("AUTHENTICATION_PAYLOAD_AMBIGUOUS");
				return;	
			}
			
			if ($strPassword != "")
			{
				$strPasswordHashSeed = MakePasswordHashSeed();
				$strPasswordHash = MakePasswordHash($strPassword,$strPasswordHashSeed);
			}
			
			$strLanguage = $this->GetParam("user_language");
			$strCountry = $this->GetParam("user_country");
			$strTimezone = $this->GetParam("user_timezone");
			
			$strEmail = $this->GetParam("user_email");
			$bEmailValidation = $this->GetParamBoolean("user_email_validation");
			$nEmailValidationDuration = $this->GetParam("user_email_validation_duration");
			if ($bEmailValidation == true && $strEmail == "")
			{
				$this->SetError("EMAIL_REQUIRED_FOR_VALIDATION");
				return;	
			}
			
			if ($this->GetParamBoolean("ignoreblacklist") == false)
			{
				if ($this->CheckUserNameBlacklist($strUserName) == false)
				{
					$this->SetError("USERNAME_BLACKLISTED");
					return;	
				}
			}
			
			$this->m_bLegacyHack = true;
			$strSQLCondition = $this->MakeIndexSQLCondition("user");
			$strSQL = MakeSQL_Select(
				$this->GetDatabaseTableName("user"),
				"*",
				$strSQLCondition);
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			$arrayData = $this->m_database->GetData();
			$nRows = ArrayCount($arrayData);
			
			if ($nRows > 0)
			{
				$this->SetError("USER_EXISTS");
				return;
			}

			
			
			$this->DatabaseLockTable($this->GetDatabaseTableName("user"));
			
			$arrayFields = array();
			$arrayFields["CREATION_DATETIME"] = RenderDateTimeNow();
			$arrayFields["LASTCHANGE_DATETIME"] = RenderDateTimeNow();
			$arrayFields["SILO_ID"] = $strSiloID;
			$arrayFields["USER_NAME"] = $strUserName;
			$arrayFields["USER_ACTIVE"] = "".intval(GetBoolValue($bActive));
			$arrayFields["PASSWORDHASH"] = $strPasswordHash;
			$arrayFields["PASSWORDHASH_SEED"] = $strPasswordHashSeed;
			$arrayFields["USER_LANGUAGE"] = $strLanguage;
			$arrayFields["USER_COUNTRY"] = $strCountry;
			if ($strTimezone != "")
			{
				$arrayFields["USER_TIMEZONE"] = $strTimezone;
			}
			
			$strValidationToken = "";
			if ($bEmailValidation == true)
			{
				$strValidationToken = $this->GenerateEmailValidationToken();
				if ($strValidationToken == false)
				{
					return;
				}
				$arrayFields["USER_EMAIL_VALIDATION_STATUS"] = "VALIDATION_SIGNIN";
				$arrayFields["USER_EMAIL_VALIDATION_TOKEN"] = $strValidationToken;
				$strDateTimeNow = RenderDateTimeNow();
				$strDateTimeEmailValidationEnd = RenderDateTime(GetTimeNow() + $nEmailValidationDuration);
				$arrayFields["USER_EMAIL_VALIDATION_END_DATETIME"] = $strDateTimeEmailValidationEnd;
				$arrayFields["USER_EMAIL_VALIDATION_NEW"] = $strEmail;	
				$arrayFields["USER_EMAIL"] = "";	
			}
			else
			{
				$arrayFields["USER_EMAIL"] = $strEmail;	
			}
			
			$strSQL = MakeSQL_Insert(
				$this->GetDatabaseTableName("user"),
				$arrayFields);
			
			
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			$strSQL = MakeSQL_Select(
				$this->GetDatabaseTableName("user"),
				"*",
				"1 ORDER BY USER_ID DESC LIMIT 1");
				
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			$arrayData = $this->m_database->GetData();
			$strNewUserID = ArrayGetValue(ArrayGetValue($arrayData,0),"USER_ID");

			$this->SetResultValue($strNewUserID,"NEW_USER_ID");
			if ($strValidationToken != "")
			{
				$this->SetResultValue($strValidationToken,"VALIDATION_TOKEN");
			}

			$this->SystemLog("INFORMATIONAL","USER_ADD",$strNewUserID.", ".$strUserName);
			
			$this->DatabaseUnlockTables();
			
			return;	
		}
	
	
		function CommandCheckUserName()
		{
			$this->Trace("CommandCheckUserName");
			
			if ($this->CheckUserNameBlacklist($this->GetParam("user_name")) == false)
			{
					$this->SetError("USERNAME_BLACKLISTED");
					return;	
			}
			
			$this->m_bLegacyHack = true;
			$strSQLCondition = $this->MakeIndexSQLCondition("user");
			if ($strSQLCondition == "")
			{
				return;
			}
			

			$strSQL = MakeSQL_Select(
				$this->GetDatabaseTableName("user"),
				"*",
				$strSQLCondition);

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			$arrayData = $this->m_database->GetData();
			
			$nRows = ArrayCount($arrayData);
			
			if ($nRows > 0)
			{
				$this->SetError("USERNAME_OCCUPIED");
				return;
			}
			return;
		}	
	
		function DeleteUser($strUserID)
		{
			$strSQLCondition = "`USER_ID` = '$strUserID'";
			$strSQL = MakeSQL_Delete(
				$this->GetDatabaseTableName("user"),
				$strSQLCondition);
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			$this->DeleteAllEntitlements($strUserID);
			$this->DeleteAllData($strUserID);		
		}
	
		function CommandDelete($strEntity)
		{
			if ($strEntity != "user")
			{
				return parent::CommandDelete($strEntity);
			}
			$this->Trace("CommandDelete");
			
			$arrayUserData = array();
			if ($this->GetUserData($arrayUserData) != true)
			{
				return;	
			}
			
			$strUserID = $arrayUserData["USER_ID"];
			
			$this->DeleteUser($strUserID);

			return;	
		}
		
		function DeleteAllEntitlements($strUserID)
		{
			$this->Trace("DeleteAllEntitlements BEGIN");
			$strURL = $this->GetWebserviceURL("system/entitlement");
			if ($strURL != "")
			{
				$arrayParams = array();
				$arrayParams["command"] = "deleteall";
				$arrayParams["user_id"] = $strUserID;
				$consumer = new CWebServiceConsumerWebApplication($this);
				$consumer->ConsumeWebService("system/entitlement",$arrayParams);
			}
			else
			{
				$this->Trace("Config system/entitlement web service url not specified.");
			}
			$this->Trace("DeleteAllEntitlements END");
		}

		function DeleteAllData($strUserID)
		{
			$this->Trace("DeleteAllData BEGIN");
			$strURL = $this->GetWebserviceURL("system/data");
			if ($strURL != "")
			{
				$arrayParams = array();
				$arrayParams["command"] = "deletecontext";
				$arrayParams["data_context_type"] = "USER";
				$arrayParams["data_context_id"] = $strUserID;
				$consumer = new CWebServiceConsumerWebApplication($this);
				$consumer->ConsumeWebService("system/data",$arrayParams);
			}
			else
			{
				$this->Trace("Config web service system/data url not specified.");
			}
			$this->Trace("DeleteAllData END");
		}



		function CommandSet($strEntity)
		{
			$this->Trace("CommandSet");
			
	
			$this->m_bLegacyHack = true;
			$strSQLCondition = $this->MakeIndexSQLCondition("user");
			if ($strSQLCondition == "")
			{
				return;	
			}
			
			$strSQL = MakeSQL_Select(
				$this->GetDatabaseTableName("user"),
				"*",
				$strSQLCondition . " LIMIT 1");

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			$arrayData = $this->m_database->GetData();
			if (ArrayCount($arrayData) != 1)
			{
				$this->SetError("USER_DOES_NOT_EXIST");
				return;	
			}
		

			$arrayFields = array();
			$arrayFields["LASTCHANGE_DATETIME"] = RenderDateTimeNow();
			
			
			// Check and set the different values
			
			if ($this->IsParam("passwordhash"))
			{
				$arrayFields["PASSWORDHASH"] = $this->GetParam("passwordhash");
			}

			if ($this->IsParam("passwordhash_seed"))
			{
				$strPasswordHashSeed = $this->GetParam("passwordhash_seed");
				$arrayFields["PASSWORDHASH_SEED"] = $strPasswordHashSeed;
			}
			else
			{
				$strPasswordHashSeed = MakePasswordHashSeed();
			}
			
			if ($this->IsParam("password")) 
			{
				$arrayFields["PASSWORDHASH_SEED"] = $strPasswordHashSeed;
				$arrayFields["PASSWORDHASH"] = MakePasswordHash($this->GetParam("password"),$strPasswordHashSeed);
			}

			if ($this->IsParam("user_active"))
			{
				$arrayFields["USER_ACTIVE"] = $this->GetParam("user_active");
			}
			
			
			if ($this->IsParam("user_language"))
			{
				$arrayFields["USER_LANGUAGE"] = $this->GetParam("user_language");
			}
			
			if ($this->IsParam("user_country"))
			{
				$arrayFields["USER_COUNTRY"] = $this->GetParam("user_country");
			}
			
			if ($this->IsParam("user_timezone"))
			{
				$arrayFields["USER_TIMEZONE"] = $this->GetParam("user_timezone");
			}
		
			if (ArrayCount($arrayFields) == 0)
			{
				$this->SetError("NO_VALUE_SPECIFIED");
				return;
			}
			
			$strSQL = MakeSQL_Update(
				$this->GetDatabaseTableName("user"),
				$arrayFields,
				$strSQLCondition);

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}

			return;
		}


		function CommandRename()
		{
			$this->Trace("CommandRename");
			
			/*
			
			Rename user
			
			1. Check if user exists
			2. Check if new user name is allowed
			3. Check if new name is free
			4. Set the new username
		
			*/
			
			$strNewUserName = $this->GetParam("new_user_name");
			$strSiloID = $this->GetParam("silo_id");
			
			// 1. Check if user exists
			
			$this->m_bLegacyHack = true;
			$strSQLCondition = $this->MakeIndexSQLCondition("user");
			if ($strSQLCondition == "")
			{
				return;	
			}
			
			$strSQL = MakeSQL_Select(
				$this->GetDatabaseTableName("user"),
				"*",
				$strSQLCondition . " LIMIT 1");

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			$arrayData = $this->m_database->GetData();
			if (ArrayCount($arrayData) != 1)
			{
				$this->SetError("USER_DOES_NOT_EXIST");
				return;	
			}


			// 2. Check if new user name is allowed
			if ($this->GetParamBoolean("ignoreblacklist") == false)
			{
				if ($this->CheckUserNameBlacklist($strNewUserName) == false)
				{
					$this->SetError("USERNAME_BLACKLISTED");
					return;	
				}
			}



			// 3. Check if new user name is free
			
			$strSQLConditionNew = "`USER_NAME` = ".MakeSQL_Helper_Value($strNewUserName)." AND `SILO_ID` = ".MakeSQL_Helper_Value($strSiloID);
			
			$strSQL = MakeSQL_Select(
				$this->GetDatabaseTableName("user"),
				"*",
				$strSQLConditionNew . " LIMIT 1");

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			$arrayData = $this->m_database->GetData();
			if (ArrayCount($arrayData) != 0)
			{
				$this->SetError("USER_EXISTS");
				return;	
			}



			// 4. Set new user name
			$arrayFields = array();
			$arrayFields["LASTCHANGE_DATETIME"] = RenderDateTimeNow();
			$arrayFields["USER_NAME"] = $strNewUserName;
						
			$strSQL = MakeSQL_Update(
				$this->GetDatabaseTableName("user"),
				$arrayFields,
				$strSQLCondition);

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}

			return;
		}


		function CommandChangeEmail()
		{
			$this->Trace("CommandChangeEmail");
	
			$this->m_bLegacyHack = true;
			$strSQLCondition = $this->MakeIndexSQLCondition("user");
			if ($strSQLCondition == "")
			{
				return;	
			}
	
	
			$arrayUserData = array();
			if ($this->GetUserData($arrayUserData) == false)
			{
				return;	
			}
		

			$arrayFields = array();
			$arrayFields["LASTCHANGE_DATETIME"] = RenderDateTimeNow();
			
		
			$strEmail = $this->GetParam("user_email");
			$bEmailValidation = $this->GetParamBoolean("user_email_validation");
			$nEmailValidationDuration = $this->GetParam("user_email_validation_duration");
			if ($bEmailValidation == true && $strEmail == "")
			{
				$this->SetError("EMAIL_REQUIRED_FOR_VALIDATION");
				return;	
			}
			
			$strValidationStatus = $arrayUserData["USER_EMAIL_VALIDATION_STATUS"];
			$this->Trace("strValidationStatus = $strValidationStatus");
			
				
			$strValidationToken = "";	
			if ($bEmailValidation == true)
			{
				// if the new email to be verified matches the previous email to be verified,
				// then reuse the validation token
				if ($arrayUserData["USER_EMAIL_VALIDATION_NEW"] == $strEmail)
				{
					$strValidationToken = $arrayUserData["USER_EMAIL_VALIDATION_TOKEN"];
				}
				if ($strValidationToken == "")
				{
					$strValidationToken = $this->GenerateEmailValidationToken();
					if ($strValidationToken == false)
					{
						return;
					}
				}
				$strNewStatus = "VALIDATION_INITIAL";
				if ($strValidationStatus == "VALIDATED")
				{
					$strNewStatus = "VALIDATION_CHANGE";
				}
				$arrayFields["USER_EMAIL_VALIDATION_STATUS"] = $strNewStatus;
				$arrayFields["USER_EMAIL_VALIDATION_TOKEN"] = $strValidationToken;
				$strDateTimeNow = RenderDateTimeNow();
				$strDateTimeEmailValidationEnd = RenderDateTime(GetTimeNow() + $nEmailValidationDuration);
				$arrayFields["USER_EMAIL_VALIDATION_END_DATETIME"] = $strDateTimeEmailValidationEnd;				
				$arrayFields["USER_EMAIL_VALIDATION_NEW"] = $strEmail;	
			}
			else
			{
				$arrayFields["USER_EMAIL"] = $strEmail;	
				$arrayFields["USER_EMAIL_VALIDATION_STATUS"] = "NOTVALIDATED";	
			}				
			
		
			$strSQL = MakeSQL_Update(
				$this->GetDatabaseTableName("user"),
				$arrayFields,
				$strSQLCondition);

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			if ($strValidationToken != "")
			{
				$this->SetResultValue($strValidationToken,"VALIDATION_TOKEN");
			}

			return;
		}



		function CommandValidateEmail()
		{
			$this->Trace("CommandValidateEmail");
			
			$strToken = $this->GetParam("user_email_validation_token");
			
			
			$strSQLCondition = "`USER_EMAIL_VALIDATION_TOKEN` = '".$strToken."'";
			$strSQL = MakeSQL_Select(
				$this->GetDatabaseTableName("user"),
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
				$this->SetError("TOKEN_NOT_FOUND");
				return;
			}
			if ($nRows > 1)
			{
				$this->SetError("TOKEN_AMBIGUOUS");
				return;
			}
			$arrayUserData = ArrayGetValue($arrayData,0);

			$strDateTimeNow = RenderDateTimeNow();

			$arrayFields = array();
			$arrayFields["LASTCHANGE_DATETIME"] = $strDateTimeNow;

			$strValidationStatus = $arrayUserData["USER_EMAIL_VALIDATION_STATUS"];
			
			switch ($strValidationStatus)
			{
				case "NOTVALIDATED":
				case "VALIDATED":
					$this->SetError("VALIDATION_STATUS_INCONSISTENCY");
					$this->CommandCleanup();
					return;
				default:
					break; 
				
			}
			
			$strValidationEndDatetime = $arrayUserData["USER_EMAIL_VALIDATION_END_DATETIME"];
			
			if ($strValidationEndDatetime < $strDateTimeNow)
			{
				$this->SetError("VALIDATION_TIMEOUT");
				return;
			}
			
			$strNewEmail = $arrayUserData["USER_EMAIL_VALIDATION_NEW"];
			if (!IsValidEmail($strNewEmail))
			{
				$this->SetError("INVALID_EMAIL_PENDING_FOR_VALIDATION");
				return;
			}
			
			$arrayFields["USER_EMAIL"] = $strNewEmail;
			$arrayFields["USER_EMAIL_VALIDATION_NEW"] = "";
			$arrayFields["USER_EMAIL_VALIDATION_END_DATETIME"] = "NULL";
			$arrayFields["USER_EMAIL_VALIDATION_STATUS"] = "VALIDATED";
			$arrayFields["USER_EMAIL_VALIDATION_TOKEN"] = "";
			
			$strSQL = MakeSQL_Update(
				$this->GetDatabaseTableName("user"),
				$arrayFields,
				$strSQLCondition);
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			$arrayUserData = ArrayMerge($arrayUserData,$arrayFields);
			
			$this->SetResultValue($arrayUserData,"USER");
			return;
		}


		function CommandAuthenticate()
		{
			$this->Trace("CommandAuthenticate");
			
			$this->m_bLegacyHack = true;
			$strSQLCondition = $this->MakeIndexSQLCondition("user");
			if ($strSQLCondition == "")
			{
				return;	
			}
			
			$strAuthenticationPayload = $this->GetParam("authenticationpayload");
			/*
			if ($strAuthenticationPayload == "")
			{
				$this->SetError("AUTHENTICATION_PAYLOAD_MISSING");
				return;	
			}
			*/

			$strSQL = MakeSQL_Select(
				$this->GetDatabaseTableName("user"),
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
				$this->SetError("USER_NOT_FOUND");
				return;
			}

			if ($nRows > 1)
			{
				$this->SetError("USER_AMBIGUOUS");
				return;
			}
			$arrayItem = ArrayGetValue($arrayData,0);

			if ($arrayItem["USER_ACTIVE"] == "0")
			{
				$this->SetError("USER_NOT_ACTIVE");
				return;
			}
			
			if ($arrayItem["USER_EMAIL_VALIDATION_STATUS"] == "VALIDATION_SIGNIN")
			{
				$this->SetError("USER_EMAIL_NOT_VALIDATED");
				return;
			}

			$strPasswordHash = $arrayItem["PASSWORDHASH"];
			$this->Trace("strPasswordHash = $strPasswordHash");
			$strPasswordHashSeed = $arrayItem["PASSWORDHASH_SEED"];
			$this->Trace("strPasswordHashSeed = $strPasswordHashSeed");

			$arrayItem = ArrayFilterByKeys($arrayItem,$this->GetPublicFieldsGet("user"));
			$this->CallbackCommandGetComplete("user",$arrayItem,"authenticate");
			$this->SetResultValue($arrayItem,"USER");


			$bAuthenticationSuccessful = false;

			if (VerifyPassword($strAuthenticationPayload,$strPasswordHash,$strPasswordHashSeed) == true)
			{  
				$this->Trace("Matching password.");
				$bAuthenticationSuccessful = true;
			}
			else
			{
				// Check Password hash directly
				if ($strPasswordHash == $strAuthenticationPayload)
				{
					$this->Trace("Matching password hash.");
					$bAuthenticationSuccessful = true;	
				}
			}



			if ($bAuthenticationSuccessful == false)
			{
				$this->SetError("USER_AUTHENTICATION_FAILED");
			}
			else
			{
				$arrayFields["LASTAUTHENTICATION_DATETIME"] = RenderDateTimeNow();
				$strSQL = MakeSQL_Update(
					$this->GetDatabaseTableName("user"),
					$arrayFields,
					$strSQLCondition);
				$this->DatabaseQuery($strSQL);
			}

			return;
		}
		
		function CommandSetData()
		{
			$this->Trace("CommandSetData");
			$strWebserviceDataURL = $this->GetWebserviceURL("system/data");
			if ($strWebserviceDataURL == "")
			{
				$this->SetError("DATAWEBSERVICE_NOT_CONFIGURED");
				return;
			}

			$arrayUserData = array();
			if ($this->GetUserData($arrayUserData) != true)
			{
				return;
			}

			$strUserID = $arrayUserData["USER_ID"];
						
			$arrayParams = array();
			$arrayParams["command"] = "set";
			$arrayParams["data_context_type"] = "USER";
			$arrayParams["data_context_id"] = $strUserID;
			$arrayParams["data_name"] = $this->GetParam("data_name");
			$arrayParams["data_content"] = $this->GetParam("data_content");
			
			$consumer = new CWebServiceConsumerWebApplication($this);			
			$consumer->ConsumeWebService("system/data",$arrayParams);
			$strError = $consumer->GetError();
			if ($strError != "")
			{
				$this->SetError($strError);	
			}
			return;
		}

		function CommandGetData()
		{
			$this->Trace("CommandGetData");
			$strWebserviceDataURL = $this->GetWebserviceURL("system/data");
			if ($strWebserviceDataURL == "")
			{
				$this->SetError("DATAWEBSERVICE_NOT_CONFIGURED");
				return;
			}
			
			$arrayUserData = array();
			if ($this->GetUserData($arrayUserData) != true)
			{
				return;
			}
			$strUserID = $arrayUserData["USER_ID"];
						
			$arrayParams = array();
			$arrayParams["command"] = "get";
			$arrayParams["data_context_type"] = "USER";
			$arrayParams["data_context_id"] = $strUserID;
			$arrayParams["data_name"] = $this->GetParam("data_name");
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService("system/data",$arrayParams);
			$strError = $consumer->GetError();
			if ($strError != "")
			{
				$this->SetError($strError);	
			}
			
			$this->SetResultArray($consumer->GetResultArray());
			return;
		}

		function CommandDeleteData()
		{
			$this->Trace("CommandDeleteData");
			$strWebserviceDataURL = $this->GetWebserviceURL("system/data");
			if ($strWebserviceDataURL == "")
			{
				$this->SetError("DATAWEBSERVICE_NOT_CONFIGURED");
				return;
			}
			
			$arrayUserData = array();
			if ($this->GetUserData($arrayUserData) != true)
			{
				return;
			}
			$strUserID = $arrayUserData["USER_ID"];
						
			$arrayParams = array();
			$arrayParams["command"] = "delete";
			$arrayParams["data_context_type"] = "USER";
			$arrayParams["data_context_id"] = $strUserID;
			$arrayParams["data_name"] = $this->GetParam("data_name");
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService("system/data",$arrayParams);
			$strError = $consumer->GetError();
			if ($strError != "")
			{
				$this->SetError($strError);	
			}
			return;
		}

		function CommandListData()
		{
			$this->Trace("CommandListData");
			$strWebserviceDataURL = $this->GetWebserviceURL("system/data");
			if ($strWebserviceDataURL == "")
			{
				$this->SetError("DATAWEBSERVICE_NOT_CONFIGURED");
				return;
			}

			
			$arrayUserData = array();
			if ($this->GetUserData($arrayUserData) != true)
			{
				return;
			}
			$strUserID = $arrayUserData["USER_ID"];
			
			
						
			$arrayParams = array();
			$arrayParams["command"] = "list";
			$arrayParams["blocksize"] = $this->GetParam("blocksize");
			$arrayParams["offset"] = $this->GetParam("offset");
			$arrayParams["filter1"] = "data_context_type";
			$arrayParams["filter1_value"] = "USER";
			$arrayParams["filter2"] = "data_context_id";
			$arrayParams["filter2_value"] = $strUserID;
			//$arrayParams["trace"] = "1";
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService("system/data",$arrayParams);
			$strError = $consumer->GetError();
			if ($strError != "")
			{
				$this->SetError($strError);	
			}
			
			$arrayData = $consumer->GetResultList();
			
			$nRows = ArrayCount($arrayData);
			
			$nTotalRows = $this->m_database->QueryFoundRows();
			$this->SetResultValue($consumer->GetResultValue("TOTALENTRIES"),"TOTALENTRIES");
			$this->SetResultValue($consumer->GetResultValue("OFFSET"),"OFFSET");
			$this->SetResultValue($consumer->GetResultValue("BLOCKSIZE"),"BLOCKSIZE");
			$this->SetResultValue($consumer->GetResultValue("MAXOFFSET"),"MAXOFFSET");

			$arrayFields = array ("DATA_ID", "DATA_NAME", "DATA_CONTENT");
	
			$list = ArrayFilterListByKeys($arrayData,$arrayFields);
			$this->SetResultValue($list,"LIST");

			return;
		}
		
		function CommandCleanup()
		{
			$this->Trace("CommandCleanup");
			
			
			// VALIDATION_SIGNIN
			$strSQL = MakeSQL_Select(
				$this->GetDatabaseTableName("user"),
				"USER_ID",
				"`USER_EMAIL_VALIDATION_END_DATETIME` < '".RenderDateTimeNow()."' "
				."AND `USER_EMAIL_VALIDATION_STATUS` = 'VALIDATION_SIGNIN'");
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			$arrayData = $this->m_database->GetData();
			foreach ($arrayData as $arrayRow)
			{
				$this->DeleteUser($arrayRow["USER_ID"]);	
			}
			

			
			// VALIDATION_INITIAL
			$arrayFields = array();
			$arrayFields["USER_EMAIL_VALIDATION_END_DATETIME"] = "";
			$arrayFields["USER_EMAIL_VALIDATION_STATUS"] = "NOTVALIDATED";
			$arrayFields["USER_EMAIL_VALIDATION_TOKEN"] = "";
			$arrayFields["USER_EMAIL_VALIDATION_NEW"] = "";
			$strSQL = MakeSQL_Update(
				$this->GetDatabaseTableName("user"),
				$arrayFields,
				"`USER_EMAIL_VALIDATION_END_DATETIME` < '".RenderDateTimeNow()."' "
				."AND `USER_EMAIL_VALIDATION_STATUS` = 'VALIDATION_INITIAL'");
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
		
			
			
			// VALIDATION_CHANGE
			$arrayFields = array();
			$arrayFields["USER_EMAIL_VALIDATION_END_DATETIME"] = "NULL";
			$arrayFields["USER_EMAIL_VALIDATION_STATUS"] = "VALIDATED";
			$arrayFields["USER_EMAIL_VALIDATION_TOKEN"] = "";
			$arrayFields["USER_EMAIL_VALIDATION_NEW"] = "";
			$strSQL = MakeSQL_Update(
				$this->GetDatabaseTableName("user"),
				$arrayFields,
				"`USER_EMAIL_VALIDATION_END_DATETIME` < '".RenderDateTimeNow()."' "
				."AND `USER_EMAIL_VALIDATION_STATUS` = 'VALIDATION_CHANGE'");
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			// VALIDATION STATUS INCONSISTENCIES
			
			// Validated but email is not set
			$arrayFields = array();
			$arrayFields["USER_EMAIL_VALIDATION_END_DATETIME"] = "NULL";
			$arrayFields["USER_EMAIL_VALIDATION_STATUS"] = "NOTVALIDATED";
			$arrayFields["USER_EMAIL_VALIDATION_TOKEN"] = "";
			$arrayFields["USER_EMAIL_VALIDATION_NEW"] = "";
			$arrayFields["USER_EMAIL"] = "";
			$strSQL = MakeSQL_Update(
				$this->GetDatabaseTableName("user"),
				$arrayFields,
				"`USER_EMAIL_VALIDATION_STATUS` = 'VALIDATED'"
				."AND `USER_EMAIL` = ''");
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}


			// Status is VALIDATION_* but no token has been set
			$arrayFields = array();
			$arrayFields["USER_EMAIL_VALIDATION_END_DATETIME"] = "NULL";
			$arrayFields["USER_EMAIL_VALIDATION_STATUS"] = "NOTVALIDATED";
			$arrayFields["USER_EMAIL_VALIDATION_TOKEN"] = "";
			$arrayFields["USER_EMAIL_VALIDATION_NEW"] = "";
			$arrayFields["USER_EMAIL"] = "";
			$strSQL = MakeSQL_Update(
				$this->GetDatabaseTableName("user"),
				$arrayFields,
				"("
				."`USER_EMAIL_VALIDATION_STATUS` = 'VALIDATION_SIGNIN'"
				."OR `USER_EMAIL_VALIDATION_STATUS` = 'VALIDATION_CHANGE'"
				."OR `USER_EMAIL_VALIDATION_STATUS` = 'VALIDATION_INITIAL'"
				.")"
				."AND `USER_EMAIL_VALIDATION_TOKEN` = ''");
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			
		}


	}
	

