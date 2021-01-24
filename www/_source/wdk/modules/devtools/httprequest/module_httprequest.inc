<?php

	require_once(GetWDKDir()."wdk_module.inc");
	require_once(GetWDKDir()."wdk_ip.inc");
	
	class CHttpRequestModule extends CModule
	{
		
		private $m_arrayErrors;
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"devtools/httprequest",
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$this->m_arrayErrors = array();
			
			//$arrayToolbarConfig = array();
			$arrayToolbarConfig = array(
				"start" => array("clear")
				);
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
				case "clear":
					return $this->StateClear();
				case "request":
					return $this->StateRequest();
				case "":
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

			if ($this->IsToolVisible("clear"))
			{
				$arrayParams = array();
				$arrayParams[$this->GetModuleVarNameState()] = "clear";
				$strLink = $this->GetWebsite()->MakeURL($arrayParams);
				$toolbarDef->AddTool($strLink, "undo", "?TID_HTTPREQUEST_TOOL_CLEAR?");
			}

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
			$this->SetState('start');
		
			$formDef = new CFormDefinition();
			
			$arrayFormParams = array();
			$arrayFormParams[$this->GetModuleVarNameState()] = 'request';		

			$formDef->SetURL($this->GetWebsite()->GetRootURL(),$arrayFormParams);

			$strURL = $this->GetModuleVar('url');
			$strMethod = $this->GetModuleVar('method');
			$strParams = $this->GetModuleVar('params');
			$bParamsRaw = GetBoolValue($this->GetModuleVar('paramsraw'));
			$strCookies = $this->GetModuleVar('cookies');
			$strHeaders = $this->GetModuleVar('headers');

			$formDef->AddControlText(
				'URL',
				$this->GetModuleVarName('url'),
				'?TID_HTTPREQUEST_LABEL_URL?',
				$strURL,
				100,500);   
			
			$formDef->AddControlSelect(
				'METHOD',
				$this->GetModuleVarName('method'),
				'?TID_HTTPREQUEST_LABEL_METHOD?',
				array(
					'get' => 'GET',
					'post' => 'POST'),
				$strMethod); 

			$formDef->AddControlSeparator();

			$formDef->AddControlTextArea(
				'PARAMS',
				$this->GetModuleVarName('params'),
				'?TID_HTTPREQUEST_LABEL_PARAMS?',
				$strParams,
				23,5); 
				
			$formDef->AddControlTextArea(
				'COOKIES',
				$this->GetModuleVarName('cookies'),
				'?TID_HTTPREQUEST_LABEL_COOKIES?',
				$strCookies,
				23,5); 

			$formDef->AddControlTextArea(
				'HEADERS',
				$this->GetModuleVarName('headers'),
				'?TID_HTTPREQUEST_LABEL_HEADERS?',
				$strHeaders,
				23,5);  

		
			$formDef->AddControlSeparator();

			$formDef->AddControlCheckbox(
				'PARAMSRAW',
				$this->GetModuleVarName('paramsraw'),
				'?TID_HTTPREQUEST_LABEL_PARAMSRAW?',
				$bParamsRaw);

			$formDef->AddControlSeparator();

			$formDef->AddControlButtonSubmit('?TID_HTTPREQUEST_BUTTON_REQUEST?');


			
			$form = new CFormElement($this->GetWebsite(),'horizontal');
			
			$this->SetOutputItem( 
				'FORM',
				$form->RenderForm(
					$formDef,
					array(),					
					$this->m_arrayErrors));
					
			$this->RenderToolBar();
			
			if ($this->GetOutputItem('RESULT') == '')
			{
				$this->SetOutputItem('RESULT','');	
			}	
			
			return 'start';
		}
		
		function StateRequest()
		{
			$strURL = $this->GetModuleVarInsecure('url');
			$strMethod = $this->GetModuleVar('method');
			$bParamsRaw = GetBoolValue($this->GetModuleVar('paramsraw'));

			if ($strMethod == 'post')
			{
				$strParams = $this->GetModuleVarInsecure("params");
				if ($bParamsRaw == true)
				{
					$arrayParams = $strParams;
				}
				else
				{
					$arrayParams = StringExplodeKeyValueList($strParams,"\n",'=');
				}
			}
			else
			{
				$arrayParams = array();
			}
			
			$strCookies = $this->GetModuleVarInsecure('cookies');
			$arrayCookies = StringExplodeKeyValueList($strCookies,"\n");

			$strHeaders = $this->GetModuleVarInsecure('headers');
			$arrayHeaders = StringExplodeKeyValueList($strHeaders,"\n",': ');
			
			if (IsValidURL($strURL) == false)
			{
				$strURL = "http://".$strURL;
				if (IsValidURL($strURL) == false)
				{
					$this->m_arrayErrors['URL'] = '?TID_HTTPREQUEST_ERROR_URL_INVALID?';
					return $this->StateStart();	
				}
			}
			
			$stopwatch = new CStopWatch();
			$stopwatch->Start();
			$strResponse = HttpRequest(
				$strURL,
				$arrayParams,
				$strMethod,
				10,
				$arrayCookies,
				$arrayHeaders,
				true);
			$stopwatch->Stop();
				
			if ($strResponse == false)
			{
				$message = new CMessageElement($this->GetWebsite(),"error");
				$this->SetOutputItem(
					"RESULT",
					$message->RenderMessage("?TID_HTTPREQUEST_ERROR_REQUEST_FAILED?"));
				return $this->StateStart();				
			}
			
			$arrayResponse = ParseHttpResponseEx($strResponse);
			
			$arrayCollection = array();
			$arrayTable = array();

			$arrayTable[] = array("?TID_HTTPREQUEST_LABEL_URL?",
				HtmlConvertLineBreaks(
					HtmlEncode(
						StringWordWrap($strURL, 75, "\n", true)
						)
					)
				);
			$arrayTable[] = array("?TID_HTTPREQUEST_LABEL_METHOD?",
				StringUpperCase($strMethod)
				);
				
			if ($strMethod == "post")
			{
				$arrayTable[] = array("?TID_HTTPREQUEST_LABEL_PARAMS?",
					HtmlConvertLineBreaks(
						HtmlEncode($strParams))
					);				
			}

			if ($strCookies != "")
			{
				$arrayTable[] = array("?TID_HTTPREQUEST_LABEL_COOKIESSENT?",
					HtmlConvertLineBreaks(
						HtmlEncode($strCookies))
					);				
			}
	
			if ($strHeaders != "")
			{
				$arrayTable[] = array("?TID_HTTPREQUEST_LABEL_HEADERSSENT?",
					HtmlConvertLineBreaks(
						HtmlEncode($strHeaders))
					);
			}


			$arrayTable[] = array("?TID_HTTPREQUEST_LABEL_RESPONSE?",
				$this->GetWebsite()->RenderNumber($stopwatch->GetSeconds(),3)
				);
			
						
			$arrayTable[] = array("?TID_HTTPREQUEST_LABEL_HTTPSTATUS?",
				HtmlEncode(
					ArrayGetValue($arrayResponse,"statuscode")
				));
					
			$arrayTable[] = array("?TID_HTTPREQUEST_LABEL_HEADERSRECEIVED?",
				HtmlConvertLineBreaks(
					HtmlEncode(ArrayGetValue($arrayResponse,"headers"))
				));




			$strCookiesLabel = '?TID_HTTPREQUEST_LABEL_COOKIESRECEIVED?';
			$arrayCookies = ArrayGetValue($arrayResponse,'cookies');
			if (ArrayCount($arrayCookies) > 0)
			{
				$strCookies = "";
				foreach ($arrayCookies as $strName => $strValue)
				{
					$strCookies .= $strName."=".$strValue."\n";
				}
				$arrayTable[] = array(
					$strCookiesLabel,
					HtmlConvertLineBreaks(
						HtmlEncode($strCookies))
					);
			}
			else
			{
				$arrayTable[] = array(
					$strCookiesLabel,
					'?TID_HTTPREQUEST_INFO_NOCOOKIES?');
			}



			$strRedirectLocation = ArrayGetValue($arrayResponse,"redirect-location");
			if ($strRedirectLocation != "")
			{
				$link = new CLinkElement($this->GetWebsite(),"default");
				if (IsValidURL($strRedirectLocation))
				{
					$strRedirectLocationHtml = $link->RenderLink($strRedirectLocation,"",true);
				}
				else
				{
					$strRedirectLocationHtml = HtmlEncode($strRedirectLocation);
				}
				$arrayTable[] = array("?TID_HTTPREQUEST_LABEL_REDIRECTLOCATION?",$strRedirectLocationHtml);
			}



			$arrayLinks = ArrayGetValue($arrayResponse,"links");
			$link = new CLinkElement($this->GetWebsite(),"default");
			$arrayLinksHTML = array();
			if (ArrayCount($arrayLinks) > 0)
			{
				foreach ($arrayLinks as $strLinkUrl)
				{
					$strLinkUrlText = HtmlConvertLineBreaks(StringWordWrap($strLinkUrl,75,"\n",true));
					if (IsValidURL($strLinkUrl))
					{
						$arrayLinksHTML[] = array(
							$link->RenderLink($strLinkUrl,$strLinkUrlText,true));
					}
					else
					{
						$arrayLinksHTML[] = array($strLinkUrlText);
					}
				}
			}
			if (ArrayCount($arrayLinksHTML) > 0)
			{
				$table = new CTableElement($this->GetWebsite(),"default");
				$arrayTable[] = array("?TID_HTTPREQUEST_LABEL_LINKS?",
					$table->RenderTable($arrayLinksHTML));
			}


/*
			$arrayTable[] = array(
				"Forms",
				HtmlEncode(ArrayRender(ArrayGetValue($arrayResponse,"forms")))
				);
*/	

			$strHeaders = ArrayGetValue($arrayResponse,"headers");
			$strContent = ArrayGetValue($arrayResponse,"content");
			$strContentLabel = "?TID_HTTPREQUEST_LABEL_CONTENT?";
			
			if (FindString($strHeaders,"Content-Encoding: gzip") != -1)
			{
				$strContent = gzdecode($strContent);
				$strContentLabel = "?TID_HTTPREQUEST_LABEL_CONTENTDECOMPRESSED?";
			}


			
			$arrayTable[] = array(
				$strContentLabel,
				HtmlConvertLineBreaks(
					HtmlEncode(
						StringWordWrap(
							$strContent, 75, "\n", true
							)
						)
					)
				);
			
	

			

						
			$arrayCollection[] = array(
				"element" => "table",
				"headerfirstcol" => true,
				"data" => $arrayTable);

			$this->IncludeElement("collection");
			$collection = new CCollectionElement($this->GetWebsite(),"default");
			
			$this->SetOutputItem( 
				"RESULT",
				$collection->RenderCollection($arrayCollection));
					
			return $this->StateStart();
		}
		
		function StateClear()
		{
			$this->SetModuleVar("url","");
			$this->SetModuleVar("method","get");
			$this->SetModuleVar("params","");
			$this->SetModuleVar("cookies","");
			$this->SetModuleVar("headers","");
			
			return $this->StateStart();
			
		}
		
		

	}

