<?php

	class CConsentModule extends CModule
	{
		private $m_formDef;
		
		function __construct(
			CWebSite $website)
		{
		
			parent::__construct(
				$website,
				'compliance/consent');
		}
		
		function OnInit()
		{
			$bResult = parent::OnInit();
			$this->m_formDef = new CFormDefinition();
			
			$arrayParams = $this->GetWebsite()->GetSessionAwareArrayVars();
			$arrayParams["consent_state"] = 'confirmation';
			
			$this->m_formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			$this->m_formDef->AddControlStatic('TEXT','?TID_TEXT_CONSENT?');
			$this->m_formDef->AddControlCheckbox('CONSENT','consent','?TID_CHECKBOX_CONSENT?',false);
			$this->m_formDef->AddControlButtonSubmit('?TID_BUTTON_NEXT?');

			$strState = $this->GetState();
			if ($strState == 'confirmation')
			{
				if ($this->GetModuleVar('consent') != '1')
				{
					$this->SetErrorMessageCode('NO_CONSENT');
				}
			}
			
			if ($this->GetWebsite()->GetVar('consent') != '1')
			{
				if ($this->GetModuleVar('consent') == '1')
				{
					$this->GetWebsite()->SetVar('consent','1');
					$this->SetModuleVar('consent','');
					return false;	
				}
				return $bResult;	
			}

			return false;	
		}

		function OnRenderModule($strState)
		{
			$form = new CFormElement($this->GetWebsite(),'default');
			$this->SetOutputItem('FORM', $form->RenderForm($this->m_formDef,array()));
			return 'default';
		}
	}

