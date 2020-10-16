<?php
	
	class CWebserviceManagerModule extends CModule
	{
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"system/webservicemanager",
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			/*
			if ($this->GetModuleConfig("groupname") != "")
			{
				$this->SetModuleVar("groupname",$this->GetModuleConfig("groupname"));
			}
			*/
			return true;
		}
		
		function CallbackRenderModule($strState)
		{
			switch ($strState)
			{
				case "details":
				{
					return $this->StateDetails();	
				}
				case "list":
				default:
				{
					return $this->StateList();	
				}
			}
		}
		
		function RenderToolbar($strWebservice)
		{
			$toolbarDef = new CToolbarDefinition();
			
			
			if ($strWebservice != "")
			{
				/*
				if ($this->GetModuleConfig("groupname") != "" || $this->GetModuleVarName("groupname") == "")
				{
					//$toolbarDef->AddTool("", "", "GROUP");
				}
				else
				{
				*/
					$arrayParams = array();
					$arrayParams[$this->GetModuleVarName("webservice")] = "";
					$arrayParams[$this->GetModuleVarNameState()] = "list";
					$strLinkToList = $this->GetWebsite()->MakeURL($arrayParams);
					
					$toolbarDef->AddTool($strLinkToList, "list", "?TID_WEBSERVICEMANAGER_TOOL_SELECTWEBSERVICE?");
				//}
					
					/*
				if ($strTestName != "")
				{
					$arrayParams = array();
					$arrayParams[$this->GetModuleVarName("testname")] = "";
					$arrayParams[$this->GetModuleVarName("groupname")] = $strGroupName;
					$arrayParams[$this->GetModuleVarNameState()] = "testlist";
					$strLinkToTestList = $this->GetWebsite()->MakeURL($arrayParams);

					$toolbarDef->AddTool($strLinkToTestList, "", $strGroupName);
					
				
				}
				*/
			}


			$toolbar = new CToolbarElement($this->GetWebsite(),"default");
			$strOutput = $toolbar->RenderToolbar($toolbarDef);

			return $strOutput;
		}

		
		
		
		function StateDetails()
		{
			
			$strWebservice = $this->GetModuleVar("webservice");
			if ($strWebservice == "")
			{
				return $this->StateList();	
			}
			
			// NAME
			
			$this->SetOutputItem("NAME",$strWebservice);
	
			
			// URL
			
			$strURL = $this->GetWebserviceURL($strWebservice);
			$this->SetOutputItem("URL",$strURL);						
			

			// LINKS
			
			$linkelement = new CLinkElement($this->GetWebsite(),"default");
			$arrayLinks = array();
			$arrayConfigLinks = $this->GetModuleConfig("webservices",$strWebservice,"links");
			array_push($arrayLinks,$strURL."?trace=1&command=help");
			array_push($arrayLinks,$strURL."?trace=1&command=selfcheck");
			array_push($arrayLinks,$strURL."?trace=1&command=install");
			if (is_array($arrayConfigLinks))
			{
				foreach ($arrayConfigLinks as $arrayConfigLink)
				{
					array_push($arrayLinks,$arrayConfigLink);
				}
			}
			$strAccessCode = $this->GetWebsite()->GetConfig("webservices",$strWebservice,"accesscode");
			$this->Trace("strAccessCode = $strAccessCode");
			if ($strAccessCode != "")
			{
				foreach ($arrayLinks as &$strLink)
				{
					$strLink .= "&accesscode=".$strAccessCode;
				}
			}
			
			$arrayLinksHTML = array();
			foreach ($arrayLinks as &$strLink)
			{
				// A bug in php causes confusion if the variable &$strLink is not used as reference. Unclear why.
				$strLinkHTML = $linkelement->RenderLink($strLink,"",true);
				array_push($arrayLinksHTML,$strLinkHTML."<br/><br/>");
			}
			
			$list = new CListElement($this->GetWebsite(),"default");
			$this->SetOutputItem("LINKS",$list->RenderList($arrayLinksHTML));
			
			
			
			
			// TESTS
			
			$strTestWebserviceURL = $this->GetWebserviceURL("system/test");
			if ($strTestWebserviceURL == "")
			{
				$message = new CMessageElement($this->GetWebsite(),"warning");
				$this->SetOutputItem("TESTMANAGER",$message->RenderMessage($this->LoadString("TID_WEBSERVICEMANAGER_MESSAGE_WARNING_NOTESTWEBSERVICE")));
			}
			else
			{
			
				$strTestGroupID = "webservice-".ReplaceString($strWebservice,"/","-");
				$this->GetWebsite()->IncludeModule("system/testmanager");
				$arrayModuleConfig = array();
				$arrayModuleConfig["hidefilteroptions"] = true;
				$arrayModuleConfig["filteroptions"] = array(
					array(
						"field" => "group_id",
						"operator" => "=",
						"value" => $strTestGroupID)
						,
					array(
						"field" => "site_path",
						"operator" => "=",
						"value" => "local")
						);
				$testmanager = new CTestManagerModule(
					$this->GetWebsite(),
					ReplaceString($strWebservice,"/","-"),
					$arrayModuleConfig);
				$this->SetOutputItem("TESTMANAGER",$testmanager->RenderModule());
			}		

			
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
			$arrayParams = array();
			$arrayParams["command"] = "help";
			$consumer->ConsumeWebService($strWebservice,$arrayParams);
			$strError = $consumer->GetError();
			if ($strError != "")
			{
				$this->SetErrorMessageCode("UNKNOWN");
				$this->SetOutputItem("HELP","");
				$this->SetOutputItem("VERSION","");
			}
			else
			{
				$console = new CConsoleElement($this->GetWebsite(),"default"); 
				$this->SetOutputItem(
					"HELP",
					$console->RenderConsole($consumer->GetHelp()));
				$this->SetOutputItem("VERSION",$consumer->GetMetaValue("VERSION"));
			}


			$this->SetOutputItem("TOOLBAR",$this->RenderToolbar(
				$strWebservice));

			
			//$this->SetModuleVar("webservice","");		
	
			return "details";
		} 

		

		
		
		
		function StateList()
		{
			$this->SetModuleVar("webservice","");
	

			// Create table
				
			$arrayData = array();
			
			$arrayRow = array();
			array_push($arrayRow,"?TID_WEBSERVICEMANAGER_LABEL_WEBSERVICES?");
			array_push($arrayData,$arrayRow);
			
			$arrayWebservices = $this->GetConfig("webservices");
			$arrayWebservices = ArraySortByKeys($arrayWebservices);
			
			foreach ($arrayWebservices as $strWebservice => $arrayWebservice)
			{
				$arrayRow = array();
				

				
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "details";
				$arrayParams[$this->GetModuleVarName("webservice")] = $strWebservice;
				$strLinkDetails = $this->GetWebsite()->MakeURL($arrayParams);

				$link = new CLinkElement($this->GetWebsite(),"default");
				array_push($arrayRow,$link->RenderLink($strLinkDetails,$strWebservice));
				
				
/*				
				$strTestAmount = "";		
				$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
				$arrayParams = array();
				$arrayParams["groupname"] = $strGroupName;
				$arrayParams["command"] = "testlist";
				$consumer->ConsumeWebService($this->GetWebserviceURL("system/test"),$arrayParams);
				$strError = $consumer->GetError();
				if ($strError == "")
				{
					$arrayList2 = $consumer->GetResultList();
					if (is_array($arrayList2))
					{
						$strTestAmount = "".ArrayCount($arrayList2);	
					}
				}
				$format = new CFormatElement($this->GetWebsite(),"integer");
				array_push($arrayRow,$format->RenderFormat($strTestAmount));
*/			
				
				array_push($arrayData,$arrayRow);				
			}
			
		
			
			$table = new CTableElement($this->GetWebsite(),"default");
			$this->SetOutputItem("LIST", $table->RenderTable($arrayData,true));
	
			return "list";	
		}


	}


