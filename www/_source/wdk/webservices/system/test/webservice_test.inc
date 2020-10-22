<?php 

	require_once(GetWDKDir()."wdk_webservice_database_entity.inc");
	require_once(GetWDKDir()."wdk_filesys.inc");
	require_once(GetWDKDir()."wdk_quality.inc");
	require_once(GetWDKDir()."wdk_list.inc");
	
	require_once(GetWDKDir()."wdk_entity_test.inc");
	
   class CTestWebService extends CDatabaseEntityWebService
	{
		function __construct(
			$strSourceDir,
			$strServiceDir,
			$arrayConfig,
			$arrayParams)
		{
			$arrayMeta = array();
			$arrayMeta["version"] = WDK_VERSION; 
			$arrayMeta["author"] = "Sascha Wildgrube";

			$arrayEntityDefinition = array(
				"site" => GetEntityDefinitionTestSite(),
				"assembly" => GetEntityDefinitionTestAssembly(),
				"group" => GetEntityDefinitionTestGroup(),
				"test" => GetEntityDefinitionTest(),
				"statushistory" => GetEntityDefinitionTestStatusHistory()
				);
												
			parent::__construct(
				"system/test",
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$arrayEntityDefinition
				);
		}
		
		function CallbackInitParameterDefinition(&$paramDef)
		{
			$paramDef->AddParameterDefinition_testpath(); 
			$paramDef->AddParameterDefinitionEmail("alertemail");
			$paramDef->AddParameterDefinitionBoolean("dotest");
			$paramDef->AddParameterDefinitionBoolean("getstatus");
			$paramDef->AddParameterDefinitionBoolean("getlog");
			parent::CallbackInitParameterDefinition($paramDef);
			
		}
		
		function CallbackInitCommandDefinition(&$commandDef)
		{
			$commandDef->AddCommandDefinition("install",array());
			//$commandDef->AddCommandDefinition("test",array("assemblyname","groupname","testname"));
			$commandDef->AddCommandDefinition("test",array("test_path"));
			$commandDef->AddCommandDefinition("testall",array("alertemail"));
			$commandDef->AddCommandDefinition("testlocal",array("alertemail"));
			$commandDef->AddCommandDefinition("testrun",array());
			$commandDef->AddCommandDefinition("alert",array("alertemail"));
			$commandDef->AddCommandDefinition("cleanup",array());
			
			$commandDef->AddCommandDefinition("gettest",array("test_path"));
			
			parent::CallbackInitCommandDefinition($commandDef);
		}
		
		
		function CallbackInit()
		{
			if ($this->IsDatabaseSupportActive() == true)
			{
				if ($this->DatabaseConnect() != true)
				{
					return false;
				}			
			}
			$this->SetResultValue($this->IsDatabaseSupportActive(),"DATABASE_SUPPORT");   
			return parent::CallbackInit();
		}
		
		
		function CallbackCommand($strCommand)
		{
			switch ($strCommand)
			{
			case "install":
				$this->CommandInstall();
				return;
	
			case "cleanup":
				$this->CommandCleanUp();
				return;
			
			case "test":
				$this->CommandTest();
				return;

			case "gettest":
				$this->CommandGetTest();
				return;

			case "testlist":
				$this->CommandTestList();
				return;
				
			case "statushistorylist":
				if ($this->IsDatabaseSupportActive() == true)
				{
					parent::CallbackCommand($strCommand);
				}
				return;

			case "sitelist":
				$this->CommandSiteList();
				return;

			case "assemblylist":
				$this->CommandAssemblyList();
				return;

			case "grouplist":
				$this->CommandGroupList();
				return;

			case "testrun":
				$this->CommandTestRun();
				return;

			case "testall":
				$this->CommandTestAll();
				return;
				
			case "testlocal":
				$this->CommandTestLocal();
				return;
				
			case "alert":
				$this->CommandAlert();
				return;

			default:
				parent::CallbackCommand($strCommand);
				return;
			}
		}
		
		function CallbackLog($strEventSeverity, $strEventID, $strDetails)
		{
			$this->CallbackLog_LogWebservice($strEventSeverity, $strEventID, $strDetails);
		}		

		function CheckParam($strParam)
		{
			switch ($strParam)
			{
			case "test_path":
				$arrayTestPath = ParseTestPath($this->GetParam($strParam));
				if ($arrayTestPath == false)
				{
					return false;
				}
				return true;
			default:
				return false;
			}
		}
		
		function IsDatabaseSupportActive()
		{
			return GetBoolValue($this->GetConfig("database_support"));	
		}
		
		function GetTestRequestTimeout()
		{
			$nTestRequestTimeout = intval($this->GetConfig("test","request_timeout"));
			if ($nTestRequestTimeout < 1)
			{
				$nTestRequestTimeout = 45;
			}
			if ($nTestRequestTimeout > 180)
			{
				$nTestRequestTimeout = 180;
			}
			return $nTestRequestTimeout;
		}
		
		function GetAlertEmail()
		{
			$strAlertEmail = $this->GetParam("alertemail");
			if ($strAlertEmail == "")
			{
				$strAlertEmail = $this->GetConfig("alert_email");
			}
			return $strAlertEmail;
		}
		
		function GetMaxAttempts()
		{
			$nMaxAttempts = intval($this->GetConfig("test","max_attempts"));
			if ($nMaxAttempts < 1)
			{
				$nMaxAttempts = 1;
			}
			if ($nMaxAttempts > 10)
			{
				$nMaxAttempts = 10;
			}
			return $nMaxAttempts;	
		}

		function MakeTestURL($strAssemblyID, $strGroupID, $strTestID)
		{
			return MakeURL(
				$this->GetConfig("tests_url"),
				array(
					"test" => "$strAssemblyID/$strGroupID/$strTestID"),
				false
				);
		}
		
		function MakeSQL_TestPathCondition(
			$strSitePath,
			$strAssemblyID,
			$strGroupID,
			$strTestID)
		{
			return 
				      "`TEST_ID` = ".MakeSQL_Helper_Value($strTestID)
				." AND `SITE_PATH` = ".MakeSQL_Helper_Value($strSitePath)
				." AND `ASSEMBLY_ID` = ".MakeSQL_Helper_Value($strAssemblyID)
				." AND `GROUP_ID` = ".MakeSQL_Helper_Value($strGroupID);
		}
		
		function MakeRemoteSitePath($strSitePath)
		{
			$arraySitePath = explode("/",$strSitePath);
			
			$strSitePath = "";
			for ($nCount = 1; $nCount < ArrayCount($arraySitePath); $nCount++)
			{
				$strSitePath .= $arraySitePath[$nCount];
			}
			if ($strSitePath == "")
			{
				$strSitePath = "local";
			}
			return $strSitePath;
		}
		
		function DoTest(
			$strTestURL,
			$strSitePath,
			$strAssemblyID,
			$strGroupID,
			$strTestID,
			$nMaxAttempts,
			&$strDateTimeExecution,
			&$strStatus,
			&$fRuntimeSeconds,
			&$strTestLog,
			&$strHash,
			&$nAttemptsCounter)
		{
			$this->Trace("TEST BEGIN");
			$strTestPath = MakeTestPath($strSitePath,$strAssemblyID,$strGroupID,$strTestID);
			
			$this->Trace("PATH: $strTestPath");
			$this->Trace("URL: $strTestURL");
			
			
			$nAttemptsCounter = 0;
			
			do
			{
				$nAttemptsCounter++;
				
				$this->Trace("Attempt: $nAttemptsCounter");

				$strTestLog = "";
				$strStatus = "FAILED";
				$fRuntimeSeconds = 0.0;
				$strDateTimeExecution = RenderDateTimeNow();
				$strHash = "";
				
				$bError = false;
	
	
				if ($this->IsDatabaseSupportActive() == true)
				{
					$strSQLCondition = $this->MakeSQL_TestPathCondition(
						$strSitePath,
						$strAssemblyID,
						$strGroupID,
						$strTestID);
					
					
					
					$arrayFields = array();
					$arrayFields["LASTEXECUTION_DATETIME"] = $strDateTimeExecution;
					$arrayFields["LASTEXECUTION_RUNTIME_SECONDS"] = "";
					$arrayFields["LASTEXECUTION_STATUS"] = "TESTING";
					$arrayFields["LASTEXECUTION_ATTEMPTS"] = $nAttemptsCounter;
					$arrayFields["LASTEXECUTION_LOG"] = "";
					$arrayFields["COUNT"] = "++";
								 
					$strSQL = MakeSQL_Update(
						$this->GetDatabaseTableName("test"),
						$arrayFields,
						$strSQLCondition);
								 
							
					$this->DatabaseQuery($strSQL);
					
					if ($this->m_database->GetAffectedRows() == 0)
					{
						// Entry does not yet exist
						$arrayFields["SITE_PATH"] = $strSitePath;
						$arrayFields["ASSEMBLY_ID"] = $strAssemblyID;
						$arrayFields["GROUP_ID"] = $strGroupID;
						$arrayFields["TEST_ID"] = $strTestID;
						$arrayFields["FIRSTEXECUTION_DATETIME"] = $strDateTimeExecution;
									 
						$strSQL = MakeSQL_Insert(
							$this->GetDatabaseTableName("test"),
							$arrayFields);
							
						if ($this->DatabaseQuery($strSQL) != true)
						{
						}
					}
				}
				
				
				$stopwatch = new CStopWatch();
				$stopwatch->Start();
	
				$strTestLog = HttpRequest($strTestURL,array(),"get",$this->GetTestRequestTimeout()); 
	
				$stopwatch->Stop();
				$fRuntimeSeconds = $stopwatch->GetSeconds();
			
				if ($strTestLog == false)
				{
					$bError = true;
					$strTestLog = "";
				}
				
				if ($bError == true)
				{
					$strStatus = "REQUESTERROR";
				}
				else
				{
					$strTestLog = trim($strTestLog);
	
					if (StringSection($strTestLog,-2,2) == "OK")
					{
						$strStatus = "PASSED";
					}
					else if (StringSection($strTestLog,-8,8) == "INACTIVE")
					{
						$strStatus = "INACTIVE";
					}
					else if (StringSection($strTestLog,-6,6) == "FAILED")   
					{
						$strStatus = "FAILED";
					}
					else if (StringSection($strTestLog,-12,12) == "REQUESTERROR")
					{
						$strStatus = "REQUESTERROR";
					}
					else
					{
						$strStatus = "UNDEFINED";
					}
					
					if (StringSection($strTestLog,0,4) == "HASH")
					{
						$strHash = StringSection($strTestLog,5,40);	
						$this->Trace("HASH: ".$strHash);
					}
					
				}
				
				
				$this->SystemLog("INFORMATIONAL","DOTEST",$strTestPath."=".$strStatus);
				
				if ($this->IsDatabaseSupportActive() == true)
				{
					$arrayFields["LASTEXECUTION_RUNTIME_SECONDS"] = RenderNumber($stopwatch->GetSeconds(),3);
					$arrayFields["LASTEXECUTION_STATUS"] = $strStatus;
					$arrayFields["LASTEXECUTION_ATTEMPTS"] = $nAttemptsCounter;
					$arrayFields["LASTEXECUTION_HASH"] = $strHash;
					$arrayFields["LASTEXECUTION_LOG"] = $strTestLog;
					$arrayFields = ArrayRemoveKey($arrayFields,"COUNT");
								 
					$strSQL = MakeSQL_Update(
						$this->GetDatabaseTableName("test"),
						$arrayFields,
						$strSQLCondition);
						
					if ($this->DatabaseQuery($strSQL) != true)
					{
					}
					
					if ($this->GetConfig("status_history","active") == true)
					{
						$arrayFieldsHistory = array();
						$arrayFieldsHistory["SITE_PATH"] 	= $strSitePath;
						$arrayFieldsHistory["ASSEMBLY_ID"] 	= $strAssemblyID;
						$arrayFieldsHistory["GROUP_ID"]		= $strGroupID;
						$arrayFieldsHistory["TEST_ID"]		= $strTestID;
						$arrayFieldsHistory["STATUS"] 			= $arrayFields["LASTEXECUTION_STATUS"];
						$arrayFieldsHistory["DATETIME"] 			= $arrayFields["LASTEXECUTION_DATETIME"];
						$arrayFieldsHistory["RUNTIME_SECONDS"] = $arrayFields["LASTEXECUTION_RUNTIME_SECONDS"];
						$arrayFieldsHistory["HASH"] 			= $arrayFields["LASTEXECUTION_HASH"];						
						if ($this->GetConfig("status_history","log") == true)
						{
							$arrayFieldsHistory["LOG"] 				= $arrayFields["LASTEXECUTION_LOG"];
						}
						
						$strSQL = MakeSQL_Insert(
							$this->GetDatabaseTableName("statushistory"),
							$arrayFieldsHistory,
							true);
									 
						if ($this->DatabaseQuery($strSQL) != true)
						{
						}								
					}
				}
				
			} while (
				($strStatus != "PASSED")
				&& 
				($strStatus != "INACTIVE")
				&&
				($nAttemptsCounter < $nMaxAttempts));

		
			$this->Trace("TEST END");
			return $bError;
		}
		
		function CommandInstall()
		{
			$this->Trace("CommandInstall");
			
		
			if ($this->IsDatabaseSupportActive() == true)
			{
				// Set up database table
				parent::CommandInstall();
				
				if ($this->GetError() != "")
				{
					return;	
				}				
				
				// Set up TestRun job
				$arrayParams = array();
				$arrayParams["command"] = "testrun";
				$this->Install_AddScheduledJob(
					"TestRun",
					$arrayParams,
					"*",
					"*",
					"*",
					"*",
					"*",
					45);
			
				// Set up TestAlert job
				$arrayParams = array();
				$arrayParams["command"] = "alert";
				$this->Install_AddScheduledJob(
					"TestAlert",
					$arrayParams,
					"*/10",
					"*",
					"*",
					"*",
					"*",
					30);
								
				// Set up TestCleanup job
				$arrayParams = array();
				$arrayParams["command"] = "cleanup";
				$this->Install_AddScheduledJob(
					"TestCleanup",
					$arrayParams,
					"45",
					"*",
					"*",
					"*",
					"*",
					30);			

			}
			else
			{
				// Set up TestAll scheduler job
				$arrayParams = array();
				$arrayParams["command"] = "testall";
				//$arrayParams["alertemail"] = $this->GetAlertEmail();
				$this->Install_AddScheduledJob(
					"TestAll",
					$arrayParams,
					"50",
					"*",
					"*",
					"*",
					"*");
			}

			return;	
		}	
		

		function CommandTest()
		{
			if ($this->CheckParam("test_path") != true)
			{
				return;	
			}
			
			$strTestPath = $this->GetParam("test_path");
			$arrayTestPath = ParseTestPath($strTestPath);
			$strSitePath = $arrayTestPath["site_path"];
			$strAssemblyID = $arrayTestPath["assembly_id"];
			$strGroupID = $arrayTestPath["group_id"];
			$strTestID = $arrayTestPath["test_id"];
		
			$strTestURL = "";
			if ($strSitePath == "local" )
			{
				$strTestURL = $this->MakeTestURL(
					$strAssemblyID,
					$strGroupID,
					$strTestID);
			}
			else
			{
				$arraySitePath = explode("/",$strSitePath);
				
				$arrayRemoteSiteConfig = $this->GetConfig("remote_sites",$arraySitePath[0]);
				if (!is_array($arrayRemoteSiteConfig))
				{
					$this->SetError("SITE_UNKNOWN");
					return;
				}

				$strRemoteSitePath = $this->MakeRemoteSitePath($strSitePath);
				
				$strURL = ArrayGetValue($arrayRemoteSiteConfig,"url");
				$strAccessCode = ArrayGetValue($arrayRemoteSiteConfig,"accesscode");
				$strFormat = ArrayGetValue($arrayRemoteSiteConfig,"format");
			
				$consumer = new CWebServiceConsumerWebApplication($this);
				$arrayParams = array();
				$arrayParams["command"] = "gettest";
				$arrayParams["test_path"] = MakeTestPath($strRemoteSitePath,$strAssemblyID,$strGroupID,$strTestID);
				$arrayParams["accesscode"] = $strAccessCode; 
				if ($strFormat != "")
				{
					$arrayParams["format"] = $strFormat; 
				}
				$consumer->ConsumeWebService($strURL,$arrayParams);
				if ($consumer->GetError() != "")	
				{
					$this->Trace("Error: \"".$consumer->GetError()."\"");		
					return;
				}
				
				$strTestURL = $consumer->GetResultValueInsecure("URL");
			
			}

			$strDateTimeExecution = "";
			$strStatus = "FAILED";
			$fRuntimeSeconds = 0.0;
			$strTestLog = "";
			$strHash = "";
			$this->DoTest(
				$strTestURL,
				$strSitePath,
				$strAssemblyID,
				$strGroupID,
				$strTestID,
				$this->GetMaxAttempts(),
				$strDateTimeExecution,
				$strStatus,
				$fRuntimeSeconds,
				$strTestLog,
				$strHash,
				$nAttemptsCounter);

			
			$arrayTest = array();

			$arrayTest["TEST_PATH"] = $strTestPath;
			$arrayTest["SITE_PATH"] = $strSitePath;
			$arrayTest["ASSEMBLY_ID"] = $strAssemblyID;
			$arrayTest["GROUP_ID"] = $strGroupID;
			$arrayTest["TEST_ID"] = $strTestID;
			$arrayTest["URL"] = $strTestURL;
			$arrayTest["LASTEXECUTION_STATUS"] = $strStatus;
			$arrayTest["LASTEXECUTION_DATETIME"] = $strDateTimeExecution;
			$arrayTest["LASTEXECUTION_RUNTIME_SECONDS"] = $fRuntimeSeconds;
			$arrayTest["LASTEXECUTION_ATTEMPTS"] = $nAttemptsCounter;
			$arrayTest["LASTEXECUTION_HASH"] = $strHash;
			$arrayTest["LASTEXECUTION_LOG"] = $strTestLog;
			
					

			
			foreach ($arrayTest as $strTag => $strValue)
			{
				$this->SetResultValue($strValue,$strTag);
			}

		}
		
	
		function CommandGetTest()
		{

			if ($this->CheckParam("test_path") != true)
			{
				return;	
			}
			
			$strTestPath = $this->GetParam("test_path");
			$arrayTestPath = ParseTestPath($strTestPath);
			$strSitePath = $arrayTestPath["site_path"];
			$strAssemblyID = $arrayTestPath["assembly_id"];
			$strGroupID = $arrayTestPath["group_id"];
			$strTestID = $arrayTestPath["test_id"];
			
			$arrayTest["TEST_PATH"] = $strTestPath;
			$arrayTest["SITE_PATH"] = $strSitePath;
			$arrayTest["ASSEMBLY_ID"] = $strAssemblyID;
			$arrayTest["GROUP_ID"] = $strGroupID;
			$arrayTest["TEST_ID"] = $strTestID;
			 

			if ($strSitePath == "local" )
			{
				$strTestFilePath = MakeTestFilePath(
					$strAssemblyID,
					$strGroupID,
					$strTestID);
					
				if (IsFile($strTestFilePath) == false)
				{
					$this->SetError("TEST_NOT_FOUND");
					return;	
				}
				
				$arrayTest["TEST_SCRIPT"] = FileRead($strTestFilePath);

				$strTestURL = $this->MakeTestURL(
					$strAssemblyID,
					$strGroupID,
					$strTestID);
					
				$arrayTest["URL"] = $strTestURL;
			}
			else
			{
				$arrayRemoteSiteConfig = $this->GetRemoteSiteBySitePath($strSitePath);
				if ($arrayRemoteSiteConfig == false)
				{
					$this->SetError("SITE_UNKNOWN");
					return;
				}
				
				$strRemoteSitePath = $this->MakeRemoteSitePath($strSitePath);
				
				$strURL = ArrayGetValue($arrayRemoteSiteConfig,"URL");
				$strAccessCode = ArrayGetValue($arrayRemoteSiteConfig,"ACCESSCODE");
				$strFormat = ArrayGetValue($arrayRemoteSiteConfig,"FORMAT");
				
				$consumer = new CWebServiceConsumerWebApplication($this);
					
				$arrayParams = array();
				$arrayParams["command"] = "gettest";
				$arrayParams["test_path"] = MakeTestPath($strRemoteSitePath,$strAssemblyID,$strGroupID,$strTestID);
				$arrayParams["accesscode"] = $strAccessCode; 
				if ($strFormat != "")
				{
					$arrayParams["format"] = $strFormat; 
				}
				
				$consumer->ConsumeWebService($strURL,$arrayParams);
				if ($consumer->GetError() != "")	
				{
					$this->Trace("Error: \"".$consumer->GetError()."\"");		
					return;
				}
				
				$arrayTest = ArrayMerge($arrayTest,$consumer->GetResultArrayInsecure());
			}


			
			
			// Check if we can get the latest status
			
			if ($this->IsDatabaseSupportActive() == true)
			{
				$this->Trace("Get the latest status from database...");

				
				$strSQLCondition = $this->MakeSQL_TestPathCondition(
					$strSitePath,
					$strAssemblyID,
					$strGroupID,
					$strTestID);
				
			
				$arrayTestStatusFields = $this->GetStorageFields("test");
				$arrayTestStatusFields = ArrayRemoveValue($arrayTestStatusFields,"URL");
				
				$strSQL = MakeSQL_Select(
					$this->GetDatabaseTableName("test"),
					$arrayTestStatusFields,
					$strSQLCondition);
				
				if ($this->DatabaseQuery($strSQL) != true)
				{
					return;
				}
				$arrayTestStatusResults = $this->DatabaseGetDataFromLastQuery();
			
				//$this->Trace($arrayTestStatusResults);
			
				if (ArrayCount($arrayTestStatusResults) == 1)
				{
					$arrayTest = ArrayMerge($arrayTest,$arrayTestStatusResults[0]);
				}
				
				//$this->Trace($arrayTest);
			}

			$arrayTest["TEST_PATH"] = $strTestPath;
			$arrayTest["SITE_PATH"] = $strSitePath;
			
			
			$arrayTest = ArrayFilterByKeys($arrayTest,$this->GetPublicFieldsGet("test"));
			
			foreach ($arrayTest as $strTag => $strValue)
			{
				$this->SetResultValue($strValue,$strTag);
			}
			
			
			
			return;	
		}
		
		function GetSiteList(
			$arrayFilterOptions,
			$arraySortOptions)
		{
			$this->Trace("GetSiteList");
			
			$arraySiteList = array();
			
			// Add local site
			$arrayLocalSite = array();
			$arrayLocalSite["SITE_PATH"] = "local";
			$arrayLocalSite["SITE_ID"] = "local";
			$arrayLocalSite["SITE_NAME"] = "Local";
			$arrayLocalSite["URL"] = $this->GetCurrentURL();
			$arraySiteList[] = $arrayLocalSite;
			
			// Add remote sites			
			$arrayRemoteSites = $this->GetRemoteSiteList($arrayFilterOptions);
			if ($arrayRemoteSites != false)
			{
				foreach ($arrayRemoteSites as $arrayRemoteSite)
				{
					$arrayRemoteSite["SITE_PATH"] = $arrayRemoteSite["SITE_ID"];
					$arraySiteList[] = $arrayRemoteSite;
				}
			}
			
			$list = new CList();
			$list->SetListArray($arraySiteList);
			$list->Filter($arrayFilterOptions);
			$list->Sort($arraySortOptions);
			$arraySiteList = $list->GetListArray();
			
			$this->Trace($arraySiteList);
			return $arraySiteList;
		}

		function GetRemoteSite($strID)
		{
			if (StringCheckCharSet($strID,CHARSET_ID) == false)
			{
				$this->Trace("\"$strID\" is not a valid remote site id.");
				$this->SetError("CONFIG_REMOTESITES_INVALID");
				return false;
			}
			$arrayRemoteSiteConfig = $this->GetConfig("remote_sites",$strID);
			if (!is_array($arrayRemoteSiteConfig))
			{
				$this->Trace("site \"$strID\" is not configured.");
				$this->SetError("CONFIG_REMOTESITES_INVALID");
				return false;
			}			
			
			$strURL = ArrayGetValue($arrayRemoteSiteConfig,"url");
			if (IsValidURL($strURL) == false)
			{
				$this->Trace("The URL specified for remote site \"$strID\" is not valid.");
				$this->SetError("CONFIG_REMOTESITES_INVALID");
				return false;
			}
			
			$strName = ArrayGetValue($arrayRemoteSiteConfig,"name");
			if ($strName == "")
			{
				$this->Trace("No name has been specified for remote site \"$strID\".");
				$this->SetError("CONFIG_REMOTESITES_INVALID");
				return false;
			}
			$strAccessCode = ArrayGetValue($arrayRemoteSiteConfig,"accesscode");

			$strFormat = ArrayGetValue($arrayRemoteSiteConfig,"format");
			if ($strFormat == "")
			{
				$strFormat = "csvpath";
			}

				
			$arrayRemoteSite = array();
			$arrayRemoteSite["SITE_ID"] = $strID;
			$arrayRemoteSite["SITE_NAME"] = $strName;
			$arrayRemoteSite["URL"] = $strURL;
			$arrayRemoteSite["ACCESSCODE"] = $strAccessCode;
			$arrayRemoteSite["FORMAT"] = $strFormat;
			return $arrayRemoteSite;	
		}
		
		function GetRemoteSiteBySitePath($strSitePath)
		{
			$arraySitePath = explode("/",$strSitePath);
			$arrayRemoteSiteConfig = $this->GetRemoteSite($arraySitePath[0]);
			return $arrayRemoteSiteConfig;
		}
		

		function GetRemoteSiteList()
		{
			$arrayRemoteSitesConfig = $this->GetConfig("remote_sites");
			if (!is_array($arrayRemoteSitesConfig))
			{
				return array();
			}
			
			$arrayRemoteSites = array();
			foreach ($arrayRemoteSitesConfig as $strID => $arrayRemoteSiteConfig)
			{
				$arrayRemoteSite = $this->GetRemoteSite($strID);
				if ($arrayRemoteSite == false)
				{
					$this->SetError("CONFIG_REMOTESITES_INVALID");
					return false;
				}
				$arrayRemoteSites[] = $arrayRemoteSite;
			}
			return $arrayRemoteSites;
			
		}
		
		function CommandSiteList()
		{
			$arrayFilterConfig = $this->GetFilterOptionsFromEntityDefinition("site");
			$arrayFilterOptions = GetFilterOptionsFromParams(
				$this->GetAllParams(),
				$arrayFilterConfig,
				"site");
				
			$arraySortConfig = $this->GetSortOptionsFromEntityDefinition("site");
			$arraySortOptions = GetSortOptionsFromParams(
				$this->GetAllParams(),
				$arraySortConfig,
				"site");	
			
			$arraySiteList = $this->GetSiteList(
				$arrayFilterOptions,
				$arraySortOptions);
				
				
			$list = ArrayFilterListByKeys(
				$arraySiteList,
				$this->GetPublicFieldsList("site"));
			
				
			$this->SetResultValue($list,"LIST");				
	
		}

		function GetLocalAssemblyList($arrayFilterOptions)
		{
			$this->Trace("GetLocalAssemblyList");
			
			$arrayAssemblyList = array();
			
			// Root assembly
			$arrayAssembly = array();
			$arrayAssembly["ASSEMBLY_ID"] = "root";
			$arrayAssembly["SITE_PATH"] = "local";
			$arrayAssembly["DIR"] = GetSourceDir();
			$arrayAssemblyList[] = $arrayAssembly;

			// WDK assembly
			$arrayAssembly = array();
			$arrayAssembly["ASSEMBLY_ID"] = "wdk";
			$arrayAssembly["SITE_PATH"] = "local";
			$arrayAssembly["DIR"] = GetWDKDir();
			$arrayAssemblyList[] = $arrayAssembly;
			
			// Other assemblies
			$strAssembliesDir = GetSourceDir()."assemblies/";
			$arrayFilesRaw = ReadDirectory($strAssembliesDir);
			
			if ($arrayFilesRaw == false)
			{
				return $arrayAssemblyList;
			}
			if (!is_array($arrayFilesRaw))
			{
				return $arrayAssemblyList;
			}
			
			$nCount = ArrayCount($arrayFilesRaw);
			for ($nIndex = 0; $nIndex < $nCount; $nIndex++)
			{
				$strAssemblyDir = $strAssembliesDir.$arrayFilesRaw[$nIndex];
				
				if (IsDirectory($strAssemblyDir) == true)
				{
					$arrayAssembly = array();
					$arrayAssembly["ASSEMBLY_ID"] = StringSection($arrayFilesRaw[$nIndex],0,-1); // strip trailing slash
					$arrayAssembly["SITE_PATH"] = "local";
					$arrayAssembly["DIR"] = $strAssemblyDir;
					$arrayAssemblyList[] = $arrayAssembly;
				}		
			}

			$list = new CList();
			$list->SetListArray($arrayAssemblyList); 
			$list->Filter($arrayFilterOptions);
			
			// Excludes
			$arrayFilterOptionsConfig = $this->GetConfigFilterOptions();
			if (is_array($arrayFilterOptionsConfig))
			{
				$list->Filter($arrayFilterOptionsConfig);
			}
			
			$list->Sort(
				array("ASSEMBLY_ID" => "asc")
					);			
			$arrayAssemblyList = $list->GetListArray();
			
			return $arrayAssemblyList;	
		}
		
		function GetAssemblyList(
			$arrayFilterOptions,
			$arraySortOptions)
		{
			$arrayAssemblyList = array();
			
			$arraySiteList = $this->GetSiteList(
				$arrayFilterOptions,
				array());
			
			foreach ($arraySiteList as $arraySite)
			{
				$strSitePath = ArrayGetValue($arraySite,"SITE_PATH");
				if ($strSitePath == "local")
				{
					// Enumerate Local Assemblies
					
					$arrayLocalAssemblyList = $this->GetLocalAssemblyList($arrayFilterOptions);
					if ($arrayLocalAssemblyList == false)  
					{
						return false;	
					}
					$arrayAssemblyList = ArrayAppend($arrayAssemblyList,$arrayLocalAssemblyList);
				}
				else
				{
					// Enumerate Remote Assemblies
					
					$strURL = ArrayGetValue($arraySite,"URL");
					$strSiteID = ArrayGetValue($arraySite,"SITE_ID");
					$strAccessCode = ArrayGetValue($arraySite,"ACCESSCODE");
					$strSiteName = ArrayGetValue($arraySite,"SITE_NAME");
					$this->Trace("GetAssemblyList: SiteID = $strSiteID, SiteName = $strSiteName");
					$consumer = new CWebServiceConsumerWebApplication($this);
					
					$arrayParams = array();
					$arrayParams["command"] = "assemblylist";
					$arrayParams["accesscode"] = $strAccessCode;
					$strFormat = ArrayGetValue($arraySite,"FORMAT");
					if ($strFormat != "")
					{
						$arrayParams["format"] = $strFormat;
					}
				
					$arrayAllParams = $this->GetAllParams();
					foreach ($arrayAllParams as $strParamName => $strParamValue)
					{
						if (FindString($strParamName,"assemblylist_filter_") == 0)
						{
							$arrayParams[$strParamName] = $strParamValue;
						}
					}

					$consumer->ConsumeWebService($strURL,$arrayParams);
					if ($consumer->GetError() != "")	
					{
						$this->Trace("Error: \"".$consumer->GetError()."\"");		
					}
					else
					{ 
						$arrayRemoteAssemblyList = $consumer->GetResultList();
						foreach ($arrayRemoteAssemblyList as &$arrayRemoteAssembly)
						{
							$strRemoteSiteID = ArrayGetValue($arrayRemoteAssembly,"SITE_PATH");
							if ($strRemoteSiteID == "local")
							{
								$arrayRemoteAssembly["SITE_PATH"] = $strSiteID;
							}
							else  
							{
								$arrayRemoteAssembly["SITE_PATH"] = $strSiteID."/".$strRemoteSiteID;
							}
							$this->Trace($arrayRemoteAssembly);
						}
						$arrayAssemblyList = ArrayAppend($arrayAssemblyList,$arrayRemoteAssemblyList);
					}
				}
			}
			
			// Filter
		
			$list = new CList();
			$list->SetListArray($arrayAssemblyList);
			$list->Filter($arrayFilterOptions);
			
			// Excludes
			$arrayFilterOptionsConfig = $this->GetConfigFilterOptions();
			if (is_array($arrayFilterOptionsConfig))
			{
				$list->Filter($arrayFilterOptionsConfig);
			}

			// Sort
			$list->Sort($arraySortOptions);
			$arrayAssemblyList = $list->GetListArray();
			
			return $arrayAssemblyList;			
		}
		
		function CommandAssemblyList()
		{
			$arrayFilterConfig = $this->GetFilterOptionsFromEntityDefinition("assembly");
			$arrayFilterOptions = GetFilterOptionsFromParams(
				$this->GetAllParams(),
				$arrayFilterConfig,
				"assembly");
				
			$arraySortConfig = $this->GetSortOptionsFromEntityDefinition("assembly");
			$arraySortOptions = GetSortOptionsFromParams(
				$this->GetAllParams(),
				$arraySortConfig,
				"assembly");		
			
			$arrayAssemblyList = $this->GetAssemblyList(
				$arrayFilterOptions,
				$arraySortOptions);
				
			$list = ArrayFilterListByKeys(
				$arrayAssemblyList,
				$this->GetPublicFieldsList("assembly"));
				
			$this->SetResultValue($list,"LIST");						
		}

		function GetLocalGroupList($strAssemblyID,$arrayFilterOptions)
		{
			//$this->Trace("GetLocalGroupList: AssemblyID = $strAssemblyID");
			$strTestGroupsDir = GetTestGroupsDir($strAssemblyID);
			$arrayFilesRaw = ReadDirectory($strTestGroupsDir);
			
			if ($arrayFilesRaw == false)
			{
				return array();
			}
			if (!is_array($arrayFilesRaw))
			{
				return array();
			}
		
			$arrayGroupList = array();
			
			$nCount = ArrayCount($arrayFilesRaw);
			for ($nIndex = 0; $nIndex < $nCount; $nIndex++)
			{
				$strGroupID = StringSection($arrayFilesRaw[$nIndex],0,-1); // strip trainling slash
				$strGroupDir = $strTestGroupsDir.$strGroupID."/";

				if (IsDirectory($strGroupDir) != true)
				{
					continue;
				}
				if (StringCheckCharSet($strGroupID,CHARSET_TESTID) != true)
				{
					continue;
				}
				
				$arrayGroup = array();
				$arrayGroup["GROUP_ID"] = $strGroupID;
				$arrayGroup["SITE_PATH"] = "local";
				$arrayGroup["ASSEMBLY_ID"] = $strAssemblyID;
				$arrayGroup["DIR"] = $strGroupDir;
				$arrayGroupList[] = $arrayGroup;
			}
			
			$list = new CList();
			$list->SetListArray($arrayGroupList); 
			$list->Filter($arrayFilterOptions);
			
			// Excludes
			$arrayFilterOptionsConfig = $this->GetConfigFilterOptions();
			if (is_array($arrayFilterOptionsConfig))
			{
				$list->Filter($arrayFilterOptionsConfig);
			}
			
			$list->Sort(
				array("GROUP_ID" => "asc")
					);
			$arrayGroupList = $list->GetListArray();
			
			return $arrayGroupList;
		}
		
		function GetGroupList(
			$arrayFilterOptions,
			$arraySortOptions)
		{
			$this->Trace("GetGroupList");
			
			$arrayGroupList = array();
			
			$arraySiteList = $this->GetSiteList(
				$arrayFilterOptions,
				array());
			
			foreach ($arraySiteList as $arraySite)
			{
				$strSitePath = ArrayGetValue($arraySite,"SITE_PATH");
				if ($strSitePath == "local")
				{
					// Enumerate Local Groups
					
					$arrayAssemblyList = $this->GetLocalAssemblyList($arrayFilterOptions);
					$this->Trace($arrayAssemblyList);
					if ($arrayAssemblyList == false)  
					{
						return false;	
					}
					foreach ($arrayAssemblyList as $arrayAssembly)
					{
						$strAssemblyID = ArrayGetValue($arrayAssembly,"ASSEMBLY_ID");
					
						$arrayLocalGroupList = $this->GetLocalGroupList($strAssemblyID,$arrayFilterOptions);
						/*if ($arrayLocalGroupList == false)  
						{
							return false;	
						}*/
						$arrayGroupList = ArrayAppend($arrayGroupList,$arrayLocalGroupList);
					}
				}
				else
				{
					// Enumerate Remote Groups
					
					$strURL = ArrayGetValue($arraySite,"URL");
					$strSiteID = ArrayGetValue($arraySite,"SITE_ID");
					$strAccessCode = ArrayGetValue($arraySite,"ACCESSCODE");
					$strSiteName = ArrayGetValue($arraySite,"SITE_NAME");
					$this->Trace("GetGroupList: SiteID = $strSiteID, SiteName = $strSiteName");
					$consumer = new CWebServiceConsumerWebApplication($this);
					
					$arrayParams = array();
					$arrayParams["command"] = "grouplist";
					$arrayParams["accesscode"] = $strAccessCode;
					$strFormat = ArrayGetValue($arraySite,"FORMAT");
					if ($strFormat != "")
					{
						$arrayParams["format"] = $strFormat;
					}
					
					$arrayAllParams = $this->GetAllParams();
					foreach ($arrayAllParams as $strParamName => $strParamValue)
					{
						if (FindString($strParamName,"grouplist_filter_") == 0)
						{
							$arrayParams[$strParamName] = $strParamValue;
						}
					}

					$consumer->ConsumeWebService($strURL,$arrayParams);
					if ($consumer->GetError() != "")	
					{
						$this->Trace("Error: \"".$consumer->GetError()."\"");		
					}
					else
					{ 
						$arrayRemoteGroupList = $consumer->GetResultList();
						foreach ($arrayRemoteGroupList as &$arrayRemoteGroup)
						{
							$strRemoteSiteID = ArrayGetValue($arrayRemoteGroup,"SITE_PATH");
							if ($strRemoteSiteID == "local")
							{
								$arrayRemoteGroup["SITE_PATH"] = $strSiteID;
							}
							else  
							{
								$arrayRemoteGroup["SITE_PATH"] = $strSiteID."/".$strRemoteSiteID;
							}
							$this->Trace($arrayRemoteGroup);
						}
						$arrayGroupList = ArrayAppend($arrayGroupList,$arrayRemoteGroupList);
					}
				}
			}
			
			// Filter
			$list = new CList();
			$list->SetListArray($arrayGroupList);
			$list->Filter($arrayFilterOptions);
			
			// Excludes
			$arrayFilterOptionsConfig = $this->GetConfigFilterOptions();
			if (is_array($arrayFilterOptionsConfig))
			{
				$list->Filter($arrayFilterOptionsConfig);
			}
			
			// Sort
			$list->Sort($arraySortOptions);
			$arrayGroupList = $list->GetListArray();
			
			return $arrayGroupList;			
		}		


		function CommandGroupList()
		{
			$arrayFilterConfig = $this->GetFilterOptionsFromEntityDefinition("group");
			$arrayFilterOptions = GetFilterOptionsFromParams(
				$this->GetAllParams(),
				$arrayFilterConfig,
				"group");
				
			$arraySortConfig = $this->GetSortOptionsFromEntityDefinition("group");
			$arraySortOptions = GetSortOptionsFromParams(
				$this->GetAllParams(),
				$arraySortConfig,
				"group");				
			
			$arrayGroupList = $this->GetGroupList(
				$arrayFilterOptions,
				$arraySortOptions);
				
			$list = ArrayFilterListByKeys(
				$arrayGroupList,
				$this->GetPublicFieldsList("group"));
				
			$this->SetResultValue($list,"LIST");						
		}

		function GetLocalTestList(
			$strAssemblyID,
			$strGroupID,
			$arrayFilterOptions)
		{
			$strGroupDir = GetTestGroupsDir($strAssemblyID).$strGroupID."/";
			//$this->Trace("GetLocalTestList(): strGroupDir = ".$strGroupDir);
			$arrayFilesRaw = ReadDirectory($strGroupDir);
			if ($arrayFilesRaw == false)
			{
				return array();
			}
			if (!is_array($arrayFilesRaw))
			{
				return array();
			}
			
			$arrayTestList = array();
			
			$nCount = ArrayCount($arrayFilesRaw);
			for ($nIndex = 0; $nIndex < $nCount; $nIndex++)
			{
				$strFileName = $arrayFilesRaw[$nIndex];
				if (FindString($strFileName,"test_") != 0)
				{
					continue;
				}
				if (StringSection($strFileName,-4) != ".php")
				{
					continue;
				}
				$strTestFilePath = $strGroupDir.$strFileName;
				if (IsFile($strTestFilePath) != true)
				{
					continue;
				}
							
				$strTestID = ReplaceString($strFileName,"test_","");
				$strTestID = ReplaceString($strTestID,".php","");
							
				if (StringCheckCharSet($strTestID,CHARSET_TESTID) != true)
				{
					$this->Trace("GetLocalTestList(): TestID = $strTestID is invalid. Please use only lowercase, dash, and digits!");
					continue;
				}
				//$this->Trace("GetLocalTestList(): TestID = $strTestID");
				
				$arrayTest = array();
			 	
				$arrayTest["TEST_PATH"] = "local:".$strAssemblyID."/".$strGroupID."/".$strTestID;
				$arrayTest["SITE_PATH"] = "local";
				$arrayTest["ASSEMBLY_ID"] = $strAssemblyID;
				$arrayTest["GROUP_ID"] = $strGroupID;
				$arrayTest["TEST_ID"] = $strTestID;
				$strTestURL = $this->MakeTestURL($strAssemblyID,$strGroupID,$strTestID);
				$arrayTest["URL"] = $this->MakeTestURL($strAssemblyID,$strGroupID,$strTestID);

				$arrayTestList[] = $arrayTest;
			}
			
			
			$list = new CList();
			$list->SetListArray($arrayTestList); 
			
			// Excludes
			$arrayFilterOptionsConfig = $this->GetConfigFilterOptions();
			if (is_array($arrayFilterOptionsConfig))
			{
				$list->Filter($arrayFilterOptionsConfig);
			}
		
			$list->Sort(
				array("TEST_ID" => "asc")
					);
			$arrayTestList = $list->GetListArray();
			
			
			return $arrayTestList;
		}


		function GetTestList(
			$arrayFilterOptions,
			$arraySortOptions,
			$bDoTest,
			$bGetStatus = true,
			$bGetLogs = false)
		{
			$arrayTestList = array();
			
			// Check if status information is required due to a filter option:
			if ($bGetStatus == false)
			{
				foreach ($arrayFilterOptions as $arrayFilterOption)
				{
					if (ArrayGetValue($arrayFilterOption,"field") == "LASTEXECUTION_STATUS")
					{
						$this->Trace("bGetStatus set to true because LASTEXECUTION_STATUS is used as a filter option");
						$bGetStatus = true;	
					}
				}
			}
			
			
			$arraySiteList = $this->GetSiteList(
				$arrayFilterOptions,
				array());
			
			foreach ($arraySiteList as $arraySite)
			{
				$strSitePath = ArrayGetValue($arraySite,"SITE_PATH");
				if ($strSitePath == "local")
				{
					// Enumerate Local Tests
					
					$arrayAssemblyList = $this->GetLocalAssemblyList($arrayFilterOptions);
					if (is_array($arrayAssemblyList))  
					{
						foreach ($arrayAssemblyList as $arrayAssembly)
						{
							$strAssemblyID = ArrayGetValue($arrayAssembly,"ASSEMBLY_ID");
							//$this->Trace("GetTestList: AssemblyID = $strAssemblyID");
							$arrayGroupList = $this->GetLocalGroupList(
								$strAssemblyID,
								$arrayFilterOptions);
							if ($arrayGroupList != false)
							{
								
								foreach ($arrayGroupList as $arrayGroup)
								{
									$strGroupID = ArrayGetValue($arrayGroup,"GROUP_ID");
									//$this->Trace("GetTestList: GroupID = $strGroupID");
									$arrayLocalTestList = $this->GetLocalTestList(
										$strAssemblyID,
										$strGroupID,
										$arrayFilterOptions);
									if ($arrayLocalTestList != false)
									{
										$arrayTestList = ArrayAppend($arrayTestList,$arrayLocalTestList);
									}
								}
							}
						}
					}
				}
				else
				{
					// Enumerate Remote Tests
					
					$strURL = ArrayGetValue($arraySite,"URL");
					$strSiteID = ArrayGetValue($arraySite,"SITE_ID");
					$strAccessCode = ArrayGetValue($arraySite,"ACCESSCODE");
					$strSiteName = ArrayGetValue($arraySite,"SITE_NAME");
					//$this->Trace("GetTestList: SiteID = $strSiteID, SiteName = $strSiteName");
					$consumer = new CWebServiceConsumerWebApplication($this);
					
					$arrayParams = array();
					$arrayParams["command"] = "testlist";
					$arrayParams["dotest"] = "0";
					$arrayParams["getstatus"] = "0";
					$arrayParams["accesscode"] = $strAccessCode;

					$strFormat = ArrayGetValue($arraySite,"FORMAT");
					if ($strFormat != "")
					{
						$arrayParams["format"] = $strFormat;
					}


					$arrayFilterConfig = $this->GetFilterOptionsFromEntityDefinition("test");
					$nParamIndex = 1;
					foreach ($arrayFilterOptions as $arrayFilterOption)
					{
						$strField = $arrayFilterOption["field"];
						if (	($strField != "LASTEXECUTION_STATUS")
							&& ($strField != "SITE_PATH"))
						{
							foreach ($arrayFilterConfig as $strConfigParam => $strConfigField)
							{
								if ($strField == $strConfigField)
								{
									$arrayParams["test_filter".$nParamIndex] = $strConfigParam;	
									$arrayParams["test_filter".$nParamIndex."_operator"] = $arrayFilterOption["operator"];
									$arrayParams["test_filter".$nParamIndex."_value"] = $arrayFilterOption["value"];
									$nParamIndex++;
								}
							}
						}
					}

					$consumer->ConsumeWebService($strURL,$arrayParams);

					$this->Trace("After consuming $strURL");
					$this->ProfilingTrace();
					
					
					if ($consumer->GetError() != "")	
					{
						$this->Trace("Error: \"".$consumer->GetError()."\"");		
					}
					else
					{ 
						$arrayRemoteSiteTestList = $consumer->GetResultListInsecure();
						foreach ($arrayRemoteSiteTestList as &$arrayRemoteTest)
						{
							$strRemoteSiteID = ArrayGetValue($arrayRemoteTest,"SITE_PATH");
							if ($strRemoteSiteID == "local")
							{
								$arrayRemoteTest["SITE_PATH"] = $strSiteID;
							}
							else  
							{
								$arrayRemoteTest["SITE_PATH"] = $strSiteID."/".$strRemoteSiteID;
							}
							$arrayRemoteTest["TEST_PATH"] = $arrayRemoteTest["SITE_PATH"].":".$arrayRemoteTest["ASSEMBLY_ID"]."/".$arrayRemoteTest["GROUP_ID"]."/".$arrayRemoteTest["TEST_ID"];
							$arrayRemoteTest = ArrayRemoveValue($arrayRemoteTest,"LASTEXECUTION_STATUS");
							$arrayRemoteTest = ArrayRemoveValue($arrayRemoteTest,"LASTEXECUTION_DATETIME");
							$arrayRemoteTest = ArrayRemoveValue($arrayRemoteTest,"LASTEXECUTION_RUNTIME_SECONDS");
						}
						$arrayTestList = ArrayAppend($arrayTestList,$arrayRemoteSiteTestList);
					}
				}
			}

			// Filter
			$list = new CList();
			
			$list->SetListArray($arrayTestList);
			$list->Filter($arrayFilterOptions);
			
			$this->Trace("After Filter");
			$this->ProfilingTrace();
			
			
			// Excludes
			$arrayFilterOptionsConfig = $this->GetConfigFilterOptions();
			if (is_array($arrayFilterOptionsConfig))
			{
				$list->Filter($arrayFilterOptionsConfig);
			}
			
			$this->Trace("After Exclude");
			$this->ProfilingTrace();


			
			$arrayTestList = $list->GetListArray();
			
			// Get Status Information

			if ($this->IsDatabaseSupportActive() == true)
			{
				if ($bGetStatus == true)
				{
					// fetch all existing test results
				
					$arrayFields = $this->GetPublicFieldsList("test");
					$arrayFields = array_intersect($arrayFields,$this->GetStorageFields("test"));
					if ($bGetLogs == true)
					{
						$arrayFields[] = "LASTEXECUTION_LOG";
					}
					
					$strSQL = MakeSQL_Select(
						$this->GetDatabaseTableName("test"),
						$arrayFields,
						"");
					
					$st = new CStopWatch();
					$st->Start();
					$bResult = $this->DatabaseQuery($strSQL);
					$st->Stop();
					$this->Trace("Seconds elapsed during Query: ".$st->GetTotalSeconds());
					
					if ($bResult == true)
					{
						$arrayTestStatus = $this->DatabaseGetDataFromLastQuery();
					
						$list = new CList();
						$list->SetListArray($arrayTestList);
						
						$listTestStatus = new CList();
						$listTestStatus->SetListArray($arrayTestStatus);
						
						$arrayMergeFields = array(
								"LASTEXECUTION_STATUS",
								"LASTEXECUTION_RUNTIME_SECONDS",
								"LASTEXECUTION_DATETIME",
								"LASTEXECUTION_HASH",
								"LASTEXECUTION_ATTEMPTS",
								"FIRSTEXECUTION_DATETIME",
								"COUNT");
						if ($bGetLogs == true)
						{
							$arrayMergeFields[] = "LASTEXECUTION_LOG";
						}

						$st = new CStopWatch();
						$st->Start();
						
						$list->Join(
							$listTestStatus,
							array(
								"SITE_PATH",
								"ASSEMBLY_ID",
								"GROUP_ID",
								"TEST_ID"),
							$arrayMergeFields);
		
						$st->Stop();
						$this->Trace("Seconds elapsed during join: ".$st->GetTotalSeconds());
						
						$arrayTestList = $list->GetListArray();
					}
				}
			}

			$list->SetListArray($arrayTestList);
			$list->Filter($arrayFilterOptions);
			$arrayTestList = $list->GetListArray();
			
			$this->Trace("After Filtering");
			$this->ProfilingTrace();

			// Execute Tests if required

			if ($bDoTest == true)
			{
				foreach ($arrayTestList as &$arrayTest)
				{
					$strStatus = "";
					$strTestLog = "";
					$strHash = "";
					$fRuntimeSeconds = 0.0;
					$strDateTimeExecution = "";
					$nAttemptsCounter = 0;
					$this->DoTest(
						ArrayGetValue($arrayTest,"URL"),
						ArrayGetValue($arrayTest,"SITE_PATH"),
						ArrayGetValue($arrayTest,"ASSEMBLY_ID"),
						ArrayGetValue($arrayTest,"GROUP_ID"),
						ArrayGetValue($arrayTest,"TEST_ID"),
						$this->GetMaxAttempts(),
						$strDateTimeExecution,
						$strStatus,
						$fRuntimeSeconds,
						$strTestLog,
						$strHash,
						$nAttemptsCounter);
					$arrayTest["LASTEXECUTION_STATUS"] = $strStatus;
					$arrayTest["LASTEXECUTION_RUNTIME_SECONDS"] = RenderNumber($fRuntimeSeconds,3);
					$arrayTest["LASTEXECUTION_ATTEMPTS"] = $nAttemptsCounter;
					$arrayTest["LASTEXECUTION_DATETIME"] = $strDateTimeExecution;
					$arrayTest["LASTEXECUTION_HASH"] = $strHash;
					if ($bGetLogs == true)
					{
						$arrayTest["LASTEXECUTION_LOG"] = $strTestLog;
					}
					
				}
			}
			else
			{
				foreach ($arrayTestList as &$arrayTest)
				{
					$strStatus = ArrayGetValue($arrayTest,"LASTEXECUTION_STATUS");
					if ($strStatus == "")
					{
						$arrayTest["LASTEXECUTION_STATUS"] = "UNKNOWN";
					}
				}
			}

			$this->Trace("After Test execution");
			$this->ProfilingTrace();
			
			$list->SetListArray($arrayTestList);
			$list->Sort($arraySortOptions);
			$arrayTestList = $list->GetListArray();			


			$this->Trace("After Sort");
			$this->ProfilingTrace();


			return $arrayTestList;
			
		}

		function CommandTestList()
		{
			
			$arrayFilterOptions = array();
			
			$arrayFilterConfig = $this->GetFilterOptionsFromEntityDefinition("test");
			$this->Trace($arrayFilterConfig);
			$arrayFilterOptions = GetFilterOptionsFromParams(
				$this->GetAllParams(),
				$arrayFilterConfig,
				"test");
			$this->Trace($arrayFilterOptions);				 
			
			$arraySortConfig = $this->GetSortOptionsFromEntityDefinition("test");
			$arraySortOptions = GetSortOptionsFromParams(
				$this->GetAllParams(),
				$arraySortConfig,
				"test");
				
			$this->Trace($arraySortOptions);
			
			$bDoTest = $this->GetParamBoolean("dotest");
			$bGetStatus = $this->GetParamBoolean("getstatus");
			
			$arrayTestList = $this->GetTestList(
				$arrayFilterOptions,
				$arraySortOptions,
				$bDoTest,
				$bGetStatus);
				
			$this->Trace("After GetTestList");
			$this->ProfilingTrace();
			
			$list = ArrayFilterListByKeys(
				$arrayTestList,
				$this->GetPublicFieldsList("test"));
				
			$this->Trace("After ArrayFilterListByKeys");
			$this->ProfilingTrace();
				
			$this->SetResultValue(ArrayCount($list),"TOTALENTRIES");
			$this->SetResultValue($list,"LIST");

			$this->Trace("After SetResultValue");
			$this->ProfilingTrace();

			
		}
		
		
		function CommandCleanUp()
		{
			if ($this->IsDatabaseSupportActive() == true)
			{
				
				// Check for status entries that exceed the storage time
				if ($this->GetConfig("status_history","active") == true)
				{
					$nStorageTimeHours = intval($this->GetConfig("status_history","status_storagetime_hours"));
					if ($nStorageTimeHours > 0)
					{
						$strConditions = "`DATETIME` < ".MakeSQL_Helper_Value(RenderDateTime(GetTimeAddHours(GetTimeNow(), - $nStorageTimeHours)));
					
						$strSQL = MakeSQL_Delete(
							$this->GetDatabaseTableName("statushistory"),
							$strConditions);
							
						if ($this->DatabaseQuery($strSQL) != true)
						{
							return;
						}
					}
				}
			}
		}

		function Alert($arrayTestList)
		{
			$arrayCounter = array();

			$errorlog = new CTraceLog();
			$testlog = new CTraceLog();
			
			$bSendAlertMail = false;
			$strAlertEmail = $this->GetAlertEmail();
			
			foreach ($arrayTestList as $arrayTest)
			{
				$bAddToLog = false;

				$strURL = $arrayTest["URL"];
				
				$strStatus = ArrayGetValue($arrayTest,'LASTEXECUTION_STATUS');
				$strTestPath = ArrayGetValue($arrayTest,'TEST_PATH');
				$strTestLog = ArrayGetValue($arrayTest,'LASTEXECUTION_LOG');
				$strLastExecutionDateTime = ArrayGetValue($arrayTest,'LASTEXECUTION_DATETIME');

				if ($strStatus == "")
				{
					$strStatus = 'UNKNOWN';
				}
				$arrayCounter[$strStatus] = intval(ArrayGetValue($arrayCounter,$strStatus)) + 1;
				
				switch ($strStatus)
				{
				case 'FAILED':
					$errorlog->Trace("$strTestPath FAILED as of $strLastExecutionDateTime");
					$bAddToLog = true;
					$bSendAlertMail = true;
					break;	
				case "REQUESTERROR":
					$errorlog->Trace("$strTestPath REQUESTERROR as of $strLastExecutionDateTime");
					$bAddToLog = true;
					$bSendAlertMail = true;
					break;	
				case "UNDEFINED":
					$errorlog->Trace("$strTestPath UNDEFINED as of $strLastExecutionDateTime");
					$bSendAlertMail = true;
					break;	
				case "UNKNOWN":
					$errorlog->Trace("$strTestPath UNKNOWN STATUS");
					$bAddToLog = true;
					$bSendAlertMail = true;
				default:
					break;
				}
				
				if ($bAddToLog == true)
				{
					$testlog->Trace("$strTestPath BEGIN");
					$testlog->Trace("URL: $strURL");
					$testlog->Trace("LAST EXECUTION: $strLastExecutionDateTime");
					$testlog->Trace("LOG BEGIN");
					$testlog->Trace("");
					$testlog->Trace($strTestLog);
					$testlog->Trace("");
					$testlog->Trace("LOG END");
					$testlog->Trace("$strTestPath END");
					$testlog->Trace("");
					$testlog->Trace("");
				}
			}
			
			$nTotal = ArrayCount($arrayTestList);
			
			if ($bSendAlertMail == true && IsValidEmail($strAlertEmail))
			{
				$strSubject = RenderDateTimeNow().' - ';
				if (ArrayGetValue($arrayCounter,'FAILED') > 0)
				{
					$strSubject .= ''.ArrayGetValue($arrayCounter,"FAILED")." TESTS FAILED (out of $nTotal)";
				}
				else if (ArrayGetValue($arrayCounter,'REQUESTERROR') > 0)
				{
					$strSubject .= ''.ArrayGetValue($arrayCounter,'REQUESTERROR')." TEST REQUESTERRORs (out of $nTotal)";
				}
				else if (ArrayGetValue($arrayCounter,'UNDEFINED') > 0)
				{
					$strSubject .= ''.ArrayGetValue($arrayCounter,'UNDEFINED')." TESTS UNDEFINED (out of $nTotal)";
				}
				else if (ArrayGetValue($arrayCounter,'UNKNOWN') > 0)
				{
					$strSubject .= ''.ArrayGetValue($arrayCounter,'UNKNOWN')." TESTS UNKNOWN STATUS (out of $nTotal)";
				}
				
				$strBody = '';
				
				$strBody .=
					'http://'.GetRootURL()." (".GetEnvID().")\n\n";
				
				$strBody .=
					$errorlog->GetTraceLog()
					."\n";
				
				$strBody .=
					"TOTAL       : ".str_pad("$nTotal",4," ",STR_PAD_LEFT)."\n";
				foreach (GetTestStatusEnum() as $strStatusIterator)
				{
					$strBody .= 
						str_pad($strStatusIterator,12," ").
						": ".
						str_pad(ArrayGetValue($arrayCounter,$strStatusIterator),4," ",STR_PAD_LEFT).
						"\n";
				}
				 
				$strBody .= 
					"\n".
					"\n".
					$testlog->GetTraceLog();
				
				$this->SendMail(
					$strAlertEmail,
					$strSubject,
					$strBody);
			}
		}
		
		function Statistics($arrayTestList)
		{
			$arrayCounter = array();
			foreach ($arrayTestList as $arrayTest)
			{
				$strStatus = ArrayGetValue($arrayTest,'LASTEXECUTION_STATUS');
				
				if ($strStatus == '')
				{
					$strStatus = 'UNKNOWN';
				}
				$arrayCounter[$strStatus] = intval(ArrayGetValue($arrayCounter,$strStatus)) + 1;
				
			}
			$nTotal = ArrayCount($arrayTestList);
			$this->SetResultValue("$nTotal",'STATISTICS','TOTAL');
			foreach (GetTestStatusEnum() as $strStatus)
			{
				$this->SetResultValue(intval(ArrayGetValue($arrayCounter,$strStatus)),'STATISTICS',$strStatus);	
			}
		}

		function CommandTestRun()
		{
			if ($this->IsDatabaseSupportActive() == false)
			{
				$this->SetError("DATABASESUPPORT_REQUIRED");
				return;	
			}
			
			$nTestrunMaxSeconds = $this->GetConfig("testrun","max_seconds");
			if ($nTestrunMaxSeconds == 0)
			{
				$nTestrunMaxSeconds = 45;
			}
			$nTestrunMaxTests = $this->GetConfig("testrun","max_tests");
			if ($nTestrunMaxTests == 0)
			{
				$nTestrunMaxTests = 10;
			}
			
			$arrayFilterOptions = array(
				array(
					"field"		=> "GROUP_ID",
					"operator"	=> "!=",
					"value"		=> "test")
				);
			
			/*
			$arraySortOptions = array(
				"LASTEXECUTION_DATETIME" => "asc"
				);
				*/
			
			
			$arrayTestList = $this->GetTestList(
				$arrayFilterOptions,
				//$arraySortOptions,
				array(),
				false,	// don't do tests
				true,		// get status from database
				true);	// get logs
			
			
			// Test execution order:
			// 1. UNKNOWN status (which includes new tests)
			// 2. REQUESTERROR status (resquest errors should always be temporary)
			// 3. All others sorted by age of last test result

			foreach ($arrayTestList as &$arrayTestPrio)
			{
				$strPriority = "";
				$strStatus = ArrayGetValue($arrayTestPrio,"LASTEXECUTION_STATUS");
				switch ($strStatus)
				{
					case "":
					case "UNKNOWN":
						$strPriority = "1";
						break;
					case "REQUESTERROR":
						$strPriority = "2";
						break;
					default:
						$strPriority = "3";
						break;
				}
				$arrayTestPrio["PRIORITY"] = $strPriority;
			}
			
			$listTestList = new CList();
			$listTestList->SetListArray($arrayTestList);
			$arraySortOptions = array(
				"PRIORITY" => "asc",
				"LASTEXECUTION_DATETIME" => "asc"
				);
			$listTestList->Sort($arraySortOptions);
			$arrayTestList = $listTestList->GetListArray();
			
			//$this->Trace("Sorted list of tests:");
			//$this->Trace($arrayTestList);
			
			$stopwatch = new CStopWatch();
			$stopwatch->Start();
			$nTestCount = 0;
			
			$arrayTestListOutput = array();
			
			$strStatus = "";

			foreach ($arrayTestList as &$arrayTest)
			{
				$this->Trace("TestRun iteration: $nTestCount");
				//$this->Trace($arrayTest);
				if (	($stopwatch->GetSeconds() > $nTestrunMaxSeconds)
					|| ($nTestCount >= $nTestrunMaxTests))
				{
					$this->Trace("Stopping testrun.");
					$this->Trace("Second ".$stopwatch->GetSeconds()." of ".$nTestrunMaxSeconds);
					$this->Trace("Iteration ".$nTestCount." of ".$nTestrunMaxTests);
					break;
				}
				
				$strStatus = "";
				$strTestLog = "";
				$strHash = "";
				$fRuntimeSeconds = 0.0;
				$strDateTimeExecution = "";
				$this->DoTest(
					ArrayGetValue($arrayTest,"URL"),
					ArrayGetValue($arrayTest,"SITE_PATH"),
					ArrayGetValue($arrayTest,"ASSEMBLY_ID"),
					ArrayGetValue($arrayTest,"GROUP_ID"),
					ArrayGetValue($arrayTest,"TEST_ID"),
					$this->GetMaxAttempts(),
					$strDateTimeExecution,	
					$strStatus,
					$fRuntimeSeconds,
					$strTestLog,
					$strHash,
					$nAttemptsCounter);
				$arrayTest["LASTEXECUTION_STATUS"] = $strStatus;
				$arrayTest["LASTEXECUTION_RUNTIME_SECONDS"] = RenderNumber($fRuntimeSeconds,3);
				$arrayTest["LASTEXECUTION_ATTEMPTS"] = $nAttemptsCounter;
				$arrayTest["LASTEXECUTION_DATETIME"] = $strDateTimeExecution;
				$arrayTest["LASTEXECUTION_HASH"] = $strHash;
				$arrayTest["LASTEXECUTION_LOG"] = $strTestLog;

				$arrayTestListOutput[] = $arrayTest;				
				$nTestCount++;					
			}
			
			$stopwatch->Stop();
			
			$this->Statistics($arrayTestList);
			$this->SetResultValue("$nTestCount","STATISTICS","TESTED");
			
			$list = ArrayFilterListByKeys(
				$arrayTestListOutput,
				$this->GetPublicFieldsList("test"));

			$this->SetResultValue($list,"LIST");
		}


		function CommandTestAll()
		{
			$arrayFilterOptions = array(
				array(
					"field"		=> "GROUP_ID",
					"operator"	=> "!=",
					"value"		=> "test")
				);
			
			
			$arrayTestList = $this->GetTestList(
				$arrayFilterOptions,
				array(),
				true,
				true,
				true);
			if ($arrayTestList == false)
			{
				return;	
			}
			
			$this->Alert($arrayTestList);
			$this->Statistics($arrayTestList);
	
			$list = ArrayFilterListByKeys(
				$arrayTestList,
				$this->GetPublicFieldsList("test"));

			$this->SetResultValue($list,"LIST");				
		}
		
		function CommandTestLocal()
		{
			$arrayFilterOptions = array(
				array(
					"field"		=> "GROUP_ID",
					"operator"	=> "!=",
					"value"		=> "test"),
				array(
					"field"		=> "SITE_ID",
					"operator"	=> "=",
					"value"		=> "local")
				);
			
			
			$arrayTestList = $this->GetTestList(
				$arrayFilterOptions,
				array(),
				true,
				true);
			if ($arrayTestList == false)
			{
				return;	
			}
			
			$this->Alert($arrayTestList);
			$this->Statistics($arrayTestList);
	
			$list = ArrayFilterListByKeys(
				$arrayTestList,
				$this->GetPublicFieldsList("test"));

			$this->SetResultValue($list,"LIST");				
		}		
		
		
		function CommandAlert()
		{
			if ($this->IsDatabaseSupportActive() == false)
			{
				$this->SetError("DATABASESUPPORT_REQUIRED");
				return;	
			}
						
			$arrayFilterOptions = array(
				array(
					"field"		=> "GROUP_ID",
					"operator"	=> "!=",
					"value"		=> "test")
				);
			
			$arrayTestList = $this->GetTestList(
				$arrayFilterOptions,
				array(),
				false,
				true,
				true);   
			if ($arrayTestList == false)
			{
				return;	
			}
			
			$this->Alert($arrayTestList);
			$this->Statistics($arrayTestList);
	
			$arrayPublicFields = $this->GetPublicFieldsList("test");
	
			$list = ArrayFilterListByKeys(
				$arrayTestList,
				$arrayPublicFields);

			$this->SetResultValue($list,"LIST");				
		}		
		
		
		function GetConfigFilterOptions()
		{
			$arrayFilterOptionsConfig = array();

			$arrayExcludeAssemblies = $this->GetConfig("exclude","assemblies");
			if (is_array($arrayExcludeAssemblies))
			{
				foreach ($arrayExcludeAssemblies as $strAssemblyID)
				{
					$arrayFilterOptionsConfig[] = array(
						"field" => "ASSEMBLY_ID",
						"operator" => "!=",
						"value" => $strAssemblyID);
				}
			}
				
			$arrayExcludeGroups = $this->GetConfig("exclude","groups");
			if (is_array($arrayExcludeGroups))
			{
				foreach ($arrayExcludeGroups as $strGroupID)
				{
					$arrayFilterOptionsConfig[] = array(
						"field" => "GROUP_ID",
						"operator" => "!=",
						"value" => $strGroupID);
				}
			}				

			$arrayExcludeTests = $this->GetConfig("exclude","tests");
			if (is_array($arrayExcludeTests))
			{
				foreach ($arrayExcludeTests as $strTestID)
				{
					$arrayFilterOptionsConfig[] = array(
						"field" => "TEST_ID",
						"operator" => "!=",
						"value" => $strTestID);
				}
			}	

						
			if (ArrayCount($arrayFilterOptionsConfig) == 0)
			{
				return false;
			}
			return $arrayFilterOptionsConfig;
		}
	}

