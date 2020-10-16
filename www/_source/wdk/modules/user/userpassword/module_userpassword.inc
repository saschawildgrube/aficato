<?php

	
	class CUserPasswordModule extends CModule
	{
		private $m_arrayErrors;
		private $m_passwordpolicy;
	
		function __construct(
			CWebSite $website,
			CPasswordPolicy $passwordpolicy)
		{
			$this->m_arrayErrors = array();
			$this->m_passwordpolicy = $passwordpolicy;
			parent::__construct($website,"user/userpassword");
		}
		
		function CallbackInit()
		{
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
			$this->SetState("modify");
			
			$strUserID = $this->GetWebsite()->GetAuthUserID();
			
			$this->Trace("strUserID = $strUserID");
			
			if ($strUserID == "")
			{
				return "nologin";
			}
			

			
			// Create Form
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "modify-submit";
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			$formDef->AddControlStatic("TEXT","?TID_USERPASSWORD_TEXT?");
			$formDef->AddControlPassword("OLD_PASSWORD",$this->GetModuleVarName("oldpassword"),"?TID_USERPASSWORD_LABEL_OLDPASSWORD?","",15,255);
			$formDef->AddControlPassword("NEW_PASSWORD1",$this->GetModuleVarName("newpassword1"),"?TID_USERPASSWORD_LABEL_NEWPASSWORD?","",15,255);
			$formDef->AddControlPassword("NEW_PASSWORD2",$this->GetModuleVarName("newpassword2"),"?TID_USERPASSWORD_LABEL_NEWPASSWORD_REPEATED?","",15,255);
			
			
			if (get_class($this->m_passwordpolicy) == "CPasswordPolicy")
			{
				$this->IncludeElement("passwordpolicy");
				$passwordpolicyelement = new CPasswordPolicyElement($this->GetWebsite(),"default");
				$formDef->AddControlStatic(
					"PASSWORDPOLICY",
					$passwordpolicyelement->RenderPasswordPolicyInfo(
						$this->m_passwordpolicy));	
			}

			
			$formDef->AddControlButtonSubmit("?TID_USERPASSWORD_BUTTON_MODIFY?");
			
			
			// RenderForm
			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem("FORM",
				$form->RenderForm(
					$formDef,
					array(),
					$this->m_arrayErrors)
				);
		
					
			
						
			return "modify";
		}
		
		function StateModifySubmit()
		{
			$this->SetState("modify-submit");
			
			
						
			$strOldPassword = $this->GetModuleVar("oldpassword");
			$this->Trace("strOldPassword = $strOldPassword");
			$strNewPassword1 = $this->GetModuleVar("newpassword1");
			$strNewPassword2 = $this->GetModuleVar("newpassword2");
			
			$this->SetModuleVar("oldpassword","");
			$this->SetModuleVar("newpassword1","");
			$this->SetModuleVar("newpassword2","");

			if ($strOldPassword == "")			
			{
				$this->SetErrorMessageCode("OLDPASSWORD_MISSING");
				$this->m_arrayErrors["OLD_PASSWORD"] = "";
				return $this->StateModify();
			}

			if ($strNewPassword1 == "")			
			{
				$this->SetErrorMessageCode("NEWPASSWORD_MISSING");
				$this->m_arrayErrors["NEW_PASSWORD1"] = "";
				return $this->StateModify();
			}
			
			
			if (is_object($this->m_passwordpolicy))
			{
				$arrayErrors = array();
				if (!$this->m_passwordpolicy->CheckPassword($strNewPassword1,$arrayErrors))
				{
					$this->SetModuleVar("new_password","");
					$this->SetModuleVar("new_password2","");
					$this->IncludeElement("passwordpolicy");	
					$passwordpolicyelement = new CPasswordPolicyElement($this->GetWebsite(),"default");
					$this->m_arrayErrors["NEW_PASSWORD1"] = $passwordpolicyelement->RenderPasswordPolicyErrors($arrayErrors);
					return $this->StateModify();					
				}	
			}			


			if ($strNewPassword1 != $strNewPassword2)			
			{
				$this->AddErrorMessageCode("NEWPASSWORD_MISMATCH");
				$this->m_arrayErrors["NEW_PASSWORD1"] = "";
				$this->m_arrayErrors["NEW_PASSWORD2"] = "";
				return $this->StateModify();
			}

			$bResult = $this->GetWebsite()->UserChangePassword($strOldPassword,$strNewPassword1);
			if ($bResult == true)
			{
				$this->SetInfoMessageCode("MODIFY_PASSWORD_SUCCESS");
			}
				
			return $this->StateModify();
		}
	}

