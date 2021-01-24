<?php



	class CWikiPadModule extends CModule
	{
		function __construct(
			CWebSite $website,
			$strWikiPageID = "default",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"cms/wikipad",
				$strWikiPageID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$bResult = parent::OnInit();
			$this->IncludeElement("wiki");
			$this->IncludeElement("tabnavigation");
			
			if ($this->GetConfigIsUserSpecific() == true)
			{
				if ($this->GetWebsite()->GetAuthUserID() == "")
				{
					return false;	
				}
			}
			
			return $bResult;
		}
		
		

		function OnRenderModule($strState)
		{
			if ($this->IsViewOnly() == true)
			{
				$strState = "view";	
			}
			
			
			switch ($strState)
			{
				case "submit":
				{
					return $this->StateSubmit();	
				}
				case "edit":
				{
					return $this->StateEdit();	
				}
				case "view":
				default:	
				{
					return $this->StateView();
				}
			}
		}

		function IsViewOnly()
		{
			return $this->GetModuleConfigBoolean("viewonly");	
		}
		
		function PrepareTabNav($strState)
		{
			if ($this->IsViewOnly() == true)
			{
				$this->SetOutputItem("TABNAV","");
				return;
			}
			
			$tabnavigationDef = new CTabNavigationDefinition();

			$arrayParams = array();
   		$arrayParams[$this->GetModuleVarNameState()] = "view";
   		$tabnavigationDef->AddTab(
      		"view",
      		"?TID_WIKIPAD_TAB_VIEW?",
      		$this->GetWebsite()->MakeURL($arrayParams));

			$arrayParams = array();
   		$arrayParams[$this->GetModuleVarNameState()] = "edit";
   		$tabnavigationDef->AddTab(
      		"edit",
      		"?TID_WIKIPAD_TAB_EDIT?",
      		$this->GetWebsite()->MakeURL($arrayParams));

			$tabnav = new CTabNavigationElement(
			   $this->GetWebsite(),
   			"default");

			$this->SetOutputItem("TABNAV",
				$tabnav->RenderTabNavigation(
					$tabnavigationDef,
					$strState));	
		}
		
		
		function StateView()
		{
			$this->Trace("StateView");
			
			$this->PrepareTabNav("view");


			$strContent = $this->GetData($this->GetDataName());
			if ($strContent === false)
			{
				$this->SetErrorMessageCode("WIKIPAD_GET");
				$this->SetOutputItem("WIKI","");
				$strContent = "";
			}

			$wiki = new CWikiElement($this->GetWebSite(),"default");
			$this->SetOutputItem(
				"WIKI",
				$wiki->RenderWiki(
					$strContent));
	
			return "view";
		}
		
		
		function StateEdit()
		{
			$this->Trace("StateEdit");

			$this->m_formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "submit";
			$this->m_formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$strContent = $this->GetModuleVarInsecure("content");
			if ($strContent == "")
			{
				$strContent = $this->GetData($this->GetDataName());
				if ($strContent == false)
				{
					$strContent = "";
				}
			}

			$nLines = intval($this->GetModuleConfig("lines"));
			if ($nLines == 0)
			{
				$nLines = 30;
			}
			$nColumns = $this->GetModuleConfig("columns");
			if ($nColumns == 0)
			{
				$nColumns = 70;
			}

			$strContent = ReplaceString($strContent,"<code>","<CODE>");
			$strContent = ReplaceString($strContent,"</code>","</CODE>");

			$bShowSubmitButtonSaveAndStay = $this->GetModuleConfig("showsubmitbuttonsaveandstay");  

			$bShowSubmitButtonOnTop = $this->GetModuleConfig("showsubmitbuttonontop");
			if ($bShowSubmitButtonOnTop == true)
			{
				$this->m_formDef->AddControlButtonSubmit("?TID_WIKIPAD_BUTTON_SAVE?");
				if ($bShowSubmitButtonSaveAndStay == true)
				{
					$this->m_formDef->AddControlButtonSubmit("?TID_WIKIPAD_BUTTON_SAVE_AND_STAY?","SUBMIT-STAY",'stay');
				}
			}
			$this->m_formDef->AddControlTextArea("CONTENT",$this->GetModuleVarName("content"),"",$strContent,$nColumns,$nLines); 
			$this->m_formDef->AddControlButtonSubmit("?TID_WIKIPAD_BUTTON_SAVE?","SUBMIT-2");
			if ($bShowSubmitButtonSaveAndStay == true)
			{
				$this->m_formDef->AddControlButtonSubmit("?TID_WIKIPAD_BUTTON_SAVE_AND_STAY?","SUBMIT-STAY-2",'stay');
			}
	
			$form = new CFormElement($this->GetWebsite(),"onecolumn");
			
			$this->SetOutputItem(
				"FORM",
				$form->RenderForm(
					$this->m_formDef,
					array()));
					
			$this->PrepareTabNav("edit");
	
			return "edit";
		}

		function StateSubmit()
		{
			$this->Trace("StateSubmit");
			
			$this->Trace($this->GetAllModuleVars());
			
			$strContent = $this->GetModuleVarInsecure("content");
			
			$strContent = ReplaceString($strContent,"<CODE>","<code>");
			$strContent = ReplaceString($strContent,"</CODE>","</code>");

			
			$this->SetState("");
		
			if ($this->SetData($this->GetDataName(),$strContent) == false)
			{
				$this->SetErrorMessageCode("WIKIPAD_SET");
				return $this->StateEdit();
			}
			$this->SetModuleVar("content","");
			
			$strSubmitValue = $this->GetVar("submit");
			
			if ($strSubmitValue == 'stay')
			{
				return $this->StateEdit();	
			}
			
			
			return $this->StateView();
		}
		
		
		function GetConfigIsUserSpecific()
		{
			return $this->GetModuleConfigBoolean("userspecific");	
		}
		
		function GetDataName()
		{
			return "wikipad_".$this->GetInstanceID();	
		}



		function GetData($strDataName)
		{
			$strDefaultContent = $this->GetModuleConfig("defaultcontent");
			if ($this->GetConfigIsUserSpecific())
			{
				return $this->GetWebsite()->GetAuthUserData($strDataName,$strDefaultContent);
			}
			else
			{
				return $this->GetWebsite()->GetData('','',$strDataName,$strDefaultContent);
			}
		}

		function SetData($strDataName,$strContent)
		{
			if ($this->GetConfigIsUserSpecific())
			{
				return $this->GetWebsite()->SetAuthUserData($strDataName,$strContent);
			}
			else
			{
				return $this->GetWebsite()->SetData('','',$strDataName,$strContent);
			}
			
		}

	}

