<?php
	
	require_once(GetWDKDir()."wdk_quality.inc");
	require_once(GetWDKDir()."wdk_module_entitymanager.inc");
	require_once(GetWDKDir()."wdk_entity_test.inc");
	

	
	class CTestManagerModule extends CEntityManagerModule
	{
	
		private $m_format_testpassed;
		private $m_format_testinactive;
		private $m_format_testfailed;
		private $m_format_testundefined;
		private $m_format_testunknown;
		private $m_format_testintest;
		
		private $m_bDatabaseSupport;

	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"system/testmanager",
				$strInstanceID,
				$arrayConfig);

				
		}
		
		function CallbackInit()
		{
			$this->IncludeElement("tabnavigation");
			$this->IncludeElement("format");
			$this->IncludeElement("console");
			$this->IncludeElement("itemmanager");
			
			$this->m_format_testpassed = 		new CFormatElement($this->GetWebsite(),"testpassed");
			$this->m_format_testinactive = 	new CFormatElement($this->GetWebsite(),"testinactive");
			$this->m_format_testfailed = 		new CFormatElement($this->GetWebsite(),"testfailed");
			$this->m_format_testundefined = 	new CFormatElement($this->GetWebsite(),"testundefined");
			$this->m_format_testunknown = 	new CFormatElement($this->GetWebsite(),"testunknown");
			$this->m_format_testintest = 		$this->m_format_testundefined;
			
			$this->m_strWebservice = "system/test";
			
			$this->m_bDatabaseSupport = false;
			
			$arrayEntityDefinitions = array(
				"test" => GetEntityDefinitionTest()
			);							
			
				
			$this->SetEntityDefinitions($arrayEntityDefinitions);

			
			return true;
		}
		
		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "runtests":
				{
					return $this->StateRunTests();	
				}
				case "runtestscontinue":
				{
					return $this->StateRunTestsContinue();	
				}
				case "test":
				{
					return $this->StateTest();	
				}
				case "testdetails":
				{
					return $this->StateTestDetails();	
				}
				default:
				{
					return $this->StateTestList();	
				}
			}
		}
		
		function CallbackPrepareToolbar()
		{


			if ($this->GetState() != "testlist")
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "testlist";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$this->AddTool(
					$strLink,
					"list",
					$this->LoadString("TID_TEST_TOOL_TESTLIST"));
			}


