<?php

	class CSignOutModule extends CModule
	{
		private $m_arrayErrors;
		private $m_strPreferredContent;
		private $m_strContentAfterSignOut; 
		
		function __construct(
			CWebSite $website,
			$strPreferredContent = ""
			//,$strContentAfterSignOut = ""
			)
		{
			$this->m_arrayErrors = array();
			$this->m_strPreferredContent = $strPreferredContent;
			//$this->m_strContentAfterSignOut = $strContentAfterSignOut;
			parent::__construct($website,"user/signout");
		}
		
		function CallbackInit()
		{
			return true;
		}
		
		
		function CallbackWebsiteInit()
		{
/*
			if ($this->GetState() == "signout")
			{
			}
	*/		
		}		
		
		function CallbackRenderModule($strState)
		{
			if ($this->GetWebSite()->GetAuthUserID() == "")
			{
				return $this->StateNoLogin();	
			}
			switch ($strState)
			{
				case "signout":
				{
					return $this->StateSignOut();
				}
				default:	
				{
					return $this->StatePreSignOut();
				}
			}
		}
		
		function StateNoLogin()
		{
			$this->SetState("nologin");
			return "nologin";
		}

		
		function StatePreSignOut()
		{
			$this->SetState("presignout");
			
			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "signout";
			if ($this->m_strPreferredContent != "")
			{
				$arrayParams["content"] = $this->m_strPreferredContent;
			}
			$formDef->SetURL($this->GetWebsite()->GetRootURL_PreferHTTPS(),$arrayParams);
			
			$formDef->AddControlStatic("TEXT","?TID_SIGNOUT_TEXT_PRESIGNOUT?");
			$formDef->AddControlButtonSubmit("?TID_SIGNOUT_BUTTON_SIGNOUT?");
			
			$form = new CFormElement($this->GetWebsite(),"default");
			
			$this->SetOutputItem(
				"FORM",
				$form->RenderForm(
					$formDef,
					$this->GetAllModuleVars(),
					$this->m_arrayErrors));
			

			return "presignout";
		}
		
		function StateSignOut()
		{
			$this->SetState("signout");

			$bConfirmation = GetBoolValue($this->GetModuleVar("confirmation"));
			$this->SetModuleVar("confirmation","");
			
						
			if ($bConfirmation == false)
			{
				$formDef = new CFormDefinition();
				
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "signout";
				if ($this->m_strPreferredContent != "")
				{
					$arrayParams["content"] = $this->m_strPreferredContent;
				}
				$formDef->SetURL($this->GetWebsite()->GetRootURL_PreferHTTPS(),$arrayParams);
				
				$formDef->AddControlCheckbox(
					"CONFIRMATION",
					$this->GetModuleVarName("confirmation"),
					"?TID_SIGNOUT_LABEL_CONFIRMATION?",
					false);
	
				$formDef->AddControlButtonSubmit("?TID_SIGNOUT_BUTTON_CONFIRM?");
				
				$form = new CFormElement($this->GetWebsite(),"default");
				
				$this->SetOutputItem(
					"FORM",
					$form->RenderForm(
						$formDef,
						$this->GetAllModuleVars(),
						$this->m_arrayErrors));
						
				$this->SetState("presignout");
				return "signout";
			}
			
			$bResult = $this->GetWebsite()->UserDelete(
				$this->GetWebsite()->GetAuthUserID());
			
			if ($bResult == false)
			{
				$message = new CMessageElement($this->GetWebsite(),"error");
				$this->SetOutputItem(
					"MESSAGE",
					$message->RenderMessage($this->GetMessage("ERROR","SIGNOUTFAILED"))
					);
				//$this->SetErrorMessageCode("SIGNOUTFAILED");
				return "error";
			}

			$message = new CMessageElement($this->GetWebsite(),"info");
			$this->SetOutputItem(
				"MESSAGE",
				$message->RenderMessage($this->GetMessage("INFO","SIGNOUTSUCCESS"))
				);

			
			$this->SetState("postsignout");
			return "postsignout";
		}		
	
	}

