<?php

	require_once(GetWDKDir()."wdk_encryption-asymmetric.inc");

	class CCreateKeyPairModule extends CModule
	{
		protected $m_aee;
		
		function __construct(
			CWebSite $website)
		{
			parent::__construct(
				$website,
				"security/createkeypair");
			$this->m_aee = new CAsymmetricEncryptionEngineGPG(
				GetGPGPath(),
				GetTempDir()
				);
		}
		
		function CallbackInit()
		{
			return true;
		}
		
		function CheckSystemRequirements()
		{
			return true;	
		}		

		


		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "create":
				{
					return $this->StateCreate();	
				}
				case "form":
				default:	
				{
					return $this->StateForm();
				}
			}
		}
		
		
		function StateForm()
		{
			$this->Trace("StateForm");
			
			if ($this->CheckSystemRequirements() != true)
			{
				$this->SetErrorMessageCode("SYSTEMREQUIREMENTS");
				return "error";	
			}


			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "create";		
			$formDef->SetURL($this->GetWebsite()->GetRootURL_PreferHTTPS(),$arrayParams);

			$strName = $this->GetModuleVar("name");
			$strComment = $this->GetModuleVar("comment");
			$strEmail = $this->GetModuleVar("email");
			if ($strName == "" || $strEmail == "")
			{
				if ($this->GetWebsite()->GetAuthUserID() != "")
				{
					$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());
									
					$arrayParams = array();
					$arrayParams["command"] = "get";
					$arrayParams["userid"] = $this->GetWebsite()->GetAuthUserID();
					$consumer->ConsumeWebService("system/user",$arrayParams);
	
					if ($consumer->GetError()=="")
					{
						if ($strName == "")
						{
							$strName = $consumer->GetResultValue("USER","USER_NAME");			
						}
						if ($strEmail == "")
						{
							$strEmail = $consumer->GetResultValue("USER","USER_EMAIL");
						}
					}
				}	
			}
			 

			$formDef->AddControlText("NAME",$this->GetModuleVarName("name"),"?TID_CREATEKEYPAIR_LABEL_NAME?",$strName,50,255);
			$formDef->AddControlText("EMAIL",$this->GetModuleVarName("email"),"?TID_CREATEKEYPAIR_LABEL_EMAIL?",$strEmail,50,255);
			$formDef->AddControlText("COMMENT",$this->GetModuleVarName("comment"),"?TID_CREATEKEYPAIR_LABEL_COMMENT?",$strComment,50,255);
			$formDef->AddControlPassword("PASSWORD",$this->GetModuleVarName("password"),"?TID_CREATEKEYPAIR_LABEL_PASSWORD?","",50,255);
			$formDef->AddControlButtonSubmit("?TID_CREATEKEYPAIR_BUTTON_CREATE?");
			
			$form = new CFormElement($this->GetWebsite(),"default");
			
			$this->SetOutputItem(
				"form",
				$form->RenderForm(
					$formDef,
					array()));
	
			return "form";
		}

		function StateCreate()
		{
			$this->Trace("StateCreate");
			
			// Check if all values are set
			
			$strName = $this->GetModuleVar("name");
			$strEmail = $this->GetModuleVar("email");
			$strComment = $this->GetModuleVar("comment");
			$strPassword = $this->GetModuleVar("password");
			
			if ($strName == "")
			{
				$this->SetErrorMessageCode("NAME_MISSING");
				return $this->StateForm();
			}

			if (StringLength($strName) < 5)
			{
				$this->SetErrorMessageCode("NAME_TOOSHORT");
				return $this->StateForm();
			}

			
			if (!IsValidEmail($strEmail))
			{
				$this->SetErrorMessageCode("EMAIL_INVALID");
				return $this->StateForm();
			}
			
			if ($strPassword == "")
			{
				$this->SetErrorMessageCode("PASSWORD_MISSING");
				return $this->StateForm();
			}
			

			$strPublicKey = "";
			$strPrivateKey = "";
			$bResult = $this->m_aee->CreateKeyPair(
				$strName,
				$strComment,
				$strEmail,
				$strPassword,
				$strPrivateKey,
				$strPublicKey);
				
			if ($bResult != true)
			{
				$this->Trace($this->m_aee->GetTraceLog());
				$this->SetErrorMessageCode("CREATIONFAILED");
				return $this->StateForm();
			}

			$console = new CConsoleElement($this->GetWebsite(),"default");
			$text = new CTextElement($this->GetWebsite(),"default");

			
			if ($strComment == "")
			{
				$strDetails = $strName . " <".$strEmail.">";
			}
			else
			{
				$strDetails = $strName . " (".$strComment.") <".$strEmail.">";
			}
			$this->SetOutputItem(
				"DETAILS",
				$text->RenderText(
					"?TID_CREATEKEYPAIR_TITLE_DETAILS?",
					SecureOutput($strDetails))
				);

			$this->SetOutputItem(
				"PUBLICKEY",
				$text->RenderText(
					"?TID_CREATEKEYPAIR_TITLE_PUBLICKEY?",
					$console->RenderConsole(
						$strPublicKey)
					)
				);
			

			$this->SetOutputItem(
				"PRIVATEKEY",
				$text->RenderText(
					"?TID_CREATEKEYPAIR_TITLE_PRIVATEKEY?",
					$console->RenderConsole(
						$strPrivateKey)
					)
				);
			
			$this->SetModuleVar("name","");
			$this->SetModuleVar("email","");
			$this->SetModuleVar("comment","");
			$this->SetModuleVar("password","");

			$this->SetModuleVar("state","");
			
			return "create";
		}

	}