/*		
			$arrayParams = array();
			$arrayParams["command"] = "testall";
			$strAccessCode = $this->GetConfig("webservices",$this->m_strWebservice,"accesscode");
			if ($strAccessCode != "")
			{
				$arrayParams["accesscode"] = $strAccessCode;
			}
			$strLink = MakeURL($this->GetWebsite()->GetWebserviceURL($this->m_strWebservice),$arrayParams);
			$this->AddTool(
				$strLink,
				"execute",
				$this->LoadStringEntity("TEST","TOOL","TESTALL"),
				true);

			$arrayParams = array();
			$arrayParams["command"] = "testlocal";
			$strAccessCode = $this->GetConfig("webservices",$this->m_strWebservice,"accesscode");
			if ($strAccessCode != "")
			{
				$arrayParams["accesscode"] = $strAccessCode;
			}
			$strLink = MakeURL($this->GetWebsite()->GetWebserviceURL($this->m_strWebservice),$arrayParams);
			$this->AddTool(
				$strLink,
				"execute",
				$this->LoadStringEntity("TEST","TOOL","TESTLOCAL"),
				true);


			$arrayParams = array();
			$arrayParams["command"] = "testlist";
			$strAccessCode = $this->GetConfig("webservices",$this->m_strWebservice,"accesscode");
			if ($strAccessCode != "")
			{
				$arrayParams["accesscode"] = $strAccessCode;
			}
			$arrayParams["dotest"] = "1";
			$arrayParams["test_filter1"] = "lastexecution_status";
			$arrayParams["test_filter1_operator"] = "in";
			$arrayParams["test_filter1_value"] = "FAILED,UNDEFINED,REQUESTERROR";
			$arrayParams["test_filter2"] = "group_id";
			$arrayParams["test_filter2_operator"] = "!=";
			$arrayParams["test_filter2_value"] = "test";			  
			$strLink = MakeURL($this->GetWebsite()->GetWebserviceURL($this->m_strWebservice),$arrayParams);
			$this->AddTool(
				$strLink,
				"execute",
				$this->LoadStringEntity("TEST","TOOL","TESTFAILED"),
				true);
*/
			
			if ($this->m_bDatabaseSupport == true)
			{
			
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "runtests";
				$arrayParams[$this->GetModuleVarName("mode")] = "failed";
				$strLink = $this->MakeURL($arrayParams,$arrayParams);
				$this->AddTool(
					$strLink,
					"fast-forward",
					$this->LoadStringEntity("TEST","TOOL","TESTFAILED"));			
					
				
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "runtests";
				$arrayParams[$this->GetModuleVarName("mode")] = "all";
				$strLink = $this->MakeURL($arrayParams,$arrayParams);
				$this->AddTool(
					$strLink,
					"forward",
					$this->LoadStringEntity("TEST","TOOL","TESTALL"));					
	
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "runtests";
				$arrayParams[$this->GetModuleVarName("mode")] = "local";
				$strLink = $this->MakeURL($arrayParams,$arrayParams);
				$this->AddTool(
					$strLink,
					"forward",
					$this->LoadStringEntity("TEST","TOOL","TESTLOCAL"));					
			}


			if ($this->GetState() == "testdetails")
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "test";
				$arrayParams[$this->GetModuleVarName("test_path")] = $this->GetModuleVar("test_path");
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$this->AddTool(
					$strLink,
					"execute",
					$this->LoadStringEntity("TEST","TOOL","TEST"));
			} 

			return;
		}
		 
		
		function RenderTestStatus($strStatus)
		{
			$strOutput = "";
			$strStatusText = $this->LoadStringEntity("TEST","STATUS",StringUpperCase($strStatus));
			switch ($strStatus)
			{
			case "PASSED":
				$strOutput = $this->m_format_testpassed->RenderFormat($strStatusText);
				break;	
			case "INACTIVE":
				$strOutput = $this->m_format_testinactive->RenderFormat($strStatusText);
				break;
			case "FAILED":
				$strOutput = $this->m_format_testfailed->RenderFormat($strStatusText);
				break;	
			case "UNDEFINED":
				$strOutput = $this->m_format_testundefined->RenderFormat($strStatusText);
				break;
			case "REQUESTERROR":
				$strOutput = $this->m_format_testfailed->RenderFormat($strStatusText);
				break;
			case "UNTESTED":
				$strOutput = $this->m_format_testunknown->RenderFormat($strStatusText);
				break;
			case "INTEST":
				$strOutput = $this->m_format_testintest->RenderFormat($strStatusText);
				break;
			default:
				$strOutput = $this->m_format_testunknown->RenderFormat($this->LoadStringEntity("TEST","STATUS","UNKNOWN"));
				break;
			}
			return $strOutput; 
		}

		
		function StateTest()
		{
			$this->Trace("StateTest");
			$this->SetState("test");			
			
			$strTestPath = $this->GetModuleVar("test_path");
			$arrayTestPath = ParseTestPath($strTestPath);
			if ($arrayTestPath == false)
			{
				return $this->StateTestList();	
			}
			
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			
			$arrayParams = array();
			$arrayParams["test_path"] = $strTestPath;
			$arrayParams["command"] = "test";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->m_bDatabaseSupport = $consumer->GetResultValue("DATABASE_SUPPORT");
			
			$strError = $consumer->GetError();
			
			if ($strError == "TEST_NOT_FOUND")
			{
				return $this->StateTestList();
			}
			if ($strError == "TEST_REQUEST_FAILED")	
			{
				$this->SetErrorMessageCode($strError);
			}
			else if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
				return "error";
			}
			
			if ($this->GetPreviousState() == "testlist" || $consumer->GetResultValue("DATABASE_SUPPORT") == false)     
			{
				$this->SetOutputItem("TESTID",$strTestPath);
				$strStatus = $consumer->GetResultValue("LASTEXECUTION_STATUS");
				$this->SetOutputItem("TESTSTATUS",$this->RenderTestStatus($strStatus));
				$this->SetInfoMessageCodeEntity("test","TESTEXECUTED");
			}
			
			if ($this->GetPreviousState() == "testlist")     
			{
				return $this->StateTestList();
			}
			else
			{
				return $this->StateTestDetails();
			}
		}
		


		function StateTestDetails()
		{
			$this->Trace("StateTestDetails");
			$this->SetState("testdetails");			
			
			$strTestPath = $this->GetModuleVar("test_path");
			$arrayTestPath = ParseTestPath($strTestPath);
			if ($arrayTestPath == false)
			{
				return $this->StateTestList();	
			}
			
			$arrayItems = array();
		
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			
			$arrayParams = array();
			$arrayParams["test_path"] = $strTestPath;
			$arrayParams["command"] = "gettest";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);

			$this->m_bDatabaseSupport = $consumer->GetResultValue("DATABASE_SUPPORT");			
			$strError = $consumer->GetError();
			
			if ($strError == "TEST_NOT_FOUND")
			{
				return $this->StateTestList();
			}
			if ($strError == "TEST_REQUEST_FAILED")	
			{
				$this->SetErrorMessageCode($strError);
			}
			else if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
				return "error";
			}
			
			
			$arrayTableBaseData = array();
			
			$arrayTableBaseData[] = array(
				$this->LoadStringEntity("TEST","LABEL","TEST_PATH"),
				$consumer->GetResultValue("TEST_PATH")
				);
				
			$link = new CLinkElement($this->GetWebsite(),"default");
			$arrayTableBaseData[] = array(
				$this->LoadStringEntity("TEST","LABEL","URL"),
				$link->RenderLink(
					$consumer->GetResultValue("URL"),
					$consumer->GetResultValue("URL"),
					true)
				);
				
			
			if ($this->m_bDatabaseSupport == true)
			{
			
				$strStatus = "";
				if ($strError == "")
				{
					$strStatus = $this->RenderTestStatus($consumer->GetResultValue("LASTEXECUTION_STATUS"));
				}
				$arrayTableBaseData[] = array(
					"?TID_TEST_LABEL_LASTEXECUTION_STATUS?",
					$strStatus
					);
	
				$arrayTableBaseData[] = array(
					"?TID_TEST_LABEL_LASTEXECUTION_ATTEMPTS?",
					$consumer->GetResultValue("LASTEXECUTION_ATTEMPTS")
					);
	
	
				$arrayTableBaseData[] = array(
					"?TID_TEST_LABEL_LASTEXECUTION_DATETIME?",
					$this->GetWebsite()->RenderDateTime($consumer->GetResultValue("LASTEXECUTION_DATETIME"))
					);
	
				$strRuntime = $consumer->GetResultValue("LASTEXECUTION_RUNTIME_SECONDS");
				if (IsFloatString($strRuntime))
				{
					$arrayTableBaseData[] = array(
						"?TID_TEST_LABEL_LASTEXECUTION_RUNTIME_SECONDS?",
						$this->GetWebsite()->RenderNumber($strRuntime,3)
						);
				}
	
				$arrayTableBaseData[] = array(
					"?TID_TEST_LABEL_LASTEXECUTION_HASH?",
					$consumer->GetResultValue("LASTEXECUTION_HASH")
					);
	
	
				$arrayTableBaseData[] = array(
					"?TID_TEST_LABEL_FIRSTEXECUTION_DATETIME?",
					$this->GetWebsite()->RenderDateTime($consumer->GetResultValue("FIRSTEXECUTION_DATETIME"))
					);
	
				$arrayTableBaseData[] = array(
					"?TID_TEST_LABEL_COUNT?",
					$consumer->GetResultValue("COUNT")
					);
			}	

							
			$table = new CTableElement($this->GetWebsite(),"default");
			$this->SetOutputItem(
				"TESTBASEDATA_TABLE",
				$table->RenderTable($arrayTableBaseData,false,true));
			
			$tabnavigationDef = new CTabNavigationDefinition();
		  
		  if ($this->m_bDatabaseSupport == true)
		  {
	   		$arrayParams = array();
	   		$arrayParams[$this->GetModuleVarName("testdetailsmode")] = "lastexecution";
	   		$tabnavigationDef->AddTab(
	      		"lastexecution",
	      		"?TID_TEST_TAB_LASTEXECUTION?",
	      		$this->GetWebsite()->MakeURL($arrayParams));
	
	   		$arrayParams = array();
	   		$arrayParams[$this->GetModuleVarName("testdetailsmode")] = "statushistory";
	   		$tabnavigationDef->AddTab(
	      		"statushistory",
	      		"?TID_TEST_TAB_STATUSHISTORY?",
	      		$this->GetWebsite()->MakeURL($arrayParams));
	     }

   		$arrayParams = array();
   		$arrayParams[$this->GetModuleVarName("testdetailsmode")] = "testscript";
   		$tabnavigationDef->AddTab(
      		"testscript",
      		"?TID_TEST_TAB_TESTSCRIPT?",
      		$this->GetWebsite()->MakeURL($arrayParams));


			$strTestDetailsMode = $this->GetModuleVar("testdetailsmode");
			if ($strTestDetailsMode == "")
			{
			  if ($this->m_bDatabaseSupport == true)
			  {
					$strTestDetailsMode = "lastexecution";	
				}
				else
				{
					$strTestDetailsMode = "testscript";	
				}
			}

			$tabnavigation = new CTabNavigationElement(
			   $this->GetWebsite(),
			   "default");
			$this->SetOutputItem(
				"TABNAV",
				$tabnavigation->RenderTabNavigation(
			   	$tabnavigationDef,
			   	$strTestDetailsMode));
			 
			$strLog = "";
			$strItemManagerOutput = ""; 
			$strTestScript = "";
			
			$linkicon = new CLinkIconElement($this->GetWebsite(),"default");
			
			if ($strTestDetailsMode == "statushistory")
			{
				$strHistoryLogDateTime = $this->GetModuleVar("historylogdatetime");
				$this->SetModuleVar("historylogdatetime","");
				$nOffset = intval($this->GetModuleVar("statushistory_offset"));
	
				$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
				$arrayParams = array();
				$arrayParams["blocksize"] = "10";
				$arrayParams["offset"] = $nOffset;
				$arrayParams["statushistory_filter1"] = "site_path";
				$arrayParams["statushistory_filter1_value"] = $arrayTestPath["site_path"];
				$arrayParams["statushistory_filter1_operator"] = "=";
				$arrayParams["statushistory_filter2"] = "assembly_id";
				$arrayParams["statushistory_filter2_value"] = $arrayTestPath["assembly_id"];
				$arrayParams["statushistory_filter2_operator"] = "=";
				$arrayParams["statushistory_filter3"] = "group_id";
				$arrayParams["statushistory_filter3_value"] = $arrayTestPath["group_id"];
				$arrayParams["statushistory_filter3_operator"] = "=";
				$arrayParams["statushistory_filter4"] = "test_id";
				$arrayParams["statushistory_filter4_value"] = $arrayTestPath["test_id"];
				$arrayParams["statushistory_filter4_operator"] = "=";
				/*if ($strHistoryLogDateTime != "")
				{
					$arrayParams["getlog"] = "1";
				}*/
				$arrayParams["command"] = "statushistorylist";
				$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
	
				$strError = $consumer->GetError();
				
				if ($strError != "")
				{
					$this->SetErrorMessageCode($strError);
					return "error";
				}
	
				$nMaxOffset = intval($consumer->GetResultValue("MAXOFFSET"));
				if ($nOffset > $nMaxOffset)
				{
					$this->SetModuleVar("statushistory_offset",$nMaxOffset);
					return $this->StateTestDetails();	
				}		




				$arrayItemList = $consumer->GetResultList();
			
				$format_strong = new CFormatElement($this->GetWebsite(),"strong");
				$link = new CLinkElement($this->GetWebsite(),"default");
			
				foreach ($arrayItemList as &$item)
				{
					$strTasks = "";
					

					$item["HASH"] = StringSection($item["HASH"],0,10);
					if ($item["HASH"] != "")
					{
						$item["HASH"] .= "...";
					}

					$strDateTime = $item["DATETIME"];
					if ($strDateTime == $strHistoryLogDateTime)
					{
						$strLog = ArrayGetValue($item,"LOG")." ";
						$item["DATETIME"] = $format_strong->RenderFormat($this->GetWebsite()->RenderDateTime($strDateTime));
						$item["RUNTIME_SECONDS"] = $format_strong->RenderFormat($this->GetWebsite()->RenderNumber($item["RUNTIME_SECONDS"],3));
						$item["HASH"] = $format_strong->RenderFormat($item["HASH"]);
					}
					else
					{
						$arrayParams = array();
						$arrayParams[$this->GetModuleVarNameState()] = "testdetails";
						$arrayParams[$this->GetModuleVarName("historylogdatetime")] = $strDateTime;
						$strLinkViewLog = $this->GetWebsite()->MakeURL($arrayParams);
						$strTasks .= $linkicon->RenderLinkIcon($strLinkViewLog,"view","?".$this->MakeEntityTID("TEST","TOOL","VIEWLOG")."?");
						$item["DATETIME"] = $link->RenderLink($strLinkViewLog,$this->GetWebSite()->RenderDateTime($strDateTime));
					} 

					
					$item["STATUS"] = $this->RenderTestStatus($item["STATUS"]);
					

					$item["TASKS"] = $strTasks;
				}

				$itemmanagerdata = new CItemManagerData();
				   
				$itemmanagerdata->SetItemList($arrayItemList);
				
				$itemmanagerdata->AddColumn("DATETIME","?TID_TEST_TABLEHEADER_HISTORY_DATETIME?","",false);
				$itemmanagerdata->AddColumn("STATUS","?TID_TEST_TABLEHEADER_HISTORY_STATUS?","",false);
				$itemmanagerdata->AddColumn("RUNTIME_SECONDS","?TID_TEST_TABLEHEADER_HISTORY_RUNTIME_SECONDS?","",false);
				$itemmanagerdata->AddColumn("HASH","?TID_TEST_TABLEHEADER_HISTORY_HASH?","",false);
				$itemmanagerdata->AddColumn("TASKS","","");

				$nPageCount = intval($consumer->GetResultValue("MAXOFFSET")) + 1;	
				$itemmanagerdata->SetPageNavigation(
					$this->GetModuleVar("statushistory_offset"),
					$nPageCount,
					$this->GetModuleVarName("statushistory_offset"),
					20);
					
				$itemmanager = new CItemManagerElement($this->GetWebsite(),"default");
				$strItemManagerOutput = $itemmanager->RenderItemManager($itemmanagerdata);
			}
			else if ($strTestDetailsMode == "testscript")
			{
				$strTestScript = htmldecode($consumer->GetResultValue("TEST_SCRIPT"));
			}
			else
			{
				$strLog = $consumer->GetResultValue("LASTEXECUTION_LOG"). " ";
			}
					
			
			$this->SetOutputItem("ITEMMANAGER",$strItemManagerOutput);


			if ($strLog != "")			
			{
				$console = new CConsoleElement(
					$this->GetWebsite(),
					"default");
				$strConsoleLog = $console->RenderConsole($strLog);
				$arrayTableLog[] = array("?TID_TEST_LABEL_TESTLOG?");
				$arrayTableLog[] = array($strConsoleLog);
				$table = new CTableElement($this->GetWebsite(),"default");
				$this->SetOutputItem(
					"TESTLOG_TABLE",
					$table->RenderTable($arrayTableLog,true,false));					
			}
			else
			{
				$this->SetOutputItem("TESTLOG_TABLE","");
			}
			
			if ($strTestScript != "")			
			{
				$console = new CConsoleElement(
					$this->GetWebsite(),
					"default");
				$strConsoleTestScript = $console->RenderConsole($strTestScript);
				$arrayTableTestScript[] = array("?TID_TEST_LABEL_TESTSCRIPT?");
				$arrayTableTestScript[] = array($strConsoleTestScript);
				$table = new CTableElement($this->GetWebsite(),"default");
				$this->SetOutputItem(
					"TESTSCRIPT_TABLE",
					$table->RenderTable($arrayTableTestScript,true,false));					
			}
			else
			{
				$this->SetOutputItem("TESTSCRIPT_TABLE","");
			}
		


			$this->RenderToolbar();

	
			return "testdetails";
		} 


		function StateTestList()
		{
			$this->Trace("StateTestList");
			$this->SetState("testlist");
			
			
			// ITEMMANAGER
			$itemmanagerdata = new CItemManagerData();
			
			$linkicon = new CLinkIconElement($this->GetWebsite(),"default");
			$link = new ClinkElement($this->GetWebsite(),"default");
			
			// Sorting
			$strSortOption = $this->GetModuleVar("testlist_sort");
			if ($strSortOption == "")
			{
				$strSortOption = "test_path";
			}
			$strSortOrder = $this->GetModuleVar("testlist_sort_order");
			if ($strSortOrder == "")
			{
				$strSortOrder = "asc";
			}
			
				
			// FILTER OPTIONS
			$formDef = new CFormDefinition();
			$formDef->SetURL(
				$this->GetWebsite()->GetRootURL(),
				$this->GetWebsite()->GetSessionAwareArrayVars());
			
			
			// Reset filter options
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarName("testlist_filter_site")] = "";
			$arrayParams[$this->GetModuleVarName("testlist_filter_assembly")] = "";
			$arrayParams[$this->GetModuleVarName("testlist_filter_group")] = "";
			$arrayParams[$this->GetModuleVarName("testlist_filter_includetestgroup")] = false;
			$strLinkReset = $this->GetWebsite()->MakeURL($arrayParams);
			$formDef->AddControlStatic(
				"FILTERRESET",
				$linkicon->RenderLinkIcon(
					$strLinkReset,
					"undo",
					"?TID_TEST_TOOL_RESETFILTER?")); 
			
			
			/*
			// Testpath Search
			$formDef->AddControlText(
				"SEARCH",
				$this->GetModuleVarName("testlist_search"),
				"Search",
				$this->GetModuleVar("testlist_search"),
				20,
				20);
			*/

			// Inclusion of group "test"
			$bIncludeTestGroup = GetBoolValue($this->GetModuleVar("testlist_filter_includetestgroup"));
		
			// Site
			$strCurrentSitePath = $this->GetModuleVar("testlist_filter_site");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$arrayParams = array();
			$arrayParams["command"] = "sitelist";
			$arrayParams["site_sort1"] = "site_name";
			$arrayParams["site_sort1_order"] = "asc";				
			$webservice->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
				return "error";				
			}
			$arraySiteList = $webservice->GetResultList();
			$arraySiteOptions = array();
			foreach ($arraySiteList as $site)
			{
				if ($site["SITE_PATH"] != "")
				{
					$arraySiteOptions[$site["SITE_PATH"]] = $site["SITE_NAME"];
				}
			}
			$arraySiteOptions = ArrayMerge(
				array(
					"" => $this->LoadString("TID_TEST_FILTEROPTION_ALLSITES")
					),
				$arraySiteOptions);
			$formDef->AddControlSelect(
				"SITE",
				$this->GetModuleVarName("testlist_filter_site"),
				$this->LoadString("TID_TEST_LABEL_FILTERSITE"),
				$arraySiteOptions,
				$strCurrentSitePath);
				
				
			// Assembly
			$strCurrentAssemblyID = $this->GetModuleVar("testlist_filter_assembly");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$arrayParams = array();
			$arrayParams["command"] = "assemblylist";
			if ($strCurrentSitePath != "")
			{
				$arrayParams["assembly_filter1"] = "site_path";
				$arrayParams["assembly_filter1_operator"] = "=";
				$arrayParams["assembly_filter1_value"] = $strCurrentSitePath;
			}		
			$arrayParams["assembly_sort1"] = "assembly_id";
			$arrayParams["assembly_sort1_order"] = "asc";				
			$webservice->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
				return "error";				
			}
			$arrayAssemblyList = $webservice->GetResultList();
			$arrayAssemblyOptions = array();
			foreach ($arrayAssemblyList as $assembly)
			{
				if ($assembly["ASSEMBLY_ID"] != "")
				{
					$arrayAssemblyOptions[$assembly["ASSEMBLY_ID"]] = $assembly["ASSEMBLY_ID"];
				}
			}
			$arrayAssemblyOptions = ArrayMerge(
				array(
					"" => $this->LoadString("TID_TEST_FILTEROPTION_ALLASSEMBLIES")
					),
				$arrayAssemblyOptions);
			$formDef->AddControlSelect(
				"ASSEMBLY",
				$this->GetModuleVarName("testlist_filter_assembly"),
				$this->LoadString("TID_TEST_LABEL_FILTERASSEMBLY"),
				$arrayAssemblyOptions,
				$strCurrentAssemblyID);
				
				



			// Group
			$strCurrentGroupID = $this->GetModuleVar("testlist_filter_group");
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$arrayParams = array();
			$arrayParams["command"] = "grouplist";
			if ($strCurrentSitePath != "")
			{
				$arrayParams["group_filter1"] = "site_path";
				$arrayParams["group_filter1_operator"] = "=";
				$arrayParams["group_filter1_value"] = $strCurrentSitePath;
			}			
			if ($strCurrentAssemblyID != "")
			{
				$arrayParams["group_filter2"] = "assembly_id";
				$arrayParams["group_filter2_operator"] = "=";
				$arrayParams["group_filter2_value"] = $strCurrentAssemblyID;
			}
			if ($bIncludeTestGroup == false)
			{
				$arrayParams["group_filter3"] = "group_id";
				$arrayParams["group_filter3_operator"] = "!=";
				$arrayParams["group_filter3_value"] = "test";
			}			
			$arrayParams["group_sort1"] = "group_id";
			$arrayParams["group_sort1_order"] = "asc";


			$webservice->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
				return "error";				
			}
			$arrayGroupList = $webservice->GetResultList();
			$arrayGroupOptions = array();
			foreach ($arrayGroupList as $group)
			{
				if ($group["GROUP_ID"] != "")
				{
					$arrayGroupOptions[$group["GROUP_ID"]] = $group["GROUP_ID"];
				}
			}
			$arrayGroupOptions = ArrayMerge(
				array(
					"" => $this->LoadString("TID_TEST_FILTEROPTION_ALLGROUPS")
					),
				$arrayGroupOptions);
			$formDef->AddControlSelect(
				"GROUP",
				$this->GetModuleVarName("testlist_filter_group"),
				$this->LoadString("TID_TEST_LABEL_FILTERGROUP"),
				$arrayGroupOptions,
				$strCurrentGroupID);

			// Include Test Group
			$formDef->AddControlCheckbox(
			   "INCLUDETESTGROUP",
			   $this->GetModuleVarName("testlist_filter_includetestgroup"),
			   $this->LoadString("TID_TEST_LABEL_INCLUDETESTGROUP"),
			   $bIncludeTestGroup);

		
				
			// Submit Button				
			$formDef->AddControlButtonSubmit($this->LoadStringEntity("TEST","BUTTON","FILTER"));
			
			if ($this->GetModuleConfig("hidefilteroptions") != true)
			{
				//$itemmanagerdata->SetFilterOptionsForm($formDef,"default","testlistfilter");
				$itemmanagerdata->SetFilterOptionsForm($formDef,"horizontal","");
			}
			
			
			$this->m_bDatabaseSupport = $webservice->GetResultValue("DATABASE_SUPPORT");
			$strCurrentStatus = "";
			
			if ($this->m_bDatabaseSupport == true)
			{
			
				// Filtering by Status
				$strCurrentStatus = $this->GetModuleVar("testlist_filter_status");
				if ($strCurrentStatus == "")
				{
					$strCurrentStatus = "error";		
				}
				$formatError = new CFormatElement($this->GetWebsite(),"testfailed");
				$formatWarning = new CFormatElement($this->GetWebsite(),"testinactive");			
				$formatOK = new CFormatElement($this->GetWebsite(),"testpassed");			
				$arrayStatusTabs = array(
					"error"		=> $formatError->RenderFormat($this->LoadString("TID_TEST_TAB_STATUS_ERROR")),
					"warning"	=> $formatWarning->RenderFormat($this->LoadString("TID_TEST_TAB_STATUS_WARNING")),
					"ok"			=> $formatOK->RenderFormat($this->LoadString("TID_TEST_TAB_STATUS_OK")),
					"all"			=> $this->LoadString("TID_TEST_TAB_STATUS_ALL")
					);
				$itemmanagerdata->SetFilterOptionsTabNavigation(
					$strCurrentStatus,
					$this->GetModuleVarName("testlist_filter_status"),
					$arrayStatusTabs);
			}
			
			
			// Get Test List	
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$arrayParams = array();
			//$arrayParams["blocksize"] = "10";
			//$arrayParams["offset"] = $nOffset;
			$arrayParams["test_sort1"] = $strSortOption;
			$arrayParams["test_sort1_order"] = $strSortOrder;


			switch ($strCurrentStatus)
			{
				case "error":
				{
					$arrayParams["test_filter1"] = "lastexecution_status";
					$arrayParams["test_filter1_operator"] = "in";
					$arrayParams["test_filter1_value"] = "FAILED,UNDEFINED,REQUESTERROR";
					break;
				}
				case "warning":
				{
					$arrayParams["test_filter1"] = "lastexecution_status";
					$arrayParams["test_filter1_operator"] = "in";
					$arrayParams["test_filter1_value"] = "INACTIVE,UNKNOWN,";
					break;					
				}
				case "ok":
				{
					$arrayParams["test_filter1"] = "lastexecution_status";
					$arrayParams["test_filter1_operator"] = "=";
					$arrayParams["test_filter1_value"] = "PASSED";
					break;					
				}
				default:
				{
					break;
				}
			}


			if ($strCurrentSitePath != "")
			{
				$arrayParams["test_filter2"] = "site_path";
				$arrayParams["test_filter2_operator"] = "=";
				$arrayParams["test_filter2_value"] = $strCurrentSitePath;
			}

			if ($strCurrentAssemblyID != "")
			{
				$arrayParams["test_filter3"] = "assembly_id";
				$arrayParams["test_filter3_operator"] = "=";
				$arrayParams["test_filter3_value"] = $strCurrentAssemblyID;
			}

			if ($strCurrentGroupID != "")
			{
				$arrayParams["test_filter4"] = "group_id";
				$arrayParams["test_filter4_operator"] = "=";
				$arrayParams["test_filter4_value"] = $strCurrentGroupID;
			}


			if ($bIncludeTestGroup == false)
			{
				$arrayParams["test_filter5"] = "group_id";
				$arrayParams["test_filter5_operator"] = "!=";
				$arrayParams["test_filter5_value"] = "test";
				
			}
			
			
			$arrayConfigFilterOptions = $this->GetModuleConfig("filteroptions");
			if (is_Array($arrayConfigFilterOptions))
			{
				$nFilterID = 6;
				foreach ($arrayConfigFilterOptions as $arrayOption)
				{
					$arrayParams["test_filter".$nFilterID] = ArrayGetValue($arrayOption,"field");
					$arrayParams["test_filter".$nFilterID."_operator"] = ArrayGetValue($arrayOption,"operator");	
					$arrayParams["test_filter".$nFilterID."_value"] = ArrayGetValue($arrayOption,"value");	
					$nFilterID++;
				}
			}
			


			
			$arrayParams["getstatus"] = "1";
			$arrayParams["command"] = "testlist";
			$webservice->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$strError = $webservice->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
				return "error";
			}
			$nCount = intval($webservice->GetResultValue("TOTALENTRIES"));
			
			if ($nCount == 0)
			{
				$message = new CMessageElement($this->GetWebsite(),"info");
				$this->SetOutputItem("MESSAGE",
					$message->RenderMessage(
						$this->LoadString("TID_TEST_INFO_NOTESTSFOUND")));
			}
			else
			{
				$this->SetOutputItem("MESSAGE","");	
			}


