<?php



	class CFeedbackModule extends CModule
	{
		private $m_formDef;
	
		function __construct(
			CWebSite $website)
		{
			parent::__construct(
				$website,
				"crm/feedback");
		}
		
		function CallbackInit()
		{
			return true;
		}
		
		

		


		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "confirmation":
				{
					return $this->StateConfirmation();	
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
			$arrayParams[$this->GetModuleVarName("state")] = "confirmation";		
			$this->m_formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$strName = $this->GetModuleVar("name");
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
			 

			$this->m_formDef->AddControlText("NAME",$this->GetModuleVarName("name"),"?TID_FEEDBACK_LABEL_NAME?",$strName,50,255);
			$this->m_formDef->AddControlText("EMAIL",$this->GetModuleVarName("email"),"?TID_FEEDBACK_LABEL_EMAIL?",$strEmail,50,255);
			$this->m_formDef->AddControlTextArea("TEXT",$this->GetModuleVarName("text"),"?TID_FEEDBACK_LABEL_FEEDBACK?",$this->GetModuleVar("text"),50,7);
			$this->m_formDef->AddControlButtonSubmit("?TID_FEEDBACK_BUTTON_SEND?");
	

			
			$form = new CFormElement($this->GetWebsite(),"default");
			
			$this->SetOutputItem( 
				"FORM",
				$form->RenderForm(
					$this->m_formDef,
					array()));
	
			return "form";
		}

		function StateConfirmation()
		{
			$this->Trace("StateConformation");
			
			// Check if all values are set
			
			$strName = $this->GetModuleVar("name");
			$strEmail = $this->GetModuleVar("email");
			$strText = utf8_decode($this->GetModuleVarInsecure("text"));
			
			if ($strName == "")
			{
				$this->SetErrorMessageCode("NAME_MISSING");
				return $this->StateForm();
			}
			
			if (!IsValidEmail($strEmail))
			{
				$this->SetErrorMessageCode("EMAIL_INVALID");
				return $this->StateForm();
			}

			if (StringLength($strText) < 100)
			{
				$this->SetErrorMessageCode("TEXT_SHORT");
				return $this->StateForm();
			}
			
			SendMail(
				$this->GetWebsite()->GetSiteTeamEmail(),
				"FEEDBACK on ".$this->GetWebsite()->GetSiteName()." from $strName",
				$strText,
				$strEmail,
				$strName);
			
			$this->SetModuleVar("name","");
			$this->SetModuleVar("email","");
			$this->SetModuleVar("text","");

			$this->SetState("");
			
			return "confirmation";
		}

	}

