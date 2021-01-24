<?php

	class CDisclaimerModule extends CModule
	{
		private $m_formDef;
		
		function __construct(
			CWebSite $website)
		{
		
			parent::__construct(
				$website,
				"compliance/disclaimer");
		}
		
		function OnInit()
		{
			$bResult = parent::OnInit();
			$this->m_formDef = new CFormDefinition();
			
			$arrayParams = $this->GetWebsite()->GetSessionAwareArrayVars();
			$arrayParams["disclaimer_state"] = "confirmation";
			
			$this->m_formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			$this->m_formDef->AddControlCheckbox("DISCLAIMER_ACCEPTED","disclaimer_accepted","?TID_CHECKBOX_DISCLAIMER?",false);
			$this->m_formDef->AddControlButtonSubmit("?TID_BUTTON_NEXT?");

			$strState = $this->GetState();
			if ($strState == "confirmation")
			{
				if ($this->GetModuleVar("accepted") != "1")
				{
					$this->SetErrorMessageCode("DISCLAIMER_NOT_ACCEPTED");
				}
			}
			
			if ($this->GetWebsite()->GetVar("disclaimerconfirmed") != "1")
			{
				if ($this->GetModuleVar("accepted") == "1")
				{
					$this->GetWebsite()->SetVar("disclaimerconfirmed","1");
					$this->SetModuleVar("accepted","");
					return false;	
				}
				return $bResult;	
			}

			return false;	
		}

		function OnRenderModule($strState)
		{
			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem("FORM", $form->RenderForm($this->m_formDef,array()));
			return "default";
		}
	}

