<?php

	require_once(GetWDKDir()."wdk_module_entitymanager.inc");
	require_once(GetWDKDir()."wdk_entity_user.inc");

	class CUserManagerModule extends CEntityManagerModule
	{
	
		function __construct(
			CWebSite $website,
			$strInstanceID,
			$arrayConfig = array())
		{
			$nSiloID = intval(ArrayGetValue($arrayConfig,"silo_id"));
			if ($nSiloID != 0 && $strInstanceID == "")
			{
				$strInstanceID = "silo$nSiloID";	
			}

			parent::__construct(
				$website,
				"system/usermanager",
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$arrayEntityDefinitions = array(
				"user" => GetEntityDefinitionUser()
			);

			$this->SetEntityDefinitions($arrayEntityDefinitions);
			
			$this->GetWebsite()->LoadStringResources("local");
			
			return parent::OnInit();
		}

		function OnRenderModule($strState)
		{
			switch ($strState)
			{
				case "":
					return $this->StateList("user");
				case "user_modifysubmit":
					return $this->StateUserModifySubmit();
				default:
					return parent::OnRenderModule($strState);
			}
		}
		
		function GetSiloID()
		{
			$nSiloID = intval($this->GetModuleConfig("silo_id"));
			return "$nSiloID";	
		}
		
		function OnCheckInputValues($strEntity,$bAdd,$arrayItem) 
		{
			$arrayErrors = parent::OnCheckInputValues($strEntity,$bAdd,$arrayItem);
			if ($strEntity == "user")
			{
				if ($bAdd == true)
				{
					$strPassword = $arrayItem["password"];
					if ($strPassword == "")
					{
						$arrayErrors["password"] = "?TID_USER_ERROR_PASSWORD_MISSING?";
					}	
				} 
			}
			return $arrayErrors;
		}

		
		function OnPrepareSelectOptions($strEntity,$strAttributeID,$strIndex)
		{
			if ($strEntity == "user")
			{
				if ($strAttributeID == "user_language")
				{
					$arrayLanguageOptions = array();
					$arrayLanguageOptions[""] = "?TID_USER_OPTION_NOPREFERREDLANGUAGE?";
					$arraySupportedLanguages = $this->GetWebsite()->GetSupportedLanguages();
					foreach ($arraySupportedLanguages as $strLanguage)
					{
						$arrayLanguageOptions[$strLanguage] = "?TID_LANGUAGE_".StringUpperCase($strLanguage)."?";
					}
					return $arrayLanguageOptions;
				}
				
				if ($strAttributeID == "user_timezone")
				{
					$arrayAllTimezones = GetAllTimezoneIDs();
					$arrayTimezoneOptions = array();
					foreach ($arrayAllTimezones as $strTimezone)
					{
						$arrayTimezoneOptions[$strTimezone] = $strTimezone;
					}
					return $arrayTimezoneOptions;
				}
				
				if ($strAttributeID == "user_country")
				{
					$arrayCountryOptions = array();
					$arrayCountryOptions[""] = "?TID_USER_OPTION_NOCOUNTRY?";
					$arrayAllCountries = GetAllCountryIDs();
					foreach ($arrayAllCountries as $strCountry)
					{
						$arrayCountryOptions[$strCountry] = "?TID_COUNTRY_".StringUpperCase($strCountry)."?";
					}
					$this->Trace($arrayCountryOptions);
					return $arrayCountryOptions;
				}
			}
			return parent::OnPrepareSelectOptions($strEntity,$strAttributeID,$strIndex);	
		}		
		
		function OnPrepareFilterSelectOptions($strEntity,$strFilter)
		{
			$arrayOptions = array();
			if ($strEntity == "user")
			{
				if ($strFilter == "filterbyactive")
				{
					$arrayOptions = array();
					$arrayOptions[""] = "?TID_OPTION_ALL?";
					$arrayOptions["1"] = "?TID_USER_OPTION_USER_ACTIVE?";
					$arrayOptions["0"] = "?TID_USER_OPTION_USER_INACTIVE?";
					return $arrayOptions;
				}
			}
			return parent::OnPrepareFilterSelectOptions($strEntity,$strFilter);
		}
		
		function OnPrepareItemDataView($strEntity,$strIndex,&$arrayItem)
		{
			if ($strEntity == "user")
			{
				$strEmail = ArrayGetValue($arrayItem,"user_email");
				if ($strEmail != "")
				{
					$strTasks = ArrayGetValue($arrayItem,"TASKS");
					$this->IncludeElement("mailicon");
					$mailicon = new CMailIconElement($this->GetWebsite(),"default");
					$strTasks .= $mailicon->RenderMailIcon($strEmail,$strEmail);
					$arrayItem["TASKS"] = $strTasks;
				}
			}
			parent::OnPrepareItemDataView($strEntity,$strIndex,$arrayItem);
			if ($strEntity == "user")
			{
				$strLastAuth = ArrayGetValue($arrayItem,"lastauthentication_datetime");
				if ($strLastAuth == "")
				{
					$arrayItem["lastauthentication_datetime"] = "?TID_USER_INFO_NOPREVIOUSAUTHENTICATION?";
				}
			}
		} 

		function OnPrepareWebserviceParams($strEntity,&$arrayParams)
		{
			if ($strEntity == "user")
			{
				$nSiloID = $this->GetSiloID();
				if (ArrayGetValue($arrayParams,"command") == "list")
				{
					$arrayParams["silo_id"] = "$nSiloID";	
					/*$arrayParams["filter4"] = "silo_id";	
					$arrayParams["filter4_value"] = "$nSiloID";*/
				}
				if (ArrayGetValue($arrayParams,"command") == "add")
				{
					$arrayParams["silo_id"] = "$nSiloID";	
				}
			}
		}


		function StateModify($strEntity)
		{
			$strState = parent::StateModify($strEntity);
			
			if ($strEntity == "user")
			{
				if ($strState == "modify")
				{
					$arrayPrivileges = $this->GetModuleConfig("privileges");
					if (!is_array($arrayPrivileges))
					{
						$arrayPrivileges = array();
					}
					if (ArrayCount($arrayPrivileges) > 0)
					{
						$strUserID = $this->GetModuleVar("user_id");
						$this->GetWebsite()->IncludeModule("useradmin/entitlementsettings");
						$entitlementsettings = new CEntitlementSettingsModule(
							$this->GetWebsite(),
							$strUserID,
							array("privileges" => $arrayPrivileges));
						$this->SetOutputItem("ENTITLEMENTSETTINGS",$entitlementsettings->RenderModule());
					}
					else
					{
						$this->SetOutputItem("ENTITLEMENTSETTINGS","");
					}
				}
			}
			return $strState;
		}


		function StateUserModifySubmit() 
		{
			$this->SetState("user_modifysubmit");
			
			$arrayItem = $this->GetItemDataFromModuleVarsInsecure("user");
			$arrayErrors = array();
			$this->OnCheckInputValues("user",false,$arrayItem,$arrayErrors);
			if (ArrayCount($arrayErrors) > 0)
			{
				$this->SetFormErrors($arrayErrors);
				return $this->StateModify("user");
			}

			
			
			$strWebservice = ArrayGetValue($this->GetEntityDefinition("user"),"webservice","name");
			
			$strUserID = $this->GetModuleVar("user_id");
			
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$arrayParams = array();
			$arrayParams["user_id"] = $strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($strWebservice,$arrayParams);
			$strError = $consumer->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
				return StateModify();
			}
			$strCurrentUserName = $consumer->GetResultValue("USER","USER_NAME");
			$strCurrentUserEmail = $consumer->GetResultValue("USER","USER_EMAIL");

			$strNewUserName = $this->GetModuleVar("user_name");
			if ($strNewUserName == "")
			{
				$this->SetErrorMessageText("?TID_USER_ERROR_RENAME_USERNAME_EMPTY?");
				return $this->StateModify("user");	
			}
			
			$strNewPassword = $this->GetModuleVar("password");
		
			
			// SET STATIC VALUES
	
			$strUserActive = $this->GetModuleVar("user_active");		
			$strUserLanguage = $this->GetModuleVar("user_language");			
			$strUserCountry = $this->GetModuleVar("user_country");
			$strUserTimezone = $this->GetModuleVar("user_timezone");
			
			$arrayLanguages = $this->GetWebsite()->GetSupportedLanguages();
			array_push($arrayLanguages,""); // to allow unsetting the language setting!
			if (ArrayValueExists($arrayLanguages,$strUserLanguage) == false)
			{
				$this->SetErrorMessageCode("LANGUAGE_NOT_SUPPORTED");
				return $this->StateModify("user");
			}
			
			if ((!IsValidCountryID($strUserCountry)) && $strUserCountry != "")
			{
				$this->SetErrorMessageCode("COUNTRY_INVALID");	
				return $this->StateModify("user");
				
			}
			
			if (!IsValidTimezoneID($strUserTimezone))
			{
				$this->SetErrorMessage("TIMEZONE_INVALID");	
				return $this->StateModify("user");
			}
			
			
						
			
			
			$arrayParams = array();
			$arrayParams["user_id"] = $strUserID;
			$arrayParams["user_active"] = $strUserActive;
			$arrayParams["user_language"] = $strUserLanguage;
			$arrayParams["user_country"] = $strUserCountry;
			$arrayParams["user_timezone"] = $strUserTimezone;
			if ($strNewPassword != "")
			{
				$arrayParams["password"] = $strNewPassword;	
			}
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($strWebservice,$arrayParams);
			
			$strError = $consumer->GetError();
			

			if ($strError == "")
			{
				$this->GetWebsite()->SetOutputItem("USER_INDEX",$strUserID);
				$this->GetWebsite()->SetOutputItem("USER_USERFRIENDLY",$strCurrentUserName);
				$this->GetWebsite()->SetInfoMessageText("?TID_USER_INFO_MODIFY_SUCCESS?");
			}
			else
			{
				$this->SetErrorMessageCode($strError);
				return $this->StateModify("user");
			}



			// RENAME
	
			if ($strNewUserName != $strCurrentUserName)
			{
				$arrayParams = array();
				$arrayParams["user_id"] = $strUserID;
				$arrayParams["new_user_name"] = $strNewUserName;
				$arrayParams["command"] = "rename";
				$consumer->ConsumeWebService($strWebservice,$arrayParams);
				
				$strError = $consumer->GetError();
	
				if ($strError == "USER_EXISTS")
				{
					$this->SetErrorMessageText("?TID_USER_ERROR_RENAME_FAILED_NAME_OCCUPIED?");
					return $this->StateModify("user");
				}
				else if ($strError != "")
				{
					$this->SetErrorMessageCode($strError);	
				}
			}
			
			// CHANGE EMAIL
			
			
			$strNewUserEmail = $this->GetModuleVar("user_email");	
			
			if ($strNewUserEmail != "")
			{
				if (IsValidEmail($strNewUserEmail) == false)	
				{
					$this->SetErrorMessageText("?TID_ERROR_FORM_INVALID_EMAIL?");	
					return $this->StateModify("user");
				}
			}
			
			if ($strCurrentUserEmail != $strNewUserEmail)
			{
				$arrayParams = array();
				$arrayParams["user_id"] = $strUserID;
				$arrayParams["user_email"] = $strNewUserEmail;
				$arrayParams["command"] = "changeemail";
				$consumer->ConsumeWebService($strWebservice,$arrayParams);
				$strError = $consumer->GetError();
				if ($strError != "")
				{
					$this->SetErrorMessageCode($strError);
					return $this->StateModify("user");
				}	
			}
					
			$this->SetModuleVar("state","");
			$this->SetModuleVar("user_language","");
			$this->SetModuleVar("user_country","");
			$this->SetModuleVar("user_timezone","");
			$this->SetModuleVar("user_email","");
			
			return $this->StateModify("user");
		}
		
		function StateView($strEntity)
		{
			$strState = parent::StateView($strEntity);
			
			if ($strEntity == "user")
			{
				if ($strState == "view")
				{
					$strUserID = $this->GetModuleVar("user_id");
					$this->GetWebsite()->IncludeModule("system/entitlementmanager");
					$entitlementmanager = new CEntitlementManagerModule(
						$this->GetWebsite(),
						$strUserID,
						array(
							"user_id" => $strUserID));
					$this->SetOutputItem("ENTITLEMENTMANAGER",$entitlementmanager->RenderModule());
				}
			}
			
			return $strState;
		
		}
		
/*		
		function OnItemAdded($strEntity,$strNewItemID,$arrayItem)
		{
			if ($strEntity == "user")
			{
				$this->GetWebsite()->UserPostCreation($strNewItemID);
			}
			parent::OnItemAdded($strEntity,$strNewItemID,$arrayItem);
		}
		*/
		
		function StateAddSubmit($strEntity)
		{
			$this->Trace("StateAddSubmit(\"$strEntity\")");
			$this->SetState($strEntity."_addsubmit");
			$this->AssertValidEntity($strEntity);
			$arrayEntity = $this->GetEntityDefinition($strEntity);
			
			$arrayItem = $this->GetItemDataFromModuleVarsInsecure($strEntity);
			$arrayErrors = $this->GetFormErrors();
			$this->OnCheckInputValues($strEntity,true,$arrayItem,$arrayErrors);
			$this->SetFormErrors($arrayErrors);
			if (ArrayCount($arrayErrors) > 0)
			{
				return $this->StateAdd($strEntity);
			}
	

			$strWebservice = ArrayGetValue($arrayEntity,"webservice","name");						
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$arrayParams = $this->GetItemWebserviceParams(
				$strEntity,
				$arrayItem);
			$arrayParams["command"] = $this->GetItemWebserviceCommand($strEntity,"add");
			$this->OnPrepareWebserviceParams($strEntity,$arrayParams);

			$strError = "";
			$arrayResult = array();
			$strUserID = $this->GetWebsite()->UserSignin( 
				ArrayGetValue($arrayParams,"user_name"),
				ArrayGetValue($arrayParams,"password"),
				$arrayParams,
				$strError,
				$arrayResult);

			if ($strError != "")
			{
				$this->SetErrorMessageCodeEntity($strEntity,$strError);					
				return $this->StateAdd($strEntity);
			}

			$strUserFriendly = ArrayGetValue($arrayItem,ArrayGetValue($arrayEntity,"userfriendlyattribute"));
			$strNewItemIndex = ArrayGetValue($arrayResult,ArrayGetValue($arrayEntity,"webservice","newitemidtag"));
			$this->SetInfoMessageCodeEntity($strEntity,"ADD_SUCCESS");
			$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_USERFRIENDLY",$strUserFriendly);
			$this->GetWebsite()->SetOutputItem(StringUpperCase($strEntity)."_INDEX",$strNewItemIndex);

			$this->ResetItemModuleVars($strEntity);

			$this->OnItemAdded($strEntity,$strNewItemIndex,$arrayItem);
			
			return $this->StateList($strEntity);
		}
		
		
		
		
		
		
	}

