<?php



	class CLoginModule extends CModule
	{
		private $m_strContentLogin;
		private $m_strContentLogout;
		private $m_strContentPostLogin;
	
		function __construct(
			CWebSite $website,
			$strContentLogin,
			$strContentLogout,
			$strContentPostLogin = "")
		{ 
			$this->m_strContentLogin = $strContentLogin;
			$this->m_strContentLogout = $strContentLogout;
			$this->m_strContentPostLogin = $strContentPostLogin;
			parent::__construct($website,"user/login");
		}
		
		function CallbackInit()
		{
			return true;
		}
		
		function CallbackWebsiteInit()
		{
			$this->Trace("CallbackWebsiteInit");
			$this->Trace("GetState() = ".$this->GetState());
			if ($this->GetState() == "loginattempt")
			{
				$this->GetWebsite()->UserLogin(
					$this->GetWebsite()->UserNameInputCorrection($this->GetModuleParam("username")),
					$this->GetModuleParam("password"));
			}
		}
		
		function CallbackRenderModule($strState)
		{
			
			// Prepare Username
			$this->GetWebsite()->SetOutputItem("USERNAME",$this->GetWebsite()->GetAuthUserName());
		
			// Prepare Status Style Login Link
			$arrayParams = array();
			$arrayParams["content"] = $this->m_strContentLogin;
			$strLogoutURL = $this->GetWebsite()->MakeURL($arrayParams);
			
			$link = new CLinkElement($this->GetWebsite(),"default");
			$this->SetOutputItem("STATUS_LOGINLINK",$link->RenderLink($strLogoutURL,"?TID_LOGIN_LINK_LOGIN?"));

			// Prepare Status Style Logout Link
			$arrayParams = array();
			$arrayParams["command"] = "logout";
			if ($this->m_strContentLogout != "")
			{
				$arrayParams["content"] = $this->m_strContentLogout;
			}
			$arrayParams[$this->GetModuleVarNameState()] = "prelogin";
			$strLogoutURL = $this->GetWebsite()->MakeURL($arrayParams);
			$this->SetOutputItem("STATUS_LOGOUTLINK",$link->RenderLink($strLogoutURL,"?TID_LOGIN_LINK_LOGOUT?"));
						
			
			
			switch ($strState)
			{
				case "loginattempt":
				{
					return $this->StateLoginAttempt();
				}
				case "postlogin":
				{
					return $this->StatePostLogin();	
				}
				case "prelogin":
				default:	
				{
					return $this->StatePreLogin();
				}
			}
		}
		
		function StatePreLogin()
		{
			$this->Trace("StatePreLogin");
			
			if ($this->GetWebsite()->GetAuthUserID() != "")
			{
				return $this->StatePostLogin();
			}

			$strUsername = $this->GetWebsite()->UserNameInputCorrection($this->GetModuleVar("username"));
			$this->Trace("\$this->GetModuleVar(\"username\") = \"$strUsername\"");
			if ($strUsername == "")
			{
				$strUsername = $this->GetWebsite()->GetAuthUsernameVar();
				$this->Trace("\$this->GetWebsite()->GetAuthUsernameVar() = \"$strUsername\"");
			}
			
			// Prepare Form Definition
			$formDef = new CFormDefinition();
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "loginattempt";
			if ($this->m_strContentLogin != "")
			{
				$arrayParams["content"] = $this->m_strContentLogin;
			}
			$formDef->SetURL($this->GetWebsite()->GetRootURL_PreferHTTPS(),$arrayParams);
			
			$formDef->AddControlText(
				"USERNAME",
				$this->GetModuleVarName("username"),
				"?TID_LOGIN_LABEL_USERNAME?",
				$strUsername,
				15,64);
			$formDef->AddControlPassword(
				"PASSWORD",
				$this->GetModuleVarName("password"),
				"?TID_LOGIN_LABEL_PASSWORD?",
				"",
				15,
				255);
			
			$formDef->AddControlButtonSubmit("?TID_LOGIN_BUTTON_LOGIN?");

			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem("FORM", $form->RenderForm($formDef,array()));
			
			$this->SetState("prelogin");
			
			return "default";
		}
		
		function StatePostLogin()
		{
			$this->Trace("StatePostLogin");
			
			if ($this->GetWebsite()->GetAuthUserID() == "")
			{
				return $this->StatePreLogin();
			}

			
			$this->SetState("postlogin");

			$this->SetOutputItem("FORM", "<p>?TID_LOGIN_INFO_LOGIN?</p>");
			
			return "postlogin";
		}


		function StateLoginAttempt()
		{
			$this->Trace("StateLoginAttempt");
		
			$strUserName = $this->GetModuleVar("username");
			$strUserName = $this->GetWebsite()->UserNameInputCorrection($strUserName);
			$strPassword = $this->GetModuleVar("password");
			
			$this->SetModuleVar("password","");

		
			if ($strUserName == "")
			{
				$this->SetErrorMessageCode("USERNAME_MISSING");
				return $this->StatePreLogin();
			}
			
			if ($strPassword == "")
			{
				$this->SetErrorMessageCode("PASSWORD_MISSING");
				return $this->StatePreLogin();
			}
			
			$strAuthUserName = $this->GetWebsite()->GetAuthUserName();
			if ($strAuthUserName == "")
			{
				
				$this->SetErrorMessageCode("INCORRECT");
				return $this->StatePreLogin();
			}
			else
			{
				return $this->StateLoginSuccess();
			}
	
		}
		
		function StateLoginSuccess()
		{
			$this->Trace("StateLoginSuccess");
			$this->SetState("postlogin");
			$this->SetInfoMessageCode("SUCCESS");
			$this->SetOutputItem("FORM", "");
			if ($this->m_strContentPostLogin != "")
			{
				$strURL = $this->GetWebsite()->MakeContentURL($this->m_strContentPostLogin);
				$this->GetWebsite()->Redirect($strURL);
			}
			return "postlogin";
		}




	}

