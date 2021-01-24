<?php

	require_once(GetWDKDir()."wdk_module.inc");
	
	class CNewsletterModule extends CModule
	{
		private $m_arrayErrors;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			$this->m_arrayErrors = array();
			parent::__construct(
				$website,
				"crm/newsletter",
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnRenderModule($strState)
		{
			$this->Trace("OnRenderModule(\"$strState\")");
			switch ($strState)
			{
				case "submit":
					return $this->StateSubmit();
				case "":
					$this->SetState("start");
				default:
					return $this->StateStart();
			}
		}
		
	
		
		function StateStart()
		{
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "submit";		

			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$strEmail = $this->GetModuleVar("email");

			$formDef->AddControlStatic("STATIC1","?TID_NEWSLETTER_TEXT_SUBSCRIBE?");
			$formDef->AddControlText("EMAIL",$this->GetModuleVarName("email"),"?TID_NEWSLETTER_LABEL_EMAIL?",$strEmail,50,255);
			$formDef->AddControlButtonSubmit("?TID_NEWSLETTER_BUTTON_SUBMIT?");
			
			$form = new CFormElement($this->GetWebsite(),"default");
			
			$this->SetOutputItem( 
				"FORM",
				$form->RenderForm(
					$formDef,
					array(),					
					$this->m_arrayErrors));
					
			
			return "start";
		}
		
		
		function StateSubmit()
		{
			$strEmail = $this->GetModuleVar("email");
			if (IsValidEmail($strEmail) == false)
			{
				$this->m_arrayErrors["EMAIL"] = "?TID_NEWSLETTER_ERROR_EMAIL_INVALID?";	
			}
			
			if (ArrayCount($this->m_arrayErrors) > 0)			
			{
				return $this->StateStart();	
			}
			
			$this->SetOutputItem("EMAIL",$strEmail);
			
			$message = new CMessageElement($this->GetWebsite(),"info");
			$this->SetOutputItem("MESSAGE",$message->RenderMessage("?TID_NEWSLETTER_INFO_SUCCESS?"));
			
			$this->SetModuleVar("email","");
			
			SendMail(
				$this->GetWebsite()->GetSiteTeamEmail(),
				$this->GetWebsite()->GetSiteName()." NEWSLETTER REGISTRATION: ".$strEmail,
				$strEmail,
				$strEmail,
				"");
			
			$this->SetState("start");
			
			return "submit";
		}
		


	}

