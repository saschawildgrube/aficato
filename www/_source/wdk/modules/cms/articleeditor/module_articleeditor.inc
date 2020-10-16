<?php

	class CArticleEditorModule extends CModule
	{
		function __construct(
			$website,
			$strDefaultArticleName,
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"cms/articleeditor",
				$strDefaultArticleName,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			if ($this->GetState() == "")
			{				
				$this->SetState("view");	
			}

/*			
			$arrayToolbarConfig = array(
				"state1" => array("tool1"),
				"state2" => array("tool1","tool2")
				);
			
			$this->SetToolbarConfig($arrayToolbarConfig);
	*/
			
			return true;
		}

		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				/*
				case "edit":
				{
					return $this->StateEdit();
				}				
				case "edit_submit":
				{
					return $this->StateEditSubmit();
				}
				*/			
				case "view":
				{
					return $this->StateView();
				}				
				default:
				{
					return $this->StateView();
				}
			}
		}
		
		
		function PrepareTabNav($strState)
		{
			$this->IncludeElement("tabnavigation");
			$tabnavigationDef = new CTabNavigationDefinition();
			
			$arrayParams = array();
   		$arrayParams[$this->GetModuleVarNameState()] = "view";
   		$tabnavigationDef->AddTab(
      		"view",
      		"?TID_ARTICLEEDITOR_TAB_VIEW?",
      		$this->GetWebsite()->MakeURL($arrayParams));

/*
			$arrayParams = array();
   		$arrayParams[$this->GetModuleVarNameState()] = "approval";
   		$tabnavigationDef->AddTab(
      		"approval",
      		"?TID_ARTICLEEDITOR_TAB_APPROVAL?",
      		$this->GetWebsite()->MakeURL($arrayParams));

			$arrayParams = array();
   		$arrayParams[$this->GetModuleVarNameState()] = "edit";
   		$tabnavigationDef->AddTab(
      		"edit",
      		"?TID_ARTICLEEDITOR_TAB_EDIT?",
      		$this->GetWebsite()->MakeURL($arrayParams));

			$arrayParams = array();
   		$arrayParams[$this->GetModuleVarNameState()] = "rename";
   		$tabnavigationDef->AddTab(
      		"rename",
      		"?TID_ARTICLEEDITOR_TAB_RENAME?",
      		$this->GetWebsite()->MakeURL($arrayParams));

			$arrayParams = array();
   		$arrayParams[$this->GetModuleVarNameState()] = "history";
   		$tabnavigationDef->AddTab(
      		"history",
      		"?TID_ARTICLEEDITOR_TAB_HISTORY?",
      		$this->GetWebsite()->MakeURL($arrayParams));

			$arrayParams = array();
   		$arrayParams[$this->GetModuleVarNameState()] = "translationstatus";
   		$tabnavigationDef->AddTab(
      		"translationstatus",
      		"?TID_ARTICLEEDITOR_TAB_TRANSLATIONSTATUS?",
      		$this->GetWebsite()->MakeURL($arrayParams));
*/


			$tabnav = new CTabNavigationElement(
			   $this->GetWebsite(),
   			"default");

			$this->SetOutputItem("TABNAV",
				$tabnav->RenderTabNavigation(
					$tabnavigationDef,
					$strState));	
		}		
		
		

