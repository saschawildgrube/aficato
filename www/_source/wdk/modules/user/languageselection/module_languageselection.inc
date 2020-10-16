<?php



	class CLanguageSelectionModule extends CModule
	{
		protected $m_strContent;
	
		function __construct(
			CWebSite $website,
			$strContent)
		{ 
			$this->m_strContent = $strContent;
			parent::__construct($website,"user/languageselection");
		}
		
		function CallbackInit()
		{
			$this->GetWebsite()->LoadStringResources("local");
			return true;
		}
		
		function CallbackRenderModule($strState)
		{
			return $this->StateDefault();
		}
		
		
		function StateDefaultStyleToolbar()
		{
			$this->Trace("StateDefaultStyleToolbar");
			
			$arrayLanguages = $this->GetWebsite()->GetConfig("languages");
			$arrayLanguageLinks = array();
			
			$linkicon = new CLinkIconElement($this->GetWebsite(),"default");
			$strToolBar = "";
			
			foreach ($arrayLanguages as $strLanguage)
			{
				if ($this->m_strContent != "")
				{
					$arrayParams = array();
					$arrayParams["content"] = $this->m_strContent;
					$arrayParams["language"] = $strLanguage;
					$strLinkURL = $this->GetWebsite()->MakeURL($arrayParams);
					
				}
				else
				{
					$strLinkURL = $this->GetWebsite()->MakeLanguageURL($strLanguage);	
				}
				
				$strToolBar .= $linkicon->RenderLinkIcon($strLinkURL,"language-".StringLowerCase($strLanguage),"?TID_LANGUAGE_".StringUpperCase($strLanguage)."?");
			}
			$this->SetOutputItem("TOOLBAR",$strToolBar);

			return "default";
		}

		function StateDefaultStyleLinklist()
		{
			$this->Trace("StateDefaultStyleLinklist");
			
			$arrayLanguages = $this->GetWebsite()->GetConfig("languages");
			$arrayLanguageLinks = array();
			
			$link = new CLinkElement($this->GetWebsite(),"default");
			
			foreach ($arrayLanguages as $strLanguage)
			{
				if ($this->m_strContent != "")
				{
					$arrayParams = array();
					$arrayParams["content"] = $this->m_strContent;
					$arrayParams["language"] = $strLanguage;
					$strLinkURL = $this->GetWebsite()->MakeURL($arrayParams);
					
				}
				else
				{
					$strLinkURL = $this->GetWebsite()->MakeLanguageURL($strLanguage);	
				}

				$strLanguageLink = $link->RenderLink($strLinkURL,"?TID_LANGUAGE_".StringUpperCase($strLanguage)."?");
				array_push($arrayLanguageLinks,$strLanguageLink);
				
			}
			
			$this->IncludeElement("text");
			
			$list = new CListElement($this->GetWebsite(),"default");			
			$strLanguageList = $list->RenderList($arrayLanguageLinks);
			$textelement = new CTextElement($this->GetWebsite(),"default");
			$strLanguageSelection = $textelement->RenderText("?TID_LANGUAGESELECTION_TITLE_LINKLIST?",$strLanguageList);
						
			$this->SetOutputItem("LINKLIST",$strLanguageSelection);
		           
			return "default";
		}
		
		function StateDefaultStyleDropdown()
		{ 
			$this->Trace("StateDefaultStyleDropdown");
			
			$arrayLanguages = $this->GetWebsite()->GetConfig("languages");

			$arrayVars =  $this->GetWebsite()->GetSessionAwareArrayVars();
			if ($this->m_strContent != "")
			{
				$arrayVars["content"] = $this->m_strContent;
			} 

			$formDef = new CFormDefinition();
			$formDef->SetURL( $this->GetWebsite()->GetRootURL(),$arrayVars);
			$arrayOptions = array();
			foreach ($arrayLanguages as $strLanguage)
			{
				$arrayOptions[$strLanguage] = "?TID_LANGUAGE_".StringUpperCase($strLanguage)."?";
			}
			$formDef->AddControlSelect("LANGUAGE","language","xxx",$arrayOptions, $this->GetWebsite()->GetLanguage());
			
			$formDef->AddControlButtonSubmit(">>");

			$form = new CFormTemplateElement($this->GetWebsite(),"default");
			$this->SetOutputItem("DROPDOWN",
				$form->RenderFormTemplate($formDef,array(),array(),"languageselectiondropdown"));
		
			return "default";
		}
		
		
		function StateDefault()
		{
			if ($this->GetStyle() == "toolbar")
			{
				return $this->StateDefaultStyleToolbar();	
			}
			if ($this->GetStyle() == "linklist")
			{
				return $this->StateDefaultStyleLinklist();	
			}
			if ($this->GetStyle() == "dropdown")
			{
				return $this->StateDefaultStyleDropdown();	
			}
			$this->GetWebsite()->Error("Unsupported style specified");
		}




	}

