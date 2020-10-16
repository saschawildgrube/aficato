<?php

	require_once('wdk_website.inc');
	
	abstract class CDynamicWebSite extends CWebSite
	{
		private $m_strSessionOwnerID;
		private $m_strAuthUserName;
				
		function __construct(
			$strRootURL,
			$strSourceDir,
			$strWWWDir,
			$arrayMeta,
			$arrayConfig,
			$arrayParams,
			$strSessionOwnerID,
			$arrayObfuscationItems = array()
			)
		{
			require_once($strSourceDir."/webservices_directory.inc");
			$arrayConfig["webservices"] = GetWebservicesDirectory(ArrayGetValue($arrayConfig,"webservices"));
			
			if (IsValidSessionMode(ArrayGetValue($arrayConfig,"session_mode")) == false)
			{
				$arrayConfig["session_mode"] = SESSION_MODE_NONE;
			}
			
			$this->m_strSessionOwnerID = $strSessionOwnerID;
			$arrayConfig["authentication_support"] = true;
						
			parent::__construct(
				$strRootURL,
				$strSourceDir,
				$strWWWDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$arrayObfuscationItems);
					
		}
		
		function CallbackInit()
		{
			return parent::CallbackInit();
		}


		function SystemLog($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails=array())
		{
			$arrayDetails["authuserid"] = $this->GetAuthUserID();
			$arrayDetails["authusername"] = $this->GetAuthUserName();
			parent::SystemLog($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails);
		}
		
		function SystemEvent($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails=array())
		{
			$arrayDetails["authuserid"] = $this->GetAuthUserID();
			$arrayDetails["authusername"] = $this->GetAuthUserName();
			parent::SystemEvent($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails);
		}

		
		function CallbackSystemEvent($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails)
		{
			parent::CallbackSystemEvent($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails);
			$this->SystemLog($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails);
		}
		
		function CallbackSystemLog($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails)
		{
			$this->Trace("CallbackLog: Log via LOG webservice.");
			
			$arrayParams = array();
			$arrayParams["command"] = "log";
			$arrayParams["reporter_id"] = "WWW";
			$arrayParams["event_severity"] = $strEventSeverity;
			$arrayParams["event_id"] = $strEventID;
			$arrayParams["event_message"] = $strEventMessage;
			$arrayParams["event_details"] = MakeURLParameters($arrayDetails,"\r\n");

			
			$nTotalAttempts = 3;
			$nAttempt = 1;
			$strError = "1";
			
			while (($strError != "") && ($nAttempt <= $nTotalAttempts))
			{
				$consumer = new CWebServiceConsumerWebApplication($this);
				$consumer->ConsumeWebService("system/log",$arrayParams);
				$strError = $consumer->GetError();
				$nAttempt++;
			}
		}
		
		function CallbackSessionCreate()
		{
			$this->Trace("CDynamicWebSite::CallbackSessionCreate");
			if (IsSessionModeServer($this->GetSessionMode()))
			{			
				$arrayParams = array();
				$arrayParams["command"] = "create";
				$arrayParams["owner_id"] = $this->m_strSessionOwnerID;
				
				$nDuration = intval($this->GetConfig("session_duration"));
				if ($nDuration == 0)
				{
					$nDuration = 60*60*12; // 12 hours is the default.
				}
				$arrayParams["duration"] = $nDuration;
				if ($this->GetConfig("session_ip") == true)
				{
					$arrayParams["user_ip"] = $this->GetClientIP();
				}
				
				$consumer = new CWebServiceConsumerWebApplication($this);
				$consumer->ConsumeWebService("system/session",$arrayParams);
				
				$strSessionID = $consumer->GetResultValue("NEW_SESSION_ID");
				return $strSessionID;
			}
			return parent::CallbackSessionCreate();
		}
	
		function CallbackSessionRead($strSessionID, &$arrayData)
		{
			$this->Trace("CDynamicWebSite::CallbackSessionRead");
			if (IsSessionModeServer($this->GetSessionMode()))
			{			
			
				$arrayData = array();
				
				$arrayParams = array();
				$arrayParams["command"] = "read";
				$arrayParams["session_id"] = $strSessionID;
				
				$consumer = new CWebServiceConsumerWebApplication($this);
				$consumer->ConsumeWebService("system/session",$arrayParams);
	
				if ($consumer->GetError()!="")
				{
					return false;
				}
				
				if ($this->GetConfig("session_ip") == true)
				{
					$strClientIP = $this->GetClientIP();
					$strSessionIP = $consumer->GetResultValue("SESSION","USER_IP");
					if ($strClientIP != $strSessionIP)
					{
						$this->Trace("CallbackSessionRead: Client IP ($strClientIP) does not match IP stored in session ($strSessionIP).");
						return false;	
					}
				}
				if ($this->m_strSessionOwnerID != $consumer->GetResultValue("SESSION","OWNER_ID"))
				{
					$this->Trace("CallbackSessionRead: Session owner IDs do not match.");	
					return false;	
				}
				
				$strData = $consumer->GetResultValue("SESSION","DATA");
						
				$this->Trace("CallbackSessionRead: Serialized data read from session:\n\"$strData\"");
				$arrayData = ArrayUnpack($strData);
				
				if (!is_array($arrayData))
				{
					$arrayData = array();
				}
	
				$arrayData = SecureParameterArray($arrayData);
						
				$this->SetAuthUserID($consumer->GetResultValue("SESSION","USER_ID_AUTHENTICATION"));
				$this->Trace("CallbackSessionRead: Authenticated user read from session:\n\"".$this->GetAuthUserID()."\"");
			
				return true;
			}
			return parent::CallbackSessionRead($strSessionID, $arrayData);
		}

		function CallbackSessionWrite($strSessionID, $arrayData)
		{
			$this->Trace("CDynamicWebSite::CallbackSessionWrite");

			if (IsSessionModeServer($this->GetSessionMode()))
			{			
			
				if ($strSessionID == "")
				{
					return false;
				}
				
				$strData = ArrayPack($arrayData);
				$this->Trace("CallbackSessionWrite: Serialized data to be written:\n\"$strData\"");
				
				$arrayParams = array();
				$arrayParams["command"] = "write";
				$arrayParams["owner_id"] = $this->m_strSessionOwnerID;
				$arrayParams["session_id"] = $strSessionID;
				$arrayParams["data"] = $strData;
				
				$consumer = new CWebServiceConsumerWebApplication($this);
				$consumer->ConsumeWebService("system/session",$arrayParams);
	
	/*
				if ($consumer->GetError()=="SESSION_NOT_FOUND") // possibly expired?
				{
					$strNewSessionID = $this->CallbackSessionCreate();
					return $this->CallbackSessionWrite($strNewSessionID,$arrayData);
				}
	*/
				
				if ($consumer->GetError()!="")
				{
					return false;
				}
				return true;
			}
			return parent::CallbackSessionWrite($strSessionID, $arrayData);
		}
		
		function CallbackSessionDestroy($strSessionID)
		{
			$this->Trace("CDynamicWebSite::CallbackSessionDestroy");
			
			if (IsSessionModeServer($this->GetSessionMode()))
			{			
			
				$arrayParams = array();
				$arrayParams["command"] = "destroy";
				$arrayParams["session_id"] = $strSessionID;
				//$arrayParams["owner_id"] = $this->m_strSessionOwnerID;
				
				$consumer = new CWebServiceConsumerWebApplication($this);
				$consumer->ConsumeWebService("system/session",$arrayParams);
				
				if ($consumer->GetError()!="")
				{
					return false;
				}
				return true;
			}
			return parent::CallbackSessionDestroy($strSessionID);
		}

		
		
		


		
		function GetSessionOwnerID()
		{
			return $this->m_strSessionOwnerID;
		}
		
	
		function CallbackUserLogin($strUserName,$strAuthenticationPayload)
		{
			$arrayParams = array();
			$arrayParams["command"] = "login";
			$arrayParams["session_id"] = $this->GetSessionID();
			$arrayParams["owner_id"] = $this->GetSessionOwnerID();
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["authenticationpayload"] = $strAuthenticationPayload;
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService("system/session",$arrayParams);

			if ($consumer->GetError()!="")
			{
				return "";
			}
						
			$strUserID = $consumer->GetResultValue("SESSION","USER_ID_AUTHENTICATION");
					
			if ($strUserID != "")
			{
				$this->SetAuthUserID($strUserID);	
			}
			
			$this->SystemEvent("INFORMATIONAL","LOGIN",$strUserName,array());
					
			return $strUserID;
				
		}
		
		function CallbackUserLogout()
		{
			$this->SystemEvent("INFORMATIONAL","LOGOUT",$this->GetAuthuserName(),array());
			
			$this->SetAuthUserID("");
			
			if ($this->GetSessionID() != "")
			{
				$arrayParams = array();
				$arrayParams["command"] = "logout";
				$arrayParams["session_id"] = $this->GetSessionID();
				$arrayParams["owner_id"] = $this->m_strSessionOwnerID;
				
				$consumer = new CWebServiceConsumerWebApplication($this);
				$consumer->ConsumeWebService("system/session",$arrayParams);
			}
			
			return true;
		}		
		
		function CallbackUserCheckName($strUsername)
		{
			return StringCheckCharSet($strUsername,CHARSET_USERNAME);	
		}
		
		function CallbackUserChangePassword($strOldPassword,$strNewPassword)
		{
			$strUserID = $this->GetAuthUserID();
			if ($strUserID == "")
			{
				return false;	
			}
			
			$consumer = new CWebServiceConsumerWebApplication($this);	

			$arrayParams = array();
			$arrayParams["user_id"] = $strUserID;
			$arrayParams["authenticationpayload"] = $strOldPassword;
			$arrayParams["command"] = "authenticate";
			$consumer->ConsumeWebService("system/user",$arrayParams);
			$strError = $consumer->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
				return false;
			}
			
			
			$arrayParams = array();
			$arrayParams["user_id"] = $strUserID;
			$arrayParams["password"] = $strNewPassword;
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService("system/user",$arrayParams);
			$strError = $consumer->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
				return false;
			}

			return true;
		}

		
		function CallbackUserLoadSettings()
		{
			$arrayParams = array();
			$arrayParams["command"] = "get";
			$arrayParams["user_id"] = $this->GetAuthUserID();
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService("system/user",$arrayParams);

			if ($consumer->GetError()!="")
			{
				return false;
			}
						
			$strUserTimezone = $consumer->GetResultValue("USER","USER_TIMEZONE");
			if ($strUserTimezone != "")
			{
				$this->SetTimezone($strUserTimezone); 
			}
			
			$strUserLanguage = $consumer->GetResultValue("USER","USER_LANGUAGE");
			if ($strUserLanguage != "")
			{
				$this->SetLanguage($strUserLanguage);
			}
			
			$strUserCountry = $consumer->GetResultValue("USER","USER_COUNTRY");
			if ($strUserCountry != "")
			{
				$this->SetCountry($strUserCountry);
			}
			
			$this->Trace("CallbackLoadUserSettings ($strUserTimezone,$strUserLanguage)");
			return true;
		}
		
		function CallbackUserSignin($strUserName,$strPassword,$arraySigninParams=array(),&$strError="",&$arrayResults=array())
		{
			$webservice = new CWebServiceConsumerWebApplication($this);	
			
			$arrayParams = array();
			
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["password"] = $strPassword;
			
			$strUserEmail = ArrayGetValue($arraySigninParams,"user_email");
			if (IsValidEmail($strUserEmail) == true)
			{
				$arrayParams["user_email"] = $strUserEmail;
				if (GetBoolValue(ArrayGetValue($arraySigninParams,"user_email_validation")) == true)
				{
					$arrayParams["user_email_validation"] = "1";		
					$arrayParams["user_email_validation_duration"] = 60*60*24*3; // 3 days
				}
			}

			if (ArrayGetValue($arraySigninParams,"user_country") != "")
			{
				$arrayParams["user_country"] = ArrayGetValue($arraySigninParams,"user_country");
			}
			if (ArrayGetValue($arraySigninParams,"user_language") != "")
			{
				$arrayParams["user_language"] = ArrayGetValue($arraySigninParams,"user_language");
			}
			if (ArrayGetValue($arraySigninParams,"user_timezone") != "")
			{
				$arrayParams["user_timezone"] = ArrayGetValue($arraySigninParams,"user_timezone");
			}

			$arrayParams["command"] = "add";
			$webservice->ConsumeWebService("system/user",$arrayParams);
			$strError = $webservice->GetError();
			
			if ($strError != "")
			{
				return false;	
			}
			
			$strUserID = $webservice->GetResultValue("NEW_USER_ID");
			$arrayResults = $webservice->GetResultArrayInsecure();
			
			$arrayDefaultPrivileges = $this->GetConfig("defaultprivileges");
			if (is_array($arrayDefaultPrivileges))
			{
				if (ArrayCount($arrayDefaultPrivileges) > 0)	
				{
					$bError = false;
					foreach ($arrayDefaultPrivileges as $strPrivilegeID)
					{
						$strPrivilegeID = StringUpperCase($strPrivilegeID);
						
						$arrayParams = array();
						$arrayParams["user_id"] = $strUserID;
						$arrayParams["command"] = "set";
						$arrayParams["privilege_id"] = $strPrivilegeID;
						$webservice->ConsumeWebService("system/entitlement",$arrayParams);
						if ($webservice->GetError() != "")
						{
							$strError = $webservice->GetError();
							$bError = true;
						}
					}
					if ($bError == true)
					{
						return false;
					}
				}
			}
			return $strUserID;	
		}
		
		
		
		
		function CallbackUserDelete($strUserID)
		{
			$arrayParams = array();
			$arrayParams["command"] = "delete";
			$arrayParams["user_id"] = $strUserID;
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService("system/user",$arrayParams);
			$strError = $consumer->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
				
				$this->Trace("CDynamicWebSite::CallbackUserDelete failed!");
				
				return false;
			}
			return true;
		}
		
	
		function CallbackCheckCondition($strCondition)
		{
			if ($strCondition == "login")
			{
				if ($this->GetAuthUserID() != "")
				{
					return true;
				}
			}
			if ($strCondition == "nologin")
			{
				if ($this->GetAuthUserID() == "")
				{
					return true;
				}
			}			
			return parent::CallbackCheckCondition($strCondition);	
		}
		

		function GetAuthUserName()
		{
			if ($this->GetAuthUserID() != "")
			{
				if ($this->m_strAuthUserName == "")
				{
					$arrayData = array();
					
					$arrayParams = array();
					$arrayParams["command"] = "get";
					$arrayParams["user_id"] = $this->GetAuthUserID();
									
					$consumer = new CWebServiceConsumerWebApplication($this);
					$consumer->ConsumeWebService("system/user",$arrayParams);
		
					if ($consumer->GetError()!="")
					{
						return false;
					}
					$strUserName = $consumer->GetResultValue("USER","USER_NAME");
					$this->m_strAuthUserName = $strUserName;
				}
					
			}
			else
			{
				$this->m_strAuthUserName = "";
			}
			return $this->m_strAuthUserName;
			
		}
		
	
		function CallbackUserInitPrivileges()
		{
			$arrayParams = array();
			$arrayParams["command"] = "get";
			$arrayParams["user_id"] = $this->GetAuthUserID();
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService("system/entitlement",$arrayParams);

			if ($consumer->GetError()!="")
			{
				return array();
			}
			
			$arrayPrivileges = array();
			
			$list = $consumer->GetResultList();
			foreach ($list as $entry)
			{
				array_push($arrayPrivileges,ArrayGetValue($entry,"PRIVILEGE_ID"));
			}
			
			return $arrayPrivileges;
		}


		function CallbackGetData($strContextType, $strContextID, $strDataName, $strDefaultContent)
		{
			$webservice = new CWebServiceConsumerWebApplication($this);
			$arrayParams = array();
			$arrayParams['command'] = 'get';
			$arrayParams['data_name'] = $strDataName;
			if ($strContextType != '')
			{
				$arrayParams['data_context_type'] = $strContextType;
				$arrayParams['data_context_id'] = $strContextID;
			}

			$webservice->ConsumeWebService('system/data',$arrayParams);
			if ($webservice->GetError() != '')
			{
				if ($webservice->GetError() == 'ITEM_NOT_FOUND')
				{
					if ($strDefaultContent != '')
					{
						return $strDefaultContent;
					}
				}
				return false;
			}
			$strContent = $webservice->GetResultValueInsecure('DATA','DATA_CONTENT');
			$strContent = urldecode($strContent);
			return $strContent;
		}
		
		function CallbackSetData($strContextType, $strContextID, $strDataName,$strContent)
		{
			$webservice = new CWebServiceConsumerWebApplication($this);
			$arrayParams = array();
			$arrayParams['command'] = 'set';
			$arrayParams['data_name'] = $strDataName;
			$arrayParams['data_content'] = StringURLEncode($strContent);
			
			if ($strContextType != '')
			{
				$arrayParams['data_context_type'] = $strContextType;
				$arrayParams['data_context_id'] = $strContextID;
			}

			$webservice->ConsumeWebService('system/data',$arrayParams);
			if ($webservice->GetError() != '')
			{
				return false;
			}
			return true;
		}		

	}

