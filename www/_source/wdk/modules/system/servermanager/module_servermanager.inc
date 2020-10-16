<?php

	require_once(GetWDKDir()."wdk_module.inc");
	require_once(GetWDKDir()."wdk_ip.inc");
	
	class CServerManagerModule extends CModule
	{
		
		private $m_arrayErrors;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"system/servermanager",
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->m_arrayErrors = array();
			
			/*
			$arrayToolbarConfig = array(
				"dashboard" => array("help"),
				"help" => array("dashboard")
				);
			*/
			$arrayToolbarConfig = array(
				"dashboard" => array("refresh")
				);
			
			$this->SetToolbarConfig($arrayToolbarConfig);
			
			return true;
		}
		
		function CallbackRenderModule($strState)
		{
			$this->Trace("CallbackRenderModule(\"$strState\")");
			switch ($strState)
			{
				case "help":
					return $this->StateHelp();
				case "":
					$this->SetState("dashboard");
				default:
					return $this->StateDashboard();
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
				$toolbarDef->AddTool($strLink, "help", "?TID_SERVERMANAGER_TOOL_HELP?");
			}


			if ($this->IsToolVisible("dashboard"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "dashboard";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "list", "?TID_SERVERMANAGER_TOOL_DASHBOARD?");
			}

			if ($this->IsToolVisible("refresh"))
			{ 
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "dashboard";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "refresh", "?TID_SERVERMANAGER_TOOL_REFRESH?");
			}


			$toolbar = new CToolbarElement($this->GetWebsite(),"default");
			$this->SetOutputItem("TOOLBAR",$toolbar->RenderToolbar($toolbarDef)); 
		}		
		
		function StateHelp()
		{
			$this->RenderToolBar();
			return "help";
		}		
		
		function GetServers()
		{
			$arrayServers = $this->GetModuleConfig("servers");
			if (is_array($arrayServers) == false)
			{
				$arrayServers = $this->GetConfig("servers");	
				if (is_array($arrayServers) == false)
				{
					$arrayServers = array();
					$arrayWebservices = GetWebservicesDirectory();
					$arrayServers[]	= array(
						"name" => "This",
						"webservice" => ArrayGetValue($arrayWebservices,"system/server")
						);
				}
			}
			if (!is_array($arrayServers))
			{
				return array();	
			}
			return $arrayServers;
		}

		
		function StateDashboard()
		{
			$this->RenderToolBar();
			
			$arrayServers = $this->GetServers();
			
			$arrayDashboard = array();			
			$arrayDashboard[] = array(
				"?TID_SERVERMANAGER_LABEL_NAME?",
				"?TID_SERVERMANAGER_LABEL_STATUS?",
				"?TID_SERVERMANAGER_LABEL_IP?",				
				"?TID_SERVERMANAGER_LABEL_LOAD_AVERAGE_1MINUTE?",
				"?TID_SERVERMANAGER_LABEL_LOAD_AVERAGE_5MINUTES?",
				"?TID_SERVERMANAGER_LABEL_LOAD_AVERAGE_15MINUTES?",
				"?TID_SERVERMANAGER_LABEL_DISK_USED_PERCENT?",
				"?TID_SERVERMANAGER_LABEL_PLESK_ACTIVE?"); 
			
			$this->IncludeElement("format");
			$formatError = new CFormatElement($this->GetWebsite(),"testfailed");
			$formatUp = new CFormatElement($this->GetWebsite(),"testpassed");
			
			foreach ($arrayServers as $arrayServer)
			{
				$strName = ArrayGetValue($arrayServer,"name");
				$strIP = "";
				$strStatus = "?TID_SERVER_STATUS_CONFIGERROR?";
				$strLoadAvg1 = "";
				$strLoadAvg5 = "";
				$strLoadAvg15 = "";
				$strDiskUsedPct = "";
				$strPleskActive = "";
				if ($strName != "")
				{
					$arrayWebservice = ArrayGetValue($arrayServer,"webservice");
					$strURL = ArrayGetValue($arrayWebservice,"url");
					if ($strURL != "")
					{
						$arrayURL = ParseURL($strURL);
						$strHost = ArrayGetValue($arrayURL,"HOST");
						if ($strHost != "")
						{
							$strIP = GetIPFromHostName($strHost);
						}

						$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());	
						$arrayParams = array();
						$arrayParams["command"] = "status";
						$strAccessCode = ArrayGetValue($arrayWebservice,"accesscode");
						if ($strAccessCode != "")
						{
							$arrayParams["accesscode"] = $strAccessCode;
						}
						$consumer->ConsumeWebService($strURL,$arrayParams,5);
						$strError = $consumer->GetError();
						if ($strError == "ACCESSCODE_REQUIRED" || $strError == "ACCESSCODE_INVALID")
						{
							$strStatus = $formatError->RenderFormat("?TID_SERVER_STATUS_CONFIGERROR?");
						}
						else if ($strError != "")
						{
							$strStatus = $formatError->RenderFormat("?TID_SERVER_STATUS_DOWN?");
						}
						else
						{
							$strStatus = $formatUp->RenderFormat("?TID_SERVER_STATUS_UP?");
							if ($strIP == "")
							{
								$strIP = $consumer->GetResultValue("IP");
							}
							$strLoadAvg1 = $consumer->GetResultValue("LOAD_AVERAGE_1MINUTE");
							if ($strLoadAvg1 > 10)
							{
								$strLoadAvg1 = $formatError->RenderFormat($strLoadAvg1);
							}
							$strLoadAvg5 = $consumer->GetResultValue("LOAD_AVERAGE_5MINUTES");
							if ($strLoadAvg5 > 10)
							{
								$strLoadAvg5 = $formatError->RenderFormat($strLoadAvg5);
							}
							$strLoadAvg15 = $consumer->GetResultValue("LOAD_AVERAGE_15MINUTES");
							if ($strLoadAvg15 > 10)  
							{
								$strLoadAvg15 = $formatError->RenderFormat($strLoadAvg15);
							}
							$strDiskUsedPct = $consumer->GetResultValue("DISK_USED_PERCENT");
							if ($strDiskUsedPct > 90)   
							{
								$strDiskUsedPct = $formatError->RenderFormat($strDiskUsedPct);
							}
							$bPleskActive = $consumer->GetResultValue("PLESK_ACTIVE");
							if ($bPleskActive == true)
							{
								$strPleskURL = "https://".$strHost.":8443/login_up.php";
								$link = new CLinkElement($this->GetWebsite(),"default");
								$strPleskActive = $link->RenderLink($strPleskURL,"?TID_ITEM_ACTIVE?",true);
							}
							else
							{
								$strPleskActive = "?TID_ITEM_INACTIVE?";
							}
						}
					} 
				}
				$arrayDashboard[] = array(
					$strName,
					$strStatus,
					$strIP,
					$strLoadAvg1,
					$strLoadAvg5,
					$strLoadAvg15,
					$strDiskUsedPct,
					$strPleskActive);
			}
			
			$table = new CTableElement($this->GetWebsite(),"default");
			$strTable = $table->RenderTable($arrayDashboard,true,false);
			$this->SetOutputItem("TABLE",$strTable);
			

			
			$this->GetWebsite()->Redirect($this->GetWebsite()->MakeSelfURL(),10);
			
			return "dashboard";
		}
		


	}

