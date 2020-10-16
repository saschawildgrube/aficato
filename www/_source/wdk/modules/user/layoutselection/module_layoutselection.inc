<?php



	class CLayoutSelectionModule extends CModule
	{
		protected $m_strContent;
	
		function __construct(
			CWebSite $website,
			$strContent)
		{ 
			$this->m_strContent = $strContent;
			$this->m_arrayLayouts = $website->GetConfig("layouts");
			parent::__construct($website,"user/layoutselection");
		}
		
		function CallbackInit()
		{
			//$this->GetWebsite()->LoadStringResources("local");
			return true;
		}
		
		function CallbackRenderModule($strState)
		{
			return $this->StateDefault();
		}
		
		
		function StateDefaultStyleLinklist()
		{
			$this->Trace("StateDefaultStyleLinklist");
			
			if (ArrayCount($this->m_arrayLayouts) == 0)
			{
				$this->SetOutputItem("LINKLIST","");
				return "default";	
			}

			
			$arrayLinks = array();
			
			$link = new CLinkElement($this->GetWebsite(),"default");
			
			foreach ($this->m_arrayLayouts as $strLayout)
			{
				$arrayParams = array();
				if ($this->m_strContent != "")
				{
					$arrayParams["content"] = $this->m_strContent;
				}
				$arrayParams["layout"] = $strLayout;
				$strLinkURL = $this->GetWebsite()->MakeURL($arrayParams);

				$strLink = $link->RenderLink($strLinkURL,"?TID_LAYOUT_".StringUpperCase($strLayout)."?");
				array_push($arrayLinks,$strLink);
				
			}
			
			$list = new CListElement($this->GetWebsite(),"default");			
			$strList = $list->RenderList($arrayLinks);
			$textelement = new CTextElement($this->GetWebsite(),"default");
			$strSelection = $textelement->RenderText("?TID_LAYOUTSELECTION_TITLE_LINKLIST?",$strList);
						
			$this->SetOutputItem("LINKLIST",$strSelection);
		           
			return "default";
		}
		
		/*
		function StateDefaultStyleDropdown()
		{ 
			$this->Trace("StateDefaultStyleDropdown");
			
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
		*/
		
		
		function StateDefault()
		{
			if ($this->GetStyle() == "linklist")
			{
				return $this->StateDefaultStyleLinklist();	
			}

			/*
			if ($this->GetStyle() == "dropdown")
			{
				return $this->StateDefaultStyleDropdown();	
			}
			*/
			$this->GetWebsite()->Error("Unsupported style specified");
		}




	}

