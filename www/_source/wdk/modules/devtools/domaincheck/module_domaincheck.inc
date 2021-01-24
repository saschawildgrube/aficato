<?php

	require_once(GetWDKDir()."wdk_module.inc");
	require_once(GetWDKDir()."wdk_ip.inc");
	
	class CDomainCheckModule extends CModule
	{
		
		private $m_arrayErrors;
		private $m_bCheckAgain;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			$website->LoadStringResources("domainstatus");
			parent::__construct(
				$website,
				"devtools/domaincheck",
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$this->m_arrayErrors = array();
			
			$arrayToolbarConfig = array(
				"start" => array("export","clear")
				); 
			
			$this->SetToolbarConfig($arrayToolbarConfig);
			
			$this->m_bCheckAgain = false;
			
			return parent::OnInit();
		}
		
	
		function OnRenderModule($strState)
		{
			$this->Trace("OnRenderModule(\"$strState\")");
			
			if ($this->GetWebsite()->GetSessionMode() == SESSION_MODE_NONE)
			{
				$message = new CMessageElement($this->GetWebsite(),"error");
				$this->SetOutputItem("MESSAGE",$message->RenderMessage("?TID_DOMAINCHECK_ERROR_SESSIONSUPPORT?")); 
				return "error";	
			}
			
			switch ($strState)
			{
/*
				case "exporttestcases":
					return $this->StateExportTestCases();
*/
				case "export":
					return $this->StateExport();
				case "clear":
					return $this->StateClear();
				case "check":
					return $this->StateCheck();
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
				$toolbarDef->AddTool($strLink, "help", "?TID_DOMAINCHECK_TOOL_HELP?");
			}
*/

/*
			if ($this->IsToolVisible("start"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "start";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "list", "?TID_DOMAINCHECK_TOOL_START?");
			}
*/

			if ($this->IsToolVisible("export"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "export";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "export", "?TID_DOMAINCHECK_TOOL_EXPORT?");
			}

			if ($this->IsToolVisible("clear"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "clear";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "delete", "?TID_DOMAINCHECK_TOOL_CLEAR?");
			}


			$toolbar = new CToolbarElement($this->GetWebsite(),"default");
			$this->SetOutputItem("TOOLBAR",$toolbar->RenderToolbar($toolbarDef)); 
		}		
		

		
		function GetDomains()
		{
			$strDomains = $this->GetModuleVarInsecure("domains");
			$strDomains = StringLowerCase($strDomains);
			$strDomains = StringTrim($strDomains);
			if ($strDomains == "")
			{
				return array();	
			}
			$arrayDomains = StringExplode($strDomains,"\n",true);
			$arrayDomains = ArrayUnique($arrayDomains);
			foreach ($arrayDomains as &$strDomain)
			{
				//$strDomain = StringFilter($strDomain,CHARSET_DOMAINNAME);	
				$strDomain = StringTrim($strDomain);
			}
			$arrayDomains = ArrayRemoveValue($arrayDomains,"");			
			
			$bWithoutHyphen = GetBoolValue($this->GetModuleVar("withouthyphen"));
			if ($bWithoutHyphen == true)
			{
				$arrayDomainsWithoutHyphen = array();
				foreach ($arrayDomains as $strDomain)
				{
					$strDomainWithoutHyphen	= StringRemoveCharacters($strDomain,"-");
					if (StringLength($strDomainWithoutHyphen) != StringLength($strDomain))
					{
						$arrayDomainsWithoutHyphen[]	= $strDomainWithoutHyphen;
					}
				}
				$arrayDomains = ArrayMerge($arrayDomains,$arrayDomainsWithoutHyphen);
			}
			$arrayDomains = ArraySort($arrayDomains);
			return $arrayDomains;
		}
		
		function GetTLDs()
		{
			$arrayDefault = array("com");
			$strTLDs = $this->GetModuleVarInsecure("tlds");	
			$strTLDs = StringTrim($strTLDs);
			if ($strTLDs == "")
			{
				return $arrayDefault;	
			}
			$strTLDs = StringLowerCase($strTLDs);
			$arrayTLDs = StringExplode($strTLDs,"\n",true);
			if (ArrayCount($arrayTLDs) == 0)
			{
				return $arrayDefault;
			}
			foreach ($arrayTLDs as &$strTLD)
			{
				$strTLD = StringFilter($strTLD,CHARSET_TLD);
			}
			return $arrayTLDs;
		}
		
		function GetResults()
		{
			$strResults = $this->GetModuleVar("results");
 			$arrayResults = ArrayUnpack($strResults);
 			if (!is_array($arrayResults))
 			{
 				return array();	
 			}
			$arrayResults = ArraySortByKeys($arrayResults,'CompareStringIgnoreHyphen');			
			return $arrayResults;
		}
		
		function SetResults($arrayResults)
		{
			if (!is_array($arrayResults))
			{
				$arrayResults = array();
			}
			$strResults = ArrayPack($arrayResults);
			$this->SetModuleVar("results",$strResults);
		}
		
		function CheckDomains()
		{
			$arrayResults = $this->GetResults();
			
			$arrayDomains = $this->GetDomains();
			$arrayTLDs = $this->GetTLDs();
			
			$nCount = 0;
			$nMaxChecks = 5;
			$this->m_bCheckAgain = false;
			foreach ($arrayDomains as $strDomain)
			{
				if ($nCount >= $nMaxChecks)
				{
					$this->m_bCheckAgain = true;
					break;	
				}
				foreach ($arrayTLDs as $strTLD)
				{
					if ($nCount >= $nMaxChecks)
					{
						$this->m_bCheckAgain = true;
						break;	
					}
					$strDomainName = $strDomain.".".$strTLD;
					
					$nStatus = ArrayGetValue($arrayResults,$strDomain,$strTLD,"status");
					if ($nStatus == DOMAIN_STATUS_UNKNOWN)
					{
						$nStatus = GetDomainStatus($strDomainName,true);
						$arrayResults[$strDomain][$strTLD]["status"] = $nStatus;
						$arrayResults[$strDomain][$strTLD]["datetime"] = time();
						$nCount++;
					}
				}
			}
			$this->SetResults($arrayResults);
		}


		function StateClear()
		{
			$this->SetResults(array());
			//$this->SetModuleVar("domains","");
			return $this->StateStart();
		}


		function StateCheck()
		{
			$arrayDomains = $this->GetDomains();
			if (ArrayCount($arrayDomains) == 0)
			{
				$this->m_arrayErrors["DOMAINS"] = "?TID_DOMAINCHECK_ERROR_DOMAINSMISSING?";
			}
			else if ($this->GetWebsite()->GetSessionMode() == SESSION_MODE_LOCAL_COOKIE)
			{
				$arrayTLDs = $this->GetTLDs();
				$nTotalNumber = ArrayCount($arrayDomains) * ArrayCount($arrayTLDs); 
				if ($nTotalNumber > 20)
				{
					$this->m_arrayErrors["DOMAINS"] = "?TID_DOMAINCHECK_ERROR_TOOMANYDOMAINS?";
				}
			}
			if (ArrayCount($this->m_arrayErrors) > 0)
			{
				return $this->StateStart();
			}
			$this->CheckDomains();
			
			if ($this->m_bCheckAgain == true)
			{
				$arrayParam = array();
				$arrayParams[$this->GetModuleVarNameState()] = "check";	
				$strURL = $this->GetWebsite()->MakeURL($arrayParams);
				$this->GetWebsite()->Redirect($strURL,0); 
				$this->AddInfoMessageCode("CHECKSINPROGRESS");
			}
			$this->GetWebsite()->MaintainScrollPosition();
			return $this->StateStart();
		}
		
		function StateStart()
		{
			$this->SetState("start");
			$this->RenderToolBar();

			
			$formDef = new CFormDefinition();
			
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "check";		

			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayParams);

			$formDef->AddControlStatic("STATIC","?TID_DOMAINCHECK_TEXT_CHECKDOMAINS?");
			
			$formDef->AddControlSeparator();
			
			$formDef->AddControlTextArea(
				"DOMAINS",
				$this->GetModuleVarName("domains"),
				"?TID_DOMAINCHECK_LABEL_DOMAINS?",
				$this->GetModuleVar("domains"),
				40,  
				6);
 
			$formDef->AddControlTextArea(
				"TLD",
				$this->GetModuleVarName("tlds"),
				"?TID_DOMAINCHECK_LABEL_TLDS?",
				$this->GetModuleVar("tlds"),
				10,
				6);

			$formDef->AddControlSeparator(); 

			$formDef->AddControlCheckbox(
				"WITHOUTHYPHEN",
				$this->GetModuleVarName("withouthyphen"),
				"?TID_DOMAINCHECK_LABEL_WITHOUTHYPHEN?",
				true);
				
			$formDef->AddControlSeparator();
			
			$formDef->AddControlButtonSubmit("?TID_DOMAINCHECK_BUTTON_CHECK?");
			
			$form = new CFormElement($this->GetWebsite(),"horizontal");
			
			$this->SetOutputItem( 
				"FORM",
				$form->RenderForm(
					$formDef,
					array(),					
					$this->m_arrayErrors));

						
			
			$arrayResults = $this->GetResults();
			
			if (ArrayCount($arrayResults) == 0)   
			{
				$this->SetOutputItem("RESULTS","");
			}
			else
			{
				$arrayTLDs = $this->GetTLDs();
			
				$arrayHeader = array("DOMAIN" => "?TID_DOMAINCHECK_TABLEHEADER_DOMAIN?");
				$arrayHeader = ArrayAppend($arrayHeader,$arrayTLDs);
	
				$arrayTable = array();
				
				$arrayTable[] = $arrayHeader;
				
				
				$this->IncludeElement("format");
				$formatFree = new CFormatElement($this->GetWebsite(),"green");
				$formatForSale = new CFormatElement($this->GetWebsite(),"blue");
				$formatTaken = new CFormatElement($this->GetWebsite(),"red");
				$linkDomain = new CLinkElement($this->GetWebsite(),"default");
				
				$arrayDomainStatus = GetDomainStatusArray();
				$bShortText = (ArrayCount($arrayTLDs) > 6)?(true):(false);   
				foreach ($arrayDomainStatus as $nKey => &$strValue)
				{
					$strPostFix = "";
					if ($bShortText == true)
					{
						$strPostFix = "_SHORT";
					}
					$strValue = $this->LoadString("TID_DOMAINSTATUS_".$strValue.$strPostFix);
					if ($bShortText == true)
					{
						$strValue = "_".$strValue."_"; 
					}
				}
				
				foreach($arrayResults as $strDomain => $arrayResult)
				{
					if ($strDomain == "")
					{
						continue;
					}
					$strDomain = HtmlEncode($strDomain);
					$arrayTableRow = array();
					$arrayTableRow[] = $strDomain;
					foreach ($arrayTLDs as $strTLD)
					{
						$nStatus = ArrayGetValue($arrayResult,$strTLD,"status");
	
						$strStatusHTML = "";					 
						if ($nStatus == DOMAIN_STATUS_FREE)
						{
							$strStatusHTML = $linkDomain->RenderLink(
								"http://www.".$strDomain.".".$strTLD,
								$formatFree->RenderFormat(ArrayGetValue($arrayDomainStatus,$nStatus)),
								true
								);
						}
						else if ($nStatus == DOMAIN_STATUS_TAKEN)
						{
							$strStatusHTML = $linkDomain->RenderLink(
								"http://www.".$strDomain.".".$strTLD,
								$formatTaken->RenderFormat(ArrayGetValue($arrayDomainStatus,$nStatus)),
								true);
						}
						else if ($nStatus == DOMAIN_STATUS_FORSALE)
						{
							$strStatusHTML = $linkDomain->RenderLink(
								"http://www.".$strDomain.".".$strTLD,
								$formatForSale->RenderFormat(ArrayGetValue($arrayDomainStatus,$nStatus)),
								true);
						}
						else if ($nStatus == DOMAIN_STATUS_INVALID)
						{
							$strStatusHTML = ArrayGetValue($arrayDomainStatus,$nStatus);
						}
						else
						{
							$strStatusHTML = ArrayGetValue($arrayDomainStatus,$nStatus);
						}						
						
						$arrayTableRow[] = $strStatusHTML;
					}
					
					$arrayTable[] = $arrayTableRow;
				}
			
				$table = new CTableElement($this->GetWebsite(),"default"); 
				
				$this->SetOutputItem( 
					"RESULTS",
					$table->RenderTable(
						$arrayTable,
						true,
						true)
					);
			}			
			
			
			
			/*
			
			$link = new CLinkElement($this->GetWebsite(),"default");
			$list = new CListElement($this->GetWebsite(),"default");
			
			$arrayList = array();

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "action1";
			$strLink = $this->GetWebsite()->MakeURL($arrayParams);
			$arrayList[] = $link->RenderLink($this->GetWebsite()->MakeURL($arrayParams),"?TID_DOMAINCHECK_TOOL_ACTION1?");

			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "action2";
			$strLink = $this->GetWebsite()->MakeURL($arrayParams);
			$arrayList[] = $link->RenderLink($this->GetWebsite()->MakeURL($arrayParams),"?TID_DOMAINCHECK_TOOL_ACTION2?");

			
			$this->SetOutputItem("LIST",$list->RenderList($arrayList));
			
			*/
			
			
			return "start";
		}
		
		function StateExport()
		{
			$arrayResults = $this->GetResults();
			if (ArrayCount($arrayResults) == 0)
			{
				$this->AddWarningMessageCode("NOTHINGTOEXPORT","DOMAINCHECK");
				return $this->StateStart();	
			}
			
			$arrayDomainStatus = GetDomainStatusArray();
			$arrayList = array();
			foreach ($arrayResults as $strDomain => $arrayTLDs)
			{
				$arrayItem = array();
				$arrayItem["DOMAIN"] = "".$strDomain;
				foreach ($arrayTLDs as $strTLD => $arrayResult)
				{
					$nStatus = ArrayGetValue($arrayResult,"status");
					
					$strStatus = ArrayGetValue($arrayDomainStatus,$nStatus);
					if ($strStatus != "")
					{
						$strStatus = $this->LoadString("TID_DOMAINSTATUS_".StringUpperCase($strStatus));	
					}
					$arrayItem["".StringUpperCase($strTLD)] = $strStatus;
				}
				$arrayList[] = $arrayItem;
			}
			
			$strCSV = $this->GetWebsite()->MakeCSV($arrayList);
			
			$strFileName = 'domains_'.RenderDateNow().'.csv'; 
			
			$this->GetWebsite()->DownloadFile(
				'text/csv',
				$strFileName,
				$strCSV);
		
			
			return $this->StateStart();	
		}
		
/*		
		function StateExportTestCases()
		{
			$arrayResults = $this->GetResults();
			if (ArrayCount($arrayResults) == 0)
			{
				$this->AddWarningMessageCode("NOTHINGTOEXPORT","DOMAINCHECK");
				return $this->StateStart();	
			}
			
			$arrayList = array();
			foreach ($arrayResults as $strDomain => $arrayTLDs)
			{
				foreach ($arrayTLDs as $strTLD => $arrayResult)
				{
					$arrayList[$strDomain.".".$strTLD] = ArrayGetValue($arrayResult,"status");
				}
			}
			
			$strPHP = "";
			foreach ($arrayList as $strURL => $strStatus) 
			{
				$strPHP .= '$this->TestCase_GetDomainStatus("'.$strURL.'",true,"'.$strStatus.'");'."\n";
			}
			
			$this->GetWebsite()->DownloadFile(
				"text/plain",
				"test.php",
				$strPHP);
			
			return $this->StateStart();	
		}
*/

	}

