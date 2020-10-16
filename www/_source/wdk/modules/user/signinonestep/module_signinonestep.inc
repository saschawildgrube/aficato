<?php

	class CSigninOneStepModule extends CModule
	{
		private $m_formDef;
	
		function __construct(
			CWebSite $website)
		{
			parent::__construct($website,"user/signinonestep");
		}
		
		function CallbackInit()
		{
			$this->m_formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "signinattempt";
			$this->m_formDef->SetURL($this->GetWebsite()->GetRootURL_PreferHTTPS(),$arrayParams);
			
			$this->m_formDef->AddControlText(
				"NEW_USERNAME",
				$this->GetModuleVarName("new_username"),
				"?TID_SIGNINONESTEP_LABEL_USERNAME?",
				"",
				15,64);
			$this->m_formDef->AddControlPassword(
				"NEW_PASSWORD",
				$this->GetModuleVarName("new_password"),
				"?TID_SIGNINONESTEP_LABEL_PASSWORD?",
				"",
				15,255);
			$this->m_formDef->AddControlPassword(
				"NEW_PASSWORD2",
				$this->GetModuleVarName("new_password2"),
				"?TID_SIGNINONESTEP_LABEL_PASSWORD_REPEAT?",
				"",
				15,255);
			$this->m_formDef->AddControlButtonSubmit(
				"?TID_SIGNINONESTEP_BUTTON_SIGNIN?");

			return true;
		}
		
		
		function CallbackWebsiteInit()
		{
			if ($this->GetState() == "signinattempt")
			{
				$this->GetWebsite()->UserLogin(
					$this->GetWebsite()->UserNameInputCorrection($this->GetModuleParam("new_username")),
					$this->GetModuleParam("new_password"));
			}
		}		
		
		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "signinattempt":
				{
					return $this->StateSigninAttempt();	
				}
				case "postsignin":
				{
					return $this->StatePostSignin();	
				}
				case "presignin":
				default:	
				{
					return $this->StatePreSignin();
				}
			}
		}

		
		function StatePreSignin()
		{
			$this->Trace("StatePreSignin");
			
			
			$arrayValues = array();
			$arrayValues[$this->GetModuleVarName("new_username")] = $this->GetModuleVar("new_username");
			
			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem(
				"FORM",
				$form->RenderForm(
					$this->m_formDef,
					$arrayValues));
	
			return "presignin";
		}
		
		function StateSigninAttempt()
		{

			$strUserName = $this->GetWebsite()->UserNameInputCorrection($this->GetModuleVar("new_username"));
			$strPassword = $this->GetModuleVar("new_password");
			$strPassword2 = $this->GetModuleVar("new_password2");
			
			$this->SetModuleVar("new_password","");
			$this->SetModuleVar("new_password2","");
			
			
			if ($strUserName == "")
			{
				$this->SetErrorMessageCode("USERNAME_MISSING");
				return $this->StatePreSignin();
			}
			 
			if ($this->GetWebSite()->UserCheckName($strUserName) == false)
			{
				$this->SetErrorMessageCode("USERNAME_NOTALLOWED");
				$this->SetModuleVar("new_username","");
				return $this->StatePreSignin();
			}			
			
			if ($strPassword == "")
			{
				$this->SetErrorMessageCode("PASSWORD_MISSING");
				return $this->StatePreSignin();
			}
			
			if ($strPassword != $strPassword2)
			{
				$this->SetErrorMessageCode("PASSWORD_MISMATCH");
				return $this->StatePreSignin();
			}

			$strError = "";
			$arrayResults = array();
			$strUserID = $this->GetWebsite()->UserSignin(
				$strUserName,
				$strPassword,
				array(),
				$strError,
				$arrayResults);
			
			if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
				return $this->StatePreSignin();
			}

			if ($strUserID == false)
			{
				$this->SetErrorMessageCode("UNKNOWN");
				return $this->StatePreSignin();
			}

			$this->Trace("New User ID: $strUserID");
			
			$bSuccess = $this->GetWebsite()->UserLogin($strUserName,$strPassword);
			
			if ($bSuccess != true)
			{
				$this->SetErrorMessageCode("LOGIN_FAILED");
				return $this->StatePreSignin();
			}
			return $this->StateSigninSuccess();
		}
		
		function StatePostSignin()
		{
			if ($this->GetWebsite()->GetAuthUserID() == "")
			{
				return $this->StatePreSignin();	
			}
			$this->SetOutputItem("MESSAGE", "");
			return "postsignin";	
		}

		function StateSigninSuccess()
		{
			$message = new CMessageElement($this->GetWebsite(),"info");
			$this->SetOutputItem("MESSAGE", $message->RenderMessage("?TID_SIGNINONESTEP_INFO_WELCOME?"));
			$this->SetState("postsignin");
			return "postsignin";	
		}


		


	}

