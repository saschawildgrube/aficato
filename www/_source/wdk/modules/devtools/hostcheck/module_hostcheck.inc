<?php

	require_once(GetWDKDir()."wdk_module.inc");
	require_once(GetWDKDir()."wdk_ip.inc");
	
	class CHostCheckModule extends CModule
	{
		
		private $m_arrayErrors;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			$website->LoadStringResources("domainstatus");
			parent::__construct(
				$website,
				"devtools/hostcheck",
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$this->m_arrayErrors = array();
			
			$arrayToolbarConfig = array();
			/*
			$arrayToolbarConfig = array(
				"start" => array("help"),
				"help" => array("start"),				
				"action1" => array("help","start"),
				"action1submit" => array("help","start","action1"),
				"action2" => array("help","start"),
				"action2submit" => array("help","start","action2")
				);
			*/
			$this->SetToolbarConfig($arrayToolbarConfig);
			
			return parent::OnInit();
		}
		
	
		function OnRenderModule($strState)
		{
			$this->Trace("OnRenderModule(\"$strState\")");
			switch ($strState)
			{
				/*case "help":
					return $this->StateHelp();*/
				case "check":
					return $this->StateCheck();
					/*
				case "action1submit":
					return $this->StateAction1Submit();
				case "action2":
					return $this->StateAction2();
				case "action2submit":
					return $this->StateAction2Submit();*/
				case "":
					$this->SetState("start");
				default:
					return $this->StateStart();
			}
		}
		
	
		function RenderToolbar()
		{
			$toolbarDef = new CToolbarDefinition();

/*
			if ($this->IsToolVisible("help"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "help";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "help", "?TID_HOSTCHECK_TOOL_HELP?");
			}


			if ($this->IsToolVisible("start"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "start";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "list", "?TID_HOSTCHECK_TOOL_START?");
			}

			if ($this->IsToolVisible("action1"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "action1";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "execute", "?TID_HOSTCHECK_TOOL_ACTION1?");
			}
			*/

/*
			if ($this->IsToolVisible("recheck"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "check";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "execute", "?TID_HOSTCHECK_TOOL_RECHECK?");
			}
			*/


			$toolbar = new CToolbarElement($this->GetWebsite(),"default");
			$this->SetOutputItem("TOOLBAR",$toolbar->RenderToolbar($toolbarDef)); 
		}		
		
		/*
		function StateHelp()
		{
			$this->RenderToolBar();
			
			return "help";
		}	
		*/	
		
		function StateStart()
		{
			$this->RenderToolBar();
			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "check";		

			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$strHost = $this->GetModuleVar("host");

			$formDef->AddControlText("HOST",$this->GetModuleVarName("host"),"?TID_HOSTCHECK_LABEL_HOST?",$strHost,100,255); 
			$formDef->AddControlButtonSubmit("?TID_HOSTCHECK_BUTTON_CHECK?");
			
			$form = new CFormElement($this->GetWebsite(),"default");
			
			$this->SetOutputItem( 
				"FORM",
				$form->RenderForm(
					$formDef,
					array(),					
					$this->m_arrayErrors));
					
			$this->RenderToolBar();
			
			if ($this->GetOutputItem("RESULT") == "")
			{
				$this->SetOutputItem("RESULT","");	
			}

			return "start";
		}
		
		function StateCheck()
		{
			$strHost = $this->GetModuleVar("host");
			$strHost = StringTrim($strHost);
			$this->SetModuleVar("host",$strHost); 

			$strHostName = "";
			$strHostIP = "";
			if (IsValidIPv4($strHost))
			{
				$strHostIP = $strHost;
			}
			else if (IsValidHostName($strHost))
			{
				$strHostName = $strHost;	
			}
			
			if ($strHostName == "" && $strHostIP == "")
			{
				$this->m_arrayErrors["HOST"] = "?TID_HOSTCHECK_ERROR_HOST_INVALID?";
				return $this->StateStart();	
			}
			
			$arrayCollection = array();
			$arrayTable = array();
			
			$table = new CTableElement($this->GetWebsite(),"default");
			
			$this->IncludeElement("collection");
			$collection = new CCollectionElement($this->GetWebsite(),"default");
			
			// Resolving host name from IP
			if ($strHostIP != "" && $strHostName == "")
			{
				$strHostName = GetHostNameFromIP($strHostIP);
				if ($strHostName == false)
				{
					$this->m_arrayErrors["HOST"] = "?TID_HOSTCHECK_ERROR_HOSTNAME_NOT_RESOLVED?";
				}
			}
			
			// Resolving IP from host name
			if ($strHostName != "" && $strHostIP == "")
			{
				$strHostIP = GetIPFromHostName($strHostName);
				if ($strHostIP == "")
				{
					if (StringSection($strHostName,0,4) != "www.")
					{
						$strHostIP = GetIPFromHostName("www.".$strHostName);
					}
					
					if ($strHostIP == "") 
					{
						$this->m_arrayErrors["HOST"] = "?TID_HOSTCHECK_ERROR_IP_NOT_RESOLVED?";
					}
				}
			}
	
			// IP
			if ($strHostIP == "")
			{
				$strHostIPdisplay = "?TID_HOSTCHECK_ERROR_IP_NOT_RESOLVED?";
			}
			else
			{
				$strHostIPdisplay = $strHostIP;
			}
			$arrayTable[] = array("?TID_HOSTCHECK_LABEL_IP?",$strHostIPdisplay);
			
			
			// Hostname
			$arrayTable[] = array("?TID_HOSTCHECK_LABEL_HOSTNAME?",$strHostName);
			
			// Reverse Lookup
			if (IsValidIPv4($strHost) == false)
			{
				$arrayTable[] = array("?TID_HOSTCHECK_LABEL_REVERSELOOKUP?",GetHostNameFromIP($strHostIP));
			}
			
			/*
			// Bash command to block an IP:
			if ($strHostIP != "")
			{
				$strCommand = "iptables -D INPUT -s $strHostIP -j DROP<br/>";
				$strCommand .= "iptables -A INPUT -s $strHostIP -j DROP<br/>";
				$strCommand .= "iptables-save &gt; /etc/iptables/rules.v4<br/>";
				$strCommand .= "init 6";
				$arrayTable[] = array("?TID_HOSTCHECK_LABEL_BASHCMD-BLOCK?",$strCommand);
			}
			*/


			// Domain status
			$nDomainStatus = DOMAIN_STATUS_UNKNOWN;
			if ($strHostName != "")
			{
				$nDomainStatus = GetDomainStatus($strHostName,true);
			}
			
			$this->IncludeElement("format");
			$formatFree = new CFormatElement($this->GetWebsite(),"green");
			$formatForSale = new CFormatElement($this->GetWebsite(),"blue");
			$formatTaken = new CFormatElement($this->GetWebsite(),"red");
			if ($nDomainStatus == DOMAIN_STATUS_FREE)
			{
				$strStatusHTML = $formatFree->RenderFormat("?TID_DOMAINSTATUS_FREE?");
			}
			else if ($nDomainStatus == DOMAIN_STATUS_TAKEN)
			{
				$strStatusHTML = $formatTaken->RenderFormat("?TID_DOMAINSTATUS_TAKEN?");
			}
			else if ($nDomainStatus == DOMAIN_STATUS_FORSALE)
			{
				$strStatusHTML = $formatForSale->RenderFormat("?TID_DOMAINSTATUS_FORSALE?");
			}
			else
			{
				$strStatusHTML = "?TID_DOMAINSTATUS_UNKNOWN?";
			}						
			$arrayTable[] = array("?TID_HOSTCHECK_LABEL_DOMAINSTATUS?",$strStatusHTML);
			
			
			
			// Host name whois
			$strWhoisServer = "";
			$strWhois = GetWhoisRecords($strHostName,$strWhoisServer);
			$strWhois = SecureOutput($strWhois);
			$strWhois = HtmlConvertLineBreaks($strWhois);
			if ($strWhoisServer == "")
			{
				$strWhoisServer = "?TID_HOSTCHECK_ERROR_WHOIS-SERVER?";
			}
			$arrayTable[] = array("?TID_HOSTCHECK_LABEL_WHOIS-HOSTNAME?",$strWhoisServer);
			if ($strWhois != "")
			{
				$arrayTable[] = array("",$strWhois);
			}
			
			// IP whois
			if ($strHostIP != "")
			{
				$strWhoisServer = "";
				$strWhois = GetWhoisRecords($strHostIP,$strWhoisServer);
				$strWhois = SecureOutput($strWhois);
				$strWhois = HtmlConvertLineBreaks($strWhois);
				$arrayTable[] = array("?TID_HOSTCHECK_LABEL_WHOIS-IP?",$strWhoisServer);
				$arrayTable[] = array("",$strWhois);
			}		
  

			// DNS Records
			$arrayDnsRecords = GetDnsRecords($strHostName);
			if ($arrayDnsRecords != false)
			{
				$arrayTableDnsRecords = array();
				foreach ($arrayDnsRecords as $strType => $arrayDnsRecordOfType)
				{
					foreach ($arrayDnsRecordOfType as $arrayDnsRecord)
					{
						foreach ($arrayDnsRecord as &$vRecordItem)
						{
							if (is_array($vRecordItem))
							{
								$arrayTableRecordItem = ArrayTransformToTable($vRecordItem);
								$vRecordItem = $table->RenderTable($arrayTableRecordItem,false,true);
							}
						}
						$arrayTableDnsRecord = ArrayTransformToTable($arrayDnsRecord);
						$strTableDnsRecord = $table->RenderTable($arrayTableDnsRecord,false,true);
						$arrayTableDnsRecords[] = array(
							$strType,
							$strTableDnsRecord);
					}
				}
				$strTableDnsRecords = $table->RenderTable($arrayTableDnsRecords,false,true);
				$arrayTable[] = array("?TID_HOSTCHECK_LABEL_DNSRECORDS?",$strTableDnsRecords);
			}
			else
			{
				$arrayTable[] = array("?TID_HOSTCHECK_LABEL_DNSRECORDS?","?TID_HOSTCHECK_ERROR_DNSRECORDS?");
			}
			
			
			// HTTP output
			$strUrl = "";
			if ($strHostName != "")
			{
				$strURL = $strHostName;
			}
			else
			{
				$strURL = $strHostIP;	
			}
			
			$strHttpOutput = HttpRequest(
				"http://".$strURL,
				array(),
				"get",
				5,
				array(),
				array(),
				true);
			if ($strHttpOutput == false)
			{
				$strHttpOutput = "?TID_HOSTCHECK_ERROR_HTTP?";
			}
			$strHttpOutput = StringWordWrap($strHttpOutput,75,"\n",true);
			$strHttpOutput = SecureOutput($strHttpOutput);
			$strHttpOutput = HtmlConvertLineBreaks($strHttpOutput);
			$arrayTable[] = array("?TID_HOSTCHECK_LABEL_HTTP?",$strHttpOutput);
				
			
			
			// HTTPS output
			$strHttpsOutput = HttpRequest(
				"https://".$strURL,
				array(),
				"get",
				5,
				array(),
				array(),
				true);
			if ($strHttpsOutput == false)
			{
				$strHttpsOutput = "?TID_HOSTCHECK_ERROR_HTTPS?";
			} 
			$strHttpsOutput = StringWordWrap($strHttpsOutput,75,"\n",true);
			$strHttpsOutput = SecureOutput($strHttpsOutput);
			$strHttpsOutput = HtmlConvertLineBreaks($strHttpsOutput);
			$arrayTable[] = array("?TID_HOSTCHECK_LABEL_HTTPS?",$strHttpsOutput);
			
			
						
			$arrayCollection[] = array(
				"element" => "table",
				"headerfirstcol" => true,
				"data" => $arrayTable);

			
			$this->SetOutputItem( 
				"RESULT",
				$collection->RenderCollection($arrayCollection));
					
			return $this->StateStart();
		}
		

	}