/*
			$nMaxOffset = max(0,intval($webservice->GetResultValue("MAXOFFSET")));
			if ($nOffset > $nMaxOffset)
			{
				$this->SetModuleVar("testlist_offset",$nMaxOffset);
				return $this->StateTestList();	
			}			
			
			*/
		
			
		
			$itemmanagerdata->AddColumn("test_path","?".$this->MakeEntityTID("TEST","TABLEHEADER","test_path")."?","test_path",false,"");
			if ($this->m_bDatabaseSupport == true)
			{
				$itemmanagerdata->AddColumn("lastexecution_datetime","?".$this->MakeEntityTID("TEST","TABLEHEADER","lastexecution_datetime")."?","lastexecution_datetime",false,"");
				$itemmanagerdata->AddColumn("lastexecution_status","?".$this->MakeEntityTID("TEST","TABLEHEADER","lastexecution_status")."?","",false,"");
			}
			$itemmanagerdata->AddColumn("TASKS","","");
			
			

			$itemmanagerdata->SetSortOptions(
				$this->GetModuleVarName("testlist_sort"),
				$strSortOption,
				$this->GetModuleVarName("testlist_sort_order"),
				$strSortOrder
				);
		
			$arrayItemList = $this->GetItemListFromWebservice(
				"test",
				$webservice);
		
		
			$link = new ClinkElement($this->GetWebsite(),"default");
			$linkicon = new CLinkIconElement($this->GetWebsite(),"default");

			$bDisplayTestGroupMessage = false;
			
			$strTidToolView = $this->LoadString("TID_TEST_TOOL_VIEW");
			$strTidToolTest = $this->LoadString("TID_TEST_TOOL_TEST");
						
			foreach ($arrayItemList as &$item)
			{

				// Link to test
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "test";
				$arrayParams[$this->GetModuleVarName("test_path")] = $item["test_path"];
				$strLinkTest = $this->GetWebsite()->MakeURL($arrayParams);


				// Link to test details
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "testdetails";
				$arrayParams[$this->GetModuleVarName("test_path")] = $item["test_path"];
				$strLinkTestDetails = $this->GetWebsite()->MakeURL($arrayParams);

				// Test Path
				$strTestPath = $item["test_path"];
				$item["test_path"] = $link->RenderLink($strLinkTestDetails,$strTestPath);
				
				// Last Execution
				$item["lastexecution_datetime"] = $this->GetWebsite()->RenderDateTime($item["lastexecution_datetime"]);
				
				// Status
				$item["lastexecution_status"] = $this->RenderTestStatus($item["lastexecution_status"]);
				
				// Tool Icons
				$strTasks = "";
				$strTasks .= $linkicon->RenderLinkIcon($strLinkTestDetails,"view",$strTidToolView);
				$strTasks .= $linkicon->RenderLinkIcon($strLinkTest,"execute",$strTidToolTest);
								
				$item["TASKS"] = $strTasks;
		
				$strTestGroup = $item["group_id"];
				if ($strTestGroup == "test")
				{
					$bDisplayTestGroupMessage = true;	
				}
			}
			
		
			$itemmanagerdata->SetItemList($arrayItemList);

			// Render item manager
			$itemmanager = new CItemManagerElement(
				$this->GetWebsite(),
				"default");
			$strItemManagerOutput = $itemmanager->RenderItemManager($itemmanagerdata);
			
			$this->SetOutputItem("ITEMMANAGER",$strItemManagerOutput);
			
			// MESSAGE TESTGROUP
			if ($bDisplayTestGroupMessage)
			{
				$message = new CMessageElement($this->GetWebsite(),"warning");
				$this->SetOutputItem("MESSAGE_TESTGROUP",
					$message->RenderMessage(
						$this->LoadString("TID_TEST_INFO_DISPLAYTESTGROUP")));
			}
			else
			{
				$this->SetOutputItem("MESSAGE_TESTGROUP","");
			}
		
			
			// TOOLBAR
			$this->RenderToolbar();
			
			return "testlist";
		}




		function GetTestList($strMode)
		{
			
			$arrayParams = array();
			$arrayParams["command"] = "testlist";
			$arrayParams["test_filter1"] = "group_id";
			$arrayParams["test_filter1_operator"] = "!=";
			$arrayParams["test_filter1_value"] = "test";			  

			$nFilterID = 2;			
			
			switch ($strMode)
			{
				case "failed":
					$arrayParams["test_filter".$nFilterID] = "lastexecution_status";
					$arrayParams["test_filter".$nFilterID."_operator"] = "in";
					$arrayParams["test_filter".$nFilterID."_value"] = "FAILED,UNDEFINED,REQUESTERROR";
					$nFilterID++;			
					break;
					
				case "all":
				default:
					break;	
			}
			
			$arrayConfigFilterOptions = $this->GetModuleConfig("filteroptions");
			if (ArrayCount($arrayConfigFilterOptions) > 0)
			{
				foreach ($arrayConfigFilterOptions as $arrayOption)
				{
					$arrayParams["test_filter".$nFilterID] = ArrayGetValue($arrayOption,"field");
					$arrayParams["test_filter".$nFilterID."_operator"] = ArrayGetValue($arrayOption,"operator");	
					$arrayParams["test_filter".$nFilterID."_value"] = ArrayGetValue($arrayOption,"value");	
					$nFilterID++;
				}
			}			
			
			$webservice = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$webservice->ConsumeWebService($this->m_strWebservice,$arrayParams);
			
			$arrayTestList = $webservice->GetResultList();
			
			return $arrayTestList;
		}
		
		function GetPendingTestList()
		{
			$strTestList = $this->GetModuleVar("testlist");
 			$arrayTestList = ArrayUnpack($strTestList);
 			if (!is_array($arrayTestList))
 			{
 				return array();	
 			}
			return $arrayTestList;
		}
		
		function SetPendingTestList($arrayTestList)
		{
			if (!is_array($arrayTestList))
			{
				$arrayTestList = array();
			}
			$strTestList = ArrayPack($arrayTestList);
			$this->SetModuleVar("testlist",$strTestList);
		}


		function StateRunTests()
		{
			$this->Trace("StateRunTests");
			
			$strOriginalState = $this->GetPreviousState();
			$this->SetModuleVar("originalstate",$strOriginalState);

			$strMode = $this->GetModuleVar("mode");
			$arrayTests = $this->GetTestList($strMode);
			$arrayTestPaths = array();
			foreach ($arrayTests as $arrayTest)
			{
				$strTestPath = ArrayGetValue($arrayTest,"TEST_PATH");
				if ($strTestPath != "")
				{
					$arrayTestPaths[] = $strTestPath;
				}
			}
			$this->SetPendingTestList($arrayTestPaths);
			return $this->StateRunTestsContinue();
		}


		function StateRunTestsContinue()
		{
			$this->Trace("StateRunTestsContinue");
			
			$strOriginalState = $this->GetModuleVar("originalstate");

			$arrayTests = $this->GetPendingTestList();
			if (ArrayCount($arrayTests) == 0)
			{
				$this->SetInfoMessageText($this->LoadStringEntity("TEST","INFO","RUNTESTS-COMPLETE"));
				return $this->CallbackRenderModule($strOriginalState);
			}

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "runtestscontinue";	
			$strURL = $this->GetWebsite()->MakeURL($arrayParams);
			$this->GetWebsite()->Redirect($strURL,0); 
			
			$this->IncludeElement('loading');
			$loading = new CLoadingElement($this->GetWebsite(),'inline');
			$this->SetInfoMessageText(
				$loading->RenderLoading()
				.ReplaceTagsWDK(
					$this->LoadStringEntity("TEST","INFO","RUNTESTS-PROGRESS"),
					array(
						"COUNT" => ArrayCount($arrayTests) - 1
						)
					)
				);
			
			$strTestPath = array_pop($arrayTests);
			$this->SetPendingTestList($arrayTests);
			
			$arrayTestPath = ParseTestPath($strTestPath);
			if ($arrayTestPath == false)
			{
				return $this->CallbackRenderModule($strOriginalState);
			}
			
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			
			$arrayParams = array();
			$arrayParams["test_path"] = $strTestPath;
			$arrayParams["command"] = "test";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			
			/*
			$strError = $consumer->GetError();
			
			if ($strError == "TEST_NOT_FOUND")
			{
				
			}
			if ($strError == "TEST_REQUEST_FAILED")	
			{
				$this->SetErrorMessageCode($strError);
			}
			else if ($strError != "")
			{
				$this->SetErrorMessageCode($strError);
				return "error";
			}
			*/
			
			/*
			if ($this->GetPreviousState() == "testlist" || $consumer->GetResultValue("DATABASE_SUPPORT") == false)     
			{
				$this->SetOutputItem("TESTID",$strTestPath);
				$strStatus = $consumer->GetResultValue("LASTEXECUTION_STATUS");
				$this->SetOutputItem("TESTSTATUS",$this->RenderTestStatus($strStatus));
				$this->SetInfoMessageCodeEntity("test","TESTEXECUTED");
			}
			
			if ($this->GetPreviousState() == "testlist")     
			{
				return $this->StateTestList();
			}
			else
			{
				return $this->StateTestDetails();
			}
			*/
			
			//$this->SetInfoMessageText("BLUBB");
			
			return $this->CallbackRenderModule($strOriginalState);
		}
		

	

	}

