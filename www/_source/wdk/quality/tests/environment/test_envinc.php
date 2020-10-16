<?php
	
	require_once(GetWDKDir()."wdk_filesys.inc");
	require_once(GetWDKDir()."wdk_mail.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Check env.inc");
		}
		 
		function
		TestCase_CheckEnvFunction($strFunctionName)
		{
			$this->Trace("\n\nCheck function: $strFunctionName");
			if (function_exists($strFunctionName) == false)
			{
				$this->SetResult(false);
				$this->Trace("TESTCASE FAILED");
				return false;
			}
			
			$this->Trace("TESTCASE PASSED");
			return true;
				
		}
		

		function CallbackTest()
		{
			parent::CallbackTest();
					
			$this->SetResult(true);
			
			if (CheckEnvironment() == false)
			{
				$this->Trace("CheckEnvironment() returned false");
				$this->SetResult(false);
			}
			
			if ($this->TestCase_CheckEnvFunction("GetDocumentRootDir") == true)
			{
				$strDocRoot = GetDocumentRootDir();
				if (IsDirectory($strDocRoot) == false)
				{
					$this->SetResult(false);
					$this->Trace("GetDocumentRootDir() returned an invalid directory: \"$strDocRoot\"");
					$this->Trace("TESTCASE FAILED");
				}
			}
		
			if ($this->TestCase_CheckEnvFunction("GetSourceDir") == true)
			{
				$strSourceDir = GetSourceDir();
				if (IsDirectory($strSourceDir) == false)
				{
					$this->SetResult(false);
					$this->Trace("GetSourceDir() returned an invalid directory: \"$strSourceDir\"");
					$this->Trace("TESTCASE FAILED");
				}
			}	
			 
			if ($this->TestCase_CheckEnvFunction("GetWDKDir") == true)
			{
				$strWDKDir = GetWDKDir();
				if (IsDirectory($strWDKDir) == false)
				{
					$this->SetResult(false);
					$this->Trace("GetWDKDir() returned an invalid directory: \"$strWDKDir\"");
					$this->Trace("TESTCASE FAILED");
				}
				if (IsFile($strWDKDir."wdk.txt") == false)
				{
					$this->SetResult(false);
					$this->Trace("$strWDKDir does not contain the file wdk.txt");
					$this->Trace("TESTCASE FAILED");
				}
			}	
			
			if ($this->TestCase_CheckEnvFunction("GetConfigDir") == true)
			{
				$strConfigDir = GetConfigDir();
				if (IsDirectory($strConfigDir) == false)
				{
					$this->SetResult(false);
					$this->Trace("GetConfigDir() returned an invalid directory: \"$strConfigDir\"");
					$this->Trace("TESTCASE FAILED");
				}
			}										

			if ($this->TestCase_CheckEnvFunction("GetWebservicesDir") == true)
			{
				$strWebservicesDir = GetWebservicesDir();
				if (IsDirectory($strWebservicesDir) == false)
				{
					$this->SetResult(false);
					$this->Trace("GetWebservicesDir() returned an invalid directory: \"$strWebservicesDir\"");
					$this->Trace("TESTCASE FAILED");
				}
			}										

			if ($this->TestCase_CheckEnvFunction("GetQualityDir") == true)
			{
				$strQualityDir = GetQualityDir();
				if (IsDirectory($strQualityDir) == false)
				{
					$this->SetResult(false);
					$this->Trace("GetQualityDir() returned an invalid directory: \"$strQualityDir\"");
					$this->Trace("TESTCASE FAILED");
				}
			}
	
			if ($this->TestCase_CheckEnvFunction("GetRootURL") == true)
			{
				$strURL = GetRootURL();
				$strURL = "http://".$strURL;
				if (IsValidURL($strURL) == false)
				{
					$this->SetResult(false);
					$this->Trace("GetRootURL() returned an invalid url: \"".GetRootURL()."\"");
					$this->Trace("TESTCASE FAILED");
				}
			}
	
			if ($this->TestCase_CheckEnvFunction("GetWebservicesURL") == true)
			{
				$strURL = GetWebservicesURL();
				$strURL = "http://".$strURL;
				if (IsValidURL($strURL) == false)
				{
					$this->SetResult(false);
					$this->Trace("GetWebservicesURL() returned an invalid url: \"".GetWebservicesURL()."\"");
					$this->Trace("TESTCASE FAILED");
				}
			}

			if ($this->TestCase_CheckEnvFunction("GetWebservicesDefaultFormat") == true)
			{
				$strFormat = GetWebservicesDefaultFormat();
				$arrayFormats = array("xml","csvpath","json");
				if (ArrayValueExists($arrayFormats,$strFormat) != true)
				{
					$this->SetResult(false);
					$this->Trace("GetWebservicesDefaultFormat() returned an invalid default format: \"".GetWebservicesDefaultFormat()."\"");
					$this->Trace("TESTCASE FAILED");
				}
			}


			if ($this->TestCase_CheckEnvFunction("GetAdminEmail") == true)
			{
				$strAdminEmail = GetAdminEmail();
				if (IsValidEmail($strAdminEmail) == false)
				{
					$this->SetResult(false);
					$this->Trace("GetAdminEmail() returned an invalid email address: \"$strAdminEmail\"");
					$this->Trace("TESTCASE FAILED");
				}
			}

			if ($this->TestCase_CheckEnvFunction("GetSystemEmail") == true)
			{
				$strSystemEmail = GetSystemEmail(); 
				if (IsValidEmail($strSystemEmail) == false)
				{
					$this->SetResult(false);
					$this->Trace("GetSystemEmail() returned an invalid email address: \"$strSystemEmail\"");
					$this->Trace("TESTCASE FAILED");
				}
			}
	
	
	
			if ($this->TestCase_CheckEnvFunction("GetSystemEmailSenderName") == true)
			{
				$strSystemEmailSenderName = GetSystemEmailSenderName();
				if ($strSystemEmailSenderName == "")
				{
					$this->SetResult(false);
					$this->Trace("GetSystemEmailSenderName() returned \"\"");
					$this->Trace("TESTCASE FAILED");
				}
			}

			if ($this->TestCase_CheckEnvFunction("GetMailDomain") == true)
			{
				$strMailDomain = GetMailDomain();
				// How to check a domain name?
			}

			if ($this->TestCase_CheckEnvFunction("GetStage") == true)
			{
				$arrayStages = array(
					"dev",
					"test",
					"preprod",
					"prod");
				$strStage = GetStage();				
				if (in_array($strStage,$arrayStages) == false)
				{
					$this->SetResult(false);
					$this->Trace("GetStage() returned an invalid stage: \"$strStage\"");
					$this->Trace("TESTCASE FAILED");
				}
			}

			if ($this->TestCase_CheckEnvFunction("GetDatabaseConfigID") == true)
			{
				$strDatabaseConfigID = GetDatabaseConfigID();
				if ($strDatabaseConfigID != "")
				{
					if (IsValidConfigID($strDatabaseConfigID) == false)
					{
						$this->SetResult(false);
						$this->Trace("GetDatabaseConfigID() returned an invalid condig id: \"$strDatabaseConfigID\"");
						$this->Trace("TESTCASE FAILED");
					}	
				}
			}

			if ($this->TestCase_CheckEnvFunction("GetOperationMode") == true)
			{
				$arrayModes = array(
					"normal",
					"readonly");
				$strOperationMode = GetOperationMode();				
				if (in_array($strOperationMode,$arrayModes) == false)
				{
					$this->SetResult(false);
					$this->Trace("GetOperationMode() returned an invalid mode: \"$strOperationMode\"");
					$this->Trace("TESTCASE FAILED");
				}
			}

			if ($this->TestCase_CheckEnvFunction("GetGpgPath") == true)
			{
				$strGpgPath = GetGpgPath();
				if ($strGpgPath == "")
				{
					$this->Trace("GetGpgPath() returned \"\". No support for gnupg is configured.");
				}
				else
				{
					$this->Trace("GetGpgPath() returned \"$strGpgPath\".");
					if (IsFile($strGpgPath,true) == false)
					{
						$this->SetResult(false);						
						$this->Trace("GetGpgPath() returned an invalid path: \"$strGpgPath\"");
						$this->Trace("TESTCASE FAILED");
					}

				
					// check the path, if set

					
				}
			}
			
	
			if ($this->TestCase_CheckEnvFunction("GetTempDir") == true)
			{
				$strTempDir = GetTempDir();
				if ($strTempDir != '')
				{
					if (IsDirectory($strTempDir) == false)
					{
						$this->SetResult(false);
						$this->Trace("GetTempDir() returned an invalid directory: \"$strTempDir\"");
						$this->Trace("TESTCASE FAILED");
					}
					else
					{
						$strTestFile = $strTempDir."envtest.txt";

						$bResult = FileWrite($strTestFile,'test');
						if ($bResult == false)
						{
							$this->SetResult(false);
							$this->Trace("FileWrite(\"$strTestFile\") returned false");
							$this->Trace('TESTCASE FAILED');
						}

						$bResult = FileWrite($strTestFile,'test');
						if ($bResult == true)
						{
							$this->SetResult(false);
							$this->Trace("FileWrite(\"$strTestFile\") returned true but it should have failed.");
							$this->Trace('TESTCASE FAILED');
						}

						$bResult = FileAddText($strTestFile,' more');
						if ($bResult == false)
						{
							$this->SetResult(false);
							$this->Trace("FileAddText(\"$strTestFile\") returned false");
							$this->Trace("TESTCASE FAILED");
						}
	
						$bResult = IsFile($strTestFile);
						if ($bResult == false)
						{
							$this->SetResult(false);
							$this->Trace("IsFile(\"$strTestFile\") returned false");
							$this->Trace("TESTCASE FAILED");
						}
	
						$bResult = DeleteFile($strTestFile);
						if ($bResult == false)
						{
							$this->SetResult(false);
							$this->Trace("DeleteFile(\"$strTestFile\") returned false");
							$this->Trace("TESTCASE FAILED");
						}
						
						$bResult = IsFile($strTestFile);
						if ($bResult == true)
						{
							$this->SetResult(false);
							$this->Trace("IsFile(\"$strTestFile\") returned true but it should not exist any more.");
							$this->Trace("TESTCASE FAILED");
						}
					}
				}
				else
				{
					$this->Trace('No temp dir specified.');
				}
			}	


			if ($this->TestCase_CheckEnvFunction("GetErrorLogPath") == true)
			{
				$strLogFile = GetErrorLogPath();
				if ($strLogFile != "")
				{
					$bLogFileTestResult = true;
					$this->Trace("GetErrorLogPath() returned \"$strLogFile\".");
					if (IsFile($strLogFile) == false)
					{
						$bLogFileTestResult = false;
						$this->Trace("IsFile() returned false!");
					}
					else
					{
						$strLog = FileRead($strLogFile);
						if ($strLog === false)
						{
							$bLogFileTestResult = false;
							$this->Trace("FileRead(\"$strLogFile\") returned false");
						}
					}
					if ($bLogFileTestResult == false)
					{
						$this->SetResult(false);						
						$this->Trace("");
						$this->Trace("The log file does not exist or the www-data user does not have access to it.");
						$this->Trace("");
						$this->Trace("If the file exists:");
						$this->Trace("Execute \"chmod a+x logs\" where 'logs' is the directory in which the log file is stored.");
						$this->Trace("");
						$this->Trace("If the file is written to a different place:");
						$this->Trace("Execute \"link /<actual_dir>/error_log /<accessible_dir>/error_log\".");
						$this->Trace("This may be \"link /var/www/vhosts/system/<vhost>/logs/error_log /var/www/vhosts/<vhost>/logs/error_log\".");
						$this->Trace("");
						$this->Trace("TESTCASE FAILED");
					}
				}
			}	



			
		}
		
			
	}
	
	

		
