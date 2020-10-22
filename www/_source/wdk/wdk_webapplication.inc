<?php

	error_reporting (E_ALL);
	ini_set('display_errors', 1);
	
	ini_set('allow_url_fopen', 0);
	ini_set('max_execution_time',120);
	ini_set('expose_php',false);
	header('X-Powered-By:'); 

	require_once("wdk_env.inc");
	require_once("wdk.inc");
	require_once("wdk_arrayprint.inc");
	require_once("wdk_array.inc");
	require_once("wdk_string.inc");
	require_once("wdk_regexp.inc");
	require_once("wdk_typeconversion.inc");
	require_once("wdk_locale.inc");
	require_once("wdk_fileio.inc");
	require_once("wdk_filesys.inc");
	require_once("wdk_utf8.inc");
	require_once("wdk_html.inc");
	require_once("wdk_datetime.inc");
	require_once("wdk_tracelog.inc");
	require_once("wdk_error.inc");
	require_once("wdk_http.inc");
	require_once("wdk_url.inc");
	require_once("wdk_domain.inc");
	require_once("wdk_config.inc");
	require_once("wdk_eventdispatcher.inc");
	require_once("wdk_websecurity.inc");
	require_once("wdk_parameter.inc");
	require_once("wdk_stopwatch.inc");
	require_once("wdk_eventseverity.inc");
	require_once("wdk_version.inc");
	require_once("wdk_ip.inc");

	abstract class CWebApplication extends CEventDispatcher
	{
		private $m_bTraceActive;
		private $m_tracelog;
		
		private $m_bCacheActive;
		
		private $m_arrayMeta;
		private $m_arrayConfig;
		
		private $m_strInstanceToken;
		private $m_strClientIP;
		private $m_strServerIP;
		private $m_strUserAgent;
		
		private $m_stopwatch;
		private $m_stopwatchWebservices;
		private $m_fSecondsElapsed;
		
		protected $m_arrayObfuscationItems;
		
		private $m_bBasicAuth;
		
		function __construct(
				$arrayMeta,
				$arrayConfig,
				$arrayObfuscationItems)
		{
			InitIsFeatureActive();
			
			parent::__construct();
			
			// Init time zone
			date_default_timezone_set('UTC');
			
			// Init profiling
			$this->m_stopwatch = new CStopWatch();
			$this->m_stopwatch->Start();
			
			// Init web service profiling
			$this->m_fSecondsElapsed = 0;
			$this->m_stopwatchWebservices = new CStopWatch();
		
			// Init obfuscation items
			$this->m_arrayObfuscationItems = array(
				"accesscode",
				"/webservice.*accesscode/",
				"password",
				"passwordhash",
				"authenticationpayload",
				"database_password");
			$this->m_arrayObfuscationItems = ArrayAppend($this->m_arrayObfuscationItems,$arrayObfuscationItems);
			
			// Instantiate trace log
			$this->m_tracelog = new CTraceLog();

			// Initialize tracing
			$this->m_bTraceActive = false;

			// Initialize meta
			$this->m_arrayMeta = $arrayMeta;

			// Initialize config
			$config = new CConfig();
			$config->AddConfigStoreLocation(GetConfigDir());
			$config->LoadConfig(GetEnvConfigID());
			$this->m_arrayConfig =  $config->GetDataArray();
			$this->m_arrayConfig = ArrayMerge($this->m_arrayConfig,$arrayConfig);

			// Initialize caching
			$this->m_bCacheActive = true;
			if ($this->GetConfigBoolean("cache_deactivate") == true)
			{
				$this->Trace("Caching deactivated by configuration!");
				$this->m_bCacheActive = false;
			}

			// Initialize instance token
			$this->m_strInstanceToken = uniqid();
			
			// Initialize client IP
			$this->m_strClientIP = $_SERVER['REMOTE_ADDR'];
			
			$strXForwardedFor = ArrayGetValue($_SERVER,'HTTP_X_FORWARDED_FOR');
			if ($strXForwardedFor != "")
			{
				if (IsValidIPv4($this->m_strClientIP) == true)
				{
					$this->m_strClientIP = $strXForwardedFor;
				}
			}
			
			// Initialize server IP
			$this->m_strServerIP = $_SERVER['SERVER_ADDR'];
			
			// Initialize user agent
			$this->m_strUserAgent = ArrayGetValue($_SERVER,'HTTP_USER_AGENT');
			
			// Init basic auth
			$this->m_bBasicAuth = false;
		}

		function GetSelfURL()
		{
			return $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'];	
		}

		function GetMeta($strMeta)
		{
			return ArrayGetValue($this->m_arrayMeta,$strMeta);	
		}
		function GetMetaArray()
		{
			return $this->m_arrayMeta;
		}
	
		function GetConfigArray()
		{
			return $this->m_arrayConfig;
		}
		function GetConfig($strConfig1,$strConfig2='',$strConfig3='',$strConfig4='')
		{
			return ArrayGetValue($this->m_arrayConfig,$strConfig1,$strConfig2,$strConfig3,$strConfig4);	
		}
		function GetConfigBoolean($strConfig1,$strConfig2="",$strConfig3="",$strConfig4="")
		{
			return GetBoolValue($this->GetConfig($strConfig1,$strConfig2,$strConfig3,$strConfig4));
		}
		
		function GetWebserviceURL($strWebservice)
		{
			return $this->GetConfig("webservices",$strWebservice,"url");
		}

		function GetWebserviceAccessCode($strWebservice)
		{
			$strAccessCode = $this->GetConfig('webservices',$strWebservice,'accesscode');
			if ($strAccessCode == '')
			{
				$strKey = 'webservice_'.ReplaceString($strWebservice,'/','-').'_accesscode';
				$strAccessCode = $this->GetConfig($strKey);
			}
			if ($strAccessCode == '')
			{
				$strAccessCode = $this->GetConfig('webservice_default_accesscode');
			}
			return $strAccessCode;
		}


		function GetInstanceToken()
		{
			return $this->m_strInstanceToken;	
		}
		
		function GetClientIP()
		{
			return $this->m_strClientIP;
		}

		function GetServerIP()
		{
			return $this->m_strServerIP;
		}
		
		function GetUserAgent()
		{
			return $this->m_strUserAgent;
		}
		
		function GetRequestMethod()
		{
			$strRequestMethod = ArrayGetValue($_SERVER,"REQUEST_METHOD");
			if ($strRequestMethod == "POST")
			{
				return "post";	
			}
			return "get";
		}

		function GetTimeZone()
		{
			return date_default_timezone_get();
		}


		function IsTraceActive()
		{
			if ($this->m_bTraceActive == true)
			{
				return true;
			}
			if ($this->m_tracelog->IsDebug() == true)
			{
				return true;	
			}
			return false;
		}

		function SetTraceActive($bTraceActive)
		{
			$this->m_bTraceActive = $bTraceActive;
		}


		function Trace($value)
		{
			if (is_array($value))
			{
				$arraySecured = $value; 
				foreach ($this->m_arrayObfuscationItems as $strItem)
				{
					$arraySecured = ArraySetValueRecursive($arraySecured,"*",$strItem);
					$arraySecured = ArraySetValueRecursive($arraySecured,"*",StringUpperCase($strItem));
				}
				$this->m_tracelog->Trace($arraySecured);
			}
			else
			{
				$this->m_tracelog->Trace($value);
			}
		}
		function TraceInsecure($value)
		{
			$this->m_tracelog->Trace($value); 
		}	
		
		function Debug($value = null)
		{
			$this->m_tracelog->Debug($value);	
		}
		
		function GetTraceLog()
		{
			return $this->m_tracelog->GetTraceLog();
		}
		
		function IsCacheActive()
		{
			return $this->m_bCacheActive;	
		}
		function SetCacheActive($bCacheActive)
		{
			$this->m_bCacheActive = $bCacheActive;
		}
		
		function ProfilingGetSeconds()
		{
			return $this->m_stopwatch->GetSeconds();
		}
		function ProfilingGetStartDateTime()
		{
			return $this->m_stopwatch->GetStart();
		}
		function ProfilingWebservicesStart()
		{
			$this->m_stopwatchWebservices->Start();
		}
		function ProfilingWebservicesStop()
		{
			$this->m_stopwatchWebservices->Stop();
		}
		function ProfilingWebservicesGetTotalSeconds()
		{
			return $this->m_stopwatchWebservices->GetTotalSeconds();
		}
		function ProfilingTrace()
		{
			$fSecondsTotal = $this->ProfilingGetSeconds();
			$fSecondsWebservices = $this->ProfilingWebservicesGetTotalSeconds(); 
			$fSecondsLocal = $fSecondsTotal-$fSecondsWebservices; 
			$this->Trace('PROFILING: TOTAL RUNTIME (s)__________: '.RenderNumber($fSecondsTotal,3).' ('.RenderNumber($fSecondsTotal-$this->m_fSecondsElapsed,3).')');
			$this->Trace('PROFILING: Web service consumption (s): '.RenderNumber($fSecondsWebservices,3));
			$this->Trace('PROFILING: Local runtime (s)__________: '.RenderNumber($fSecondsLocal,3));	
			$this->Trace('PROFILING: MEMORY CONSUMPTION_________: '.memory_get_usage());
			$this->m_fSecondsElapsed = $fSecondsTotal;
		}
		function SetTracePrefix($strPrefix)
		{
			$this->m_tracelog->SetPrefix($strPrefix);
		}
		function RenderTraceLog()
		{
			return $this->m_tracelog->RenderTraceLog();	
		}
		
		final		
		function GetAdminEmail()
		{
			return $this->GetConfig('admin_email');
		}
		
		function SendErrorMailToAdmin()
		{
			return $this->SendMailToAdmin(
				'ERROR',
				$this->GetTraceLog());
		}

		function SendMailToAdmin($strSubject,$strMessage = "")
		{
			$strAdminEmail = $this->GetAdminEmail();
			if (IsValidEmail($strAdminEmail) == false)
			{
				$this->Trace("Mail is NOT sent to admin: $strSubject");
				return false;	
			}
			$bResult = $this->SendMail(
				$strAdminEmail,
				$strSubject,
				$strMessage);
			if ($bResult == true)
			{
				$this->Trace("Mail sent to admin ($strAdminEmail): $strSubject");
			}
			else
			{
				$this->Trace("Failed to send mail to admin ($strAdminEmail): $strSubject");
			}
			return $bResult;
		}		

		function SendMail($strRecipient,$strSubject,$strMessage)
		{
			return SendMail(
				$strRecipient,
				$strSubject,
				$strMessage,
				GetSystemEmail(),
				GetSystemEmailSenderName());
		}		
		
		
		function SystemLog($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails=array())
		{
			if ($this->GetConfigBoolean("disable_log") == true)
			{
				$this->Trace("SYSTEM LOG: $strEventSeverity: $strEventID: Logging is deactivated by config.");
				return;
			}
			return $this->CallbackSystemLog($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails);
		}
		
		function SystemEvent($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails=array())
		{
			if ($this->GetConfigBoolean("disable_events") == true)
			{
				$this->Trace("SYSTEM EVENT: $strEventSeverity: $strEventID: Event Management is deactivated by config.");
				return;
			}
			$arrayDetails["instancetoken"] = $this->GetInstanceToken();			
			return $this->CallbackSystemEvent($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails);
		}

		function CallbackSystemLog($strEventSeverity,$strEventID,$strEventMessage,$arrayDetails)
		{
			$this->Trace("SYSTEM LOG: ".RenderValue($strEventSeverity).": ".RenderValue($strEventID).": ".RenderValue($strEventMessage));
			$this->Trace("No log handler implemented.");
		}
	
		function CallbackSystemEvent($strEventSeverity,$strEventID,$strEventMessage, $arrayDetails)
		{
			$this->Trace("SYSTEM EVENT: ".RenderValue($strEventSeverity).": ".RenderValue($strEventID).": ".RenderValue($strEventMessage));
			$this->Trace($arrayDetails);
			
			$this->SystemLog($strEventSeverity, $strEventID, $strEventMessage, $arrayDetails);
			
			if (GetEventSeverityNumeric($strEventSeverity) <= $this->GetEmailAlertSeverityThreshold())
			{
				$strSubject = $strEventSeverity.' - '.$strEventID;
				if ($strEventMessage != "")
				{
					$strSubject .= ' - '.$strEventMessage;
				}
				$this->SendMailToAdmin(
					$strSubject,
					ArrayRender($arrayDetails)
					."\n\n".
					$this->GetTraceLog());
			}
		}
		
		function GetEmailAlertSeverityThreshold()
		{
			$strThreshold = $this->GetConfig('email_alert_severity_threshold');
			if ($strThreshold == '')
			{
				return SEVERITY_ERROR;
			}
			$nThreshold = intval($strThreshold);
			if (ArrayKeyExists(GetEventSeverities(),$nThreshold) == true)
			{
				return $nThreshold;	
			}
			return SEVERITY_ERROR;
		}
		
		function TriggerEvent($strEventID, $arrayParams)
		{
			$this->Trace('TriggerEvent("'.$strEventID.'", ...) BEGIN');
			parent::TriggerEvent($strEventID, $arrayParams);
			$this->Trace('TriggerEvent("'.$strEventID.'", ...) END');
		}
		
		function CheckUnintendedOutput()
		{
			if ($this->GetConfigBoolean('check_unintendedoutput') == false)
			{
				return true;	
			}
			$strOB = ob_get_contents();
			if ($strOB != '')
			{
				$this->Trace('There is unintended output in the buffer:');
				$this->Trace($strOB);
				$this->SystemEvent(
					'WARNING',
					'UNINTENDED_OUTPUT',
					'There is unintended output before the web application rendering started!',
					array(
						'output' => $strOB
						)
					);
				return false;
			}
			return true;
		}
		
		
		final
		function BasicAuth()
		{
			$this->Trace('Basic Authentication');
			
			$strUser = $this->GetBasicAuthUser();
			$strPassword = $this->GetBasicAuthPassword();
			
			$this->m_bBasicAuth = $this->CallbackBasicAuth(
				$this->GetBasicAuthUser(),
				$this->GetBasicAuthPassword());

			if ($this->m_bBasicAuth == false)
			{
				$strText = $this->CallbackBasicAuthGetText();
				
  			header('WWW-Authenticate: Basic realm="'.$strText.'"');
  			header('HTTP/1.0 401 Unauthorized');
				
				$strOutput = $this->CallbackBasicAuthRenderFailed();
			}
			else
			{
				$strOutput = $this->CallbackBasicAuthRenderSuccess();
			}
			if ($strOutput != '')
			{
				print($strOutput);
				return false;
			}
			return true;
		}

		final
		function IsBasicAuthActive()
		{
			return $this->GetConfigBoolean("basicauth");
		}
		
		final
		function IsBasicAuth()
		{
			return $this->m_bBasicAuth;	
		}
		
		final
		function GetBasicAuthUser()
		{
			return ArrayGetValue($_SERVER,'PHP_AUTH_USER');	
		}

		final
		function GetBasicAuthPassword()
		{
			return ArrayGetValue($_SERVER,'PHP_AUTH_PW');	
		}
		
		function CallbackBasicAuthGetText()
		{
			return "";	
		}
		
		function CallbackBasicAuth($strUser,$strPassword)
		{
			return false;	
		}
		
		function CallbackBasicAuthRenderFailed() 
		{
			return 'Authentication Failed';	
		}
		
		function CallbackBasicAuthRenderSuccess() 
		{
			return '';
		}		
		
		function TraceVersionWDK() 
		{
			$this->Trace(ArrayGetValue(GetVersionInfoWDK(),'VERBOSE'));
		}

	
	}