/*
		function RenderToolbar()
		{
			$toolbarDef = new CToolbarDefinition();


			if ($this->IsToolVisible("tool1"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "state2";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "url", "?TID_XXX_TOOL_TOOL1?");
			}


			if ($this->IsToolVisible("tool2"))
			{
			
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "state1";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "url", "?TID_XXX_TOOL_TOOL2?");
			}

			
			
			
			$toolbar = new CToolbarElement($this->GetWebsite(),"default");
			$this->SetOutputItem("TOOLBAR",$toolbar->RenderToolbar($toolbarDef)); 
		}
		*/
		
		function GetArticleName()
		{
			$strArticleName = $this->GetConfig("articlename");
			if ($strArticleName == "")
			{
				$strArticleName = $this->GetModuleVar("article");
			}
			if ($strArticleName == "")
			{
				$strArticleName = $this->GetInstanceID();
			}
			if ($strArticleName == "")
			{
				return false;	  
			}
			return $strArticleName;
		}
		
		
		function GetArticle($strArticleName,$strLanguage,$strCountry,$strType)
		{
			$arrayArticle = array();
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());
			$arrayParams = array();
			$arrayParams["command"] = "get";
			$arrayParams["name"] = $strArticleName;
			$arrayParams["language"] = $strLanguage;
			$arrayParams["country"] = $strCountry;
			$arrayParams["type"] = $strType;
			$arrayParams["update_lastaccess"] = "1";
			$webservice->ConsumeWebService("cms/article",$arrayParams);
			if ($webservice->GetError()!="")
			{
				return false;
			}
			$arrayArticle["textformat"] = $webservice->GetResultValueInsecure("ARTICLE","TEXTFORMAT");
			$arrayArticle["title"] = $webservice->GetResultValueInsecure("ARTICLE","TITLE");
			$arrayArticle["content"] = $webservice->GetResultValueInsecure("ARTICLE","CONTENT");
			return $arrayArticle;
		}
		
		function StateView()
		{
			$this->SetState("view");
			
			$this->PrepareTabNav("view");
			$this->SetOutputItem("HEADER","");			
			$this->SetOutputItem("TITLE","");
			$this->SetOutputItem("CONTENT","");
			$this->SetOutputItem("FOOTER","");			

			
			$strArticleName = $this->GetArticleName();
			$strType = "STATIC";
			
			if ($strArticleName == false)
			{
				$this->AddWarningMessageCode("ARTICLE-NOT-SPECIFIED");
				return "view";
			}
			
			$strContent = "";
			$strFormat = "";
			
			$arrayArticle = $this->GetArticle(
				$strArticleName,
				$this->GetWebsite()->GetLanguage(),
				$this->GetWebsite()->GetCountry(),
				$strType,
				$strContent,
				$strFormat); 
				
			if ($arrayArticle == false)
			{
				$this->AddWarningMessageCode("ARTICLE-NOT-FOUND");
				return "view";
			}
			
			$this->SetOutputItem("TITLE",ArrayGetValue($arrayArticle,"title"));
			
			$this->IncludeElement("article");
			$article = new CArticleElement($this->GetWebsite(),"default");
			
			$strRenderedArticle = $article->RenderArticle(
				ArrayGetValue($arrayArticle,"content"),
				ArrayGetValue($arrayArticle,"textformat"));
			$this->SetOutputItem("CONTENT",$strRenderedArticle);
			
		
			return "view";
		}


		function StateEdit()
		{
			$this->SetState("edit");
			
			
	
	/*
			// TOOLBAR
			$this->RenderToolbar();

			// FORM
			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "state1-submit";
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);
			
			$formDef->AddControlText("VALUE1",$this->GetModuleVarName("value1"),"Value 1",$this->GetModuleVar("value1"),40,40);
			$formDef->AddControlTextArea("VALUE2",$this->GetModuleVarName("value2"),"Value 2",$this->GetModuleVar("value2"),50,10);
			
			$formDef->AddControlButtonSubmit("Submit");
			
			$form = new CFormElement($this->GetWebsite(),"default");
			
			$this->SetOutputItem("FORM",$form->RenderForm($formDef,array()));
		*/
			
			return "edit";
		}
		
		function StateEditSubmit()
		{
			$this->SetState("edit-submit");
			
			/*
			$strValue1 = $this->GetModuleVar("value1");
			$strValue2 = $this->GetModuleVar("value2");
			
			
	
			$this->SetModuleVar("value1","");
			$this->SetModuleVar("value2","");
			*/
			
			return "edit-submit";
		}

	
		
	}


	class CViewerArticleEditorModule extends CArticleEditorModule
	{
		function __construct(
			$website,
			$strDefaultArticleName = "")
		{
			$arrayConfig["readonly"] = true;
			parent::__construct(
				$website,
				$strDefaultArticleName,
				$arrayConfig);
		}
	}


	class CWikiArticleEditorModule extends CArticleEditorModule
	{
		function __construct(
			$website,
			$strDefaultArticleName = "",
			$arrayConfig = array())
		{
			$arrayConfig["format"] = "WIKI";
			$arrayConfig["type"] = "STATIC";
			
			// set config to enable all wiki typical functions
			
			parent::__construct(
				$website,
				$strDefaultArticleName,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			
			return true;
		}
	}

