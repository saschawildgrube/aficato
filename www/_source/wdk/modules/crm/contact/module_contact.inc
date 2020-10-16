<?php



	class CContactModule extends CModule
	{
		private $m_formDef;
		private $m_arrayErrors;
	
		function __construct(
			CWebSite $website,
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"crm/contact",
				"",
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			return true;
		}
		
		

		


		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "submit":
				{
					return $this->StateSubmit();	
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

			$this->m_formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "submit";
			$this->m_formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			if ($this->GetModuleConfig("text_bottom") == false)
			{
				$this->m_formDef->AddControlTextArea("TEXT",$this->GetModuleVarName("text"),"?TID_CONTACT_LABEL_TEXT?","",40,7); 				
			}
			
			if ($this->GetModuleConfig("salutation_hide") == false)
			{
				$arraySalutation = array();
				$arraySalutation["mrs"] = "?TID_SALUTATION_MRS?";
				$arraySalutation["mr"] = "?TID_SALUTATION_MR?";
				$this->m_formDef->AddControlSelect("SALUTATION",$this->GetModuleVarName("salutation"),"?TID_CONTACT_LABEL_SALUTATION?",$arraySalutation,"mr");
			}
			$this->m_formDef->AddControlText("NAME",$this->GetModuleVarName("name"),"?TID_CONTACT_LABEL_NAME?","",50,255);
			if ($this->GetModuleConfig("company_hide") == false)
			{
				$this->m_formDef->AddControlText("COMPANY",$this->GetModuleVarName("company"),"?TID_CONTACT_LABEL_COMPANY?","",50,255);
			}
			if ($this->GetModuleConfig("phone_hide") == false)
			{
				$this->m_formDef->AddControlText("PHONE",$this->GetModuleVarName("phone"),"?TID_CONTACT_LABEL_PHONE?","",50,255);
			}
			$this->m_formDef->AddControlText("EMAIL",$this->GetModuleVarName("email"),"?TID_CONTACT_LABEL_EMAIL?","",50,255);
			
			if ($this->GetModuleConfig("text_bottom") == true)
			{
				$this->m_formDef->AddControlTextArea("TEXT",$this->GetModuleVarName("text"),"?TID_CONTACT_LABEL_TEXT?","",40,7); 
			}

			
			$this->m_formDef->AddControlButtonSubmit("?TID_CONTACT_BUTTON_SEND?");
			
			$form = new CFormElement($this->GetWebsite(),"default");
			 
			$this->SetOutputItem(
				"FORM",
				$form->RenderForm(
					$this->m_formDef,
					$this->GetAllModuleVars(),
					$this->m_arrayErrors)
				);
	
			return "form";
		}

		function StateSubmit()
		{
			$this->Trace("StateSubmit");
			
			// Check if all values are set
			
			$strSalutation = $this->GetModuleVar("salutation");
			$strName = $this->GetModuleVarInsecure("name");
			$strEmail = $this->GetModuleVar("email");
			$strText = utf8_decode($this->GetModuleVarInsecure("text"));
			$strPhone = utf8_decode($this->GetModuleVarInsecure("phone"));
			$strCompany = utf8_decode($this->GetModuleVarInsecure("company"));
			
			if ($this->GetModuleConfig("salutation_hide") == false)
			{
				if ($strSalutation == "mr" || $strSalutation == "mrs")
				{
					$strSalutation = $this->LoadString("TID_SALUTATION_".StringUpperCase($strSalutation));
				}
			}
			
			if ($strName == "")
			{
				$this->m_arrayErrors["NAME"] = "?TID_CONTACT_ERROR_NAME_MISSING?";	
			}

			if ($strText == "")
			{
				$this->m_arrayErrors["TEXT"] = "?TID_CONTACT_ERROR_TEXT_MISSING?";	
			}

			
			if (!IsValidEmail($strEmail))
			{
				$this->m_arrayErrors["EMAIL"] = "?TID_CONTACT_ERROR_EMAIL_INVALID?";	
				//$this->SetErrorMessageCode("EMAIL_INVALID");
				//return $this->StateForm();
			}
			
			if (ArrayCount($this->m_arrayErrors) > 0)
			{
				return $this->StateForm();
			}

			$strMessage = 
				$strText.
				"\n\n".   
				(($this->GetModuleConfig("hide_salutation") == false)?($strSalutation." "):("")).$strName."\n\n".
				(($this->GetModuleConfig("hide_company") == false)?("Company: ".$strCompany."\n"):("")).
				(($this->GetModuleConfig("hide_phone") == false)?("Phone: ".$strPhone."\n"):(""))
				;
			
			$strContactEmail = $this->GetModuleConfig("email");
			if (!IsValidEmail($strContactEmail))
			{
				$strContactEmail = $this->GetWebsite()->GetSiteTeamEmail();
			}
			
			SendMail( 
				$strContactEmail,
				ReplaceTagsWDK($this->LoadString("TID_CONTACT_MAIL_SUBJECT"),array("NAME" => $strName)),
				$strMessage,
				$strEmail,
				$strName);
			
			$this->SetModuleVar("salutation","");
			$this->SetModuleVar("name","");
			$this->SetModuleVar("company","");
			$this->SetModuleVar("phone","");
			$this->SetModuleVar("email","");
			$this->SetModuleVar("text","");

			$this->SetState("");
			
			$message = new CMessageElement($this->GetWebsite(),"info");
			$this->SetOutputItem("MESSAGE",
				$message->RenderMessage(
					"?TID_CONTACT_TEXT_SUBMIT?"));
			
			return "submit";
		}

	}

