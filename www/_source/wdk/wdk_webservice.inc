<?php

	
	require_once('wdk_webapplication.inc');
 
 	require_once('wdk_arrayprint.inc'); 

	require_once('wdk_json.inc'); 
	require_once('wdk_xml.inc');
	require_once('wdk_pathcontent.inc');
	require_once('wdk_csv.inc');
	require_once('wdk_mail.inc');
	require_once('wdk_arraytextfile.inc');
	require_once('wdk_parameter.inc');
	require_once('wdk_command.inc');
	require_once('wdk_webserviceconsumer_webapplication.inc');

	abstract class CWebService extends CWebApplication
	{
		private $m_strName;
		
		private $m_strSourceDir;
		private $m_strServiceDir;
		
		private $m_strCurrentURL;
		private $m_strProtocol;
		private $m_strEncoding;
		private $m_strDocType;
		private $m_strFormat;
		private $m_arraySupportedFormatsDefault;

		private $m_arrayParams;
				
		private $m_arrayResult;
		private $m_arrayResultEnvelope;
		private $m_strErrorID;
		
		private $m_paramDef;
		private $m_commandDef;
		
		private $m_bCallbackInitParameterDefinition;
		private $m_bCallbackInitCommandDefinition;
		private $m_bCallbackInit;
		
	
		function __construct(
			$strName,
			$strSourceDir,
			$strServiceDir,
			$arrayMeta,
			$arrayConfig,
			$arrayParams,
			$arrayObfuscationItems
			)
		{
			// Performace measurement
			$stopwatch = new CStopWatch();
			$stopwatch->Start();
			
			parent::__construct(
				$arrayMeta,
				$arrayConfig,
				$arrayObfuscationItems);
			
			// Init trace
			$this->SetTracePrefix("WEBSERVICE: ");

			// Manage Name
			$this->m_strName = $strName;
			$this->Trace('Name         : '.$this->GetName());
			$this->Trace('Version      : '.$this->GetMeta('version'));
			
			// Server
			$this->Trace('Server name  : '.$_SERVER["SERVER_NAME"]);	
			$this->Trace('Server IP    : '.$this->GetServerIP());	
			
			// Client
			$this->Trace('Client IP    : '.$this->GetClientIP());	

			// Instance
			$this->Trace('InstanceToken: '.$this->GetInstanceToken());	


			// Init Error
			$this->m_strErrorID = "";

			// Trace time			
			$this->Trace('Current time : '.RenderDateTimeNow(). ' UTC');
	
			// Init output format
			$this->m_arraySupportedFormatsDefault = array("json","xml","csv","csvpath","printr","html");
			$arraySupportedFormats = $this->CallbackGetSupportedFormats();
			if (!is_array($arraySupportedFormats))
			{
				$arraySupportedFormats = $this->m_arraySupportedFormatsDefault;
			}
			else if (ArrayCount($arraySupportedFormats) == 0)
			{
				$arraySupportedFormats = $this->m_arraySupportedFormatsDefault;
			}
			
			$strFormat = GetWebservicesDefaultFormat();
			if (ArrayValueExists($arraySupportedFormats,$strFormat))
			{
				$this->m_strFormat = $strFormat;
			}
			else
			{
				$this->m_strFormat = $arraySupportedFormats[0];
			}
	
			// Check Environment
			if (CheckEnvironment() == false)
			{
				$this->Trace("CheckEnvironment() returned false. Check env.inc if all environment functions have been defined.");
				$this->SetError("ENVIRONMENT_INVALID");
			}
	
			// Manage Protocols
    	$this->m_strProtocol = "http";
			if (ArrayKeyExists($_SERVER,"HTTPS"))
			{
	     	if ($_SERVER["HTTPS"] == true)
	      {
	      	$this->m_strProtocol = "https";
	      }
			}
			$bPossibleProtocolSecurityContraint = false;
			$arrayProtocols = $this->GetConfig("protocols");
			if ((!is_array($arrayProtocols)) && (ArrayCount($arrayProtocols) < 1))
			{
				$this->SetError("CONFIG_PROTOCOL_NOT_SPECIFIED");
			}
			else
			{
				if (ArrayValueExists($arrayProtocols,$this->m_strProtocol) == false)
				{
					$this->SetError("PROTOCOL_NOT_SUPPORTED");
					if ($this->m_strProtocol == "http")
					{
						$bPossibleProtocolSecurityContraint = true;
					}
				}
			}
			//$this->Trace("Protocol     : ".$this->m_strProtocol);
	
			// Manage current URL
			$this->m_strCurrentURL = $this->m_strProtocol ."://". $_SERVER["SERVER_NAME"] . $_SERVER["REQUEST_URI"];
			$this->Trace("Request URL  : ".$this->m_strCurrentURL);
			
	
						
			// Initialize Parameters
			$this->m_arrayParams = PrepareParameterArray();
			$bAccessCodeParamViaGET = ArrayKeyExists(ArrayGetValue($GLOBALS,"_GET"),"accesscode");
			LockParameterSources();
			
			$this->m_arrayParams = ArrayMerge($this->m_arrayParams,$arrayParams);

			$arrayDefaultParams = array("trace","command","format","accesscode","nocache","csvpath");
			
			// Manage commands
			$this->m_commandDef = new CCommandDefinition();
			if ($this->GetError() == "")
			{
				$this->m_bCallbackInitCommandDefinition = false;
				$this->CallbackInitCommandDefinition($this->m_commandDef);
				if ($this->m_bCallbackInitCommandDefinition == false)
				{
					$this->Trace("CallbackInitCommandDefinition() must call its parent function!");
					$this->SetError("IMPLEMENTATION");
				}
				else
				{
					// Add generic commands:
					$this->m_commandDef->AddCommandDefinition_help();
					$this->m_commandDef->AddCommandDefinition_selfcheck();
					$this->m_commandDef->AddCommandDefinition_install();


					// Check command and related parameters
					$strError = $this->m_commandDef->CheckCommand($this->GetCommand(),$this->m_arrayParams,$arrayDefaultParams);
					if ($strError != "")
					{
						$this->SetError($strError);
					}
				}
			}
			
			
			
			// Manage parameters
			$this->m_paramDef = new CParameterDefinition();
			$this->m_paramDef->AddParameterDefinitionBoolean("trace");
			$this->m_paramDef->AddParameterDefinitionString("command",1,40,CHARSET_COMMAND);
			$this->m_paramDef->AddParameterDefinitionString("accesscode",0,60,CHARSET_ACCESSCODE);
			$this->m_paramDef->AddParameterDefinitionEnum("format",ArrayAppend(array(''),$arraySupportedFormats));
			$this->m_paramDef->AddParameterDefinitionBoolean("nocache");
			$this->m_paramDef->AddParameterDefinitionString("csvpath",0,100,CHARSET_XMLTAG."/");

			$this->m_bCallbackInitParameterDefinition = false;
			$this->CallbackInitParameterDefinition($this->m_paramDef);
			if ($this->m_bCallbackInitParameterDefinition == false)
			{
				$this->Trace("CallbackInitParameterDefinition() must call its parent function!");
				$this->SetError("IMPLEMENTATION");
			}

			if ($this->GetError() == "")
			{
				if ($this->m_paramDef->IsValid() != true)
				{
					$this->Trace("this->m_paramDef->IsValid() != true");
					$this->SetError("PARAMETERDEFINITION_INVALID");
				}
			}
				
			if ($this->GetError() == "")
			{
				$arrayParamErrors = array();
				if ($this->m_paramDef->CheckParameters($this->m_arrayParams, $arrayParamErrors, false) > 0)
				{
					$this->SetTraceActive(true);
					$this->Trace("Trace activated by parameter error.");
					$this->Trace("CheckParameters reported errors!");
					$this->TraceInsecure($arrayParamErrors);
					$this->SetError("PARAMETER_CHECK_FAILED");
				}
			}
			
			foreach ($this->m_arrayParams as $strParamName => $strParamValue)
			{
				if ($this->GetError() == "")
				{
					$bResult = $this->CallbackCheckParameter($this->GetCommand(),$strParamName,$strParamValue);
					if (($this->GetError() == "") && ($bResult == false))
					{
						$this->Trace("CallbackCheckParameter(\"".$this->GetCommand()."\",\"$strParamName\",\"$strParamValue\") returned false, but an error has not been set.");
						$this->SetError("PARAMETER_CHECK_FAILED");
					}
				}
			}


			
			// Initialize Tracing
			if ($this->GetParamBoolean("trace"))
			{
				$this->SetTraceActive(true);
				$this->Trace("Trace activated by parameter.");
			}
			
			// Initialize Caching
			if ($this->IsCacheActive() == true)
			{
				if ($this->GetParam("nocache") == true)
				{
					$this->SetCacheActive(false);	
				}	
			}
			$this->Trace("Caching is ".(($this->IsCacheActive())?("ACTIVE"):("INACTIVE")));
			
	

			// Manage directories
			$this->m_strSourceDir = $strSourceDir;
			//$this->Trace("m_strSourceDir = $this->m_strSourceDir");
			
			$this->m_strServiceDir = $strServiceDir;
			//$this->Trace("m_strServiceDir = $this->m_strServiceDir");
            
			if (StringSection($this->m_strServiceDir,-1) != "/")
			{
				$this->m_strServiceDir .= "/";
			}
			//$this->Trace("m_strServiceDir = $this->m_strServiceDir (After adding trailing slash)");

			
		
			// Init Results
			$this->m_arrayResult = array();
			$this->m_arrayResultEnvelope = array();

			// Output Format
			$strFormat = $this->GetParam("format");
			if (ArrayValueExists($arraySupportedFormats,$strFormat))
			{
				$this->m_strFormat = $strFormat;
			}
				
			// Do some debug output
			/*
			$this->Trace("m_arrayMeta:");
			$this->Trace($this->GetMetaArray());
			*/
			
			/*
			$this->Trace("m_arrayConfig:");
			$this->Trace($this->GetConfigArray());
			*/
			
			$this->Trace("Parameters:");
			$this->Trace($this->m_arrayParams);
			
			
			
			$this->Trace(
				"Email Alert Severity Threshold: "
				.ArrayGetValue(
					GetEventSeverities(),
					$this->GetEmailAlertSeverityThreshold())
				);
		
		
			// Determine default access code, if 'accesscodes' has not been explicitly set to false.
			$arrayAccessCodes = $this->GetConfig("accesscodes");
			if (!is_array($arrayAccessCodes))
			{
				if ($arrayAccessCodes !== false)
				{
					$strDefaultAccessCode = $this->GetWebserviceAccessCode($strName);
					if ($strDefaultAccessCode != '')
					{
						$this->m_arrayConfig['accesscodes'] = array($strDefaultAccessCode);
					}
				}
			}
		
						
			// Check Access Codes
			$bAccessCodeMatch = false;	
			if (is_array($arrayAccessCodes))
			{
				if (ArrayCount($arrayAccessCodes) > 0)
				{
					$this->Trace("An Access Code is required to use this web service.");
					$strAccessCodeParam = $this->GetParam("accesscode");
					if ($strAccessCodeParam == "")
					{
						$this->SetError("ACCESSCODE_REQUIRED");
					}
					else
					{
						foreach ($arrayAccessCodes as $strAccessCode)
						{
							if ($strAccessCode != '')
							{
								if ($strAccessCode == $strAccessCodeParam)
								{ 
									$bAccessCodeMatch = true;
									break;
								}
							}
						}
						if ($bAccessCodeMatch == false)
						{
							$this->SystemEvent(
								"ERROR",
								"ACCESSCODE_INVALID",
								"Invalid access code has been used to call this web service");
							$this->SetError("ACCESSCODE_INVALID");
						}
					}
				}
			}
			
			// ACCESS CODE SECURITY CONTRAINT
			
			// Ignore contraints within the local IP stack and access logs
			if ($this->GetServerIP() != $this->GetClientIP()) 
			{
				if ($bAccessCodeMatch)
				{
					$strSecurityWarning = "";
					if ($bPossibleProtocolSecurityContraint)
					{
						$strSecurityWarning = "The web service has been requested via http (which is not supported) and a valid access code has been supplied. This means that the access code has been provided over an insecure channel.\nThe access code should be changed and the request method should be reviewed now.";
					}
					else if ($bAccessCodeParamViaGET && !$bPossibleProtocolSecurityContraint)
					{ 
						$strSecurityWarning = "The access code has been supplied via GET which is insecure. The access code may now be found in the server's access log.\nThe access code should be changed and the request method should be reviewed now.";
					}
					if ($strSecurityWarning != "")
					{
						$this->SystemEvent(
							"WARNING",
							"ACCESSCODE_EXPOSED",
							"Access code of web service ".$this->GetName()." is potentially exposed.",
							array(
								"details" => $strSecurityWarning
								)
							);	
					}
				}
			}
		
			// META
			$this->SetResultEnvelopeValue($this->GetName(),					"META","NAME");
			$this->SetResultEnvelopeValue($this->GetMeta("version"),"META","VERSION");
			$this->SetResultEnvelopeValue($this->GetMeta("author"),	"META","AUTHOR");
			
			// REQUEST
			$this->SetResultEnvelopeValue($this->m_strCurrentURL,		"REQUEST","GET");
			
			// PARAMETER
			foreach ($this->m_arrayParams as $strParamName => $strParamValue)
			{
				if (ArrayValueExists($this->m_arrayObfuscationItems,StringLowerCase($strParamName)))
				{
					if ($strParamValue != "")
					{
						$strParamValue = "*";
					}
				}
				$this->SetResultEnvelopeValue($strParamValue,"REQUEST","PARAMETERS",StringUppercase($strParamName));
			}
				


			// HELP
			if ($this->GetParamBoolean("help") == true)
			{
				$strHelp = $this->GetHelp();
				if ($this->GetError() == "")
				{
					$this->SetResultEnvelopeValue($strHelp,"HELP");
				}
			}
			
			// MANAGE CAPACITY CONSTRAINTS
			
			$fLoadTolerance = floatval($this->GetConfig("load_tolerance",$this->GetCommand()));
			if ($fLoadTolerance > 0)
			{
				require_once("wdk_system.inc");
				$this->Trace("Load tolerance for command \"".$this->GetCommand()."\": ".RenderNumber($fLoadTolerance,1));
				$arrayLoadAverage = array();
				$bResult = GetSystemAverageLoad($arrayLoadAverage);
				if ($bResult)
				{
					$fLoad = floatval(ArrayGetValue($arrayLoadAverage,"1minute"));
					if ($fLoad > $fLoadTolerance)
					{
						$strMessage = "Load is ".RenderNumber($fLoad,1); 
						$this->Trace($strMessage);
						$this->SetError("CAPACITY_CONSTRAINT");
						$this->SystemEvent("WARNING","CAPACITY_CONSTRAINT",$strMessage);
					}
				}
			}
			
			
			
			
			// RESULT
			if ($this->GetError() == "")
			{
				$this->m_bCallbackInit = false;
				$bInit = $this->CallbackInit();
				if ($this->m_bCallbackInit == false)
				{
					$this->Trace("CallbackInit() must call its parent function!");
					$this->SetError("IMPLEMENTATION");
				}
				else
				{
					if ($bInit != true)
					{
						if ($this->m_strErrorID == "")
						{
							$this->m_strErrorID = "INIT_FAILED";
						}	
					}
					else
					{
						$this->CommandDispatcher();
					}
				}
			}
			
			
						
			// ERROR OR RESULT
			if ($this->m_strErrorID == "")
			{
				$this->SetResultEnvelopeValue($this->m_arrayResult,"RESULT");
			}
			else
			{
				$this->SetResultEnvelopeValue($this->m_strErrorID,"ERROR");
			}
			
			
			
			// PROFILING
			$fSecondsElapsed = $this->ProfilingGetSeconds();
						
			$this->SetResultEnvelopeValue(floatval(RenderNumber($fSecondsElapsed,3)),
				"PROFILING","RUNTIME_SECONDS");
			$this->SetResultEnvelopeValue(memory_get_usage(),
				"PROFILING","MEMORYUSAGE_BYTES");
			$this->SetResultEnvelopeValue(RenderDateTime($this->ProfilingGetStartDateTime()),
				"PROFILING","REQUEST_DATETIME");
			$this->SetResultEnvelopeValue(RenderDateTime(GetTimeNow()),								
				"PROFILING","RESPONSE_DATETIME");


			// CHECK FOR UNINTENDED OUTPUT
			$this->CheckUnintendedOutput();

			// VERSION DISPLAY
			$this->TraceVersionWDK(); 

			// TRACE
			if ($this->IsTraceActive())
			{
				$this->SetResultEnvelopeValue($this->GetTraceLog(),'TRACE');
			}

			$this->RenderWebService();
		}
		
		function CommandDispatcher()
		{
			$strCommand = $this->GetCommand();
			
			switch ($strCommand)
			{
			case 'help':
				$this->CommandHelp();
				return;

			case 'selfcheck':
				$this->CommandSelfCheck();
				return;
				
			case 'install':
				$this->CommandInstall();
				return;	
				
			case '':
				$this->SetError('COMMAND_MISSING');
				return;
				
			default:
				$this->CallbackCommand($strCommand);
				return;
			}
		}
		
		function CallbackCommand($strCommand)
		{
			$this->SetError("COMMAND_NOT_IMPLEMENTED");
			return;
		}
		
		function CallbackError($strError)
		{
			return;	
		}
		
		function CallbackCheckParameter($strCommand,$strParamName,$strParamValue)
		{
			return true;	
		}
		
		function CallbackGetSupportedFormats()
		{
			return $this->m_arraySupportedFormatsDefault;
		}
				
		final
		function GetName()
		{
			return $this->m_strName;	
		}
		
		final
		function SetError($strErrorID, $bOverride = false)
		{
			$strPreviousErrorID = $this->GetError();
			if ($strPreviousErrorID == '')
			{
				$this->m_strErrorID = $strErrorID;
				$this->Trace("Error: $strErrorID");
				$this->CallbackError($strErrorID);
			}
			else if ($bOverride == true)
			{
				$this->m_strErrorID = $strErrorID;
				$this->Trace("Error: $strErrorID (override)");
				$this->CallbackError($strErrorID);
			}
			else
			{
				$this->Trace("Additional Error: $strErrorID");
				$this->CallbackError($strErrorID);
			}
		}
		
		final
		function GetError()
		{
			return $this->m_strErrorID;
		}
		
		final
		function GetAllParams()
		{
			return $this->m_arrayParams;
		}
		
		final
		function SetAllParams($arrayParams)
		{
			$this->Trace("SetAllParams()");
			$this->Trace($arrayParams);
			$this->m_arrayParams = $arrayParams;	
		}
		
		final
		function SetParam($strParam,$vValue)
		{
			ArraySetValue($this->m_arrayParams,$vValue,$strParam);
		}
				
		final
		function GetParam($strParam)
		{
			return ArrayGetValue($this->m_arrayParams,$strParam);	
		}
		
		final
		function GetParamBoolean($strParam)
		{
			$strParam = $this->GetParam($strParam);
			switch ($strParam)
			{
			case "":
				return false;
			case "1":
				return true;
			case "true":
				return true;
			default:
				return false;
			}
		}
		
		final
		function GetParamInteger($strParam)
		{
			return intval(ArrayGetValue($this->m_arrayParams,$strParam));	
		}
		
		final
		function IsParam($strParam)
		{
			return ArrayKeyExists($this->m_arrayParams,$strParam);
		}
		
		final
		function GetSourceDir()
		{
			return $this->m_strSourceDir;
		}
		
		final
		function GetServiceDir()
		{
			return $this->m_strServiceDir;
		}
		
		final
		function GetCurrentURL()
		{
			return $this->m_strCurrentURL;	
		}

		
		function GetCommand()
		{
			return StringLowerCase($this->GetParam("command"));	
		}
		
		function GetFormat()
		{
			return $this->m_strFormat;	
		}

		function SetResultArray($arrayResult)
		{
			$this->m_arrayResult = $arrayResult;
		}
		function GetResultArray()
		{
			return $this->m_arrayResult;	
		}
		function GetResultValue($key1,$key2="",$key3="",$key4="",$key5="",$key6="",$key7="")
		{
			return ArrayGetValue(
				$this->m_arrayResult,
				$key1,$key2,$key3,$key4,$key5,$key6,$key7);	
		}
		function SetResultValue($value,$key1,$key2="",$key3="",$key4="",$key5="",$key6="",$key7="")
		{
			ArraySetValue(
				$this->m_arrayResult,
				ArrayKeysUpperCaseRecursive($value),
				StringUpperCase($key1),
				StringUpperCase($key2),
				StringUpperCase($key3),
				StringUpperCase($key4),
				StringUpperCase($key5),
				StringUpperCase($key6),
				StringUpperCase($key7));
		}
		
		function SetResultEnvelopeValue($value,$key1,$key2="",$key3="",$key4="",$key5="",$key6="",$key7="")
		{
			ArraySetValue(
				$this->m_arrayResultEnvelope,
				ArrayKeysUpperCaseRecursive($value),
				StringUpperCase($key1),
				StringUpperCase($key2),
				StringUpperCase($key3),
				StringUpperCase($key4),
				StringUpperCase($key5),
				StringUpperCase($key6),
				StringUpperCase($key7));
		}
		
		function MakeCommandAndParameterHelp()
		{
			$strHelp = "COMMAND REFERENCE\n";
			$strHelp .= "\n";
			$arrayCommandDef = $this->m_commandDef->GetCommandDefs();
			foreach ($arrayCommandDef as $strCommand => $commandDef)
			{
				$strHelp .=	"\t$strCommand\n";
				$arrayCommandParams = ArrayGetValue($commandDef,"parameter");
				if (!is_array($arrayCommandParams))
				{
					$arrayCommandParams = array();
				}
			
				foreach ($arrayCommandParams as $strParam)
				{
					$strHelp .=	"\t\t$strParam\n";
				}
				$strHelp .= "\n";
			}
			$strHelp .= "\n";
			return $strHelp;
		}
		
		function GetHelp()
		{
			$strHelpFile = $this->GetSourceDir()."help.txt";
			if (!IsFile($strHelpFile))
			{
				$this->SetError("HELP_FILE_NOT_FOUND");
				return "";
			}
			$strHelp = FileRead($strHelpFile);
			if ($strHelp == false)
			{
				$this->SetError("HELP_FILE_READ_FAILED");
				return "";
			}
			
			
			$strHelp = ReplaceString(
				$strHelp, 
				"{COMMAND_REFERENCE}",
				$this->MakeCommandAndParameterHelp());
		
			return $strHelp;
		}


		
		function CallbackInit()
		{
			$this->m_bCallbackInit = true;
			return true;			
		}
		
		function CallbackInitParameterDefinition(&$paramDef)
		{
			$this->m_bCallbackInitParameterDefinition = true;
		}
		function CallbackInitCommandDefinition(&$commandDef)
		{
			$this->m_bCallbackInitCommandDefinition = true;
		}

		function CallbackSystemLog($strEventSeverity, $strEventID, $strEventMessage,$arrayDetails)
		{
			$this->Trace("SYSTEM LOG: ".RenderValue($strEventSeverity).": ".RenderValue($strEventID).": ".RenderValue($strEventMessage));
			if (ArrayCount($arrayDetails) > 0)
			{
				$this->Trace($arrayDetails);
			}
			
			$arrayParams = array();
			$arrayParams["command"] = "log";
			//$arrayParams["logmail"] = "1";
			$arrayParams["reporter_id"] = StringUpperCase($this->GetName());
			$arrayParams["event_severity"] = $strEventSeverity;
			$arrayParams["event_id"] = $strEventID;
			$arrayParams["event_message"] = $strEventMessage;
			$arrayParams["event_details"] = MakeURLParameters($arrayDetails);
			//$arrayParams["datetime"] = "";
			$strURL = $this->GetWebserviceURL("system/log");
			if ($strURL == "")
			{
				$strErrorText = "Config system/log web service url not specified.";
				$this->Trace($strErrorText);
				$this->SendMailToAdmin($strErrorText,$this->GetTraceLog());
			}
			else
			{
				$consumer = new CWebServiceConsumerWebApplication($this);
				
				$nTotalAttempts = 2; 
				$nAttempt = 1;
				$strError = "1";
				while (
							($strError != "")
					&&  ($strError != "PARAMETER_CHECK_FAILED")
					&&	($nAttempt <= $nTotalAttempts)
					)
				{
					$consumer->ConsumeWebService("system/log",$arrayParams);
					$strError = $consumer->GetError();
					$nAttempt++;
				}
			
				if ($strError != "")
				{
					$strErrorText = "ERROR: ".$consumer->GetError()." (after $nTotalAttempts)";
					$this->Trace($strErrorText);
					$this->SendMailToAdmin($strErrorText,$this->GetTraceLog());
				}
			}
		}
		

		

		


		function Install_AddScheduledJob(
			$strJobName,
			$arrayJobParams,
			$strMinute,
			$strHour,
			$strDayOfMonth,
			$strMonth,
			$strDayOfWeek,
			$nTimeoutSeconds = 0)
		{
			$this->Trace("Install_AddScheduledJob BEGIN");
			
			$strSchedulerURL = $this->GetWebserviceURL("system/scheduler");
			if ($strSchedulerURL == "")
			{
				$this->Trace("Web service URL for system/scheduler not specified.");
				$this->SetError("CONFIG");
				return;
			}
			
			$strJobPostParams = MakeURLParameters($arrayJobParams,"\n");

			// Set up scheduler job
			$arrayParams = array();
			$arrayParams["job_name"] = $strJobName;
			$arrayParams["job_url"] = $this->GetName();
			$arrayParams["job_postparams"] = $strJobPostParams;
			$arrayParams["job_active"] = "1";
			$arrayParams["schedule_minute"] = $strMinute;
			$arrayParams["schedule_hour"] = $strHour;
			$arrayParams["schedule_dayofmonth"] = $strDayOfMonth;
			$arrayParams["schedule_month"] = $strMonth;
			$arrayParams["schedule_dayofweek"] = $strDayOfWeek;
			$arrayParams["timeout_seconds"] = $nTimeoutSeconds;
			
			$arrayParams["command"] = "add";
			$consumer = new CWebServiceConsumerWebApplication($this);
			$consumer->ConsumeWebService("system/scheduler",$arrayParams);
			if ($consumer->GetError() != "" && $consumer->GetError() != "JOB_NAMEOCCUPIED")
			{
				$this->SetError($consumer->GetError());
				return;
			}
	
			$this->Trace("Install_AddScheduledJob END");
		}		


		function CommandInstall()
		{
			return;
		}


		
		function CommandHelp()
		{
			if ($this->GetParamBoolean("help") == false) // if it's true, it has already been added!
			{
				$strHelp = $this->GetHelp();
				if ($this->GetError() == "")
				{
					$this->SetResultEnvelopeValue($strHelp,"HELP");
				}
			}
		}
		
		function CommandSelfCheck()
		{
			$this->Trace("CWebservice::CommandSelfCheck()");
			// The default implementation does not check anything currently.	
		}
		
		function SendErrorMailToAdmin()
		{
			$strError = $this->GetError();
			if ($strError != "")
			{
				$strError = " :".$strError;
			}
			$this->SendMailToAdmin(
				"ERROR".$strError,
				$this->GetTraceLog());
		}
 

		function SendMail($strRecipient,$strSubject,$strMessage)
		{
			SendMail(
				$strRecipient,
				$strSubject,
				$strMessage,
				GetSystemEmail(),
				GetSystemEmailSenderName() ." - ".$this->GetName());
		}

		function RenderWebServiceXML()
		{
			$xmlResultEnvelope = new CXMLDocument();
			$xmlResultEnvelope->SetName("WEBSERVICE");
			$xmlResultEnvelope->SetAttribute("NAME",$this->m_strName);
			$xmlResultEnvelope->SetAttribute("VERSION",$this->GetMeta("version"));
			$xmlResultEnvelope->SetRecursiveArray($this->m_arrayResultEnvelope);

			header('Content-type: application/xml');
			return $xmlResultEnvelope->RenderDocument();
		}

		function RenderWebServiceCSV()
		{
			header('Content-type: text/plain');
			
			$strError = $this->GetError();
			if ($strError != "")
			{
				$value = array(array("ERROR" => $strError));
			}
			else
			{
				$strPath = $this->GetParam("csvpath");
				if ($strPath == "")
				{
					$strPath = $this->CallbackGetDefaultCsvPath();
				}
				if ($strPath == "")
				{
					$value = $this->GetResultArray();
					if (!is_array($value))
					{
						$value = array(array());	
					}
					foreach ($value as &$item)
					{
						if (is_array($item))
						{
							unset($item);	
						}
					}
				}
				else
				{
					$arrayPath = StringExplode($strPath,"/",true);
									
					$value = $this->GetResultValue(
						ArrayGetValue($arrayPath,0),
						ArrayGetValue($arrayPath,1),
						ArrayGetValue($arrayPath,2),
						ArrayGetValue($arrayPath,3),
						ArrayGetValue($arrayPath,4),
						ArrayGetValue($arrayPath,5),
						ArrayGetValue($arrayPath,6));
				}
					
				if (!is_array($value))
				{
					$value = array(array($strPath => "".$value));
				}
				
				if (ArrayKeyExists($value,0) == false) 
				{
					// there is no key named "0" -> this is a named array
					// So we treat it like a list with one entry
					$value = array($value);
				}
			}
			return MakeCSV_RFC4180($value);   
		}
		
		function CallbackGetDefaultCsvPath()
		{
			return "";	
		}


		function RenderWebServiceCSVPATH()
		{
			header('Content-type: text/plain');
			return RenderPathContent($this->m_arrayResultEnvelope);
		}


		function RenderWebServicePRINTR()
		{
			header('Content-type: text/plain');
			ob_start();
			ArrayPrint($this->m_arrayResultEnvelope);
			$strOutput = ob_get_contents();
			ob_end_clean();
			return $strOutput;	
		}


		function RenderWebServiceJSON()
		{
			header('Content-type: application/json');
			$strOutput = JsonEncode($this->m_arrayResultEnvelope);
			if ($strOutput == false)
			{
				$strLastError = json_last_error_msg();
				$strOutput = '{"ERROR": "JSON_ENCODE_FAILED", "JSON_ERROR_MSG": "'.$strLastError.'"}';
			}
			return $strOutput;	
		}

		function RenderArrayHTMLTable($arrayInput,$nIndent = 0)
		{
			if (ArrayCount($arrayInput) < 1)
			{
				return "";
			}	
			$strIndent = StringRepeat("\t",$nIndent);
			$strOutput = $strIndent."<table>\n";
			//$strOutput .= $strIndent."\t<tr><th>Key</th><th>Value</th></tr>\n";
			foreach ($arrayInput as $vKey => $vValue)
			{
				$strOutput .= $strIndent."\t<tr>\n";
				$strOutput .= $strIndent."\t\t<th>".HtmlEncode(RenderValue($vKey))."</th>\n";
				if (ArrayCount($vValue) > 0)
				{
					$strOutput .= $strIndent."\t\t<td>\n"; 					
					$strOutput .= $this->RenderArrayHTMLTable($vValue,$nIndent+3); 
					$strOutput .= $strIndent."\t\t</td>\n";
				}
				else
				{
					$strOutput .= $strIndent."\t\t<td>".HtmlConvertLineBreaks(HtmlEncode(RenderValue($vValue)))."</td>\n";
				}
				$strOutput .= $strIndent."\t</tr>\n";
			}
			$strOutput .= $strIndent."</table>\n";
			return $strOutput;
		}

		function RenderWebServiceHTML()
		{
			//header('Content-type: application/json');

			$strOutput = "<html>\n";
			$strOutput .= "\t<head>\n";
			$strOutput .= "\t\t<title>".$this->GetName()."</title>\n";
			$strOutput .= "\t\t<style>\n";
			$strOutput .= "\t\t\tth,tr{vertical-align: top;text-align: left;}\n";
			$strOutput .= "\t\t\tth{background-color:#aaa;}\n";
			$strOutput .= "\t\t\ttd{background-color:#ccc;}\n";
			$strOutput .= "\t\t</style>\n";
			$strOutput .= "\t</head>\n";
			$strOutput .= "\t<body>\n";
			$strOutput .= $this->RenderArrayHTMLTable($this->m_arrayResultEnvelope,2);
			$strOutput .= "\t</body>\n</html>";
			return $strOutput;	
		}
        
		function RenderWebService()
		{
			switch ($this->GetFormat())
			{
				case "printr":
					$strOutput = $this->RenderWebServicePRINTR();
					break;
				case "csvpath":
					$strOutput = $this->RenderWebServiceCSVPATH();
					$fSecondsElapsed = $this->ProfilingGetSeconds();
					$strOutput.="\nTOTAL_ELAPSED_SECONDS,".RenderNumber($this->ProfilingGetSeconds(),3);
					break;
				case "csv":
					$strOutput = $this->RenderWebServiceCSV();
					break;
				case "html":
					$strOutput = $this->RenderWebServiceHTML();
					break;
				case "xml":
					$strOutput = $this->RenderWebServiceXML();
					$fSecondsElapsed = $this->ProfilingGetSeconds();
					$strOutput.="\n<!--Total Elapsed Seconds: ".RenderNumber($this->ProfilingGetSeconds(),3)."-->";
					break;
				case "json":
				default:				
					$strOutput = $this->RenderWebServiceJSON();
					break;
			}
			print($strOutput);
		}
	}

