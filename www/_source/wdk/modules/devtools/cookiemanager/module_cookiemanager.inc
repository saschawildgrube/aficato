<?php

	require_once(GetWDKDir()."wdk_module.inc");
	require_once(GetWDKDir()."wdk_cookie.inc");
	
	class CCookieManagerModule extends CModule
	{
		
		private $m_arrayErrors;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"devtools/cookiemanager",
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->m_arrayErrors = array();
			
			$arrayToolbarConfig = array(
				"list" => array("help","refresh","set"),
				"help" => array("list","set"),				
				"set" => array("help","list"),
				"setsubmit" => array("help","list","set"),
				"unset" => array("help","list","set")
				);
			
			$this->SetToolbarConfig($arrayToolbarConfig);
			$this->SetOutputItem("MESSAGE","");
			
			return true;
		}
		
	
		function CallbackRenderModule($strState)
		{
			$this->Trace("CallbackRenderModule(\"$strState\")");
			switch ($strState)
			{
				case "help":
					return $this->StateHelp();
				case "set":
					return $this->StateSet();
				case "setsubmit":
					return $this->StateSetSubmit();
				case "unset":
					return $this->StateUnset();
				case "":
					$this->SetState("list");
				default:
					return $this->StateList();
			}
		}
		
	
		function RenderToolbar()
		{
			$toolbarDef = new CToolbarDefinition();

			if ($this->IsToolVisible("help"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "help";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "help", "?TID_COOKIEMANAGER_TOOL_HELP?");
			}


			if ($this->IsToolVisible("list"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "list";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "list", "?TID_COOKIEMANAGER_TOOL_LIST?");
			}

			if ($this->IsToolVisible("refresh"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "refresh";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "reload", "?TID_COOKIEMANAGER_TOOL_REFRESH?");
			}


			if ($this->IsToolVisible("set"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "set";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "add", "?TID_COOKIEMANAGER_TOOL_SET?");
			}


			$toolbar = new CToolbarElement($this->GetWebsite(),"default");
			$this->SetOutputItem("TOOLBAR",$toolbar->RenderToolbar($toolbarDef)); 
		}		
		
		function StateHelp()
		{
			$this->RenderToolBar();
			
			return "help";
		}		
		
		function StateList()
		{
			$this->SetState("list");
			$this->RenderToolBar();
			
			$this->IncludeElement("itemmanager");
			
			$itemmanagerdata = new CItemManagerData();
			
			/*
			$strFilterOptionTab = $this->GetWebsite()->GetVar("itemmanager_filteroptions_tab");
			if ($strFilterOptionTab == "")
			{
			   $strFilterOptionTab = "all";   
			}
			*/
			
			$linkicon = new CLinkIconElement($this->GetWebsite(),"default");

			$arrayCookies = $this->GetWebsite()->GetAllCookies();
			
			$arrayCookieTable = array();
			foreach ($arrayCookies as $strCookie => $strValue)
			{
				$arrayRow = array();
				$arrayRow["COOKIE"] = $strCookie;
				$arrayRow["VALUE"] = $strValue;
			
				$strTasks = "";
			
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "set";
				$arrayParams[$this->GetModuleVarName("cookie")] = $strCookie;
				$arrayParams[$this->GetModuleVarName("value")] = $strValue;
				$strTaskLink = $this->GetWebsite()->MakeURL($arrayParams);
		
				$strTasks .= $linkicon->RenderLinkIcon(
					$strTaskLink,
					"modify",
					"?TID_COOKIEMANAGER_TOOL_SET?");

				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "unset";
				$arrayParams[$this->GetModuleVarName("cookie")] = $strCookie;
				$strTaskLink = $this->GetWebsite()->MakeURL($arrayParams);
		
				$strTasks .= $linkicon->RenderLinkIcon(
					$strTaskLink,
					"delete",
					"?TID_COOKIEMANAGER_TOOL_UNSET?");

				
				$arrayRow["TASKS"] = $strTasks;
				
				$arrayCookieTable[] = $arrayRow;
			}
			$itemmanagerdata->SetItemList($arrayCookieTable);
			
			/*
			$arrayFilterOptionTabs = array(
			   "all" => "All",
			   "even" => "Even",
			   "uneven" => "Uneven"
			   );
			$itemmanagerdata->SetFilterOptionsTabNavigation(
			   $strFilterOptionTab,
			   "itemmanager_filteroptions_tab",
			   $arrayFilterOptionTabs);
			*/
			
			
			$itemmanagerdata->AddColumn("COOKIE","?TID_COOKIEMANAGER_HEADER_COOKIE?","",false);
			$itemmanagerdata->AddColumn("VALUE","?TID_COOKIEMANAGER_HEADER_VALUE?","",false);
			$itemmanagerdata->AddColumn("TASKS","?TID_TABLEHEADER_TASKS?","",false);
			 
			/*
			$arrayColumnTabs = array(
			   "primary" => "Primary",
			   "secondary" => "Secondary",
			   "tertiary" => "Tertiary",
			   "unused" => "Unused");
		 
			$itemmanagerdata->SetColumnsTabNavigation(
			   $this->GetWebsite()->GetVar("itemmanager_columns_tab"),
			   "itemmanager_columns_tab",
			   $arrayColumnTabs);
			*/			   
			
			/*
			$itemmanagerdata->SetSortOptions(
			   "itemmanager_sort_option",
			   $this->GetVar("itemmanager_sort_option"),
			   "itemmanager_sort_order",
			   $this->GetVar("itemmanager_sort_order")
			   );
			*/
			   
			/*
			$itemmanagerdata->SetPageNavigation(
			   $this->GetWebsite()->GetVar("itemmanager_offset"),
			   10,
			   "itemmanager_offset",
			   20);
			 */
			
			
			$itemmanager = new CItemManagerElement($this->GetWebsite(),"default");
			
			$this->SetOutputItem(
				"LIST",
				$itemmanager->RenderItemManager($itemmanagerdata));
			
			return "list";
		}
		
		function StateSet()
		{
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "setsubmit";		

			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$strCookie = $this->GetModuleVar("cookie");
			$strValue = $this->GetModuleVar("value");
			$nDuration = intval($this->GetModuleVar("duration"));
			if ($nDuration == 0)
			{
				$nDuration = 60;	
			}

			$formDef->AddControlStatic("STATIC1","?TID_COOKIEMANAGER_TEXT_SETCOOKIE?");
			$formDef->AddControlText("COOKIE",$this->GetModuleVarName("cookie"),"?TID_COOKIEMANAGER_LABEL_COOKIE?",$strCookie,50,255);
			$formDef->AddControlText("VALUE",$this->GetModuleVarName("value"),"?TID_COOKIEMANAGER_LABEL_VALUE?",$strValue,50,255);
			$formDef->AddControlText("DURATION",$this->GetModuleVarName("duration"),"?TID_COOKIEMANAGER_LABEL_DURATION?","$nDuration",5,5);
			$formDef->AddControlButtonSubmit("?TID_COOKIEMANAGER_BUTTON_SETCOOKIE?");
			
			$form = new CFormElement($this->GetWebsite(),"default");
			
			$this->SetOutputItem( 
				"FORM",
				$form->RenderForm(
					$formDef,
					array(),
					$this->m_arrayErrors));
					
			$this->RenderToolBar();
			
			return "set";
		}
		
		function StateSetSubmit()
		{
			$strCookie = $this->GetModuleVar("cookie");
			$strValue = $this->GetModuleVar("value");
			$nDuration = intval($this->GetModuleVar("duration"));
			if ($nDuration == 0)
			{
				$nDuration = 60;	
			}
			
			if ($strCookie == "")
			{
				$this->m_arrayErrors["COOKIE"] = "?TID_COOKIEMANAGER_ERROR_COOKIE_EMPTY?";	
			}
			else if (IsValidCookieName($strCookie) == false)
			{
				$this->m_arrayErrors["COOKIE"] = "?TID_COOKIEMANAGER_ERROR_COOKIE_INVALID?";					
			}
			
			
			if (ArrayCount($this->m_arrayErrors) > 0)			
			{
				return $this->StateSet();	
			}
			
			$this->GetWebsite()->SetCookie($strCookie,$strValue,60*$nDuration);
			
			$this->SetOutputItem("COOKIE",$strCookie);
			$message = new CMessageElement($this->GetWebsite(),"info");
			$this->SetOutputItem("MESSAGE",$message->RenderMessage("?TID_COOKIEMANAGER_INFO_SET_SUCCESS?"));
			
			$this->SetModuleVar("cookie","");
			$this->SetModuleVar("value","");
			


			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "list";		
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$formDef->AddControlStatic("STATIC","?TID_COOKIEMANAGER_TEXT_COOKIESET?");
			$formDef->AddControlButtonSubmit("?TID_BUTTON_NEXT?");
			
			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem("FORM",$form->RenderForm($formDef,array()));
			
			$this->RenderToolBar();
			
			return "setsubmit";
		}
		

		function StateUnset()
		{
			$strCookie = $this->GetModuleVar("cookie");
			
			if ($strCookie == "")
			{
				return $this->StateList();
			}
			
			
			$this->GetWebsite()->DeleteCookie($strCookie);
			
			$this->SetOutputItem("COOKIE",$strCookie);
			$message = new CMessageElement($this->GetWebsite(),"info");
			$this->SetOutputItem("MESSAGE",$message->RenderMessage("?TID_COOKIEMANAGER_INFO_UNSET_SUCCESS?"));
			
			$this->SetModuleVar("cookie","");
			$this->SetModuleVar("value","");



			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "list";		
			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$formDef->AddControlStatic("STATIC","?TID_COOKIEMANAGER_TEXT_COOKIEUNSET?");
			$formDef->AddControlButtonSubmit("?TID_BUTTON_NEXT?");
			
			$form = new CFormElement($this->GetWebsite(),"default");
			$this->SetOutputItem("FORM",$form->RenderForm($formDef,array()));
			
			$this->RenderToolBar();
			
			return "unset";
		}



	}

