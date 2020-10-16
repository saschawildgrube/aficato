<?php

	
	class CUserSettingsModule extends CModule
	{
	
		function __construct(
			CWebSite $website)
		{
			parent::__construct($website,"user/usersettings");
		}
		
		function CallbackInit()
		{
			$this->GetWebsite()->LoadStringResources("local");
			return true;
		}

		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "modify-submit":
				{
					return $this->StateModifySubmit();
				}
				case "modify":
				default:
				{
					return $this->StateModify();
				}
			}
		}
		

		
		function StateModify()
		{
			
			$strUserID = $this->GetWebsite()->GetAuthUserID();
			
			$this->Trace("strUserID = $strUserID");
			
			if ($strUserID == "")
			{
				return "nologin";
			}
			
				
			
			// Get information about user
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());
			$arrayParams = array();
			$arrayParams["user_id"] = $strUserID;
			$arrayParams["command"] = "get";
			
			
			$consumer->ConsumeWebService("system/user",$arrayParams);
			
			if ($consumer->GetError() != "")
			{
				return "error";	
			}
			
			$strUserName = $consumer->GetResultValue("USER","USER_NAME");
			$strUserLanguage = $consumer->GetResultValue("USER","USER_LANGUAGE");
			$strUserCountry = $consumer->GetResultValue("USER","USER_COUNTRY");
			$strUserTimezone = $consumer->GetResultValue("USER","USER_TIMEZONE");
			
			
			
			// Create Form
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "modify-submit";
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			$formDef->AddControlStatic("TEXT","?TID_USERSETTINGS_TEXT?");
			
			$arraySupportedLanguages = $this->GetWebsite()->GetSupportedLanguages();
			$arrayLanguageOptions = array();
			$arrayLanguageOptions[""] = "?TID_USERSETTINGS_OPTION_NOPREFERREDLANGUAGE?";
			foreach ($arraySupportedLanguages as $strLanguage)
			{
				$arrayLanguageOptions[$strLanguage] = "?TID_LANGUAGE_".StringUpperCase($strLanguage)."?";
			}
			$formDef->AddControlSelect("USERLANGUAGE",$this->GetModuleVarName("userlanguage"),"?TID_USERSETTINGS_LABEL_USERLANGUAGE?",$arrayLanguageOptions,$strUserLanguage);
			
			$arrayAllCountries = GetAllCountryIDs();
			$arrayCountryOptions = array();
			$arrayCountryOptions[""] = "?TID_USERSETTINGS_OPTION_NOCOUNTRY?";
			foreach ($arrayAllCountries as $strCountry)
			{
				$arrayCountryOptions[$strCountry] = "?TID_COUNTRY_".StringUpperCase($strCountry)."?";
			}
			$formDef->AddControlSelect("USERCOUNTRY",$this->GetModuleVarName("usercountry"),"?TID_USERSETTINGS_LABEL_USERCOUNTRY?",$arrayCountryOptions,$strUserCountry);


			$arrayAllTimezones = GetAllTimezoneIDs();
			$arrayTimezoneOptions = array();
			foreach ($arrayAllTimezones as $strTimezone)
			{
				$arrayTimezoneOptions[$strTimezone] = "$strTimezone";
			}
			$formDef->AddControlSelect("USERTIMEZONE",$this->GetModuleVarName("usertimezone"),"?TID_USERSETTINGS_LABEL_USERTIMEZONE?",$arrayTimezoneOptions,$strUserTimezone);


			$formDef->AddControlButtonSubmit("?TID_USERSETTINGS_BUTTON_EDIT?");
			
			
			// RenderForm
			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem("FORM", $form->RenderForm($formDef,array()) );
		
			
								
			$this->SetModuleVar("state","");
			$this->SetModuleVar("userid","");
			
			return "modify";
		}
		
		function StateModifySubmit()
		{
			
			
			$strUserID = $this->GetWebsite()->GetAuthUserID();
			
			$strUserLanguage = $this->GetModuleVar("userlanguage");
			$strUserCountry = $this->GetModuleVar("usercountry");
			$strUserTimezone = $this->GetModuleVar("usertimezone");
			
			$arrayLanguages = $this->GetWebsite()->GetSupportedLanguages("languages");
			array_push($arrayLanguages,""); // to allow unsetting the language setting!
			if (ArrayValueExists($arrayLanguages,$strUserLanguage) == false)
			{
				$this->SetErrorMessageCode("LANGUAGE_NOT_SUPPORTED");	
				return $this->StateEdit();
			}
			
			if ((!IsValidCountryID($strUserCountry)) && $strUserCountry != "")
			{
				$this->SetErrorMessageCode("COUNTRY_INVALID");	
				return $this->StateEdit();
				
			}
			
			if (!IsValidTimezoneID($strUserTimezone))
			{
				$this->SetErrorMessageCode("TIMEZONE_INVALID");	
				return $this->StateEdit();
			}
			
			
						
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			
			$arrayParams = array();
			$arrayParams["user_id"] = $strUserID;
			$arrayParams["user_language"] = $strUserLanguage;
			$arrayParams["user_country"] = $strUserCountry;
			$arrayParams["user_timezone"] = $strUserTimezone;
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService("system/user",$arrayParams);
			
			$strError = $consumer->GetError();
			

			if ($strError == "")
			{
				$this->SetInfoMessageCode("USER_EDIT_SUCCESS");
				
				$this->GetWebsite()->SetTimezone($strUserTimezone); 

				$this->SetModuleVar('state','');
			}
			else
			{
				$this->AddErrorMessageCode($strError);	
			}

					
			$this->SetModuleVar("state","");
			//$this->SetModuleVar("userid","");
			$this->SetModuleVar("userlanguage","");
			$this->SetModuleVar("usercountry","");
			$this->SetModuleVar("usertimezone","");


			
			return $this->StateModify();
		}
		
		
		
		
		
		
		
	}

