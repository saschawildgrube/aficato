<?php
 
	require_once('wdk_webapplication.inc');

	require_once('wdk_mail.inc');
	require_once('wdk_encryption-symmetric.inc');
	require_once('wdk_passwordpolicy.inc');
	require_once('wdk_language.inc');
	require_once('wdk_locale.inc');
	require_once('wdk_country.inc');
	require_once('wdk_currency.inc');
	require_once('wdk_datetime.inc');
	require_once('wdk_localresources.inc');
	require_once('wdk_routing.inc');
	require_once('wdk_websitesatellite.inc');
	require_once('wdk_assemblyline.inc');
	require_once('wdk_plugin_website.inc');
	require_once('wdk_element.inc');
	require_once('wdk_module.inc');
	require_once('wdk_content.inc'); 
	require_once('wdk_content_login.inc');
	require_once('wdk_content_admin.inc');
	require_once('wdk_content_systemadmin.inc');
	require_once('wdk_content_cmsadmin.inc');
	require_once('wdk_content_devtools.inc');
	require_once('wdk_csv.inc');
	require_once('wdk_datetime.inc');
	require_once('wdk_w3c.inc');
	require_once('wdk_html.inc');
	require_once('wdk_webserviceconsumer_webapplication.inc');
	require_once('wdk_quality.inc');
	require_once('wdk_httpdownload.inc');
	require_once('wdk_cookie.inc');
	require_once('wdk_session.inc');
	require_once('wdk_bot.inc');
	require_once('wdk_layout.inc');
	require_once('wdk_navigationtree.inc');

	
	abstract
	class CWebSite extends CWebApplication
	{
		private $m_strSourceDir;
		private $m_strWWWRootDir;
		private $m_strStyleSheetsDir;
		private $m_strJavaScriptsDir;
		private $m_strImagesDir;
		private $m_strFontsDir;
		private $m_strVideosDir;
		private $m_strDownloadDir;
		
		private $m_strRootURLnoprotocol;
		private $m_strRootURL;
		private $m_strCurrentURL;
		private $m_strProtocol;

		private $m_arraySession;
		private $m_arrayParams;
		private $m_arrayVars;
		private $m_arrayCookies;
		
		private $m_arrayProtocols;
		
		private $m_strErrorMessageCode;
		private $m_strWarningMessageCode;
		private $m_strInfoMessageCode;

		private $m_arrayMessages;
		
		private $m_arrayHeaders;
		private $m_arrayJavaScripts;
		private $m_arrayStyleSheets;

		private $m_bMonolingual;
		private $m_bFallbackLanguageSupported;
	
		private $m_res;
		private $m_bUseRes;
		private $m_arrayLoadedStringResources;
		private $m_arrayResText;
    
    private $m_strFrameworkID;
    
		private $m_arrayOutputItems;
		private $m_arrayBodyItems;
		private $m_arrayGenericOutputItems;
		private $m_arrayLayoutConfigItems;
		
		private $m_bTraceActive;
		private $m_tracelog;
		
		private $m_bProfiling;
		
		private $m_nSessionMode;
		private $m_strSessionID;
		
		private $m_bSubsequentRequest;
		
		private $m_strOldCSRFToken;

		private $m_bAuthenticationSupport;
		private $m_strAuthUserID;
		private $m_arrayPrivileges;
		
		private $m_arrayConditions;
		
		private $m_bCheckUnresolvedTID;
		private $m_bCheckInvalidTID;
		
		private $m_bModuleRegistrationCompleted;
		
		private $m_assemblyRoot;
		private $m_assemblyWDK;
		private $m_assemblyline;
		private $m_bOnInitAssemblyLineCalled;
		
		private $m_arrayPlugins;
		
		private $m_routing;
		private $m_bOnInitRoutingMapCalled;
		
		private $m_bOnInit;
		
		private $m_arrayDownloadFile;
		
		private $m_elementIcon;
		private $m_elementLink;
		private $m_elementTel;
		private $m_elementSourceCode;
		private $m_elementYouTube;
		private $m_elementCountDown;
		
		private $m_arrayDynamicNavigationTrees;
		
		function __construct(
			$strRootURL,
			$strSourceDir,
			$strWWWDir,
			$arrayMeta,
			$arrayConfig,
			$arrayParams,
			$arrayObfuscationItems = array()
			)
		{
			parent::__construct(
				$arrayMeta,
				$arrayConfig,
				$arrayObfuscationItems);
	
			// Init Tracing	
			$this->SetTracePrefix('WEBSITE: ');
			$this->TraceVersionWDK();
			$this->Trace('Environment: '.GetEnvID());
			$this->Trace('PHP Version: '.phpversion());
			$this->Trace('Server IP  : '.$this->GetServerIP());
			$this->Trace('Client IP  : '.$this->GetClientIP());
			$this->Trace('User Agent : '.$this->GetUserAgent());

			// Performace measurement
			$this->m_bProfiling = true;
			
			// Check Environment
			if (CheckEnvironment() == false)
			{
				$this->Error('CheckEnvironment returned false. Check out env.inc if all environemnt functions have been defined!');
			}
			
			// Plugin array
			$this->m_arrayPlugins = array();
			
			// Subsequent request detection
			$this->m_bSubsequentRequest = false;

			// Module Registration
			$this->m_bModuleRegistrationCompleted = false;
			
			// Download File
			$this->m_arrayDownloadFile = array();
			
			// Dynamic Navigation Tree
			$this->m_arrayDynamicNavigationTree = array();
			
			// Output Items
			$this->m_arrayOutputItems = array();
			
			// Body Items (items that are placed directly before the closing body tag)
			$this->m_arrayBodyItems = array();
			
			// Layout Config Items
			$this->m_arrayLayoutConfigItems = array();

			// Init Generic Output Items
			$this->m_arrayGenericOutputItems = array();
			$this->RegisterGenericOutputItem('URL',array('content','language'));
			$this->RegisterGenericOutputItem('ICON',array('id'));
			$this->RegisterGenericOutputItem('IMAGEURL',array('id','ext','context'));
			$this->RegisterGenericOutputItem('FONTURL',array('id','context'));
			$this->RegisterGenericOutputItem('VIDEOURL',array('id','context'));
			$this->RegisterGenericOutputItem('IMAGE',array('id','ext','style','text'));
			$this->RegisterGenericOutputItem('TEASER',array('title','text','style','linkurl','linktext','externallink'));
			$this->RegisterGenericOutputItem('IMAGETEASER',array('title','text','style','linkurl','linktext','imageurl','externallink'));
			$this->RegisterGenericOutputItem('LINK',array('url','content','text','style','externallink'));
			$this->RegisterGenericOutputItem('PHONE',array('tel','text'));
			$this->RegisterGenericOutputItem('DOWNLOADURL',array('filepath'));
			$this->RegisterGenericOutputItem('YOUTUBE',array('id'));
			$this->RegisterGenericOutputItem('PORTFOLIO',array(
				'payload1','title1','text1','linktext1','linkurl1','imageurl1','external1',
				'payload2','title2','text2','linktext2','linkurl2','imageurl2','external2',
				'payload3','title3','text3','linktext3','linkurl3','imageurl3','external3',
				'payload4','title4','text4','linktext4','linkurl4','imageurl4','external4',
				'payload5','title5','text5','linktext5','linkurl5','imageurl5','external5',
				'payload6','title6','text6','linktext6','linkurl6','imageurl6','external6',
				'payload7','title7','text7','linktext7','linkurl7','imageurl7','external7',
				'payload8','title8','text8','linktext8','linkurl8','imageurl8','external8',
				'payload9','title9','text9','linktext9','linkurl9','imageurl9','external9',
				'payload10','title10','text10','linktext10','linkurl10','imageurl10','external10',
				'payload11','title11','text11','linktext11','linkurl11','imageurl11','external11',
				'payload12','title12','text12','linktext12','linkurl12','imageurl12','external12',				
				'payload13','title13','text13','linktext13','linkurl13','imageurl13','external13',				
				'payload14','title14','text14','linktext14','linkurl14','imageurl14','external14',				
				'payload15','title15','text15','linktext15','linkurl15','imageurl15','external15',				
				'payload16','title16','text16','linktext16','linkurl16','imageurl16','external16',				
				'payload17','title17','text17','linktext17','linkurl17','imageurl17','external17',				
				'payload18','title18','text18','linktext18','linkurl18','imageurl18','external18',				
				'payload19','title19','text19','linktext19','linkurl19','imageurl19','external19',				
				'payload20','title20','text20','linktext20','linkurl20','imageurl20','external20',				
				'style'));
			$this->RegisterGenericOutputItem('COUNTDOWN',array('datetime','timezone'));
			

			// Init Timezone Default
			$this->SetTimeZone('UTC');

			// Initialize Meta
			if ($this->GetMeta('version') == '')
			{
				$this->Error("Meta: No version specified.");
			}
			if ($this->GetMeta("author") == "")
			{
				$this->Error("Meta: No author specified.");
			}
			if ($this->GetMeta("copyright") == "")
			{
				$this->Error("Meta: No copyright specified.");
			}

			// Manage Cookies and parameters
			$this->m_arrayCookies = SecureParameterArray($_COOKIE);
			foreach ($this->m_arrayCookies as &$strCookieValue) 
			{
				$strCookieValue = StringURLDecode($strCookieValue);
			}
						
			$this->m_arrayParams = PrepareParameterArray();
			
			LockParameterSources();			
			
			$this->m_arrayParams = ArrayMerge($this->m_arrayParams,$arrayParams);
			$paramDef = new CParameterDefinition();
			$paramDef->AddParameterDefinitionString("info",3,40,CHARSET_TID,false);
			$paramDef->AddParameterDefinitionString("warning",3,40,CHARSET_TID,false);
			$paramDef->AddParameterDefinitionString("error",3,40,CHARSET_TID,false);
			$arrayParamErrors = array();
			if ($paramDef->CheckParameters($this->m_arrayParams, $arrayParamErrors, true) > 0)
			{
				$this->Trace("Parameters:");
				$this->Trace($this->m_arrayParams);
				$this->Trace("CheckParameters reported errors:");
				$this->Trace($arrayParamErrors);
				$this->Error("Parameter Check Failed");
			}

			// Set Tracing by Parameter
			$this->SetTraceActive(false);
			if (GetBoolValue($this->GetParam('trace')) == true)
			{
				$this->SetTraceActive(true);
				$this->Trace("Trace activated by parameter.");
			}
			
			
			// Set Caching by parameter
			if ($this->IsCacheActive() == true)
			{
				if ($this->GetConfigBoolean("cache_allow_nocache") == true)
				{
					if ($this->GetParam("nocache") == true)
					{
						$this->SetCacheActive(false);	
					}	
				}	
			}
			 
			$this->Trace("Caching is ".(($this->IsCacheActive())?("ACTIVE"):("INACTIVE")));
			
			
			// Initialize Variables - First Time
			$this->m_arrayVars = $this->m_arrayParams;

			
			// Init Messages
			$this->m_arrayMessages = array();
			$this->m_arrayMessages["info"] = array();
			$this->m_arrayMessages["warning"] = array();
			$this->m_arrayMessages["error"] = array();

			
			// Manage directories
			$this->m_strSourceDir = $strSourceDir;
			$this->m_strWWWDir = $strWWWDir;
			if (StringSection($this->m_strWWWDir,-1) != "/")
			{
				$this->m_strWWWDir .= "/";
			}
			$this->m_strStyleSheetsDir = 	$strWWWDir . "css/";
			$this->m_strJavaScriptsDir = 	$strWWWDir . "js/";
			$this->m_strImagesDir = 			$strWWWDir . "images/";
			$this->m_strFontsDir = 				$strWWWDir . "fonts/";
			$this->m_strVideosDir = 			$strWWWDir . "videos/";
			$this->m_strDownloadDir = 		$strWWWDir . "download/";

			// Manage Protocols
    	$this->m_strProtocol = "http";
			if (ArrayKeyExists($_SERVER,"HTTPS"))
			{
				if ($_SERVER["HTTPS"] == true)
				{
					$this->m_strProtocol = "https";
				}
			}
			$arrayProtocols = $this->GetConfig("protocols");
			if ((!is_array($arrayProtocols)) && (ArrayCount($arrayProtocols) < 1))
			{
				$this->Error("Config: No supported protocol specified.");
			}
			if (ArrayValueExists($arrayProtocols,$this->m_strProtocol) == false)
			{
				$strTargetURL = $arrayProtocols[0] . "://". $_SERVER["SERVER_NAME"] . $_SERVER["REQUEST_URI"];
				RedirectAndExit($strTargetURL);
			}
			$this->m_arrayProtocols = $arrayProtocols;
			
						
			// Manage Root URL
			if ($strRootURL == "")
			{
				$this->Error("strRootURL must not be empty.");		
			}
			if (strstr($strRootURL,":"))
			{
				$this->Error("strRootURL (\"".$strRootURL."\") must be specified without protocol.");	
			}
			$this->m_strRootURLnoProtocol = $strRootURL;
			$this->Trace("Root URL (without protocol): ".$this->m_strRootURLnoProtocol);
			$strProtocol = "http://";
			if ($this->m_strProtocol == "https")
			{
				$strProtocol = "https://";
			}
			$this->m_strRootURL = $strProtocol . $this->m_strRootURLnoProtocol;
			$this->Trace("Root URL: ".$this->m_strRootURL);



			// Manage current URL
			$this->m_strCurrentURL = $strProtocol . $_SERVER["SERVER_NAME"] . $_SERVER["REQUEST_URI"];
			$this->Trace("Current URL: ".$this->m_strCurrentURL);

			
			// Initialize Authentication Support
			$this->m_bAuthenticationSupport = $this->GetConfigBoolean("authentication_support");
			$this->m_strAuthUserID = "";
			$this->m_arrayPrivileges = array();
			
			// Conditions
			$this->m_arrayConditions = array();

			// Initialize Additional Headers
			$this->m_arrayHeaders = array();
			
			// Initialize List of java scripts
			$this->m_arrayJavaScripts = array();

			// Initialize List of additional style sheet files
			$this->m_arrayStyleSheets = array();

			
			// Initialize elements
			$this->m_elementIcon = null;
			$this->m_elementLink = null;
			$this->m_elementTel = null;
			$this->m_elementSourceCode = null;
			$this->m_elementYouTube = null;



			// Manage SEO URLs
			$strSEOURL = '';
			$strSEOURLParam = $this->GetConfig('seourl');
			if ($strSEOURLParam != '')
			{
				$strSEOURL = $this->GetParam($strSEOURLParam);
				$strSEOURL = StringRemoveTrailingCharacters($strSEOURL,'/');

				// This is to compensate deviating behaviour in Chrome Browser
				// (bling engine?) which stores sent cookie data when requesting the
				// favicon.ico.
				// All other browsers (IE, Edge, Safari, Firefox) don't do that.
				if ($strSEOURL == 'favicon.ico')
				{
					// This means the favicon.ico does not exist and
					// we should not interpret 'favicon.ico' as a SEO
					// URL at all
					$strSEOURL = '';
					$this->SetVar($strSEOURLParam,'');
					
					// And we mark this request as a subsequent request:
					$this->SetSubsequentRequest();
					
					// And we prepare the rendering/loading of the favicon by
					// turning this into an image command request
					$this->SetParam("command","image");
					$this->SetParam("id","favicon");
				}
				
				// The actual parsing of the SEO URL is done later, after the session
				// is initialized.
			}
			
			

			// Rendering the sitemap.xml does not require session support
			if ($this->GetCommand() == 'sitemap')
			{
				$this->SetSubsequentRequest();
			}



			// Manage Session
			$this->m_strSessionID = "";
			$this->m_nSessionMode = intval($this->GetConfig("session_mode"));
			if (IsValidSessionMode($this->m_nSessionMode) != true)
			{
				$this->m_nSessionMode = SESSION_MODE_NONE;
			}


			$bSessionCreationError = false;
			$this->m_arraySession = array();
			if ($this->GetSessionMode() != SESSION_MODE_NONE)
			{
				$bCreateSession = true;
				$this->m_strSessionID = $this->ExtractSessionID();
				if ($this->m_strSessionID != "")
				{
					$this->Trace("SessionID: ".$this->m_strSessionID);
					if ($this->OnSessionRead($this->m_strSessionID,$this->m_arraySession) == true)
					{
						$bCreateSession = false;
					}
				}
				if ($this->IsSubsequentRequest() == true)
				{
					$bCreateSession = false;
				}
				if ($bCreateSession == true)
				{
					$this->m_strSessionID = $this->OnSessionCreate();
					if ($this->m_strSessionID == "")
					{
						$bSessionCreationError = true;
					}
					else
					{
						$this->DeploySessionID($this->m_strSessionID);
					}
				}
			}


			$bActivateTrace = false;
			
			
		
			// Set Tracing by Session (if it's not deactivated by parameter)
			if (GetBoolValue(ArrayGetValue($this->m_arraySession,'trace')) == true)
			{
				if ((!(ArrayKeyExists($this->m_arrayParams,'trace') == true) && (GetBoolValue(ArrayGetValue($this->m_arrayParams,'trace')) == false)))
				{
					$this->SetTraceActive(true);
					$this->Trace("Trace activated by session.");
				}
			}
			
			// Initialize Variables - Second time
			$this->m_arrayVars = ArrayMerge($this->m_arraySession,$this->m_arrayParams);
			
			
			// CSRF Protection
			$bCSRFInvalidWarning = false;
			if ($this->IsCSRFProtectionActive())
			{
				if ($this->GetSessionMode() != SESSION_MODE_NONE)
				{
					$this->Trace('CSRF Protection is active!');
					
					// Do not apply CSRF protection in case of subsequent requests
					if ($this->IsSubsequentRequest() == true)
					{
						$this->Trace('CSRF protection is NOT APPLIED for subsequent requests!');
					}
					else
					{
						$bCSRFValid = $this->ExtractAndVerifyCSRFToken();
						if ($bCSRFValid == false)
						{
							$bCSRFInvalidWarning = true;
							$this->Trace('CSRF validation failed. User is now logged out!');
							$this->UserLogout();
							$this->m_arrayVars = array();
						}
						$this->DeployNewCSRFToken();
					}
				}
			}
			
		
			
			// Initialize Assembly Line
			$this->m_assemblyRoot = new CAssembly($this,"root",$strSourceDir);
			$this->m_assemblyWDK = new CAssembly($this,"wdk",GetWDKDir());
			$this->m_assemblyline = new CAssemblyLine($this);
			$this->m_assemblyline->AddAssembly($this->m_assemblyRoot);
			$this->m_bOnInitAssemblyLineCalled = false;
			$this->OnInitAssemblyLine($this->m_assemblyline);
			if ($this->m_bOnInitAssemblyLineCalled != true)
			{
				$this->Error("OnInitAssemblyLine in a derived class must invoke the parent instance function.");	
			}
			$this->m_assemblyline->AddAssembly($this->m_assemblyWDK);
					
			// Initialize Plugins
			$this->m_arrayPlugins = $this->OnInitPlugins();
			if (IsArray($this->m_arrayPlugins) == false)
			{
				FatalError('OnInitPlugins must return an array!');	
			}
			foreach ($this->m_arrayPlugins as $plugin)
			{
				if (get_parent_class($plugin) != 'CWebsitePlugin')
				{
					FatalError('All plugins must be derived from CWebsitePlugin!');	
				}
			}
	
			// Render Image
			if ($this->GetCommand() == "image")
			{
				$this->RenderImage();
				// RenderImage calls exit in all cases!				
			}			

			// Render Font
			if ($this->GetCommand() == "font")
			{
				$this->RenderFont();
				// RenderFont calls exit in all cases!
			}			

			// Render Video
			if ($this->GetCommand() == "video")
			{
				$this->RenderVideo();
				// RenderVideo calls exit in all cases!
			}			


			// Render Css
			if ($this->GetCommand() == "css")
			{
				$this->RenderCss();
				// RenderCss calls exit in all cases!	
			}

			// Handle Download
			if ($this->GetCommand() == "download")
			{
				$this->RenderDownload();
				// RenderDownload does not exit!
			}	
		
			$this->Trace("RenderDateTimeNow() = \"".RenderDateTimeNow()."\""); 
			
			$this->Trace('Configuration:');
			$this->Trace($this->GetConfigArray());

			$this->Trace('Cookies:');
			$this->Trace($this->GetAllCookies());
			
			$this->Trace('Parameters:');
			$this->Trace($this->m_arrayParams);


			if ($this->GetSessionMode() != SESSION_MODE_NONE)
			{
				$this->Trace('Session Data:');
				$this->Trace($this->m_arraySession);  
			} 
			 
			$this->Trace('Variables:');
			$this->Trace($this->GetAllVars());

			

      



			// Manage Local Resources
			$this->m_bUseRes = true;
			if ($this->GetVar('nores')=='1')
			{
				$this->m_bUseRes = false;
			}
			$this->m_res = new CLocalResources();
			$this->m_arrayResText = array();
			$this->m_arrayLoadedStringResources = array();



			// Manage Language
			$arrayLanguages = $this->GetSupportedLanguages();
			if (!is_array($arrayLanguages) || (ArrayCount($arrayLanguages) < 1))
			{
				$this->Error("Config: At least one supported language must be specified.");
			}
			if (ArrayCount($arrayLanguages) == 1)
			{
				$this->m_bMonolingual = true;
			}
			else
			{
				$this->m_bMonolingual = false;
			}
			foreach ($arrayLanguages as $strLanguage)
			{
				if (IsValidLanguageID($strLanguage) == false)
				{
					$this->Error("Config: \"$strLanguage\" is not a valid language id.");
				}
			}
			$this->SetLanguage(GetLanguage($this->GetVar("language")));
			
			// Manage fallback language
			$this->m_bFallbackLanguageSupported = false;
			$strFallbackLanguage = $this->GetFallbackLanguage();			
			if ($strFallbackLanguage != "")
			{
				if (IsValidLanguageID($strFallbackLanguage) == false)
				{
					$this->Error("Config: An invalid language id has been specified for the fallback language: \"$strLanguage\"");
				}
				else
				{
					$this->m_bFallbackLanguageSupported = true;	 
				}
			}
			
			
			

			// if the requested language is not in the list of supported languages...
			if (ArrayValueExists($arrayLanguages,$this->GetLanguage()) == false)
			{
				// ...set default language from config
				$this->SetLanguage($arrayLanguages[0]);
			}
			
			$this->Trace("GetLanguage = ".$this->GetLanguage());
			
			// Manage Country
			$strDefaultCountry = $this->GetConfig("country");

			// Manage Message supplied by parameter
			$this->m_strErrorMessageCode = $this->GetVar("error");
			$this->SetVar("error","");
			$this->m_strWarningMessageCode = $this->GetVar("warning");
			$this->SetVar("warning","");
			$this->m_strInfoMessageCode = $this->GetVar("info");
			$this->SetVar("info","");

			

			// Manage Framework
			$this->SetFramework($this->GetVar("framework"));
			if ($this->GetFramework() == "")
			{
				$this->SetFramework("default");
			}


			// Manage Layout
			$strLayout = $this->GetLayout();
			if ($strLayout == "")
			{
				$strLayout = "default";
			}
			$this->SetLayout($strLayout);
			
				         
			// Manage area and content
			if ($this->GetArea() == "")
			{
				$this->SetArea("default");
			}
			
			if ($this->GetContent() == "")
			{
				$this->SetContent("index");
			}
			

			// Manage Routing Maps
			$this->m_routing = new CRoutingMap();
			$this->m_bOnInitRoutingMapCalled = false;
			$this->OnInitRoutingMap();
			if ($this->m_bOnInitRoutingMapCalled != true)
			{
				$this->Error("OnInitRoutingMap in a derived class must invoke the parent instance function.");	
			}

		
			// Render Sitemap
			if ($this->GetCommand() == "sitemap")
			{
				$this->RenderSitemap();
				// RenderSitemap calls exit in all cases!	
			}
			
			
			// Interpret SEO URLs
			if ($strSEOURL != '')
			{
				$this->SetVar($strSEOURLParam,'');
				$this->OnParseSEOURL($strSEOURL);
			}

			
			// Generate URL to validate the current page
			$strValidatorURL = w3cGetValidatorURL($this->GetCurrentURL()); 
			$this->SetOutputItem("W3CLINK",$strValidatorURL);

			
			
			// Manage unresolved TID tag check
			$this->m_bCheckUnresolvedTID = false;
			if (GetBoolValue($this->GetConfig("check_unresolvedtid")) == true)
			{
				$this->m_bCheckUnresolvedTID = true;
				$this->Trace("Unresolved TID check activated by config.");
			}

			// Manage invalid TID tag check
			$this->m_bCheckInvalidTID = false;
			if (GetBoolValue($this->GetConfig("check_invalidtid")) == true)
			{
				$this->m_bCheckInvalidTID = true;
				$this->Trace("Invalid TID check activated by config.");
			}

		
			// Initialize some output items
			$this->SetOutputItem("DATETIME","");
			$this->SetOutputItem("USERSTATUS","");
			$this->SetOutputItem("LANGUAGESELECTION","");
			$this->SetOutputItem("MODULE_LOGIN_FORM","");
		
		
			$this->LoadStringResources('wdk');
			// WDK Ressources have lowest priority
			
			$this->LoadStringResources('default');
			// This is because getmessage directly loads an error text and OnInit might render modules 

			// Initialize "powered by"
			$this->SetOutputItem("POWEREDBY","");

			// Initialize standard time items
			$strCurrentYear = RenderDateTimeFormat(time(),"%Y",$this->GetTimeZone(),$this->GetLanguage());

			// Initialize WDK Documentation related items
			$this->SetOutputItem("WDK","WDK");
			$this->SetOutputItem("WDK_AUTHOR","Sascha Wildgrube");
			$this->SetOutputItem("WDK_COPYRIGHT_PERIOD",WDK_COPYRIGHT_SINCE."-".$strCurrentYear);
			$this->SetOutputItem("WDK_DOCS_URL","http://www.websitedevkit.com/");
			$this->SetOutputItem("WDK_SCAFFOLDER_URL","http://www.websitedevkit.com/scaffolder/");
			$this->SetOutputItem("WDK_SYSTEMREQUIREMENT_PHP_VERSION",'7.2.24');
			$this->SetOutputItem("WDK_SYSTEMREQUIREMENT_UBUNTU_VERSION",'18.04');
			$this->SetOutputItem("WDK_SYSTEMREQUIREMENT_APACHE_VERSION",'2.4.29');
			$this->SetOutputItem('WDK_SYSTEMREQUIREMENT_MARIADB_VERSION','10.1.38');
			
		
			// Include standard elements
			// The ''.' is to defuse the deprecation test
			$this->IncludeElement(''.'icon');
			$this->IncludeElement(''.'link');
			$this->IncludeElement(''.'linkicon');
			$this->IncludeElement(''.'list');
			$this->IncludeElement(''.'navigation');
			$this->IncludeElement(''.'message');
			$this->IncludeElement(''.'messagestack');
			$this->IncludeElement(''.'form');
			$this->IncludeElement(''.'toolbar');
			$this->IncludeElement(''.'table');

		
			// Initialize Logo
			$linkLogo = new CLinkElement($this,'logo');
			$this->SetOutputItem('LOGO',$linkLogo->RenderLink($this->GetRootURL(),'?TID_TITLE?'));

			
			// Initialize CTA
			$this->SetOutputItem("CTA","");
		
		
			// Init
			$this->Trace('OnInit START');
			$this->m_bOnInit = false;
			$bInitSuccess = $this->OnInit();
			$this->Trace('OnInit END');
			if ($this->m_bOnInit == false)
			{
				$this->Error('OnInit overloads must invoke parent implementation.');	
			}
			if ($bInitSuccess != true)
			{
				$this->Error('OnInit did not return true.');	
			}
			
			// Init User
			$this->UserInit();


			// Initialize Copyright items
			$strCopyrightSince = $this->GetMeta('copyright_since');
			if ($strCopyrightSince == '')
			{
				$this->SetOutputItem('COPYRIGHT_PERIOD',$strCurrentYear);
			}
			else if ($strCopyrightSince == $strCurrentYear)
			{
				$this->SetOutputItem('COPYRIGHT_PERIOD',$strCurrentYear);		
			}
			else if ($strCopyrightSince > $strCurrentYear)
			{
				$this->SetOutputItem('COPYRIGHT_PERIOD',$strCopyrightSince);
			}
			else
			{
				$this->SetOutputItem('COPYRIGHT_PERIOD',$strCopyrightSince.'-'.$strCurrentYear);
			}
			$this->SetOutputItem('COPYRIGHT','?TID_COPYRIGHT?');
			
			
			// Module Initializer
			$this->m_bModuleRegistrationCompleted = true;
			$this->TriggerEvent("website-module-init",array());

			
			// Command Handler
			$strCommand = $this->GetCommand();
			if (StringCheckCharSet($strCommand,CHARSET_COMMAND) == true)
			{
				$bContinue = $this->OnCommand($strCommand);
				if ($bContinue == false)
				{
					exit();	
				}
			}

			// Basic Authentication
			if ($this->IsBasicAuthActive() == true)
			{
				$this->Trace("IsBasicAuthActive() == true");
				$bContinue = $this->BasicAuth();
				if ($bContinue == false)
				{
					exit();	
				}
			}

			
			
			// Remove values that by definition are one-time parameters
			$this->SetVar("command","");
			$this->SetVar($this->GetAuthUsernameVarName(),"");
			$this->SetVar($this->GetAuthPasswordVarName(),"");



			/*
			$this->Trace("m_arrayConfig:");
			$this->Trace($this->GetConfigArray());
			
			$this->Trace("m_arrayParams:");
			$this->Trace($this->m_arrayParams);
			
			$this->Trace("m_arrayVars:");
			$this->Trace($this->m_arrayVars);
			*/
			
			

			// Manage Message Codes
			$this->AddErrorMessageCode($this->m_strErrorMessageCode);
			$this->AddWarningMessageCode($this->m_strWarningMessageCode);
			$this->AddInfoMessageCode($this->m_strInfoMessageCode);

			// Add Errors and Warnings resulting from the initialization process
			if ($bSessionCreationError == true)
			{
				$this->AddErrorMessageCode("SESSION_CREATE"); 
			}
			if ($bCSRFInvalidWarning == true)
			{
				$this->AddWarningMessageCode("CSRF_TOKEN_INVALID"); 
			}
			
			
			// Render Output
			$strOutput = $this->RenderWebSite();

			// UTF-8 Encode
			$strOutput = StringEncodeUTF8($strOutput);


			// RECOVER PREVIOUS CSRF TOKEN IF A FILE IS READY FOR DOWNLOAD
			if ($this->IsDownloadFileSet() == true)
			{
				$this->RecoverPreviousCSRFToken();
			}
			
			// Store Session Data
			if ($this->GetSessionMode() != SESSION_MODE_NONE)
			{
				if ($this->GetSessionID() != "")
				{
					if ($this->IsSubsequentRequest() == false)
					{
						$this->Trace($this->m_arrayVars);
						if ($this->OnSessionWrite($this->m_strSessionID,$this->m_arrayVars) != true)
						{
							$this->Trace("ERROR: Session data could not be written.");
							$this->SystemEvent(
								"WARNING",
								"SESSION_WRITE_FAILED",
								"Session data could not be written!",
								array(
									"sessionid" => $this->m_strSessionID
								)
							);
						}
					}
				}
			}
			
			// DOWNLOAD FILE IF REQUESTED
			if ($this->IsDownloadFileSet())
			{
				DownloadFileAndExit(
					$this->m_arrayDownloadFile["mimetype"],	
					$this->m_arrayDownloadFile["filename"],
					$this->m_arrayDownloadFile["contents"]);
			}
			
			// UNRESOLVED TID CHECK
			$this->CheckUnresolvedTID($strOutput);
			
			// UNINTENDED OUTPUT CHECK
			$this->CheckUnintendedOutput();

			// PROFILING
			$this->ProfilingTrace();

			// Add trace output if required        
			if ($this->IsTraceActive() == true)
	    {
	    	if (FindString($strOutput,"{TRACE}") != -1)
	    	{
	    		$strOutput = ReplaceString($strOutput,"{TRACE}",$this->RenderTraceLog());
	    	}
				else if (FindString($strOutput,"</body>") != -1)
				{
					$strOutput = ReplaceString($strOutput,"</body>",$this->RenderTraceLog()."</body>");
				}
				else
				{
					$strOutput .= $this->RenderTrace();
				}
			}
			else
			{
				$strOutput = ReplaceString($strOutput,"{TRACE}","");
			}


			// PROFILING
			if ($this->m_bProfiling)
			{
				$fSecondsElapsed = $this->ProfilingGetSeconds();
				$strProfiling = "\n";
				$strProfiling .= "<!--Elapsed Seconds: ".RenderNumber($fSecondsElapsed,3)."-->\n";
				$strProfiling .= "<!--Memory Consumed: ".memory_get_usage()."-->\n";
     		$strOutput .= $strProfiling;
     	}
     	
			// FINAL OUTPUT
			print($strOutput);
		}
		
		final
		function GetSourceDir()
		{
			return $this->m_strSourceDir;	
		}
		
		final
		function GetAssembliesDir()
		{
			return $this->GetSourceDir()."assemblies/";	
		}
		final
		function GetAssemblyDir($strAssemblyID)
		{
			return $this->GetAssembliesDir() . $strAssemblyID . "/";	
		}
		
		final
		function GetDataDir()
		{
			return $this->m_assemblyRoot->GetDataDir();
		}
		final
		function GetDataFilePath($strFilePath)
		{
			return $this->GetAssemblyLine()->GetDataFilePath($strFilePath);
		}		
		
		
		final
		function GetNavigationDir()
		{
			return $this->m_assemblyRoot->GetNavigationDir();
		}		

		final
		function GetModulesDir()
		{
			return $this->m_assemblyRoot->GetModulesDir();
		}
		
		final
		function GetModuleSourceDir($strModuleName)
		{
			$arrayModuleNameComponents = explode("/",$strModuleName);
			if (!is_array($arrayModuleNameComponents) || ArrayCount($arrayModuleNameComponents) != 2)
			{
				$this->Trace($arrayModuleNameComponents);
				$this->Error("GetModuleDir(\"$strModuleName\"): Invalid name");
			}
			$strModuleGroup = $arrayModuleNameComponents[0];
			$strModuleID = $arrayModuleNameComponents[1];
			$strModuleSourceDir = $this->GetSourceDir()."modules/".$strModuleGroup."/".$strModuleID."/";
			return $strModuleSourceDir;
		}



		final
		function GetAssemblyLine()
		{
			return $this->m_assemblyline;	
		}
		
		final
		function GetAssembly($strAssemblyID)
		{
			return $this->m_assemblyline->GetAssembly($strAssemblyID);
		}


		final
		function IncludePlugin($strPluginID)
		{
			$strFile = $this->GetAssemblyLine()->GetPluginSourceFilePath($strPluginID);
			if ($strFile == false)
			{
				$this->Error('IncludePlugin: Plugin source file not found: "'.$strPluginID.'"');
			}
			require_once($strFile);
		} 

		final
		function GetPlugin($strPluginID)
		{
			foreach ($this->m_arrayPlugins as $plugin)
			{
				if ($strPluginID == $plugin->GetID())
				{
					return $plugin;	
				}
			}
			return null;
		}


		final
		function IncludeElement($strElementID)
		{
			$strFile = $this->GetAssemblyLine()->GetElementSourceFilePath($strElementID);
			if ($strFile == false)
			{
				$this->Error('IncludeElement: Element source file not found: "'.$strElementID.'"');
			}
			require_once($strFile);
		} 
		
		final
		function IncludeModule($strModulePath)
		{
			$strModuleSourceFile = $this->GetAssemblyLine()->GetModuleSourceFilePath($strModulePath);
			if ($strModuleSourceFile == false)
			//$strModuleSourcePath = $this->GetModuleSourcePath($strModuleName);
			{
				$this->Error("IncludeModule: Module Source file not found: \"$strModulePath\"");
			}
			require_once($strModuleSourceFile);	
		}
		
		final
		function RegisterModule(CModule $module)
		{
			if ($this->m_bModuleRegistrationCompleted == true)
			{
				$this->Error("Module registration completed. Please use RegisterModules only in or before invokation of OnInit.");	
			}
			$this->RegisterEvent("website-module-init",$module);
			$this->RegisterEvent("website-user-delete",$module);
		}

		final
		function GetWWWDir()
		{
			return $this->m_strWWWDir;	
		}

		final
		function GetStyleSheetsDir()
		{
			return $this->m_strStyleSheetsDir;	
		}

		final
		function GetJavaScriptsDir()
		{
			return $this->m_strJavaScriptsDir;	
		}


		final
		function GetImagesDir()
		{
			return $this->m_strImagesDir;	
		}

		final
		function GetFontsDir()
		{
			return $this->m_strFontsDir;	
		}

		final
		function GetVideosDir()
		{
			return $this->m_strVideosDir;	
		}


		final
		function GetDownloadDir()
		{
			return $this->m_strDownloadDir;	
		}

		final
		function GetParams()
		{
			return SecureOutput($this->m_arrayParams);
		}
		final
		function GetParamsInsecure()
		{
			$this->Trace("WARNING: Use of GetParamsInsecure() is a possible security contraint!");
			return $this->m_arrayParams;
		}
				
		final
		function GetParam($strParam)
		{
			return SecureOutput(
				ArrayGetValue($this->m_arrayParams,$strParam));	
		}
		final
		function GetParamInsecure($strParam)
		{
			$this->Trace("WARNING: Use of GetParamInsecure() is a possible security contraint!");
			return ArrayGetValue($this->m_arrayParams,$strParam);
		}

		
		final
		function IsParam($strParam)
		{
			return ArrayKeyExists($this->m_arrayParams,$strParam);
		}

		final
		function SetParam($strParam,$vValue)
		{
			ArraySetValue($this->m_arrayParams,$vValue,$strParam);
		}

		
		final
		function GetVar($strVar)
		{
			return SecureOutput($this->GetVarInternal($strVar));
		}

		final
		function IsVar($strVar)
		{
			return ArrayKeyExists($this->m_arrayVars,$strVar);
		}


		final
		function GetVarInsecure($strVar)
		{
			$this->Trace("WARNING: Use of GetVarInsecure() is a possible security contraint!");
			return $this->GetVarInternal($strVar);
		}
		
		protected
		final
		function GetVarInternal($strVar)
		{
			if ($this->CheckVarName($strVar) == false)
			{
				$this->Error("GetVar() or GetVarInsecure() called with invalid var name parameter: \"$strVar\". Var names are also used for http request parameters and may only contain the following set of characters: \"".CHARSET_PARAMETERNAME."\"");	
			}
			return ArrayGetValue($this->m_arrayVars,$strVar);
		}
		
		final
		function CheckVarName($strVar)
		{
			if (StringCheckCharSet($strVar,CHARSET_PARAMETERNAME) == false)
			{
				return false;
			}
			return true;
		}


		final		
		function GetAllVars()
		{
			return SecureOutput($this->m_arrayVars);
		}

		final		
		function GetAllVarsInsecure()
		{
			$this->Trace("WARNING: Use of GetAllVarsInsecure() is a possible security contraint!");
			return $this->m_arrayVars;
		}

	
		final	
		function SetVar($strVar, $strValue)
		{
			if ($strValue != "")
			{
				$this->m_arrayVars[$strVar] = $strValue;
			}
			else
			{
				$this->m_arrayVars = ArrayRemoveKey($this->m_arrayVars,$strVar);
			}
		}
	
		final
		function GetCommand()
		{
			return $this->GetVar("command");	
		}

		final
		function IsCSRFProtectionActive()
		{
			if ($this->GetAuthUserID() == "")
			{
				return false;	
			}
			$bCSRFDefuse = GetBoolValue($this->GetConfig("csrf_defuse"));
			if ($bCSRFDefuse == true)
			{
				return false;
			}
			if ($this->GetCommand() == 'image')
			{
				return false;	
			}
			return true;
		}

		final
		function ExtractAndVerifyCSRFToken()
		{
			$strCSRFTokenSession = ArrayGetValue($this->m_arraySession,	"csrf-token");
			$strCSRFTokenParam = $this->GetParam("csrf-token");
			
			if ($strCSRFTokenParam == "")
			{
				return false;	
			}
			if ($strCSRFTokenParam != $strCSRFTokenSession)
			{
				return false;
			}
			return true;
		}
		
		final
		function DeployNewCSRFToken()
		{
			$this->m_strOldCSRFToken = $this->GetCSRFToken();
			$strNewToken = MakeNumericToken(40);
			$this->SetVar("csrf-token",$strNewToken);
		}
		
		final
		function RecoverPreviousCSRFToken()
		{
			$this->SetVar("csrf-token",$this->m_strOldCSRFToken);	
		}
		
		
		final		
		function GetCSRFToken()
		{
			return $this->GetVar("csrf-token");
		}

		


		function GetSessionMode()
		{
			return $this->m_nSessionMode;	
		}

		function GetSessionCookieDuration()
		{
			return 60*60*24*2;	
		}

		function ExtractSessionID()
		{
			switch ($this->GetSessionMode())
			{
				case SESSION_MODE_SERVER_URL:
					return $this->GetParam("sessionid");
				case SESSION_MODE_SERVER_COOKIE:
					return $this->GetCookie("sessionid");
				case SESSION_MODE_SERVER_COMBINED:
					$strParam = $this->GetParam("sessionid");
					$strCookie = $this->GetCookie("sessionid");
					return StringGlue(array($strParam,$strCookie));
				case SESSION_MODE_LOCAL_COOKIE:
					return "local";
				default:
					return "";
			}
		}
		
		function DeploySessionID($strSessionID)
		{
			switch ($this->GetSessionMode())
			{
				case SESSION_MODE_SERVER_COOKIE:
					$this->SetCookie("sessionid",$strSessionID,$this->GetSessionCookieDuration());
					break;
				case SESSION_MODE_SERVER_COMBINED:
					$arraySlices = StringSlice($strSessionID,2);
					$this->SetCookie("sessionid",$arraySlices[1],$this->GetSessionCookieDuration());
					break;
				case SESSION_MODE_SERVER_URL:
				default:
					break;
			}
		}

		function GetSessionAwareArrayVars()
		{
			$arrayVarsSession = array();
			$arrayVarsSession["content"] = $this->GetContent();
			if ($this->IsCSRFProtectionActive())
			{
				$arrayVarsSession["csrf-token"] = $this->GetCSRFToken();
			}
			
			switch ($this->GetSessionMode())
			{
				case SESSION_MODE_SERVER_URL:
					$arrayVarsSession["sessionid"] = $this->GetSessionID();
					return $arrayVarsSession;

				case SESSION_MODE_SERVER_COOKIE:
					return $arrayVarsSession;

				case SESSION_MODE_SERVER_COMBINED:
					$arraySlices = StringSlice($this->GetSessionID(),2);
					$arrayVarsSession["sessionid"] = $arraySlices[0];
					return $arrayVarsSession;

				case SESSION_MODE_LOCAL_COOKIE:
					return $arrayVarsSession;

				default:
					return $this->GetAllVars();
			}
		}
		
		
		/*
			Subsequent requests are requests initiated by the browser when loading
			the actual page requested by a user.
			
			Typical subsequent requests are requests for static assets where no script
			execution is involved.
			But it also includes the request for favicon.ico which may eventually not
			exist and initiate script based error handling.
		
		*/
		
		final
		function SetSubsequentRequest()
		{
			$this->m_bSubsequentRequest = true;
			$this->Trace("SetSubsequentRequest");
		}
		
		final
		function IsSubsequentRequest()
		{
			if ($this->m_bSubsequentRequest == true)
			{
				return true;	
			}
			$this->m_bSubsequentRequest = $this->OnIsSubsequentRequest();
			return $this->m_bSubsequentRequest;
		}
		
		function OnIsSubsequentRequest()
		{
			$strCommand = $this->GetCommand();
			if ($strCommand == 'image')
			{
				return true;
			}
			if ($strCommand == 'download')
			{
				return true;
			}
			if ($strCommand == 'css')
			{
				return true;
			}
			if ($strCommand == 'video')
			{
				return true;
			}
			if ($strCommand == 'font')
			{
				return true;
			}
			return false;
		}
		
		/*
		This is a hack to compensate different behaviour of IE8 and FF3
		while transferring a button's inner area. IE8 will send a html comment
		while FF3 will strip it away. As a result, different values are sent
		to the server.
		
		See also CFormElement::RenderControlButtonSubmit()
		*/
		final
		function GetSubmitVarInsecure()
		{
			$strSubmitVar = $this->GetVarInsecure("submit");
			$strVar = StringGetFromEmbracingTags($strSubmitVar,"<!--\"","\"-->");
			if ($strVar == false)
			{
				return $strSubmitVar;	
			}
			return $strVar;
		}
		final
		function GetSubmitVar()
		{
			return SecureOutput($this->GetSubmitVarInsecure());
		}


		protected
		final
		function GetCookieInternal($strCookie)
		{
			if (!IsValidCookieName($strCookie))
			{
				$this->Error("GetCookie() or GetCookieInsecure() called with invalid var name parameter: \"$strCookie\". Cookie names may only contain the following set of characters: \"".CHARSET_COOKIE."\" and it must not start with a digit.");	
			}
			return ArrayGetValue($this->m_arrayCookies,$strCookie);
		}
		final
		function GetCookie($strCookie)
		{
			return SecureOutput($this->GetCookieInternal($strCookie));
		}
		final
		function GetCookieInsecure($strCookie)
		{
			$this->Trace("WARNING: Use of GetCookieInsecure() is a possible security contraint!");
			return $this->GetCookieInternal($strCookie);
		}
		final
		function GetAllCookies()
		{
			return SecureOutput($this->m_arrayCookies);	
		}
		final
		function GetAllCookiesInsecure()
		{
			$this->Trace("WARNING: Use of GetAllCookiesInsecure() is a possible security contraint!");
			return $this->m_arrayCookies;	
		}
		
		
		final
		function SetCookie($strCookie,$strValue,$nTimeOutSeconds)
		{
			if (!IsValidCookieName($strCookie))
			{
				return false;	
			}
			$arrayURL = ParseURL($this->GetRootURL());
			$strDomain = $arrayURL["HOST"];
			$strPath = $this->GetRootPath();
			
			$strValue = StringURLEncode($strValue);
			
			if (StringLength($strValue) > 4000)
			{
				$this->Error("Cookie value too long! ");	
			}

			$timeExpiration = time()+intval($nTimeOutSeconds);

			$ea = new CErrorAmnesty();
			$ea->Start();
			
			$bResult = setcookie(
				$strCookie,
				$strValue,
				$timeExpiration,
				$strPath,
				$strDomain,
				//($this->GetProtocol()=="https")?(true):(false),
				false,
				false);

			$ea->End();			

			$this->Trace("SetCookie: $strDomain$strPath $strCookie: $strValue");
				
			if ($bResult == false)
			{
				$this->Error("SetCookie failed! Please check if there was unintended output before the use of SetCookie()!");
			}
				
			return $bResult;
		}
		final
		function DeleteCookie($strCookie)
		{
			if (!IsValidCookieName($strCookie))
			{
				return false;	
			}

			$arrayURL = ParseURL($this->GetRootURL());
			$strDomain = $arrayURL["HOST"];
			
			
			setcookie(
				$strCookie,
				false,
				time()-(60*60*24),
				$this->GetRootPath(),
				$strDomain,
				//($this->GetProtocol()=="https")?(true):(false),
				false,
				false);			
				
				return true;
		}
		final
		function DeleteAllCookies($strPrefix = "")
		{
			foreach ($this->m_arrayCookies as $strCookie => $strValue)
			{
				$bDeleteCookie = true;
				if ($strPrefix != "")
				{
					$nPos = FindString($strCookie,$strPrefix);
					if ($nPos != 0)
					{
						$bDeleteCookie = false;
					}	
				}	
				if ($bDeleteCookie == true)
				{
					$this->DeleteCookie($strCookie);
				}
			}
		}


		final
		function GetOutputItem($strItem)
		{
			return ArrayGetValue($this->m_arrayOutputItems,$strItem);	
		}
		final		
		function SetOutputItem($strItem, $strContents)
		{
			$this->m_arrayOutputItems[$strItem] = $strContents;	
		}
		final		
		function SetOutputItems($arrayItems)
		{
			$this->m_arrayOutputItems = ArrayMerge($this->m_arrayOutputItems,$arrayItems);	
		}
		
		final
		function GetBodyItem($strItem)
		{
			return ArrayGetValue($this->m_arrayBodyItems,$strItem);	
		}
		final
		function GetAllBodyItems()
		{
			return $this->m_arrayBodyItems;
		}
		final		
		function SetBodyItem($strItem, $strContents)
		{
			$this->m_arrayBodyItems[$strItem] = $strContents;	
		}
		final		
		function SetBodyItems($arrayItems)
		{
			$this->m_arrayBodyItems = ArrayMerge($this->m_arrayBodyItems,$arrayItems);	
		}
		
		
		final
		function GetLayoutConfigItem($strItem)
		{
			return ArrayGetValue($this->m_arrayLayoutConfigItems,$strItem);	
		}
		final		
		function SetLayoutConfigItem($strItem, $strContents)
		{
			$this->m_arrayLayoutConfigItems[$strItem] = $strContents;	
		}
		final		
		function SetLayoutConfigItems($arrayItems)
		{
			$this->m_arrayLayoutConfigItems = ArrayMerge($this->m_arrayLayoutConfigItems,$arrayItems);	
		}
		final
		function GetAllLayoutConfigItems()
		{
			return $this->m_arrayLayoutConfigItems;
		}

		final
		function GetProtocol()
		{
			return $this->m_strProtocol;	
		}
		final
		function GetSupportedProtocols()
		{
			return $this->m_arrayProtocols;
		}
		final
		function IsProtocolSupported($strProtocol)
		{
			return ArrayValueExists($this->GetSupportedProtocols(),$strProtocol); 
		}
		
		final		
		function GetCurrentURL()
		{
			return $this->m_strCurrentURL;	
		}
		
		final		
		function GetRootURL()
		{
			return $this->m_strRootURL;	
		}
		
		final
		function GetRootURL_PreferHTTPS()
		{
			if (ArrayValueExists($this->m_arrayProtocols,"https") == false)
			{
				$strURL = $this->m_arrayProtocols[0] . "://";
			}
			else
			{
				$strURL = "https" . "://";				
			}
			$strURL .= $this->m_strRootURLnoProtocol;	
			return $strURL;
		}

		final
		function GetRootPath()
		{
			$arrayParsed = ParseURL($this->GetRootURL());
			$strPath = ArrayGetValue($arrayParsed,"PATH");			
			if (StringSection($strPath,0,1) != "/")
			{
				$strPath = "/".$strPath;	
			}
			return $strPath;
		}


		final
		function MakeCssURL($strLayout)
		{
			$arrayParams = array();
			$arrayParams["layout"] = $strLayout;
			$arrayParams["command"] = "css";
			return MakeURL($this->m_strRootURL,$arrayParams,true);			
		}

/*
		final
		function MakeJavaScriptURL($strScriptID)
		{
			$strFile = $this->MakeJavaScriptFileName($strScriptID);
			if (IsFile($this->GetJavaScriptDir().$strFile))
			{
				return $this->GetJavaScriptURL($strFile);	
			}
			return $this->MakeJavaScriptURL($strScriptID,array(),false);
		}
		*/

		
		function GetImageRootURL($strImageFile="")
		{
			return $this->GetRootURL()."images/".$strImageFile;
		}
		
		final  
		function GetLayoutImageURL($strImageID,$strImageFileExtention='png')
		{
			$strFile = $this->MakeLayoutImageFileName($strImageID,$this->GetLayout(),$strImageFileExtention);
			if (IsFile($this->GetImagesDir().$strFile))
			{
				return $this->GetImageRootURL($strFile);	
			}
			$strFile = $this->MakeLayoutImageFileName($strImageID,"default",$strImageFileExtention);
			if (IsFile($this->GetImagesDir().$strFile))
			{
				return $this->GetImageRootURL($strFile);	
			}
			return $this->MakeImageURL($strImageID,array(),false);
		}		

		final
		function GetImageURL($strImageID,$strImageFileExtention)
		{
			if ($strImageFileExtention == "")
			{
				$strFile = $this->MakeImageFileName($strImageID,"svg");
				if (IsFile($this->GetImagesDir().$strFile))
				{
					return $this->GetImageRootURL($strFile);	
				}
				$strFile = $this->MakeImageFileName($strImageID,"png");
				if (IsFile($this->GetImagesDir().$strFile))
				{
					return $this->GetImageRootURL($strFile);	
				}
				$strFile = $this->MakeImageFileName($strImageID,"jpg");
				if (IsFile($this->GetImagesDir().$strFile))
				{
					return $this->GetImageRootURL($strFile);	
				}
				$strFile = $this->MakeImageFileName($strImageID,"gif");
				if (IsFile($this->GetImagesDir().$strFile))
				{
					return $this->GetImageRootURL($strFile);	
				}
			}
			$strFile = $this->MakeImageFileName($strImageID,$strImageFileExtention);
			if (IsFile($this->GetImagesDir().$strFile))
			{
				return $this->GetImageRootURL($strFile);	
			}

			if ($strImageFileExtention == "")
			{
				$strFile = $this->MakeLayoutImageFileName($strImageID,$this->GetLayout(),"svg");
				if (IsFile($this->GetImagesDir().$strFile))
				{
					return $this->GetImageRootURL($strFile);	
				}	
				$strFile = $this->MakeLayoutImageFileName($strImageID,$this->GetLayout(),"png");
				if (IsFile($this->GetImagesDir().$strFile))
				{
					return $this->GetImageRootURL($strFile);	
				}
				$strFile = $this->MakeLayoutImageFileName($strImageID,$this->GetLayout(),"jpg");
				if (IsFile($this->GetImagesDir().$strFile))
				{
					return $this->GetImageRootURL($strFile);	
				}
				$strFile = $this->MakeLayoutImageFileName($strImageID,$this->GetLayout(),"gif");
				if (IsFile($this->GetImagesDir().$strFile))
				{
					return $this->GetImageRootURL($strFile);	
				}
			}
			$strFile = $this->MakeLayoutImageFileName($strImageID,$this->GetLayout(),$strImageFileExtention);
			if (IsFile($this->GetImagesDir().$strFile))
			{
				return $this->GetImageRootURL($strFile);	
			}
			
			$arrayParams = array();
			if ($strImageFileExtention != "")
			{
				$arrayParams["ext"] = $strImageFileExtention;
			}
			return $this->MakeImageURL($strImageID,$arrayParams,false);
		}		

		function GetFontRootURL($strFontFile="")
		{
			return $this->GetRootURL()."fonts/".$strFontFile;
		}


		final
		function GetFontURL($strFontID)
		{
			$strFile = $this->MakeFontFileName($strFontID);
			if (IsFile($this->GetFontsDir().$strFile))
			{
				return $this->GetFontRootURL($strFile);	
			}
			return $this->MakeFontURL($strFontID,false);
		}		

		function GetVideoRootURL($strVideoFile="")
		{
			return $this->GetRootURL()."videos/".$strVideoFile;
		}


		final
		function GetVideoURL($strVideoID)
		{
			$strFile = $this->MakeVideoFileName($strVideoID);
			if (IsFile($this->GetVideosDir().$strFile))
			{
				return $this->GetVideoRootURL($strFile);	
			}
			return $this->MakeVideoURL($strVideoID,false);
		}		



		function GetDownloadRootURL($strFilePath = "")   
		{
			return $this->GetRootURL()."download/".$strFilePath;
		}

		
		final
		function GetDownloadURL($strFilePath)
		{
			if (IsFile($this->GetDownloadDir().$strFilePath))
			{
				return $this->GetDownloadRootURL().StringURLEncode($strFilePath);
			}
			return $this->MakeDownloadURL($strFilePath);
		}
		
		final
		function GetTimeZone()
		{
			$strTimeZone = $this->GetVar("timezone");	
			if (!IsValidTimezoneID($strTimeZone))
			{
				return "UTC";
			}
			return $strTimeZone;
		}

		final
		function SetTimeZone($strTimeZone)
		{
			if (IsValidTimezoneID($strTimeZone))
			{
				$this->SetVar("timezone",$strTimeZone);
			}
		}
		
		final
		function RenderDate($time) 
		{
			$strCountryID = $this->GetCountry();
			$strFormat = $this->OnGetDateLocalFormat($strCountryID);
			if ($strFormat == "")
			{
				$strFormat = GetDateLocalFormat($strCountryID);
			}
			$strOutput = RenderDateTimeFormat(
				$time,
				$strFormat,
				$this->GetTimeZone(),
				$this->GetLanguage());
			return $strOutput;
		}

		final
		function RenderTime($time)
		{
			$strCountryID = $this->GetCountry();
			$strFormat = $this->OnGetTimeLocalFormat($strCountryID);
			if ($strFormat == "")
			{
				$strFormat = GetTimeLocalFormat($strCountryID);
			}
			$strOutput = RenderDateTimeFormat(
				$time,
				$strFormat,
				$this->GetTimeZone(),
				$this->GetLanguage());
			return $strOutput;
		}
		
		final
		function RenderDateTime($time)
		{
			$strCountryID = $this->GetCountry();
			$strFormat = $this->OnGetDateTimeLocalFormat($strCountryID);
			if ($strFormat == "")
			{ 
				$strFormat = GetDateTimeLocalFormat($strCountryID);
			}
			$strOutput = RenderDateTimeFormat(
				$time,
				$strFormat,
				$this->GetTimeZone(),
				$this->GetLanguage());
			return $strOutput;
		}
		
		final
		function RenderNumber($fValue,$nPrecision=0,$strMetric="",$bUseSeparator = true)
		{
			$fValue = floatval($fValue);
			/*
			if (($fValue == "") || ($fValue == null))
			{
				return "";	
			}
			*/
			$arraySettings = GetLocaleSettings(
				$this->GetCountry(),
				$this->GetLanguage());
			if ($bUseSeparator == false)
			{
				$arraySettings["DIGIT_GROUPING_SEPARATOR"] = "";	
			}
			return RenderNumber(
				$fValue,
				$nPrecision,
				$strMetric,
				$arraySettings);	
		}

		final
		function RenderCurrency($fValue,$strCurrencyID)
		{
			if ( ($fValue == "")
				|| ($fValue == null)
				|| !IsValidCurrencyID($strCurrencyID))
			{
				return '';	
			}
			$arraySettings = GetLocaleSettings(
				$this->GetCountry(),
				$this->GetLanguage());
			return RenderNumber($fValue,2,GetCurrencySymbol($strCurrencyID),$arraySettings);	
		}
		
		
		final
		function GetLanguage()
		{
			return $this->GetVar("language");	
		}

		final
		function SetLanguage($strLanguage)
		{
			$strLanguage = StringUpperCase($strLanguage);
			if ($this->IsLanguageSupported($strLanguage))
			{
				$this->SetVar("language",$strLanguage);
			}
		}

		final
		function GetSupportedLanguages()
		{
			return $this->GetConfig("languages");
		}

		final
		function GetFallbackLanguage()
		{
			return $this->GetConfig("fallbacklanguage");
		}
		
		final
		function IsFallbackLanguageSupported()
		{
			return $this->m_bFallbackLanguageSupported;
		}


		final
		function IsLanguageSupported($strLanguage)
		{
			return ArrayValueExists($this->GetSupportedLanguages(),$strLanguage); 
		}
		
		final
		function IsMonolingual()
		{
			return $this->m_bMonolingual;	
		}

		final
		function GetSupportedCountries()
		{
			$arrayCountries = $this->GetConfig("countries");
			if (is_array($arrayCountries) == true)
			{
				if (ArrayCount($arrayCountries))
				{
					return $arrayCountries;	
				}	
			}
			return GetAllCountryIDs();
		}

		final
		function IsCountrySupported($strCountry)
		{
			return ArrayValueExists($this->GetSupportedCountries(),$strCountry); 
		}

		final
		function GetCountry()
		{
			$strCountry = $this->GetVar("country");	
			if ($strCountry == "")
			{
				$strCountry = $this->GetConfig("defaultcountry");	
			}
			return $strCountry;
		}
		
		final
		function SetCountry($strCountry)
		{
			if (IsValidCountryID($strCountry) || ($strCountry == ""))
			{
				$this->SetVar("country",$strCountry);	
			}	
		}

		final
		function GetSupportedCurrencies()
		{
			$arrayCurrencies = $this->GetConfig("currencies");
			if (is_array($arrayCurrencies) == true)
			{
				if (ArrayCount($arrayCurrencies))
				{
					return $arrayCurrencies;	
				}	
			}
			return GetAllCurrencyIDs();
		}

		final
		function IsCurrencySupported($strCurrency)
		{
			return ArrayValueExists($this->GetSupportedCurrencies(),$strCurrency); 
		}


		
		final
		function GetLayout()
		{
			return $this->GetVar("layout");	
		}
		final
		function SetLayout($strLayout)
		{
			if (ArrayValueExists($this->GetSupportedLayouts(),$strLayout) == false)
			{
				$strLayout = "default";	
			}
			$this->SetVar("layout",$strLayout);	
		}
		
		final
		function GetSupportedLayouts()
		{
			$arrayLayouts = $this->OnGetSupportedLayouts();
			if (ArrayCount($arrayLayouts) >= 1)
			{
				return $arrayLayouts;
			}
			return array("default");				
		}
		
		function OnGetSupportedLayouts()
		{
			return $this->GetConfig("layouts");
		}

		final		
		function GetFramework()
		{
			return $this->m_strFrameworkID;
		}
		final		
		function SetFramework($strFramework)
		{
			$this->m_strFrameworkID = $strFramework;
		}

		final
		function GetContent()
		{
			return $this->GetVar("content");	
		}
		final		
		function SetContent($strContent)
		{
			$this->SetVar("content",$strContent);	
		}

		final
		function GetArea()
		{
			return $this->GetVar("area");	
		}
		
		final
		function SetArea($strArea)
		{
			$this->SetVar("area",$strArea);	
		}
		
		final
		function Error($strText)
		{
			$this->SystemLog("ERROR","FATALERROR", $strText);
			FatalError(
				$strText,
				$this->GetTraceLog());
		}

		final		
		function GetSessionID()
		{
			return $this->m_strSessionID;
		}


		final
		function SetErrorMessageCode($strMessageCode,$strPrefix="")
		{
			if ($strMessageCode != "")
			{
				$this->SetErrorMessageText(
					$this->GetMessage(
						"error",
						$strMessageCode,
						$strPrefix));
			}
		}
		
		final		
		function SetWarningMessageCode($strMessageCode,$strPrefix="")
		{
			if ($strMessageCode != "")
			{
				$this->SetWarningMessageText(
					$this->GetMessage(
						"warning",
						$strMessageCode,
						$strPrefix));
			}
		}
		
		final		
		function SetInfoMessageCode($strMessageCode,$strPrefix="")
		{
			if ($strMessageCode != "")
			{
				$this->SetInfoMessageText(
					$this->GetMessage(
						"info",
						$strMessageCode,
						$strPrefix));
			}
		}

		final
		function SetErrorMessageText($strMessageText)
		{
			$this->m_arrayMessages["error"] = array();
			$this->Trace("Reset error messages");
			if ($strMessageText != "")
			{
				$this->AddErrorMessageText($strMessageText);
			}
		}
		
		final		
		function SetWarningMessageText($strMessageText)
		{
			$this->m_arrayMessages["warning"] = array();
			$this->Trace("Reset warning messages");
			if ($strMessageText != "")
			{
				$this->AddWarningMessageText($strMessageText);
			}
		}
		
		final		
		function SetInfoMessageText($strMessageText)
		{
			$this->m_arrayMessages["info"] = array();
			$this->Trace("Reset info messages");
			if ($strMessageText)
			{
				$this->AddInfoMessageText($strMessageText);
			}
		}
		
		final
		function AddErrorMessageText($strMessageText)
		{
			$this->Trace("AddErrorMessageText(\"$strMessageText\")");
			if ($strMessageText != "")
			{
				$this->m_arrayMessages["error"][] = $strMessageText;
			}			
		}

		final
		function AddWarningMessageText($strMessageText)
		{
			$this->Trace("AddWarningMessageText(\"$strMessageText\")");
			if ($strMessageText != "")
			{
				$this->m_arrayMessages["warning"][] = $strMessageText;
			}			
		}

		final
		function AddInfoMessageText($strMessageText)
		{
			$this->Trace("AddInfoMessageText(\"$strMessageText\")");
			if ($strMessageText != "")
			{
				$this->m_arrayMessages["info"][] = $strMessageText;
			}			
		}

		final
		function AddErrorMessageCode($strMessageCode,$strPrefix="")
		{
			$this->Trace("AddErrorMessageCode(\"$strMessageCode\")");
			if ($strMessageCode != "")
			{
				$this->m_arrayMessages["error"][] = $this->GetMessage("error",$strMessageCode,$strPrefix);
			}			
		}

		final
		function AddWarningMessageCode($strMessageCode,$strPrefix="")
		{
			$this->Trace("AddWarningMessageCode(\"$strMessageCode\")");
			if ($strMessageCode != "")
			{
				$this->m_arrayMessages["warning"][] = $this->GetMessage("warning",$strMessageCode,$strPrefix);
			}			
		}

		final
		function AddInfoMessageCode($strMessageCode,$strPrefix="")
		{
			$this->Trace("AddInfoMessageCode(\"$strMessageCode\")");
			if ($strMessageCode != "")
			{
				$this->m_arrayMessages["info"][] = $this->GetMessage("info",$strMessageCode,$strPrefix);
			}			
		}
		
		final
		function IsError()
		{
			return ArrayCount(ArrayGetValue($this->m_arrayMessages,"error"));
		}

		final
		function AddJavaScript($strTitle,$strID = "")
		{
			if ($strID == "")
			{
				$strID = $strTitle;	
			}
			$this->m_arrayJavaScripts[$strTitle] = array(
				"id" => $strID);	
		}

		final
		function AddJavaScriptURL($strTitle,$strURL,$strIntegrity='')
		{
			$this->m_arrayJavaScripts[$strTitle] = array(
				'url' => $strURL,
				'integrity' => $strIntegrity);	
		}

		final
		function AddJavaScriptFile($strTitle,$strFilePath)
		{
			$this->m_arrayJavaScripts[$strTitle] = array(
				"filepath" => $strFilePath);	
		}


		final
		function AddJavaScriptSourceCode($strTitle,$strSourceCode)
		{
			$this->m_arrayJavaScripts[$strTitle] = array(
				"sourcecode" => $strSourceCode);	
		}

		final
		function AddStyleSheet($strTitle,$strID='')
		{
			if ($strID == '')
			{
				$strID = $strTitle;	
			}			
			$this->m_arrayStyleSheets[$strTitle] = array(
				'id' => $strID);	
		}

		final
		function AddStyleSheetFilePath($strTitle,$strFilePath)
		{
			$this->m_arrayStyleSheets[$strTitle] = array(
				'filepath' => $strFilePath);	
		}


		final
		function AddStyleSheetURL($strTitle,$strURL,$strIntegrity='')
		{
			$this->m_arrayStyleSheets[$strTitle] = array(
				'url' => $strURL,
				'integrity' => $strIntegrity);	
		}

		final
		function AddStyleSheetSourceCode($strTitle,$strSourceCode)
		{
			$this->m_arrayStyleSheets[$strTitle] = array(
				'sourcecode' => $strSourceCode);	
		}



		final
		function AddHeader($strHeader)
		{
			$this->m_arrayHeaders[] = $strHeader;	
		}

		final
		function Redirect($strURL,$nSeconds = 0)
		{
			$nSeconds = intval($nSeconds);
			$this->AddHeader('<meta http-equiv="refresh" content="'.$nSeconds.'; URL='.$strURL.'"/>');
		}

		
		final
		function EnforceHTTPS()
		{
			if ($this->GetProtocol() != "https")
			{
				if ($this->IsProtocolSupported("https"))
				{
					$strURL = $this->MakeProtocolURL("https",$this->GetParams(),false);
					RedirectAndExit($strURL);
				}	
			}	
		}
		
		final		
		function LoadStringResources($strID,$strLanguage = '')
		{
			if ($strLanguage == "")
			{
				$strLanguage = $this->GetLanguage();
			}
			
			if (ArrayGetValue($this->m_arrayLoadedStringResources,$strLanguage,$strID) == true)
			{
				$this->Trace('LoadStringResources: '.$strID.' for language '.$strLanguage.' is already loaded.');
				return true;	
			}
			$this->m_arrayLoadedStringResources[$strLanguage][$strID] = true;
			
			
			$strFile = $this->GetAssemblyLine()->GetResourceFilePath($strLanguage,$strID);
			if ($strFile == false)
			{
				$this->Trace("Warning: Can not find resources for \"$strID\" for language \"$strLanguage\".");
			}
			else
			{
				$bResult = $this->m_res->LoadStringResourceFile($strLanguage,$strFile);
				if ($bResult == false)
				{
					$this->Trace("Warning: Failed to load resource file \"$strFile\".");
				}
				else
				{
					return true;
				}
			}

			$strFallbackLanguage = $this->GetFallbackLanguage();
			if (!IsValidLanguageID($strFallbackLanguage))
			{
				$this->Trace("Warning: No fallback language configured.");
				return false;
			}
			else
			{
				$this->Trace("Warning: Trying fallback language \"$strFallbackLanguage\".");	
			}
			
			$strFile = $this->GetAssemblyLine()->GetResourceFilePath($strFallbackLanguage,$strID);
			if ($strFile == false)
			{
				$this->Trace("Warning: Can not find resources for \"$strID\" for fallback language \"$strFallbackLanguage\".");
			}
			else
			{
				$bResult = $this->m_res->LoadStringResourceFile($strFallbackLanguage,$strFile);
				if ($bResult == false)
				{
					$this->Trace("Warning: Failed to load resource file \"$strFile\".");
				}
				else
				{
					return true;
				}
			}
			
			return false;
		}
	
		final		
		function LoadString($strID,$strLanguage="")
		{
			if ($strLanguage == "")
			{
				$strLanguage = $this->GetLanguage();
			}
			if ($this->m_bUseRes == true)
			{
				$strString = $this->m_res->LoadString($strID,$strLanguage);
				if ($strString != "")
				{
					return $strString;
				}
				if ($this->IsFallbackLanguageSupported() == true)
				{
					if ($this->GetLanguage() != $this->GetFallbackLanguage())
					{
						return $this->m_res->LoadString($strID,$this->GetFallbackLanguage());
					}
				}
				return "";	
			}
			else
			{
				$this->Trace("LoadString: \$this->m_bUseRes == false: returning \"$strID\"");
				return $strID;
			}
		}
		
		final 
		function LoadAllStrings($strLanguage = "")
		{
			if ($this->m_bUseRes == true)
			{
				if ($strLanguage == "")
				{
					$strLanguage = $this->GetLanguage();
				}
				return $this->m_res->GetStrings($strLanguage);
			}
			return array();
		} 

		
		final		
		function LoadRoutingMap($strID,$strLanguage = "")
		{
			if ($strLanguage == "")
			{
				$arrayLanguages = $this->GetSupportedLanguages();
			}
			else
			{
				$arrayLanguages = array($this->GetLanguage());
			}
			
			foreach ($arrayLanguages as $strLanguage)
			{
				$strFile = $this->GetAssemblyLine()->GetRoutingMapFilePath($strLanguage,$strID);
				if ($strFile == false)
				{
					$this->Trace("Warning: Could not load routing map \"$strID\" for language \"$strLanguage\".");
				}
				else
				{
					$this->m_routing->LoadRoutingMapFile($strLanguage,$strFile);
				}
			}
		}		
		
		final 
		function GetRoutingAlias($strLanguage,$strContextType,$strContextID)
		{
			return $this->OnGetRoutingAlias($strLanguage,$strContextType,$strContextID);
		}
		
		function OnGetRoutingAlias($strLanguage,$strContextType,$strContextID)
		{
			// -1 means: take a random alias if there is more than one alias available for a context id
			return $this->m_routing->GetRoutingAlias($strLanguage,$strContextType,$strContextID,-1);
		}

		final 
		function GetRoutingContext($strContextType,$strAlias,&$strLanguage)
		{
			return $this->OnGetRoutingContext($strContextType,$strAlias,$strLanguage);
		}
		
		function OnGetRoutingContext($strContextType,$strAlias,&$strLanguage)
		{
			$arrayLanguages = $this->GetSupportedLanguages();
			if ($strLanguage != "")
			{
				$arrayLanguages = ArrayRemoveValue($arrayLanguages,$strLanguage);
				$arrayLanguages = ArrayAppend(array($strLanguage),$arrayLanguages);
			}
			
			foreach ($arrayLanguages as $strLanguage2)
			{
				$strContext = $this->m_routing->GetRoutingContext($strLanguage2,$strContextType,$strAlias);
				if ($strContext != $strAlias)
				{
					$strLanguage = $strLanguage2; 
					return $strContext;
				}
			}
			return $strAlias;
		}


		final
		function GetFrameworkLayoutFile()
		{
			$strLayoutFile = $this->GetAssemblyLine()->GetFrameworkLayoutFilePath(
				$this->GetFramework(),
				$this->GetLayout());
			if (IsFile($strLayoutFile))
			{
				return $strLayoutFile;
			}			
 			return "";
		}

		final
		function MakeURL($arrayParams,$bEncoded=true)
		{
			if (is_array($arrayParams) == false)
			{
				$arrayParams = array();
			}
			$arrayParams = ArrayMerge($this->GetSessionAwareArrayVars(), $arrayParams);
			$strURL = $this->OnPrepareMakeURL($this->m_strRootURL,$arrayParams,$bEncoded);
			$strURL = MakeURL($strURL,$arrayParams,$bEncoded);
			return $strURL;
		}

		final
		function MakeSitemapURL($arrayParams,$bEncoded=true)
		{
			if (is_array($arrayParams) == false)
			{
				$arrayParams = array();
			}
			$strContent = ArrayGetValue($arrayParams,'content');
			if ($strContent == 'index')
			{
				$arrayParams = ArrayRemoveKey($arrayParams,'content');	
			}
			$strURL = $this->OnPrepareMakeURL($this->m_strRootURL,$arrayParams,$bEncoded);
			return MakeURL($strURL,$arrayParams,$bEncoded);
		}

		function OnPrepareMakeURL($strRootURL,&$arrayParams,$bEncoded)
		{
			$strURL = $strRootURL;
			
			$arrayLayouts = $this->GetSupportedLayouts();
			if (ArrayCount($arrayLayouts) <= 1) 
			{
				$arrayParams = ArrayRemoveKey($arrayParams,'layout');	
			}
			else
			{
				$strLayout = $this->GetLayout();
				$strLayoutParam = ArrayGetValue($arrayParams,'layout');
				if ($strLayout == $strLayoutParam)
				{
					$arrayParams = ArrayRemoveKey($arrayParams,'layout');
				}
			}
			
			if (ArrayGetValue($arrayParams,'area')=='default')
			{
				$arrayParams = ArrayRemoveKey($arrayParams,'area');	
			}
			if ($this->IsMonolingual())
			{
				$arrayParams = ArrayRemoveKey($arrayParams,'language');	
			}
			
			$strSEOURLParam = $this->GetConfig('seourl');
			if ($strSEOURLParam != '')
			{
				$strContent = ArrayGetValue($arrayParams,'content');
				$arrayParams = ArrayRemoveKey($arrayParams,'content');							
				if ($strContent == '')
				{
					if ($this->GetContent() != 'index')
					{
						$strContent = $this->GetContent();
					}
				}
				$strLanguage = ArrayGetValue($arrayParams,'language');
				$arrayParams = ArrayRemoveKey($arrayParams,'language');	
				$strAlias = '';
				if ($strLanguage == '')
				{
					$strAlias = $this->GetRoutingAlias(
						$this->GetLanguage(),
						'content',
						$strContent);
				}
				else
				{
					$strAlias = $this->GetRoutingAlias(
						$strLanguage,
						'content',
						$strContent);
					$strURL .= StringLowerCase($strLanguage).'/';
				}
				if ($strAlias == 'index')
				{
					// we are creating a link to the main page
					if ($this->GetSessionMode() == SESSION_MODE_NONE)
					{
						// No session means that there is no hidden content parameter that we need to override in the URL.
						$strAlias = '';
					}
				}
				if ($strAlias != '')
				{
					$strURL .= $strAlias.'/';
				}
			}
			return $strURL;
		}


		final
		function MakeImageURL($strImageID,$arrayParams2=array(),$bEncoded=true)
		{
			$arrayParams = array();
			$arrayParams["layout"] = $this->GetLayout();
			$arrayParams["command"] = "image";
			$arrayParams["id"] = $strImageID;
			$arrayParams = ArrayAppend($arrayParams,$arrayParams2);
			return MakeURL($this->m_strRootURL,$arrayParams,$bEncoded);
		}

		final
		function MakeFontURL($strFontID,$bEncoded=true)
		{
			$arrayParams = array();
			$arrayParams["command"] = "font";
			$arrayParams["id"] = $strFontID;
			return MakeURL($this->m_strRootURL,$arrayParams,$bEncoded);
		}

		final
		function MakeVideoURL($strVideoID,$bEncoded=true)
		{
			$arrayParams = array();
			$arrayParams["command"] = "video";
			$arrayParams["id"] = $strVideoID;
			return MakeURL($this->m_strRootURL,$arrayParams,$bEncoded);
		}


		final
		function MakeDownloadURL($strFilePath,$bEncoded=true)
		{
			$arrayParams = array();
			$arrayParams["command"] = "download";
			$arrayParams["filepath"] = $strFilePath;
			return MakeURL($this->m_strRootURL,$arrayParams,$bEncoded);
		}

		final
		function MakeProtocolURL($strNewProtocol,$arrayParams, $bEncoded=true)
		{
			if (is_array($arrayParams) == false)
			{
				$arrayParams = array();
			}
	   	$arrayParams = ArrayMerge($this->GetSessionAwareArrayVars(), $arrayParams);
	   	
	   	$strURL = "";
		   
		  if (in_array($strNewProtocol,$this->m_arrayProtocols) == false)
			{
				$strURL = $this->m_arrayProtocols[0] . "://";
			}
			else
			{
				$strURL = $strNewProtocol . "://";				
			}
			$strURL .= $this->m_strRootURLnoProtocol;
			$strURL = $this->OnPrepareMakeURL($strURL,$arrayParams,$bEncoded);			
			return MakeURL($strURL,$arrayParams,$bEncoded);
		}

		final		
		function MakeSelfURL()
		{
				return $this->MakeURL(array());
		}
		
		final		
		function MakeIndexURL()
		{
			$arrayParams["content"] = "index";
			return $this->MakeURL($arrayParams);
		}
		
		final		
		function MakeLanguageURL($strLanguage)
		{
			$arrayParams["language"] = $strLanguage;
			return $this->MakeURL($arrayParams);
		}
		
		final		
		function MakeContentURL($strNewContent)
		{
			$arrayParams["content"] = $strNewContent;
			return $this->MakeURL($arrayParams);
		}

		final
		function MakeLayoutCssFileName($strLayout="default")
		{
			return "layout_".$strLayout.".css";
		}
		
		final
		function MakeLayoutImageFileName($strImageID,$strLayout="default",$strExtention="png")
		{
			return "layout_".$strLayout."_".$strImageID.".".$strExtention;
		}
		
		final
		function MakeImageFileName($strImageID,$strExtention="png")
		{
			return $strImageID.".".$strExtention;
		}

		final
		function MakeFontFileName($strFontID)
		{
			return $strFontID;
		}

		final
		function MakeVideoFileName($strVideoID)
		{
			return $strVideoID;
		}

		
		final
		function PrepareDocType()
		{
			$this->SetOutputItem("DOCTYPE",MakeDocType("html5"));
		}

		final
		function RenderWDKHelp()
		{
			return 
MakeDocType("html5")."\n".
"<html>
	<head>
		<title>WDK HELP</title>
	</head>
	<body>
		<h1>WDK HELP</h1>
		<pre>".
FileRead(GetWDKDir() . "wdk_help.txt").
"		<pre/>		
	</body>
</html>";
			
		}

		function OnRenderSpecialPages()
		{
			if (GetBoolValue($this->GetConfig("wdkhelp_disable")) == false)
			{
				if (GetBoolValue($this->GetParam("wdkhelp")) == true)
				{
					return $this->RenderWDKHelp();
				}
			}
			return "";
		}

		final
		function RenderSpecialPages()
		{
			return $this->OnRenderSpecialPages();
		}
		
		final
		function RenderHeader($strInput)
		{
			$strHeader = "<head>\n";
			$strHeader .= "\t<title>".$this->GetWebsiteTitle()."</title>\n";
			$strHeader .= "\t<meta charset=\"utf-8\"/>\n";  
			$strHeader .= "\t<meta name=\"author\" content=\"" .$this->GetMeta("author"). "\"/>\n";
			$strHeader .= "\t<meta name=\"description\" content=\"".$this->GetWebsiteDescription()."\"/>\n";
			$strHeader .= "\t<meta name=\"keywords\" content=\"".$this->GetWebsiteKeywords()."\"/>\n";
	   	$strHeader .= "\t<meta name=\"format-detection\" content=\"telephone=no\"/>\n"; // This is to avoid numbers to be erroneously detected as telephone numbers on mobile devices.
			$strHeader .= "\t<meta name=\"robots\" content=\"index,follow\"/>\n";
	  	//$strHeader .= "	<meta name=\"language\" content=\"".StringLowerCase($this->GetLanguage())."\"/>\n";
	   	//$strHeader .= "	<meta http-equiv=\"content-language\" content=\"".StringLowerCase($this->GetLanguage())."\"/>\n";

	   	
	   	// This is to prevent IE from searching the browserconfig.xml file
	   	$strHeader .= "\t<meta name=\"msapplication-config\" content=\"none\"/>\n";
	   	
	   	// Support Google to identify the URL that is really relevant for this page
	   	$arrayParams = array();
	   	$arrayParams["content"] = $this->GetContent();
	   	$strPreferredLink = $this->MakeSitemapURL($arrayParams,true);
	   	$strHeader .= "\t<link rel=\"canonical\" href=\"".$strPreferredLink."\" />\n";
	   	
	   	if ($this->IsMonolingual() == false)
	   	{
	   		$arrayLanguages = $this->GetSupportedLanguages();
	   		foreach ($arrayLanguages as $strLanguage)
	   		{
			   	$arrayParams['language'] = $strLanguage;
	   			$strLanguageLink = $this->MakeSitemapURL($arrayParams,true);
	   			$strHeader .= "\t<link rel=\"alternate\" href=\"".$strLanguageLink."\" hreflang=\"".StringLowerCase($strLanguage)."\" />\n";
	   		}
	   	}

			foreach ($this->m_arrayHeaders as $strAdditionalHeader)
			{
				$strHeader .= "\t".$strAdditionalHeader."\n";
			}
			
			$strHeader .= "</head>\n\n";
			
			$strOutput = ReplaceTagsBraces($strInput,array('HEADER' => $strHeader));
		
			return $strOutput;
		}
		
		final
		function RenderHeaderStyleSheets($strInput)
		{
			$strHeader = '';


			// Load layout configuration
			
			$strLayoutConfigFile = $this->GetAssemblyLine()->GetLayoutConfigFilePath($this->GetLayout());
						
			$arrayLayoutConfig = array();			
			if ($strLayoutConfigFile != '')
			{
				$this->SetLayoutConfigItems(LoadNamedArrayFromTextFile($strLayoutConfigFile));	
				//$this->Trace('Layout config items:');
				//$this->Trace($this->GetAllLayoutConfigItems());
			}
	
			// Add additional CSS files to be included
			foreach ($this->m_arrayStyleSheets as $strTitle => $arrayStyleSheet)
			{
				//$this->Trace('RenderHeaderStyleSheets: Adding style sheet: '.$strTitle.':');
				//$this->Trace($arrayStyleSheet);
				$strURL = ArrayGetValue($arrayStyleSheet,'url');
				if ($strURL != '')
				{
					if (IsValidURL($strURL) == true)
					{
						$strIntegrityAttribute = '';
						$strIntegrity = ArrayGetValue($arrayStyleSheet,'integrity');
						if ($strIntegrity != '')
						{
							$strIntegrityAttribute = ' integrity="'.$strIntegrity.'" crossorigin="anonymous"';
						}
						$strHeader .= '	<link href="'.$strURL.'" rel="stylesheet"'.$strIntegrityAttribute.'>'."\n";						
					}
					continue;
				}
				
				$strID = ArrayGetValue($arrayStyleSheet,'id');
				if ($strID != '')
				{
					if (IsFile($this->m_strStyleSheetsDir.$strID.'.css'))
					{
						$strHeader .= '	<link href="'.$this->GetRootURL().'css/'.$strID.'.css" rel="stylesheet">'."\n";
					}
					else
					{
						$strCssFile = $this->GetAssemblyLine()->GetStyleSheetFilePath($strID);
						if ($strCssFile != '')
						{
							$strCss = FileRead($strCssFile);
							if ($strCss != '')
							{						
								if (ArrayCount($this->GetAllLayoutConfigItems()) > 0)
								{
									$strCss = ReplaceTagsBraces($strCss,$this->GetAllLayoutConfigItems());
								}			
								
								$strHeader .= "	<style media=\"screen\">\n";
								$strHeader .= "		/*<![CDATA[*/\n";
								$strHeader .= $strCss;
								$strHeader .= "		/*]]>*/\n";
								$strHeader .= "	</style>\n"; 						
							}
						}
					}
					continue;
				}
				
				$strSourceCode = ArrayGetValue($arrayStyleSheet,'sourcecode');
				if ($strSourceCode != '')
				{
					$strHeader .= "	<style media=\"screen\">\n";
					$strHeader .= "		/*<![CDATA[*/\n";
					$strHeader .= $strSourceCode;
					$strHeader .= "		/*]]>*/\n";
					$strHeader .= "	</style>\n"; 
				}
			}
	
			// Add Layout CSS

			$strLayoutCssFile = $this->GetAssemblyLine()->GetLayoutCssFilePath($this->GetLayout());
			if ($strLayoutCssFile != "")
			{
				$strCss = FileRead($strLayoutCssFile);
				if ($strCss != "")
				{
					if (	(ArrayCount($this->GetAllLayoutConfigItems()) == 0)
						&& 	(IsFile($this->m_strStyleSheetsDir.GetFileNameFromPath($strLayoutCssFile))))
					{
						$strHeader .= '	<link href="'.$this->GetRootURL().'css/'.GetFileNameFromPath($strLayoutCssFile).'" rel="stylesheet">'."\n";
					}
					else
					{
						if (ArrayCount($this->GetAllLayoutConfigItems()) > 0)
						{
							$strCss = ReplaceTagsBraces($strCss,$this->GetAllLayoutConfigItems());
						}
						$strHeader .= "	<style media=\"screen\">\n";
						$strHeader .= "		/*<![CDATA[*/\n";
						$strHeader .= $strCss;
						$strHeader .= "		/*]]>*/\n";
						$strHeader .= "	</style>\n";
					}
				}
			}

			$strOutput = ReplaceString($strInput,'</head>',$strHeader."\n</head>");
			return $strOutput;
		}
		
		
		final
		function LoadContent($strContentID)
		{
			if ($strContentID == "")
			{
				return false;	
			}
			$strContentFilePHP = $this->GetAssemblyLine()->GetContentIncFilePath(
				$strContentID,
				$this->GetLanguage());
			
      if ($strContentFilePHP != "")
      {
      	$this->Trace("CMyContent");
        require_once($strContentFilePHP);
        $content = new CMyContent($this,$strContentID); 
      }
      else
      {
      	$this->Trace("CContent");
      	$content = new CContent($this,$strContentID);	
      }
      
    	$strContentHTML = $content->RenderContent();
			return $strContentHTML;
		}
		
		final		
		function RenderContent()
		{
			$strContentID = $this->GetContent();
			
			$arrayRequiredConditions = $this->OnGetRequiredConditionsForContent($strContentID);
			if (is_array($arrayRequiredConditions))
			{
				foreach ($arrayRequiredConditions as $strCondition => $strError)
				{
					if ($strCondition == "")
					{
						$this->Error("OnGetRequiredConditionsForContent() returned an invalid array. The keys must contain the condition and the values must contain the error if the condition is not met - usually INSUFFICIENT_PRIVILEGES.");	
					}
					if ($this->CheckCondition($strCondition) != true)
					{
						$this->Trace("RenderContent: Condition check for \"$strCondition\" has failed.");
						$this->SetErrorMessageCode($strError);
						$this->SetOutputItem("CONTENT","");
						return;
					}
					else
					{
						$this->Trace("RenderContent: Condition check passed for \"$strCondition\".");
					}
				}	
			}			
			
			$this->Trace("OnRenderSpecialContent START");
			$strSpecialContent = $this->OnRenderSpecialContent($strContentID);
			$this->Trace("OnRenderSpecialContent END");
			
			if ($strSpecialContent != "")
			{
				$this->Trace("RenderContent: Special content is used.");
				$this->SetOutputItem("CONTENT",$strSpecialContent);	
				return;
			}
    	$strContentHTML = $this->LoadContent($strContentID);
      if ($strContentHTML === false)
    	{
    		$strContentHTML = $this->OnRenderContentException();
    	}
    	if ($strContentHTML !== false)
    	{
				$this->SetOutputItem("CONTENT",$strContentHTML);
			}			
		}
		
		final		
		function PrepareMessages()
		{
			// MESSAGE STACK
			$messagestack = new CMessageStackElement($this,"default");
			$this->SetOutputItem(
				"MESSAGES",
				$messagestack->RenderMessageStack(
					$this->GetAllMessages()));
			
			   
		   // UNDER CONSTRUCTION
		   $strContentHTML="";
			if ($this->GetConfigBoolean("underconstruction"))
			{
				$strMessage = $this->GetMessage("warning","UNDERCONSTRUCTION");
				$msg = new CMessageElement($this,"underconstruction");
				$strContentHTML = $msg->RenderMessage($strMessage);
		   }
		   $this->SetOutputItem("MESSAGE_UNDERCONSTRUCTION",$strContentHTML);
		}

		final		
		function PrepareOpenGraph()
		{
			if ($this->GetConfigBoolean('opengraph'))
			{
				$strImageURL = $this->OnGetOpenGraphImageURL();
				if ($strImageURL != '')
				{
					$this->AddHeader('<meta property="og:image" content="'.$strImageURL.'">');
				}
		   	$arrayParams = array();
		   	$arrayParams["content"] = $this->GetContent();
		   	$strURL = $this->MakeSitemapURL($arrayParams,true);
				$this->AddHeader('<meta property="og:url" content="'.$strURL.'">');
				$this->AddHeader('<meta property="og:type" content="website">');
				$this->AddHeader('<meta property="og:title" content="'.$this->OnGetWebsiteTitle().'">');
				$this->AddHeader('<meta property="og:image:alt" content="'.$this->OnGetWebsiteTitle().'">');
				$this->AddHeader('<meta property="og:description" content="'.$this->OnGetWebsiteDescription().'">');
			}			
		}
		
		final
		function GetAllMessages()
		{
			return $this->m_arrayMessages;
		}

		final
		function GetErrorMessages()
		{
			return ArrayGetValue($this->m_arrayMessages,"error");
		}

		final
		function GetWarningMessages()
		{
			return ArrayGetValue($this->m_arrayMessages,"warning");
		}

		final
		function GetInfoMessages()
		{
			return ArrayGetValue($this->m_arrayMessages,"info");
		}

		final
		function CountAllMessages()
		{
			return 		ArrayCount($this->GetErrorMessages())
							+ ArrayCount($this->GetWarningMessages())
							+ ArrayCount($this->GetInfoMessages());
		}

		
		final
		function GetMessage($strMessageType,$strMessageID,$strPrefix = "")
		{
			if ($strMessageType == "error")
			{
				$this->SetOutputItem("ERROR",StringUpperCase($strMessageID));
			}
			$strPrefix = StringUpperCase($strPrefix);
			if ($strPrefix != "")
			{
				$strPrefix .= "_";	
				$strResID = StringUpperCase("TID_".$strPrefix.$strMessageType."_".$strMessageID);
				$this->Trace("GetMessage: Trying \"$strResID\"");
				$strMessage = $this->LoadString($strResID);
				if (($strMessage !== false) && ($strMessage != ""))
				{
					$this->Trace("GetMessage: \"$strMessage\"");
					return $strMessage;
				}
			} 
			$strResID = StringUpperCase("TID_".$strMessageType."_".$strMessageID);
			$this->Trace("GetMessage: Trying \"$strResID\"");
			$strMessage = $this->LoadString($strResID);
			if (($strMessage !== false) && ($strMessage != ""))
			{
				$this->Trace("GetMessage: \"$strMessage\"");				
				return $strMessage;
			}
			if ($strMessageType == "error")
			{
				$strMessage = $this->LoadString("TID_ERROR_UNKNOWN");
				if (($strMessage !== false) && ($strMessage != ""))
				{
					$this->Trace("GetMessage: \"$strMessage\"");					
					return $strMessage;
				}
			}
			// Fallback to English standard messages
			if ($strMessageType == "error")
			{
				$strMessage = "An unknown error has occured (ERRORCODE={ERROR}).";
				$this->Trace("GetMessage: \"$strMessage\"");		
				return $strMessage;
			}
			if ($strMessageType == "underconstruction")
			{
				$strMessage = "This site is under construction";
				$this->Trace("GetMessage: \"$strMessage\"");		
				return $strMessage;

			}
			if ($strMessageType == "warning")
			{
				$strMessage = "A condition has occured that produced a warning.";
				$this->Trace("GetMessage: \"$strMessage\"");		
				return $strMessage;

			}
			$this->Error("Could not get message for type \"$strMessageType\" and id \"$strMessageID\" (prefix=\"$strPrefix\"). LoadString returned: \"$strMessage\"");
		}		
		
		
		final
		function ReplaceAllOutputItems($strOutput)
		{
			$strOutput = $this->ReplaceOutputItems($strOutput);	
			$strOutput = $this->ReplaceGenericOutputItems($strOutput);
			return $strOutput;
		}
		
		final
		function ReplaceGenericOutputItems($strOutput)
		{
			$nFirstTagStartPos = 0;

			$nOutputLen = StringLength($strOutput);			
			$arrayTags = array();
			foreach ($this->m_arrayGenericOutputItems as $strItemID => $arrayAttributeList)
			{
				$nNextTagStartPos = $nFirstTagStartPos-1;
				
				$strTagStart = "{".$strItemID;
				$strTagEnd = "}";
				
				do
				{
					$nNextTagStartPos++;

					$nNextTagStartPos = FindString($strOutput,$strTagStart,false,$nNextTagStartPos);
					if ($nNextTagStartPos != -1)
					{
						$strFound = "";
						$nNextTagStartPos = FindStringMultiple(
							$strOutput, 
							array(
								$strTagStart.$strTagEnd,
								$strTagStart." ",
								$strTagStart."\t",
								$strTagStart."\n",
								$strTagStart."\r\n"
								),
							$strFound,
							false,
							$nNextTagStartPos);
	
						if ($nNextTagStartPos != -1)
						{
							
							$nNextTagEndPos = FindString($strOutput, $strTagEnd, false, $nNextTagStartPos);
							if ($nNextTagEndPos == -1)
							{
								$this->Error("Syntax Error after \"{".$strItemID."\": Missing end tag \"$strTagEnd\". nNextTagStartPos=$nNextTagStartPos. nNextTagEndPos=$nNextTagEndPos.");
							}
							if ($nNextTagEndPos < $nNextTagStartPos)
							{
								$this->Error("nNextTagEndPos($nNextTagEndPos) < nNextTagStartPos($nNextTagStartPos)");	
							}
							
							$strItemTag = StringSection(
								$strOutput,
								$nNextTagStartPos,
								$nNextTagEndPos - $nNextTagStartPos + StringLength($strTagEnd)
								);
								
							$arrayAttributes = array();
							foreach ($arrayAttributeList as $strAttributeName)
							{
								$arrayAttributes[$strAttributeName] = GetAttributeFromXMLTag($strItemTag,$strAttributeName);
							} 
							
							$strOutputItem = $this->OnRenderGenericOutputItem(
								$strItemID,
								$arrayAttributes);
								
							if ($strOutputItem != '')
							{
								$arrayTags[$strItemTag] = $strOutputItem;
							}
						}
					}
				} while ($nNextTagStartPos != -1);
			}
			return ReplaceTags($strOutput,$arrayTags,'',''); 
		}


		
		final
		function RegisterGenericOutputItem(
			$strItem,
			$arrayAttributes = array())
		{
			$strItem = StringUpperCase($strItem);
			$this->m_arrayGenericOutputItems[$strItem] = $arrayAttributes;
		}
		
		function OnRenderGenericOutputItem($strItem, $arrayAttributes)
		{
			foreach ($this->m_arrayPlugins as $plugin)
			{
				$strOutput = $plugin->OnRenderGenericOutputItem($strItem, $arrayAttributes);
				if ($strOutput != '')
				{
					return $strOutput;	
				}
			}
			
			switch ($strItem)
			{
			case 'URL':
				$arrayParams = array();
				$arrayParams["content"] = ArrayGetValue($arrayAttributes,"content");
				$strLanguage = ArrayGetValue($arrayAttributes,"language");
				if ($strLanguage == "")
				{
					$arrayParams["language"] = $this->GetLanguage();
				}
				else
				{
					$arrayParams["language"] = $strLanguage;
				}
				return $this->MakeURL($arrayParams);
			
			case "ICON":
				if ($this->m_elementIcon == null)
				{
					$this->m_elementIcon = new CIconElement($this,'default');	
				}
				return $this->m_elementIcon->RenderIcon(ArrayGetValue($arrayAttributes,'id'));
				
			case "IMAGEURL":
				$strID = ArrayGetValue($arrayAttributes,"id");
				if ($strID == "")
				{
					return "";	
				}
				$strExt = ArrayGetValue($arrayAttributes,"ext");
				$strContext = ArrayGetValue($arrayAttributes,"context");
				if ($strContext == "")
				{
					$strContext = "html";
				}
				$strURL = $this->GetImageURL($strID,$strExt);

				if ($strContext == "html")
				{
					$strURL = HtmlEncode($strURL);
				}
				return $strURL;
			
			case "FONTURL":
				$strID = ArrayGetValue($arrayAttributes,"id");
				if ($strID == "")
				{
					return "";	
				}
				$strContext = ArrayGetValue($arrayAttributes,"context");
				if ($strContext == "")
				{
					$strContext = "html";
				}
				$strURL = $this->GetFontURL($strID);

				if ($strContext == "html")
				{
					$strURL = HtmlEncode($strURL);
				}
				return $strURL;							


			case "VIDEOURL":
				$strID = ArrayGetValue($arrayAttributes,"id");
				if ($strID == "")
				{
					return "";	
				}
				$strContext = ArrayGetValue($arrayAttributes,"context");
				if ($strContext == "")
				{
					$strContext = "html";
				}
				$strURL = $this->GetVideoURL($strID);

				if ($strContext == "html")
				{
					$strURL = HtmlEncode($strURL);
				}
				return $strURL;							

							
			case "IMAGE":
				$strID = ArrayGetValue($arrayAttributes,"id");
				if ($strID == "")
				{
					return "";	
				}
				$strExt = ArrayGetValue($arrayAttributes,"ext");
				$strStyle = ArrayGetValue($arrayAttributes,"style");
				if ($strStyle == "")
				{
					$strStyle = "default";
				}
				$strText = ArrayGetValue($arrayAttributes,"text");
				$strURL = $this->GetImageURL($strID,$strExt);

				$this->IncludeElement("image");
				$elementImage = new CImageElement($this,$strStyle);
				return $elementImage->RenderImage($strURL,$strText);

			case "TEASER":
				$strStyle = ArrayGetValue($arrayAttributes,"style");
				if ($strStyle == "")
				{
					$strStyle = "default";
				}
				$this->IncludeElement("teaser");
				$teaser = new CTeaserElement($this,$strStyle);
				return $teaser->RenderTeaser(
					ArrayGetValue($arrayAttributes,"title"),
					ArrayGetValue($arrayAttributes,"text"),
					ArrayGetValue($arrayAttributes,"linkurl"),
					ArrayGetValue($arrayAttributes,"linktext"),
					GetBoolValue(ArrayGetValue($arrayAttributes,"externallink"))
					);

			case "IMAGETEASER":
				$strStyle = ArrayGetValue($arrayAttributes,"style");
				if ($strStyle == "")
				{
					$strStyle = "default";
				}
				$this->IncludeElement("imageteaser");
				$teaser = new CImageTeaserElement($this,$strStyle);
				return $teaser->RenderImageTeaser(
					ArrayGetValue($arrayAttributes,"imageurl"),
					ArrayGetValue($arrayAttributes,"title"),
					ArrayGetValue($arrayAttributes,"text"),
					ArrayGetValue($arrayAttributes,"linkurl"),
					ArrayGetValue($arrayAttributes,"linktext"),
					GetBoolValue(ArrayGetValue($arrayAttributes,"externallink"))
					);

			case "LINK":
				$strStyle = ArrayGetValue($arrayAttributes,'style');
				if ($strStyle == "")
				{
					$strStyle = 'default';
				}
				$strURL = ArrayGetValue($arrayAttributes,'url');
				$strContent = ArrayGetValue($arrayAttributes,'content');
				if ($strURL == '' && $strContent != '')
				{
					$strURL = $this->MakeContentURL($strContent);
				}
				if ($strStyle == 'default')
				{
					if ($this->m_elementLink == null)
					{
						$this->m_elementLink = new CLinkElement($this,'default');	
					}
					return $this->m_elementLink->RenderLink(
						$strURL,
						ArrayGetValue($arrayAttributes,'text'),
						ArrayGetValue($arrayAttributes,'externallink'));
					
				}
				else
				{
					$elementLink = new CLinkElement($this,$strStyle);	
					return $elementLink->RenderLink(
						$strURL,
						ArrayGetValue($arrayAttributes,'text'),
						ArrayGetValue($arrayAttributes,'externallink'));
				}

			case "PHONE":
				if ($this->m_elementTel == null)
				{
					$this->IncludeElement("tel");
					$this->m_elementTel = new CTelElement($this,"default");	
				}
				return $this->m_elementTel->RenderTel(
					ArrayGetValue($arrayAttributes,"tel"),
					ArrayGetValue($arrayAttributes,"text"));

			case "DOWNLOADURL":
				$strFilePath = ArrayGetValue($arrayAttributes,"filepath");
				if ($strFilePath == "")
				{
					return "";	
				}
				$strURL = $this->GetDownloadURL($strFilePath);
				//$strURL = HtmlEncode($strURL);
				return $strURL;

			case "YOUTUBE":
				$strID = ArrayGetValue($arrayAttributes,"id");
				if ($this->m_elementYouTube == null)
				{
					$this->IncludeElement("youtube");
					$this->m_elementYouTube = new CYouTubeElement($this,"default");	
				}
				return $this->m_elementYouTube->RenderYouTube($strID);
							

			case "PORTFOLIO":
				$strStyle = ArrayGetValue($arrayAttributes,"style");
				if ($strStyle == "")
				{
					$strStyle = "default";
				}
				
				$this->IncludeElement("portfolio");
				$portfolio = new CPortfolioElement($this,$strStyle);
				
				$arrayPortfolio = array();
				for ($nIndex = 1; $nIndex <= 20; $nIndex++)
				{
					$arrayTeaser = array();
					$strPayload = ArrayGetValue($arrayAttributes,"payload".$nIndex);
					if ($strPayload != "")
					{
						$arrayTeaser["payload"] = $strPayload;
					}
					else 
					{
						$strTitle = ArrayGetValue($arrayAttributes,"title".$nIndex);
						if ($strTitle != "")
						{
							$arrayTeaser["imageurl"] = ArrayGetValue($arrayAttributes,"imageurl".$nIndex);
							$arrayTeaser["title"] = ArrayGetValue($arrayAttributes,"title".$nIndex);
							$arrayTeaser["text"] = ArrayGetValue($arrayAttributes,"text".$nIndex);
							$arrayTeaser["linktext"] = ArrayGetValue($arrayAttributes,"linktext".$nIndex);
							$arrayTeaser["linkurl"] = ArrayGetValue($arrayAttributes,"linkurl".$nIndex);
							$arrayTeaser["external"] = GetBoolValue(ArrayGetValue($arrayAttributes,"external".$nIndex));
						}
					}
					
					if (ArrayCount($arrayTeaser) > 0)
					{
						$arrayPortfolio[] = $arrayTeaser;	
					}
					
				}
				
				if (ArrayCount($arrayPortfolio) > 0)
				{
					return $portfolio->RenderPortfolio($arrayPortfolio);
				}
				return "";								

			case "COUNTDOWN":
				if ($this->m_elementCountDown == null) 
				{
					$this->IncludeElement('countdown');
					$this->m_elementCountDown = new CCountDownElement($this,'default');	
				}
				$strDateTime = ArrayGetValue($arrayAttributes,'datetime');
				$strTimeZone = ArrayGetValue($arrayAttributes,'timezone');
				$time = GetTime($strDateTime,$strTimeZone);
				return $this->m_elementCountDown->RenderCountDown($time);   
						
			default:
				$this->Error('OnRenderGenericOutputItem: No function handles item id: '.$strItem);
			}
		}
		
		

		final
		function ReplaceOutputItems($strSource)
		{
		   return ReplaceTagsBraces($strSource,$this->m_arrayOutputItems);
		}

		final
		function InitReplaceTextCache()
		{
			if (ArrayCount($this->m_arrayResText) == 0)
			{
				$arrayRes = array();
				if ($this->IsFallbackLanguageSupported() == true)
				{
					if ($this->GetFallbackLanguage() != $this->GetLanguage())
					{
						$arrayRes = $this->m_res->GetStrings($this->GetFallbackLanguage());
					}
				}
				$arrayRes = ArrayMerge($arrayRes,$this->m_res->GetStrings($this->GetLanguage()));
				foreach ($arrayRes as $strKey => $strValue)
				{
					$this->m_arrayResText[StringSection($strKey,4)] = ReplaceString($strValue,"\n","<br/>"); 
				}
			}
			return $this->m_arrayResText;
		}	

		
		final
		function ResetReplaceTextCache()
		{
			$this->m_arrayResText = array();
		}	
		
		final		
		function ReplaceText($strSource,$bClearCache = true)
		{
			$arrayResText = $this->InitReplaceTextCache();
			$this->Trace("ReplaceText: restags=".ArrayCount($this->m_arrayResText)." textlength=".StringLength($strSource));
			$strOutput = ReplaceTags(
				$strSource,
				$arrayResText,
				"?TID_", 
				"?");
			if ($bClearCache == true)
			{
				$this->ResetReplaceTextCache();
			}
			return $strOutput;
		}

		final
		function RenderCode($strInput)
		{
			$nCount = 0;
			$strOutput = "";
			do
			{
				$nCodeStartPos = FindString($strInput,"<code>");
				if ($nCodeStartPos != -1)
				{
					$strOutput .= StringSection($strInput,0,$nCodeStartPos);
					$strInput = StringSection($strInput,$nCodeStartPos+6);
					
					$nCodeEndPos = FindString($strInput,"</code>");
					if ($nCodeEndPos != -1)
					{
						$strCode = StringSection($strInput,0,$nCodeEndPos);
						if ($this->m_elementSourceCode == null)
						{
							$this->IncludeElement("sourcecode");
							$this->m_elementSourceCode = new CSourceCodeElement($this,"default");	
						}
						$strOutput .= $this->m_elementSourceCode->RenderSourceCode($strCode);
						$strInput = StringSection($strInput,$nCodeEndPos+7);
					}
					else
					{
						$strOutput .= $strInput;
						$strInput = "";
					}
				}
				else
				{
					$strOutput .= $strInput;
					$strInput = "";
				}
			}
			while ($strInput != "" && $nCount < 20);  

			return $strOutput; 
		}


		
		final
		function RenderJavaScripts($strInput)
		{
			$strScriptHTML = '';
			
			// Make sure some scripts come first if they exist
			$this->m_arrayJavaScripts = ArrayKeyFirst($this->m_arrayJavaScripts,'wdk-trace-activate');
			$this->m_arrayJavaScripts = ArrayKeyFirst($this->m_arrayJavaScripts,'wdk');
			$this->m_arrayJavaScripts = ArrayKeyFirst($this->m_arrayJavaScripts,'jquery');
			
			foreach ($this->m_arrayJavaScripts as $strTitle => $arrayJavaScript)
			{
				$strURL = ArrayGetValue($arrayJavaScript,'url');
				if (IsValidURL($strURL) == true)
				{
					$strIntegrityAttribute = '';
					$strIntegrity = ArrayGetValue($arrayJavaScript,'integrity');
					if ($strIntegrity != '')
					{
						$strIntegrityAttribute = ' integrity="'.$strIntegrity.'" crossorigin="anonymous"';
					}
					
					$strScriptHTML .= "\n".'<script src="'.$strURL.'"'.$strIntegrityAttribute.'></script>';
					continue;
				}
				
				$strSourceCode = ArrayGetValue($arrayJavaScript,'sourcecode');
				if ($strSourceCode != "")
				{
					$strScriptHTML .= "\n<script>\n$strSourceCode\n</script>";
					continue;
				}
				
				$strScriptID = ArrayGetValue($arrayJavaScript,'id');
				if ($strScriptID != '')
				{
					if (IsFile($this->m_strJavaScriptsDir.$strScriptID.'.js') == true)
					{
						$strScriptHTML .= "\n".'<script src="'.$this->GetRootURL().'js/'.$strScriptID.'.js"></script>';
						continue;
					}
					$strJavaScriptFile = $this->GetJavaScriptFilePath($strScriptID);
				}
				else
				{
					$strJavaScriptFile = 	ArrayGetValue($arrayJavaScript,'filepath');
				}

				if ($strJavaScriptFile != "")
				{
					$strJavaScriptFileContent = FileRead($strJavaScriptFile);
					if ($strJavaScriptFileContent != '')
					{
						$strScriptHTML .= "\n<script>\n$strJavaScriptFileContent\n</script>";
						continue;
					}
				}
			}
			
			$strScriptHTML = $this->ReplaceOutputItems($strScriptHTML);
			
			$nLengthBefore = StringLength($strInput);
			$strOutput = ReplaceTagsWDK($strInput,array("SCRIPTS" => $strScriptHTML));
			$nLengthAfter = StringLength($strOutput);
			
			if ($nLengthBefore == $nLengthAfter) // Nothing happened! So fallback to the body tag
			{
				$strOutput = ReplaceString($strOutput,"</body>",$strScriptHTML."\n\n</body>");
			}
			
			$this->m_arrayJavaScripts = array();

			return $strOutput;
		}

		final  
		function RenderWebSite()
		{
			$this->Trace('RenderWebsite BEGIN');
			$this->ProfilingTrace();

			// Initialize default placeholders
			$this->SetOutputItem('ROOTURL', $this->GetRootURL());
			$this->SetOutputItem('ROOTPATH', $this->GetRootPath());
			$this->SetOutputItem('SELFURL',$this->MakeSelfURL());
			$this->SetOutputItem('INDEXURL',$this->MakeIndexURL());

			$this->SetOutputItem('CSSPATH',$this->GetRootURL() . 'css/');
			$this->SetOutputItem('IMAGEPATH',$this->GetImageRootURL());
			$this->SetOutputItem('DOWNLOADPATH', $this->GetDownloadRootURL());
			
			$this->SetOutputItem('LAYOUT',$this->GetLayout());
			$this->SetOutputItem('LANGUAGE',StringLowerCase($this->GetLanguage()));

			// Render special pages			
			$strOutput = $this->RenderSpecialPages();
			if ($strOutput != '')
			{
				return $strOutput;	
			}
			
			$this->PrepareDocType();

			// Render Content
			$this->Trace('Rendering content START');			
			$this->OnPreContent();
			$this->RenderContent();
			$this->OnPostContent();
			$this->Trace('Rendering content END');
			$this->ProfilingTrace();
			
			// Prepare Layout
			$strLayoutFilePHP = $this->GetAssemblyLine()->GetLayoutIncFilePath($this->GetLayout());
      if ($strLayoutFilePHP != "")
      {
      	$this->Trace("CMyLayout");
        require_once($strLayoutFilePHP);
        $layout = new CMyLayout($this,$this->GetLayout()); 
      }
      else
      {
      	$this->Trace("CLayout");
      	$layout = new CLayout($this,$this->GetLayout());	
      }
    	$bLayoutInit = $layout->Init();
    	if ($bLayoutInit == false)
    	{
    		$this->Error("Layout init failed.");	
    	}

			$this->Trace("Loading framework file START");
			$strLayoutFrameworkFile = $this->GetFrameworkLayoutFile();
			if ($strLayoutFrameworkFile == "")
			{
				$this->Error("No framework layout file found.");
			}
	
			$strOutput = FileRead($strLayoutFrameworkFile);
			$this->Trace("Loading framework file END");
			$this->ProfilingTrace();
			
			// Render Navigation
			$this->Trace("OnRenderNavigation START");
			$this->OnRenderNavigation($strOutput);
			$this->Trace("OnRenderNavigation END");
			$this->ProfilingTrace();
			
			// Render Message Stack
			$this->Trace("PrepareMessages START");
			$this->PrepareMessages();
			$this->Trace("PrepareMessages END");
			$this->ProfilingTrace();
			
			// Prepare OpenGrapg
			$this->Trace("PrepareOpenGraph START");
			$this->PrepareOpenGraph();
			$this->Trace("PrepareOpenGraph END");
			$this->ProfilingTrace();
		

			// Render Header
			$this->Trace("RenderHeader START");
			$strOutput = $this->RenderHeader($strOutput);
			$this->Trace("RenderHeader END");
			$this->ProfilingTrace();
			
			
			$this->CheckInvalidTID();
			$this->ProfilingTrace();
			
			
			$arrayBodyItems = $this->GetAllBodyItems();
			if (ArrayCount($arrayBodyItems) > 0)
			{
				// No body tag, no body items!
				if (FindString($strOutput,"</body>") != -1)
				{
					foreach ($arrayBodyItems as $strBodyItem)
					{
						$strOutput = ReplaceString($strOutput,"</body>","\t".$strBodyItem."\n</body>");
					}
				}
			}

			if ($this->IsTraceActive())
			{
				$this->AddJavaScript('wdk');
				$this->AddJavaScript('wdk-trace-activate');
			}

			$this->Trace("Tag replacement START");
			
			$stopwatchTagReplacement = new CStopWatch();
			$stopwatchTagReplacement->Start();
			
			$stopwatchReplaceOutputItems = new CStopWatch();
			$stopwatchReplaceText = new CStopWatch();
			$stopwatchReplaceGenericOutputItems = new CStopWatch();
			
			$nIteration = 5;
			$nOldOutputStrLen = StringLength($strOutput);

			$this->ResetReplaceTextCache();
			
			
			while ($nIteration > 0)
			{
				$this->Trace("Rendering: $nIteration iterations to go");

				// Replace output items tags
				$this->ProfilingTrace();
				$this->Trace("Rendering: ReplaceOutputItems()");
				$stopwatchReplaceOutputItems->Start();
				$strOutput = $this->ReplaceOutputItems($strOutput);
				$stopwatchReplaceOutputItems->Stop();
				
				$nNewOutputStrLenInterim = StringLength($strOutput);
				if ($nNewOutputStrLenInterim != $nOldOutputStrLen)
				{
				  $this->ProfilingTrace();
					$this->Trace("Rendering: ReplaceOutputItems() (2)");
					$stopwatchReplaceOutputItems->Start();
				  $strOutput = $this->ReplaceOutputItems($strOutput);
				  $stopwatchReplaceOutputItems->Stop();
				}

				// Replace all text resource identifiers
				if ($this->m_bUseRes == true)
				{
					$this->ProfilingTrace();
					$this->Trace("Rendering: ReplaceText()");
					
					$stopwatchReplaceText->Start();
					//$this->Trace($strOutput);
					$strOutput = $this->ReplaceText($strOutput,false);
					$stopwatchReplaceText->Stop();
				}
	   
				// Replace generic output items (e.g. "URL")
				$this->ProfilingTrace();
				$this->Trace("Rendering: ReplaceGenericOutputItems()");
				$stopwatchReplaceGenericOutputItems->Start();
				$strOutput = $this->ReplaceGenericOutputItems($strOutput);
				$stopwatchReplaceGenericOutputItems->Stop();
				
				$this->ProfilingTrace();
				$this->Trace("Rendering: RenderJavaScripts()"); 
				$strOutput = $this->RenderJavaScripts($strOutput);

				
				$nIteration--;
   	      
 	      if ($nIteration > 0)
 	      {
 	      	$nNewOutputStrLen = StringLength($strOutput);
 	      	$this->Trace("Rendering: Old length: $nOldOutputStrLen");
 	      	$this->Trace("Rendering: New length: $nNewOutputStrLen");
 	      	if ($nNewOutputStrLen == $nOldOutputStrLen)
 	      	{
 	      		$nIteration = 0;
 	      	}
 	      	else
 	      	{
 	      		$nOldOutputStrLen = $nNewOutputStrLen;
 	      	}
 	      }
			}
			$stopwatchTagReplacement->Stop();
         
			$this->Trace("Tag replacement END");
			$this->ProfilingTrace();
			
			// Render Style Sheets
			$this->Trace("RenderHeaderStyleSheets START");
			$strOutput = $this->RenderHeaderStyleSheets($strOutput);
			$this->Trace("RenderHeaderStyleSheets END");
			$this->ProfilingTrace();
	
			// Render Code
			$this->Trace("RenderCode START");
			$strOutput = $this->RenderCode($strOutput);
			$this->Trace("RenderCode END");
			$this->ProfilingTrace();
		

			$this->Trace("Rendering: Total seconds elapsed during tag replacement___________: ".RenderNumber($stopwatchTagReplacement->GetTotalSeconds(),3));
			$this->Trace("Rendering: Total seconds elapsed in ReplaceOutputItems()__________: ".RenderNumber($stopwatchReplaceOutputItems->GetTotalSeconds(),3));
			$this->Trace("Rendering: Total seconds elapsed in ReplaceText()_________________: ".RenderNumber($stopwatchReplaceText->GetTotalSeconds(),3));
			$this->Trace("Rendering: Total seconds elapsed in ReplaceGenericOutputItems()___: ".RenderNumber($stopwatchReplaceGenericOutputItems->GetTotalSeconds(),3));
			
			$this->Trace("RenderWebsite END");
			
			return $strOutput;         
		}

		
		
		function OnInitAssemblyLine(&$assemblyLine)
		{
			$this->m_bOnInitAssemblyLineCalled = true;
		}

		function OnInitRoutingMap()
		{
			$this->m_bOnInitRoutingMapCalled = true;
		}

		
		function OnInit()
		{
			$this->m_bOnInit = true;
			
			foreach ($this->m_arrayPlugins as $plugin)
			{
				$bResult = $plugin->OnInit();
				if ($bResult == false)
				{
					$this->Trace('Plugin '.$plugin->GetID().' returned false.');
					return false;
				}
			}

			
			return true;
		}
		
		function OnInitPlugins()
		{
			return array();	
		}
		
		function OnCommand($strCommand)
		{
			if ($strCommand == "logout")
			{
				$this->UserLogout();
				return true;
			}
			if ($strCommand == "login")
			{
				$this->UserLogin(
					$this->GetParam($this->GetAuthUsernameVarName()),
					$this->GetParam($this->GetAuthPasswordVarName()));
				return true;
			}
			return true;	
		}
		
		final
		function RenderImage()
		{
			$strImageID = $this->GetParam('id');
			$strExtention = $this->GetParam('ext');
			
			$image = null;
			$bSuccess = false;
			
			if (StringCheckCharSet($strImageID,CHARSET_IMAGEID))
			{
				if ($strImageID != '')
				{
					$bSuccess = $this->OnCreateImage($image,$strImageID);
					if ($bSuccess == false)
					{
						$bSuccess = $this->RenderStaticImage($strImageID,$strExtention);
						if ($bSuccess == true)
						{
							exit();	
						}
					}
				}
			}
				
			if ($image == null || $bSuccess == false)
			{
				$this->CreateImageError($image);	
			}
			
			switch ($strExtention)
			{
			case 'ico':
				header('Content-type: image/x-icon');
				imagebmp($image);
			default:
				header('Content-type: image/png');
				imagepng($image);
			}
		
			imagedestroy($image);
			exit;	
		}


		final
		function RenderFont()
		{
			$strFontID = $this->GetParam("id");
			
			$strBinary = null;
			if ($strFontID == "")
			{
				exit();	
			}
			if (StringCheckCharSet($strFontID,CHARSET_FONTID) == false)
			{
				exit();	
			}
			$strFilePath = $this->GetFontFilePath($strFontID);	
			if ($strFilePath == false)
			{
				exit();	
			}
			$strBinary = FileReadBinary($strFilePath);
			if ($strBinary == false)
			{
				exit();	
			}
			
			/*
			
				http://www.iana.org/assignments/media-types/media-types.xhtml
				.eot -> application/vnd.ms-fontobject (as from December 2005)
				.otf -> application/font-sfnt (as from March 2013)
				.svg -> image/svg+xml (as from August 2011)
				.ttf -> application/font-sfnt (as from March 2013)
				.woff -> application/font-woff (as from January 2013)
			*/

			if (StringSection($strFilePath,-3) == "eot")
			{
				header('Content-type: application/vnd.ms-fontobject');
			}
			else if (StringSection($strFilePath,-3) == "otf")
			{
				header('Content-type: application/font-sfnt');
			}
			else if (StringSection($strFilePath,-3) == "svg")
			{
				header('Content-type: image/svg+xml');
			}			
			else if (StringSection($strFilePath,-3) == "ttf")
			{
				header('Content-type: application/font-sfnt');
			}				
			else if (StringSection($strFilePath,-4) == "woff")
			{
				header('Content-type: application/font-woff');
			}
			else if (StringSection($strFilePath,-5) == "woff2")
			{
				header('Content-type: application/font-woff');
			}
			else
			{
				header('Content-type: application/octet-stream');
			}
			print($strBinary);
			exit;	

		}


		final
		function RenderVideo()
		{
			$strVideoID = $this->GetParam("id");
			
			$strBinary = null;
			if ($strVideoID != "")
			{			
				if (StringCheckCharSet($strVideoID,CHARSET_VIDEOID))
				{
					$strFilePath = $this->GetVideoFilePath($strVideoID);	
					if ($strFilePath != false)
					{
						$strBinary = FileReadBinary($strFilePath);
					}
				}
			}
			
			/*
			
				http://www.iana.org/assignments/media-types/media-types.xhtml

			*/

/*
			if (StringSection($strFilePath,-3) == "eot")
			{
				header('Content-type: application/vnd.ms-fontobject');
			}
			else if (StringSection($strFilePath,-3) == "otf")
			{
				header('Content-type: application/font-sfnt');
			}
			else if (StringSection($strFilePath,-3) == "svg")
			{
				header('Content-type: image/svg+xml');
			}			
			else if (StringSection($strFilePath,-3) == "ttf")
			{
				header('Content-type: application/font-sfnt');
			}				
			else if (StringSection($strFilePath,-4) == "woff")
			{
				header('Content-type: application/font-woff');
			}
			else if (StringSection($strFilePath,-5) == "woff2")
			{
				header('Content-type: application/font-woff');
			}
			else
			{
				header('Content-type: application/octet-stream');
			}
			*/
			print($strBinary);
			exit;	

		}


		
		function CreateImageError(&$image)
		{
			if ($image != null)
			{
				imagedestroy($image);	
			}
			$nWidth = 20;
			$nHeight = 20;
			
			$image = imagecreate($nWidth,$nHeight);
			$colorBackground =  imagecolorallocate ( $image, 255, 255, 255);
			$colorForeground =  imagecolorallocate ( $image, 255, 0, 0);
			imagefilledrectangle($image, 0, 0, $nWidth, $nHeight, $colorBackground);
			imageline ($image , 3, 3 , $nWidth-3, $nHeight-3, $colorForeground );
			imageline ($image , $nWidth-3, 3 , 3, $nHeight-3, $colorForeground );
			
			return true;
		}
		
		function CreateImageDefaultFavicon(&$image)
		{
			if ($image != null)
			{
				imagedestroy($image);	
			}
			$nWidth = 16;
			$nHeight = 16;
			
			$image = imagecreate($nWidth,$nHeight);
			$colorBackground =  imagecolorallocate ( $image, 255, 255, 255);
			$colorForeground =  imagecolorallocate ( $image, 0, 0, 255);
			imagefilledrectangle($image, 0, 0, $nWidth, $nHeight, $colorBackground);
			// Top
			imageline ($image , 0, 0 , $nWidth-1, 0, $colorForeground );
			// Bottom
			imageline ($image , 0, $nHeight-1 , $nWidth-1, $nHeight-1, $colorForeground );
			// Left
			imageline ($image , 0, 0 , 0, $nHeight-1, $colorForeground );
			// Right
			imageline ($image , $nWidth-1, 0 , $nWidth-1, $nHeight-1, $colorForeground );
			
			return true;
		}		
		
		function OnCreateImage(&$image,$strImageID)
		{
			foreach ($this->m_arrayPlugins as $plugin)
			{
				$bSuccess = $plugin->OnCreateImage($image,$strImageID);
				if ($bSuccess == true)
				{
					return true;
				}
			}
			if ($strImageID == 'favicon')
			{
				return $this->CreateImageDefaultFavicon($image);	
			}
			return false;
		}
		
		final
		function RenderStaticImage($strImageID,$strExtention)
		{
			$strFilePath = $this->GetImageFilePath($strImageID,$strExtention);	
			if ($strFilePath == false)
			{
				$strFilePath = $this->GetLayoutImageFilePath($strImageID,$this->GetLayout(),$strExtention);
				if ($strFilePath == false)
				{
					return false;
				}
			}
			
			if (StringSection($strFilePath,-3) == "png")
			{
				header('Content-type: image/png');
				print(FileReadBinary($strFilePath));
				return true;
			}
			if (StringSection($strFilePath,-3) == "gif")
			{
				header('Content-type: image/gif');
				print(FileReadBinary($strFilePath));
				return true;
			}
			if (StringSection($strFilePath,-3) == "jpg")
			{
				header('Content-type: image/jpeg');
				print(FileReadBinary($strFilePath));
				return true;
			}
			
			return false;
		}
		

		final
		function RenderCss()
		{
			header("Content-type: css");
			$this->OnRenderCss();
			exit();
		}

		function OnRenderCss()
		{
			return $this->RenderStaticCss();
		}


		final
		function RenderDownload()
		{
			$strFilePathRel = $this->GetParam("filepath");
			if ($strFilePathRel == "")
			{
				return;	
			}
			 
			$strFilePath = $this->GetAssemblyLine()->GetDownloadFilePath($strFilePathRel);
			if ($strFilePath == false)
			{
				return;	
			}
			$strContent = FileReadBinary($strFilePath);
			if ($strContent == false)
			{
				return;	
			}
			$strMimeType = GetMimeTypeFromPath($strFilePath);
			$this->DownloadFile(
				$strMimeType,
				GetFileNameFromPath($strFilePathRel),
				$strContent);

			return;
		}

		final
		function RenderSitemap()
		{
			// Creates a sitemap according to http://www.sitemaps.org/protocol.html
		
			header('Content-type: application/xml');
			
			$this->SetContent('');
			$arraySitemap = $this->OnMakeSitemap();
			print('<?xml version="1.0" encoding="UTF-8"?>
<urlset
	xmlns="http://www.sitemaps.org/schemas/sitemap/0.9"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.sitemaps.org/schemas/sitemap/0.9 http://www.sitemaps.org/schemas/sitemap/0.9/sitemap.xsd"
	>
');
			if (!is_array($arraySitemap))
			{
				print("<!-- Sitemap is empty -->");
			}
			else
			{
				foreach ($arraySitemap as $arrayURL)
				{
					$strLoc = ArrayGetValue($arrayURL,"loc");
					if ($strLoc != "")
					{
						$strChangefreq = 	ArrayGetValue($arrayURL,"changefreq");
						$strPriority = 	ArrayGetValue($arrayURL,"priority");
						
						print("\t<url>\n");
						print("\t\t<loc>$strLoc</loc>\n");
						if ($strChangefreq != "")
						{
							print("\t\t<changefreq>$strChangefreq</changefreq>\n");
						}
						if ($strPriority != "")
						{
							print("\t\t<priority>$strPriority</priority>\n");
						}
						print("\t</url>\n");
					}
				}
			}
			print("</urlset>\n");			
			if ($this->IsTraceActive() == true)
			{
				print("<!--\n");
				print($this->GetTraceLog());	
				print("\n-->\n");
			}		
			exit;	
		}

		function OnMakeSitemap()
		{
			$arraySitemap = array();
			
			$arrayLanguages = $this->GetSupportedLanguages();
			foreach ($arrayLanguages as $strLanguage)
			{
				$navigationtree = new CNavigationTree();
				$navigationtree->LoadNavigationTree($this,'',$strLanguage);
				$navigationtree->SetContextParameterName("content");
				$navigationtree->FilterItemsForSitemap($this);
			
				$arrayItems = $navigationtree->GetNavigationItemArray();
			
				foreach ($arrayItems as $navItem)
				{
					$strLoc = $navItem->GetURL($this,"content",true,$strLanguage);
					$this->Trace($strLoc);
					$strPriority = RenderNumber(1.0 - (floatval($navItem->m_nLevel) * 0.1),1);
					$arraySitemap[] = array(
						"loc" => $strLoc, 
						"priority" => $strPriority
						);
				}
			}
			
			if (ArrayCount($arraySitemap) == 0)
			{
				$arraySitemap = array(
					array(
						"loc" => $this->GetRootURL(),
						"priority" => "1.0")
					);
			}
				
			return $arraySitemap;
		}


		final
		function GetLayoutCssFilePath($strLayout)
		{
			/*
			1. Check for Layout, fallback to default
			2. Check for static css dir then for assemblies
			*/
						
			$strFilePath = $this->GetCssDir() . $this->MakeLayoutCssFileName($strLayout);
			$this->Trace($strFilePath);
			if (IsFile($strFilePath))
			{
				return $strFilePath;	
			}
			
			$strFilePath = $this->GetAssemblyLine()->GetLayoutCssFilePath($strLayout);
			$this->Trace($strFilePath);
			if ($strFilePath != false)
			{
				return $strFilePath;	 
			}			

			$strFilePath = $this->GetCssDir() . $this->MakeLayoutCssFileName("default");
			$this->Trace($strFilePath);
			if (IsFile($strFilePath))
			{
				return $strFilePath;	
			}
			
			$strFilePath = $this->GetAssemblyLine()->GetLayoutCssFilePath("default");
			$this->Trace($strFilePath);
			if ($strFilePath != false)
			{
				return $strFilePath;	
			}			
			return false;			
		}

		final
		function RenderStaticCss()
		{
			$strFilePath = $this->GetLayoutCssFilePath($this->GetLayout());
			if ($strFilePath == false)
			{
				print("/* ERROR: CSS file not found! */");
				return;
			}

			$strCss = FileRead($strFilePath);
			if ($strCss === false)
			{
				print("/* ERROR: CSS file could not be read! */");
				return;
			}
			print($strCss);
		}


		
		final
		function GetImageFilePath($strImageID,$strExtension)
		{
			/*
			1. Check for static images dir then for assemblies
			2. Check for png, then for jpg, then for gif
			*/

			if ($strExtension == "")
			{
				$strFilePath = $this->GetImageFilePath($strImageID,"png");
				if ($strFilePath != false)
				{
					return $strFilePath;	
				}
				$strFilePath = $this->GetImageFilePath($strImageID,"jpg");
				if ($strFilePath != false)
				{
					return $strFilePath;	
				}
				$strFilePath = $this->GetImageFilePath($strImageID,"gif");
				if ($strFilePath != false)
				{
					return $strFilePath;	
				}
				return false;
			}
				
				
			$strFilePath = $this->GetImagesDir() . $this->MakeImageFileName($strImageID,$strExtension);
			$this->Trace($strFilePath);
			if (IsFile($strFilePath))
			{
				return $strFilePath;	
			}
			
			$strFilePath = $this->GetAssemblyLine()->GetImageFilePath($strImageID.".".$strExtension);
			$this->Trace($strFilePath);
			if (IsFile($strFilePath))
			{
				return $strFilePath;
			}
			return false;
		}
		
		
		final
		function GetLayoutImageFilePath($strImageID, $strLayout, $strExtension)
		{
			/*
			1. Check for Layout, fallback to default
			2. Check for static images dir within assemblies
			*/
			
			if ($strExtension == "")
			{
				$strFilePath = $this->GetLayoutImageFilePath($strImageID, $strLayout, "png");
				if ($strFilePath != false)
				{
					return $strFilePath;	
				}
				$strFilePath = $this->GetLayoutImageFilePath($strImageID, $strLayout, "jpg");
				if ($strFilePath != false)
				{
					return $strFilePath;	
				}
				$strFilePath = $this->GetLayoutImageFilePath($strImageID, $strLayout, "gif");
				if ($strFilePath != false)
				{
					return $strFilePath;	
				}
			}
						
			$strFilePath = $this->GetImagesDir() . $this->MakeLayoutImageFileName($strImageID,$strLayout,$strExtension);
			$this->Trace($strFilePath);
			if (IsFile($strFilePath))
			{
				return $strFilePath;	
			}
			
			$strFilePath = $this->GetAssemblyLine()->GetLayoutImageFilePath($strImageID,$strLayout,$strExtension);
			$this->Trace($strFilePath);
			if (IsFile($strFilePath))
			{
				return $strFilePath;	 
			}			

			if ($strLayout != "default")
			{
				$strFilePath = $this->GetImagesDir() . $this->MakeLayoutImageFileName($strImageID,"default",$strExtension);
				$this->Trace($strFilePath);
				if (IsFile($strFilePath))
				{
					return $strFilePath;	
				}
				
				$strFilePath = $this->GetAssemblyLine()->GetLayoutImageFilePath($strImageID,"default",$strExtension);
				$this->Trace($strFilePath);
				if (IsFile($strFilePath))
				{
					return $strFilePath;	
				}			
			}


			return false;			
		}


		final
		function GetFontFilePath($strFontID)
		{
			$strFilePath = $this->GetFontsDir() . $this->MakeFontFileName($strFontID);
			if (IsFile($strFilePath))
			{
				return $strFilePath;	
			}
			
			$strFilePath = $this->GetAssemblyLine()->GetFontFilePath($strFontID);
			if (IsFile($strFilePath))
			{
				return $strFilePath;
			}
			return false;
		}

		final
		function GetVideoFilePath($strVideoID)
		{
			$strFilePath = $this->GetVideosDir() . $this->MakeFontFileName($strVideoID);
			if (IsFile($strFilePath))
			{
				return $strFilePath;	
			}
			
			$strFilePath = $this->GetAssemblyLine()->GetVideoFilePath($strVideoID);
			if (IsFile($strFilePath))
			{
				return $strFilePath;
			}
			return false;
		}
		
		
		final
		function GetJavaScriptFilePath($strScriptID)
		{
			$strFilePath = $this->GetJavaScriptsDir() . $strScriptID.".js";
			if (IsFile($strFilePath))
			{
				return $strFilePath;	
			}
			$strFilePath = $this->GetAssemblyLine()->GetJavaScriptFilePath($strScriptID);
			if (IsFile($strFilePath))
			{
				return $strFilePath;	 
			}			
			return false;			
		}		
		
		final
		function GetNavigationTree($strNavigationID,$strContextParameter = "content")
		{
			if ($strNavigationID == "")
			{
				$strNavigationID = "default";
			}
			return $this->OnGetNavigationTree($strNavigationID,$strContextParameter);	
		}
		
		function OnGetNavigationTree($strNavigationID,$strContextParameter)
		{
			$navigationtree = new CNavigationTree();
			$bResult = $navigationtree->LoadNavigationTree($this,$strNavigationID);
			if ($bResult == false && $strNavigationID == "sitemap")
			{
				$navigationtree->LoadNavigationTree($this,"default");
			}
			$navigationtree->SetContextParameterName($strContextParameter);			
			if ($strContextParameter == "content")
			{
				$navigationtree->SetCurrentContext($this->GetContent());
			}
			return $navigationtree;
		}
		
		
		function OnRenderNavigation($strOutput)
		{
		}
		
		function OnRenderSpecialContent($strContentID)
		{
			return "";
		}
		
		final
		function DownloadFile(
			$strMimeType,
			$strFileName,
			$strContents)
		{
			$this->Trace("DownloadFile");
			$this->Trace("Mime Type: $strMimeType");
			$this->Trace("File Name: $strFileName");
			
			if ($this->IsDownloadFileSet())
			{
				$this->Trace("Warning: Repeated invocation of DownloadFile() will override previous setting.");	
			}
			
			$this->m_arrayDownloadFile["mimetype"] = $strMimeType;
			$this->m_arrayDownloadFile["filename"] = $strFileName;
			$this->m_arrayDownloadFile["contents"] = $strContents;
			
			$strDownloadToken = $this->GetVar("downloadtoken");
			if ($strDownloadToken != '')
			{
				$this->SetVar('downloadtoken','');
				$this->SetCookie('downloadtoken',$strDownloadToken,120);
			}
		}
		
		final
		function IsDownloadFileSet()
		{
			if (ArrayGetValue($this->m_arrayDownloadFile,"mimetype") == "")
			{
				return false;
			}
			if (ArrayGetValue($this->m_arrayDownloadFile,"filename") == "")
			{
				return false;
			}
			if (ArrayGetValue($this->m_arrayDownloadFile,"contents") == "")
			{
				return false;
			}
			return true;
		}
			
		function OnGetWebsiteTitle()
		{
			foreach ($this->m_arrayPlugins as $plugin)
			{
				$strTitle = $plugin->OnGetWebsiteTitle();
				if ($strTitle != '')
				{
					return $strTitle;	
				}
			}
			
			$strTitle = '';
			if (IsValidContentID($this->GetContent()) == true)
			{
				$strContentUC = StringUppercase($this->GetContent());
				if (FindString($this->GetContent(),'.') == -1) 
				{
					if ($strContentUC != 'INDEX')
					{
						$strTitle = $this->LoadString('TID_TITLE_'.$strContentUC);
						if ($strTitle == '')
						{
							$strTitle = $this->LoadString('TID_NAVIGATION_'.$strContentUC);	
						}
					}
				}
			}
			$strTitleDefault = $this->LoadString('TID_TITLE_DEFAULT');
			if ($strTitleDefault == '')
			{
				$strTitleDefault = $this->LoadString('TID_TITLE');
			}
			if ($strTitleDefault == '')
			{
				$strTitleDefault = $this->GetSiteName();	
			}
			if ($strTitle == '')
			{
				return $strTitleDefault;	
			}
			
			return $strTitle . ' - ' . $strTitleDefault;
		}


		function OnGetWebsiteDescription()
		{
			foreach ($this->m_arrayPlugins as $plugin)
			{
				$strDescription = $plugin->OnGetWebsiteDescription();
				if ($strDescription != '')
				{
					return $strDescription;	
				}
			}		
			
			$strText = $this->LoadString('TID_META_DESCRIPTION_'.StringUppercase($this->GetContent()));
			if ($strText != '')
			{
				return $strText;
			}
			$strText = $this->LoadString('TID_META_DESCRIPTION_DEFAULT');
			if ($strText != '')
			{
				return $strText;
			}
			return '';	
		}

		function OnGetWebsiteKeywords()
		{
			$strText = $this->LoadString('TID_META_KEYWORDS_'.StringUpperCase($this->GetContent()));
			$strTextDefault = $this->LoadString('TID_META_KEYWORDS_DEFAULT');
			if ($strTextDefault != '')
			{
				if ($strText != '')
				{
					$strText = $strText.', '.$strTextDefault;   
				}
				else
				{
					$strText = $strTextDefault;	
				}
			}
			return $strText;
		}
		
		function OnGetOpenGraphImageURL()
		{
			foreach ($this->m_arrayPlugins as $plugin)
			{
				$strImageURL = $plugin->OnGetOpenGraphImageURL();
				if ($strImageURL != '')
				{
					return $strImageURL;	
				}
			}					
			return $this->GetImageURL('opengraph','png');	
		}

		
		function OnSessionCreate()
		{
			$this->Trace('CWebsite::OnSessionCreate');
			if ($this->GetSessionMode() == SESSION_MODE_LOCAL_COOKIE)
			{
				return 'local';					
			}
			$strSessionID = '';
			return $strSessionID;			
		}
		
		function OnSessionRead($strSessionID, &$arrayData)
		{
			$this->Trace("CWebsite::OnSessionRead");
			$arrayData = array();
			if ($this->GetSessionMode() == SESSION_MODE_LOCAL_COOKIE)
			{
				$arrayData = $this->GetAllCookiesInsecure();
				return true;					
			}
			return false;
		}

		function OnSessionWrite($strSessionID, $arrayData)
		{
			$this->Trace("CWebsite::OnSessionWrite");
			if ($this->GetSessionMode() == SESSION_MODE_LOCAL_COOKIE)
			{
				foreach ($arrayData as $strKey => $strValue)
				{
					$this->SetCookie($strKey,$strValue,$this->GetSessionCookieDuration());	
				}
				return true;					
			}
			return false;
		}
		
		function OnSessionDestroy($strSessionID)
		{
			$this->Trace("OnSessionDestroy() is not implemented.");
			return false;
		}


		function OnLoadUserSettings()
		{
			$this->Error("OnLoadUserSettings() is not implemented.");
			return false;
		}

		function OnPreContent()
		{
		}

		function OnPostContent()
		{
		}

		function OnRenderContentException()
		{
			$navigationtree = $this->GetNavigationTree("");
			$navigation = new CNavigationElement($this,"portfolio");
			$strOutput = $navigation->RenderNavigationCurrentPathPortfolio($navigationtree);
			if ($strOutput != "")
			{
				return $strOutput;	
			}
			
    	$this->SetErrorMessageCode("CONTENT_NOT_FOUND");
			return $this->LoadContent("sitemap");
		}

		
		function OnUserLogin($strUserName,$strAuthenticationPayload)
		{
			$this->Error("OnUserLogin() is not implemented.");
			return false;			
		}
		
		function OnUserLogout()
		{
			$this->Error("OnUserLogout() is not implemented.");
			return false;
		}

		function OnUserCheckName($strUsername)
		{
			$this->Error("OnUserCheckName() is not implemented.");
			return false;
		}

		function OnUserNameInputCorrection($strUsername)
		{
			$strUsername = StringLowerCase($strUsername);
			$strUsername = trim($strUsername);
			return $strUsername;
		}

		function OnUserChangePassword($strOldPassword,$strNewPassword)
		{
			foreach ($this->m_arrayPlugins as $plugin)
			{
				$bResult = $plugin->OnUserChangePassword($strOldPassword,$strNewPassword);
				if ($bResult == false)
				{
					return false;
				}
			}
			return true;
		}

		function OnUserSignin($strUsername,$strPassword,$arrayParams=array(),&$strError='',&$arrayResults=array())
		{
			$this->Error('OnUserSignin() is not implemented.');
			return false;
		}
		

		function OnUserDelete($strUserID)
		{
			foreach ($this->m_arrayPlugins as $plugin)
			{
				$bResult = $plugin->OnUserDelete($strUserID);
				if ($bResult == false)
				{
					return false;
				}
			}
			return true;
		}


		function OnParseSEOURL($strSEOURL)
		{
			$strLanguage = $this->GetLanguage();
			$this->Trace('SEOURL: '.$strSEOURL);
			if ($strSEOURL != '')
			{
				$arraySegments = StringExplode($strSEOURL,'/');
				$this->Trace($arraySegments);
				$nSegments = ArrayCount($arraySegments);
				if ($nSegments == 1)
				{
					$strSegment = $arraySegments[0];
					if (ArrayValueExists($this->GetSupportedLanguages(),StringUpperCase($strSegment)))
					{
						$this->SetContent('index');
						$this->SetLanguage(StringUpperCase($strSegment));    
					}
					else
					{
						$this->SetContent(
							$this->GetRoutingContext(
								'content',
								$arraySegments[0],
								$strLanguage));
					}
				}
				else if ($nSegments > 1)
				{
					$strLanguage = $arraySegments[0];
					$this->SetLanguage($strLanguage);  
					$this->SetContent(
						$this->GetRoutingContext(
							'content',
							$arraySegments[1],
							$strLanguage));
				}
			}	
		}

		function OnCheckCondition($strCondition)
		{
			$bResult = GetBoolValue(ArrayGetValue($this->m_arrayConditions,StringUpperCase($strCondition)));
			if ($bResult == true)
			{
				return true;
			}
			return $this->CheckPrivilege(StringUpperCase($strCondition));
		}

		function OnGetRequiredConditionsForContent($strContentID)
		{
			return array();	
		}
		
		function OnUserInitPrivileges()
		{
			$this->Error('OnUserInitPrivileges() is not implemented.');
			return array();	
		}
		
		function OnGetMessage($strMessageType,$strMessageID)
		{
			$this->Error("OnGetMessage() is not implemented.");
			return '';
		}
		
		function OnGetDateLocalFormat($strCountryID)
		{
			return '';	
		}

		function OnGetTimeLocalFormat($strCountryID)
		{
			return '';	
		}

		function OnGetDateTimeLocalFormat($strCountryID)
		{
			$strDateFormat = $this->OnGetDateLocalFormat($strCountryID);
			if ($strDateFormat == "")
			{
				$strDateFormat = GetDateLocalFormat($strCountryID);	
			}
			$strTimeFormat = $this->OnGetTimeLocalFormat($strCountryID);
			if ($strTimeFormat == "")
			{
				$strTimeFormat = GetTimeLocalFormat($strCountryID);	 
			}
			return $strDateFormat." ".$strTimeFormat;     
		}
		
		final
		function GetAuthUsernameVarName()
		{
			return "username";	
		}
		final		
		function GetAuthPasswordVarName()
		{
			return "password";
		}
		final		
		function GetAuthUsernameVar()
		{
			return $this->GetVar($this->GetAuthUsernameVarName());
		}
		final
		function SetAuthUsernameVar($strUsername)
		{
			$this->SetVar($this->GetAUthUsernameVarName(),$strUsername);	
		}
		final		
		function GetAuthPasswordVar()
		{
			return $this->GetVar($this->GetAuthPasswordVarName());			
		}
		
		final
		function GetAuthUserID()
		{
			return $this->m_strAuthUserID;
		}
		
		final
		function SetAuthUserID($strUserID)
		{
			$this->m_strAuthUserID = $strUserID;
		}


		final
		function UserInit()
		{
			$this->Trace("UserInit");
			if ($this->m_bAuthenticationSupport == true)
			{
				if ($this->GetAuthUserID() != "")
				{
					$this->UserLoadSettings();
					$this->UserInitPrivileges();	
					$this->Trace("PRIVILEGES:");
					$this->Trace($this->m_arrayPrivileges);
				}
				else
				{
					$this->SetAuthUserID("");
					$this->m_arrayPrivileges = array();	
				}
			}
		}

		
		final
		function UserLogin($strUserName,$strAuthenticationPayload)
		{
			if (IsSessionModeServer($this->GetSessionMode()) == false)
			{
				$this->Error("UserLogin() requires a server based session!");
				return false;			
			}
			
			$bSuccess = $this->OnUserLogin($strUserName,$strAuthenticationPayload);
			if ($bSuccess != true)
			{
				return false;
			}
			$this->UserInit();
			if ($this->IsCSRFProtectionActive())
			{
				$this->DeployNewCSRFToken();
			}
			return true;
		}
		
		final
		function UserLogout()
		{
			$this->Trace('UserLogout');
			$bResult = $this->OnUserLogout();
			$this->m_arrayPrivileges = array();	
			if ($bResult == false)
			{
				return false;
			}
			$this->SessionDestroy();
			return true;
		}
		
		final
		function UserNameInputCorrection($strUsername)
		{
			return $this->OnUserNameInputCorrection($strUsername);	
		}
		
		final
		function UserCheckName($strUsername)
		{
			$bResult = $this->OnUserCheckName($strUsername);
			$this->Trace('UserCheckName("'.$strUsername.'") returned '.RenderBool($bResult));
			return $bResult;
		}
		
		final
		function UserChangePassword($strOldPassword,$strNewPassword)
		{
			$bResult = $this->OnUserChangePassword($strOldPassword,$strNewPassword);
			if ($bResult == false)
			{
				return false;	
			}
			$arrayParam = array();
			$arrayParam["oldpassword"] = $strOldPassword; 
			$arrayParam["newpassword"] = $strNewPassword; 
			$this->TriggerEvent("website-user-changepassword",$arrayParam);
			return $bResult;
			
		}

		final
		function UserInitPrivileges()
		{
			$this->m_arrayPrivileges = $this->OnUserInitPrivileges();
		}

		final
		function UserLoadSettings()
		{
			return $this->OnUserLoadSettings();	
		}

		final
		function UserSignin($strUserName,$strPassword,$arraySigninParams = array(),&$strError = "",&$arrayResults = array())
		{
			if ($this->UserCheckName($strUserName) != true)
			{
				$strError = "USERNAME_INVALID";
				return false;	
			}
			$strNewUserID = $this->OnUserSignin($strUserName,$strPassword,$arraySigninParams,$strError,$arrayResults);
			if ($strNewUserID == false)
			{
				return false;	
			}
			$arrayEventParams = array();
			$arrayEventParams["userid"] = $strNewUserID; 
			$this->TriggerEvent("website-user-signin",$arrayEventParams);
			return $strNewUserID;
		}


		final
		function UserDelete($strUserID)
		{
			$this->Trace('UserDelete');
			if ($strUserID == $this->GetAuthUserID())
			{
				$bResult = $this->UserLogout();	
				if ($bResult == false)
				{
					$this->Trace('CWebsite::UserDelete: UserLogout returned false.');
					return false;	
				}
			}
			$arrayParam = array();
			$arrayParam['userid'] = $strUserID; 
			$this->TriggerEvent('website-user-delete',$arrayParam);
			return $this->OnUserDelete($strUserID);	
		}
		
		final
		function GetAuthUserData($strDataName,$strDefaultContent='')
		{
			$strUserID = $this->GetAuthUserID();
			if ($strUserID == '')
			{
				return false;
			}
			return $this->OnGetData('USER',$strUserID,$strDataName,$strDefaultContent);	
		}

		final
		function SetAuthUserData($strDataName,$strContent)
		{
			$strUserID = $this->GetAuthUserID();
			if ($strUserID == '')
			{
				return false;
			}
			return $this->OnSetData('USER',$strUserID,$strDataName,$strContent);	
		}
		
		final
		function GetData($strContextType, $strContextID, $strDataName,$strDefaultContent='')
		{
			return $this->OnGetData($strContextType, $strContextID, $strDataName, $strDefaultContent);	
		}

		final
		function SetData($strContextType, $strContextID, $strDataName, $strContent)
		{
			return $this->OnSetData($strContextType, $strContextID, $strDataName, $strContent);
		}
		
		function OnGetData($strContextType, $strContextID, $strDataName, $strDefaultContext)
		{
			$this->Error("OnGetData() is not implemented.");
			return false;	
		}

		function OnSetData($strContextType, $strContextID, $strDataName, $strContext)
		{
			$this->Error("OnGetData() is not implemented.");
			return false;	
		}		


		
		final
		function SessionDestroy()
		{
			$bResult = true;
			if ($this->GetSessionMode() != SESSION_MODE_NONE)
			{
				if ($this->GetSessionID() != "")
				{
					$bResult = $this->OnSessionDestroy($this->GetSessionID());
				}
			}
			$this->m_strSessionID = "";			
			return $bResult;
		}

		final
		function CheckCondition($strCondition)
		{
			$bInvert = false;
			if (StringSection($strCondition,0,1) == "!")
			{
				$bInvert = true;
				$strCondition	= StringSection($strCondition,1);
			}
			$bResult = $this->OnCheckCondition($strCondition);
			if ($bInvert == true)
			{
				return !$bResult;	
			}
			return $bResult;
		}
		
		final
		function SetCondition($strCondition, $bValue)
		{
			$this->m_arrayConditions[StringUpperCase($strCondition)] = GetBoolValue($bValue);	
		}

		final
		function CheckPrivilege($strPrivilegeName)
		{
			return ArrayValueExists($this->m_arrayPrivileges,StringUpperCase($strPrivilegeName));
		}
		
		function GetSiteName()
		{
			return GetMailDomain();
		}
		
		function GetSiteTeamEmail()
		{
			return "no-reply@".GetMailDomain();
		}

		function GetSiteTeamName()
		{
			return "The ".GetMailDomain(). " Team";
		}
		
		final
		function GetWebsiteTitle()
		{
			return $this->OnGetWebsiteTitle();
		}
		
		final
		function GetWebsiteDescription()
		{
			return $this->OnGetWebsiteDescription();
		}		

		final
		function GetWebsiteKeywords()
		{
			return $this->OnGetWebsiteKeywords();
		}		

		
		
		final 
		function MakeCSV(
			$arrayList)
		{
			$strFormat = $this->GetConfig("csvformat");
			return MakeCSV_Format(
				$strFormat,
				$arrayList);
		}		

		final 
		function ParseCSV(
			$strCSV,
			$bHeaderRow = true,
			$IncludeHeadersInResult = false)
		{
			$strFormat = $this->GetConfig("csvformat");
			return ParseCSV_Format(
				$strFormat,
				$strCSV,
				$bHeaderRow,
				$IncludeHeadersInResult);
		}
		
		final
		function MaintainScrollPosition()
		{
			$this->RestoreScrollPosition();
			$this->RememberScrollPosition();			
		}

		final
		function RememberScrollPosition()
		{
			$this->ActivateJQuery();
			$this->AddJavaScript('wdk');
			$this->AddJavaScript('wdk-scrollposition-remember');
		}

		final
		function RestoreScrollPosition()
		{
			$this->ActivateJQuery();
			$this->AddJavaScript('wdk');
			$this->AddJavaScript('wdk-scrollposition-restore');
		}
		
		function ActivateGoogleAnalytics($strTrackingID)
		{
			if (StringLength($strTrackingID) < 13)
			{
				return false;	
			}
			if (FindString($strTrackingID,"UA-") != 0)
			{
				return false;
			}
			$this->AddJavaScript('google-analytics');
			$this->SetOutputItem("GOOGLEANALYTICS_TRACKINGID",$strTrackingID);
			return true;
		}

		function ActivateGoogleConversionTracking($strConversionID, $strLabel)
		{
			if (StringLength($strConversionID) != 9)
			{
				return false;	
			}
			if (StringCheckCharSet($strConversionID,CHARSET_NUMERIC) != true)
			{
				return false;
			}
			$this->AddJavaScript('google-conversion-params');
			$this->AddJavaScriptURL('google-conversion','https://www.googleadservices.com/pagead/conversion.js');
			$this->SetOutputItem("GOOGLECONVERSION_CONVERSIONID",$strConversionID);
			$this->SetOutputItem("GOOGLECONVERSION_LABEL",$strLabel);
			$this->SetBodyItem('google-conversion','<noscript><div style="display:inline;"><img height="1" width="1" style="border-style:none;" alt="" src="//www.googleadservices.com/pagead/conversion/{GOOGLECONVERSION_CONVERSIONID}/?label={GOOGLECONVERSION_LABEL}&amp;guid=ON&amp;script=0"/></div></noscript>');
			return true;
		}

		function ActivateFacebookSDK()
		{
			$this->AddJavaScript('facebook-sdk');
			$this->SetBodyItem('facebook-sdk','<div id="fb-root"></div>');
		}		

		function ActivateJQuery()
		{
			//$this->AddJavaScriptURL('jquery','https://code.jquery.com/jquery-1.12.4.min.js','sha256-ZosEbRLbNQzLpnKIkEdrPv7lOy9C27hHQ+Xp8a4MxAQ=');
			//$this->AddJavaScriptURL('jquery','https://code.jquery.com/jquery-3.5.1.slim.min.js','sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj');
			$this->AddJavaScriptURL('jquery','https://code.jquery.com/jquery-3.5.1.min.js','sha512-bLT0Qm9VnAYZDflyKcBaQ2gg0hSYNQrJ8RilYldYQ1FxQYoCLtUjuuRuZo+fjqhx/qtq/1itJ0C2ejDxltZVFg==');
		
		
		
		}		

		function ActivateBootstrap3()
		{
			$this->ActivateJQuery();
			$this->AddStyleSheetURL('bootstrap','https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css','sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u');
			$this->AddJavaScriptURL('bootstrap','https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js','sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa');
		}

		function ActivateBootstrap4()
		{
			$this->AddStyleSheetURL('bootstrap','https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css','sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2');
			$this->AddJavaScriptURL('bootstrap','https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/js/bootstrap.bundle.min.js','sha384-ho+j7jyWK8fNQe+A12Hb8AhRq26LrZ/JpcUGGOn+Y7RsweNrtN/tE3MoK7ZeZDyx');
		}

		
		function ActivateFontAwesome4()
		{ 
			$this->AddStyleSheetURL('fontawesome4','https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css');	
		}

		function ActivateFontAwesome5()
		{ 
			$this->AddJavaScriptURL('fontawesome5','https://use.fontawesome.com/releases/v5.0.8/js/all.js','sha384-SlE991lGASHoBfWbelyBPLsUlwY1GwNDJo3jSJO04KZ33K2bwfV9YBauFfnzvynJ');	
		}


		function ActivateReact()
		{
			$this->ActivateJQuery();
			
			$this->AddJavaScriptURL('react','https://unpkg.com/react@16/umd/react.development.js'); 
			$this->AddJavaScriptURL('react-dom','https://unpkg.com/react-dom@16/umd/react-dom.development.js'); 

			$this->AddJavaScript('wdk');
			$this->AddJavaScript('wdk-react');

			return true;
		}		

		function ActivateD3v5()
		{
			$this->AddJavaScriptURL('d3v5','https://d3js.org/d3.v5.min.js'); 
			return true;
		}		
		

		
		
		final
		function SetDynamicNavigationTree($strNavigationID, CNavigationTree $navTree)
		{
			if ($strNavigationID == "")
			{
				return false;	
			} 
			$this->m_arrayDynamicNavigationTree[$strNavigationID] = $navTree;
		}

		final
		function GetDynamicNavigationTree($strNavigationID)
		{
			return ArrayGetValue($this->m_arrayDynamicNavigationTree,$strNavigationID);
		}

		function CheckUnresolvedTID($strOutput)
		{
			if ($this->m_bCheckUnresolvedTID == true)
			{
				$this->Trace("Checking for unresolved TID tags in output...");
				$nTIDpos = FindString($strOutput,"?TID_");
				if ($nTIDpos != -1)
				{
					$this->Trace("\"?TID_\" found in output:");
					$strOutputSection = StringSection($strOutput,$nTIDpos,40);
					$this->Trace($strOutputSection);
					$this->SystemEvent(
						"WARNING",
						"UNRESOLVED_TID",
						"Unresolved TID tags found in output!",
						array(
							"output" => $strOutputSection
							)
						);
				}
			}
		}
		
		function CheckInvalidTID()
		{
			// Check for invalid TIDs if required
			if ($this->m_bCheckInvalidTID == true)
			{
				$this->Trace('Checking for invalid TIDs...');
				
				$arrayErrors = GetInvalidTIDs($this->m_res->GetStrings($this->GetLanguage()));
				if (ArrayCount($arrayErrors) > 0)
				{
					$this->Trace('Invalid TID detected');	
					$this->Trace($arrayErrors);
					$this->SystemEvent(
						'WARNING',
						'INVALID_TID',
						'Invalid TIDs found',
						array(
							'output' => $arrayErrors
							)
						);
					
				}
			}
		}
		
		function OnBasicAuthGetText()
		{
			return $this->GetSiteName();
		}		
		
		function OnBasicAuthRenderFailed() 
		{
			$this->SetErrorMessageCode("HTTP401");
			return "";	
		}

		function OnBasicAuthRenderSuccess() 
		{
			return "";	
		}

	}
	
