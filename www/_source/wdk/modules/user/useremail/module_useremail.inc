<?php

	
	class CUserEmailModule extends CModule
	{
		private $m_arrayErrors;
		private $m_bAllowUnset;
		private $m_bEmailValidation;
		private $m_strEmailValidationTokenParameterName;
		private $m_nEmailValidationDuration;
	
		function __construct(
			CWebSite $website,
			$bAllowUnset = true,
			$bEmailValidation = false,
			$strEmailValidationTokenParameterName = "verification",
			$nEmailValidationDuration = 259200) // 3 days = 60 * 60 * 24 * 3 seconds = 259200 seconds
		{
			$this->m_arrayErrors = array();
			$this->m_bAllowUnset = $bAllowUnset;
			$this->m_bEmailValidation = $bEmailValidation;
			$this->m_strEmailValidationTokenParameterName = $strEmailValidationTokenParameterName;
			$this->m_nEmailValidationDuration = $nEmailValidationDuration;
			parent::__construct($website,"user/useremail");
		}
		
		function CallbackInit()
		{
			return true;
		}
		
		function CallbackWebsiteInit()
		{
		}		

		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "validation":
				{
					return $this->StateValidation();
				}
				case "modify-submit":
				{
					return $this->StateModifySubmit();
				}
				case "modify":
				{
					return $this->StateModify();
				}
				default:
				{
					if ($this->GetVar($this->m_strEmailValidationTokenParameterName) != "")
					{
						return $this->StateValidation();
					}
					else
					{
						return $this->StateModify();
					}
				}
			}
		}
		

		
		function StateModify()
		{
			$this->SetState("modify");
			
			$strUserID = $this->GetWebsite()->GetAuthUserID();
			
			$this->Trace("strUserID = $strUserID");
			
			if ($strUserID == "")
			{
				return "nologin";
			}
			
			
			// Get current email
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());
			$arrayParams = array();
			$arrayParams["user_id"] = $strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService(
				"system/user",
				$arrayParams);
			if ($consumer->GetError() != "")
			{
				return "error";	
			}
			$strEmail = $consumer->GetResultValue("USER","USER_EMAIL");
		
			
			// Create Form
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "modify-submit";
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			$formDef->AddControlStatic("TEXT","?TID_USEREMAIL_TEXT?");
			
			// Email Text input
			$formDef->AddControlText(
				"EMAIL",
				$this->GetModuleVarName("email"),
				"?TID_USEREMAIL_LABEL_EMAIL?",
				$strEmail,
				40,
				255);
				
			
			$strValidationToken = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_TOKEN");	
			if ($strValidationToken != "")
			{
				$strNewEmail = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_NEW");	
				$this->SetOutputItem("NEWEMAIL",$strNewEmail);
				
				$message = new CMessageElement($this->GetWebsite(),"info");
				$strOutput = $message->RenderMessage("?TID_USEREMAIL_INFO_VALIDATION_PENDING?");
				$formDef->AddControlStatic("INFO",$strOutput);
			}
				
			$formDef->AddControlButtonSubmit("?TID_USEREMAIL_BUTTON_MODIFY?");
			
			
			// RenderForm
			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem("FORM",
				$form->RenderForm(
					$formDef,
					array(),
					$this->m_arrayErrors) );
						
			return "modify";
		}
		
		function StateModifySubmit()
		{
			$this->SetState("modify-submit");
			
			$strUserID = $this->GetWebsite()->GetAuthUserID();		
			
			// Get current email
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());
			$arrayParams = array();
			$arrayParams["user_id"] = $strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService(
				"system/user",
				$arrayParams);
			if ($consumer->GetError() != "")
			{
				return "error";	
			}
			$strCurrentEmail = $consumer->GetResultValue("USER","USER_EMAIL");
						
			$strNewEmail = $this->GetModuleVar("email");
			$this->Trace("strNewEmail = $strNewEmail");
			
			if ($strNewEmail == $strCurrentEmail)
			{
				$this->SetInfoMessageText("?TID_USEREMAIL_INFO_EMAIL_IDENTICAL?");	
				return $this->StateModify();
			}
			
			//$this->SetModuleVar("email","");

			if ($this->m_bAllowUnset == false)
			{
				if ($strNewEmail == "")
				{
					//$this->SetErrorMessageText("?TID_USEREMAIL_ERROR_EMAIL_EMPTY?");
					$this->m_arrayErrors["EMAIL"] = "?TID_USEREMAIL_ERROR_EMAIL_EMPTY?";
					return $this->StateModify();
				}
			}

			if ($strNewEmail != "")
			{
				if (IsValidEmail($strNewEmail)!=true)			
				{
					//$this->SetErrorMessageText("?TID_USEREMAIL_ERROR_EMAIL_INVALID?");
					$this->m_arrayErrors["EMAIL"] = "?TID_USEREMAIL_ERROR_EMAIL_INVALID?";
					return $this->StateModify();
				}
			}


			if (($this->m_bEmailValidation == true) && ($strNewEmail != ""))
			{
				$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
				$arrayParams = array();
				$arrayParams["user_id"] = $strUserID;
				$arrayParams["user_email"] = $strNewEmail;
				$arrayParams["user_email_validation"] = "1";
				$arrayParams["user_email_validation_duration"] = $this->m_nEmailValidationDuration;
				$arrayParams["command"] = "changeemail";
				$consumer->ConsumeWebService(
					"system/user",
					$arrayParams);
				$strError = $consumer->GetError();
				if ($strError != "")
				{
					$this->SetErrorMessageCode($strError);
					return $this->StateModify();
				}
				$strToken = $consumer->GetResultValue("VALIDATION_TOKEN");
				$arrayParams = array();
				$arrayParams[$this->m_strEmailValidationTokenParameterName] = $strToken;
				$strValidationLink = MakeURL($this->GetWebsite()->GetRootURL(),$arrayParams);
				
				
				$arrayTags = array();
				$arrayTags["VALIDATION_LINK"] = $strValidationLink;
				$arrayTags["SITENAME"] = $this->GetWebsite()->GetSiteName();
				$arrayTags["SITETEAM"] = $this->GetWebsite()->GetSiteTeamName();
				$strMessage = $this->GetWebsite()->LoadString("TID_USEREMAIL_EMAILBODY_EMAILVALIDATION");
				$strMessage = ReplaceTagsWDK($strMessage,$arrayTags);
				
				
				SendMail(
					$strNewEmail,
					$this->GetWebsite()->LoadString("TID_USEREMAIL_EMAILSUBJECT_EMAILVALIDATION"),
					$strMessage,
					$this->GetWebsite()->GetSiteTeamEmail(),
					$this->GetWebsite()->GetSiteTeamName(),
					
					array());
					
				$message = new CMessageElement($this->GetWebsite(),"info");
				$strMessage = $message->RenderMessage("?TID_USEREMAIL_INFO_VALIDATION_EMAIL_SENT?");
				$this->SetOutputItem("MESSAGE",$strMessage);
				
				
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "modify";
				$strBackLinkURL = $this->GetWebsite()->MakeURL($arrayParams);
				$link = new CLinkElement($this->GetWebsite(),"default");
				$strBackLink = $link->RenderLink($strBackLinkURL,"?TID_USEREMAIL_LINK_BACK?");
				$this->SetOutputItem("BACKLINK",$strBackLink);
					
				return "validationemailsent"; 
			}
			else
			{

				$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
				$arrayParams = array();
				$arrayParams["user_id"] = $strUserID;
				$arrayParams["user_email"] = $strNewEmail;
				$arrayParams["user_email_validation"] = "0";
				$arrayParams["command"] = "changeemail";
				$consumer->ConsumeWebService(
					"system/user",
					$arrayParams);
				$strError = $consumer->GetError();
				if ($strError != "")
				{
					$this->SetErrorMessageCode($strError);
					return $this->StateModify();
				}
	
				if ($strNewEmail != "")
				{
					$this->GetWebsite()->SetInfoMessageText("?TID_USEREMAIL_INFO_CHANGE_EMAIL_SUCCESS?");
				}
				else
				{
					$this->GetWebsite()->SetInfoMessageText("?TID_USEREMAIL_INFO_UNSET_EMAIL_SUCCESS?");
				}
				 
				return $this->StateModify();
			}
		}

		function StateValidation()
		{ 
			$this->SetState("validation");
			$this->SetState("");
			$strToken = $this->GetVar($this->m_strEmailValidationTokenParameterName);
			$this->SetVar($this->m_strEmailValidationTokenParameterName,"");
			if ($strToken == "")
			{
				return "nologin";	
			}
			
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$arrayParams = array();
			$arrayParams["user_email_validation_token"] = $strToken;
			$arrayParams["command"] = "validateemail";
			$consumer->ConsumeWebService(
				"system/user",
				$arrayParams);
			$strError = $consumer->GetError();
			$this->Trace($strError);
			if ($strError == "TOKEN_NOT_FOUND")
			{
				$this->GetWebsite()->SetWarningMessageText("?TID_USEREMAIL_ERROR_TOKEN_NOT_FOUND?");
				return "nologin";
			}
			if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
			}
			$this->SetInfoMessageText("?TID_USEREMAIL_INFO_VALIDATION_SUCCESSFUL?");
			return "nologin";
		}
			
		
	}

