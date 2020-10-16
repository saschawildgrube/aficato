<?php

	require_once(GetWDKDir().'wdk_webserviceconsumer_webapplication.inc');
	require_once(GetWDKDir().'wdk_ssl.inc');

	abstract class CUnitTest extends CWebApplication
	{
		private $m_strName;
		 
		private $m_bActive;
		private $m_bResult;
		private $m_bVerbose;
		
		private $m_bCallbackInit;
		private $m_bCallbackTest;
		private $m_bCallbackCleanup;
		
		
		function __construct($strName,$arrayConfig = array())
		{
			parent::__construct(
				array(),
				$arrayConfig,
				array());
			
			$this->m_strName = $strName;
			
			$this->m_bResult = false;
			$this->m_bActive = true;
			$this->m_bRequestError = false;
			$this->m_bVerbose = false;
			
			$this->m_bCallbackInit = false;
			$this->m_bCallbackTest = false;
			$this->m_bCallbackCleanup = false;
			
			if ($this->m_strName == "")
			{
				$this->Trace('ERROR: UNIT TEST WITHOUT A NAME!');	
				$this->SetResult(false);
				return;
			}
									
			$this->Trace("UNIT TEST \"$strName\"");
			$this->Trace('URL '.$this->GetSelfURL());
			$this->Trace('');
			
			/*
			$arrayParams = PrepareParameterArray();
			if (ArrayCount($arrayParams) > 0)
			{
				$this->Trace("ERROR: No parameters allowed!");
				$this->SetResult(false);
				return;
			}
			*/
			
			$this->Trace("INIT BEGIN");
			$bResultInit = $this->CallbackInit();
			if ($this->m_bCallbackInit == false)
			{
				$this->Trace("ERROR: CallbackInit must invoke parent implementation!");
				$this->SetResult(false);
				return;
			}
			if ($bResultInit == false)
			{
				$this->Trace("ERROR: Init failed!");
				$this->SetResult(false);
				return;
			}
			$this->Trace("INIT END");
			$this->Trace("");
			
			if ($this->m_bActive != true)
			{
				$this->Trace("Test set inactive during initialization!");
				return;	
			}
			
			$this->Trace("TEST BEGIN");
			$this->CallbackTest();
			if ($this->m_bCallbackTest == false)
			{
				$this->Trace("ERROR: CallbackTest must invoke parent implementation! Test will fail.");
				$this->SetResult(false);
			}
			$this->Trace("TEST END");
			$this->Trace("");
		}
		
		function Cleanup()
		{
			$this->Trace("CLEANUP BEGIN");
			$bResultCleanup = $this->CallbackCleanup();
			if ($this->m_bCallbackCleanup == false)
			{
				$this->Trace("ERROR: CallbackCleanup must invoke parent implementation! Test will fail.");
				$this->SetResult(false);
			}
			if ($bResultCleanup == false)
			{
				$this->Trace("ERROR: Cleanup failed!");
				$this->SetResult(false);
			}
			$this->Trace("CLEANUP END");
			$this->Trace("");
		}
		
		function __destruct()
		{
			$this->Cleanup();
			if (!$this->m_bActive)
			{
    			print("INACTIVE\n");
			}
			$this->ProfilingTrace();
			print($this->GetTraceLog());
			print("\n\n");
			if ($this->m_bRequestError == true)
			{
				print("REQUESTERROR");
			}
			else
			{
				print(($this->m_bResult == true)?("OK"):("FAILED"));
			}
			if (!$this->m_bActive)
  		{
  			print("\nINACTIVE");	
  		} 
		}

		function SetResult($bResult)
		{
			$this->m_bResult = $bResult;	
		}
		function GetResult()
		{
			return $this->m_bResult;	
		}		
		
		function SetRequestError($bRequestError)
		{
			$this->m_bRequestError = $bRequestError;	
		}
		function GetRequestError()
		{
			return $this->m_bRequestError;	
		}
		
		function SetActive($bActive)
		{
			$this->m_bActive = $bActive;	
		}

		function SetVerbose($bVerbose)
		{
			$this->SetTraceActive($bVerbose);
			$this->m_bVerbose = $bVerbose;	
		}
		function GetVerbose()
		{
			return $this->m_bVerbose;	
		}
		
		function RequireWebservice($strWebservice)
		{
			if ($this->GetWebserviceURL($strWebservice) == "")
			{
				$this->Trace("Web service ".$strWebservice." is not configured for this web application system. This test is deactivated.");
				$this->SetActive(false);	
			}
		}

		
		function CallbackInit()
		{
			$this->m_bCallbackInit = true;
			return true;
		}
		function CallbackTest()
		{
			$this->m_bCallbackTest = true;	
		}
		function CallbackCleanup()
		{
			$this->m_bCallbackCleanup = true;
			return true;
		}




		function TestCase_CheckURL(
			$strURL,
			$arrayExpectedPatternWhitelist = array(),
			$arrayExpectedPatternBlacklist = array(),
			$arrayRegExpPattern = array(),
			$arrayParams = array(),
			$arrayCookies = array(),
			$arrayHeaders = array(),
			$strMode = "get",
			$nTimeoutSeconds = 15,
			$bSetRequestErrorOnTimeout = true,
			$bBinary = false,
			$nSSLCertValidityThresholdDays = 0)
		{			
	
			$bResult = true;
			if ($strMode == "get")
			{
				$this->Trace("Test URL: ".MakeURL($strURL,$arrayParams,false) . " ($nTimeoutSeconds)");
			}
			else
			{
				$this->Trace("Test URL: ".$strURL);
				$this->Trace("Post parameters:");
				$this->Trace($arrayParams);
			}
			if (ArrayCount($arrayCookies))
			{
				$this->Trace("Cookies:");
				$this->Trace($arrayCookies);	
			}
			if (ArrayCount($arrayHeaders))
			{
				$this->Trace("Headers:");
				$this->Trace($arrayHeaders);	
			}
			
			//$this->Trace("Timeout : ".$nTimeoutSeconds);

			$strResponse = HttpRequest(
				$strURL,
				$arrayParams,
				$strMode,
				$nTimeoutSeconds,
				$arrayCookies,
				$arrayHeaders,
				true);
				
			if ($strResponse == false)
			{
				$this->Trace("Failed to load $strURL");
				if ($bSetRequestErrorOnTimeout == true)
				{
					$this->Trace("Test result is now set to REQUESTERROR!");
					$this->SetRequestError(true);
				}
				else
				{
					$this->SetResult(false);
				}
				return false;
			}
			
			if ($strResponse != false)
			{
				$arrayResponse = ParseHttpResponse($strResponse);
				$strStatusCode = ArrayGetValue($arrayResponse,'statuscode');
				$this->Trace('Status code is '.$strStatusCode);
				if (StringSection($strStatusCode,0,1) == '5')
				{
					//$this->Trace('Status code is '.$strStatusCode);
					$bResult = false;
				}
				$strContent = ArrayGetValue($arrayResponse,'content');
			}				
			
			
			if ($bBinary == false)
			{
				$strContent = StringRemoveControlChars($strContent);
			}
			else
			{
				$strContent = bin2hex($strContent);
				$this->Trace(StringCutOff($strContent));
			}
			
			if ($this->GetVerbose())
			{
				$this->Trace("Result length after removing control chars: ".StringLength($strContent));
			}
		
			if (is_array($arrayExpectedPatternWhitelist))
			{
				if ($this->GetVerbose())
				{
					$this->Trace("Checking for must-have patterns");
				}
				foreach ($arrayExpectedPatternWhitelist as $strPattern)       
				{
					if ($bBinary == true)
					{
						$strPattern = bin2hex($strPattern);
					}
					$strPattern = StringRemoveControlChars($strPattern);
					if (FindString($strContent,$strPattern) == -1)    
					{
						$this->Trace("Expected pattern not found:\n\"".StringCutOff($strPattern,300)."\"");	
						$bResult = false;
					}
				}
			}
			if (is_array($arrayExpectedPatternBlacklist))
			{
				if ($this->GetVerbose())
				{
					$this->Trace("Checking for forbidden patterns");
				}
				foreach ($arrayExpectedPatternBlacklist as $strPattern)
				{
					if ($bBinary == true)
					{
						$strPattern = bin2hex($strPattern);
					}
					$strPattern = StringRemoveControlChars($strPattern);
					if (FindString($strContent,$strPattern) != -1)
					{
						$this->Trace("Unexpected pattern (\"".StringCutOff($strPattern,200)."\") found!");	
						$bResult = false;
					}
				}
			}
			if (is_array($arrayRegExpPattern))
			{
				if ($this->GetVerbose())
				{
					$this->Trace("Checking for regular expressions");
				}
				foreach ($arrayRegExpPattern as $strPattern)
				{
					if (RegExpMatch($strContent,$strPattern) != true)
					{
						$this->Trace("Regular Expression did not match (\"".StringCutOff($strPattern,100)."\")!");	
						$bResult = false;
					}	
				}	
			}
			
			if ($nSSLCertValidityThresholdDays > 0)
			{
				$arrayURL = ParseURL($strURL);
				$strProtocol = ArrayGetValue($arrayURL,'PROTOCOL');
				if ($strProtocol == 'https')
				{
					$strHost = ArrayGetValue($arrayURL,'HOST');
					$arrayCerts = GetSSLCertificatesFromHost($strHost);
					$arrayCert = ArrayGetValue($arrayCerts,0);
					if (!is_array($arrayCert))
					{
						$this->Trace("Could not retrieve SSL certificate.");	
						$bResult = false;
					}
					else
					{
						$timeValidTo = intval(ArrayGetValue($arrayCert,'validTo_time_t'));
						$strDateTimeValidTo = RenderDateTime($timeValidTo);
						$nSecondsRemainingThreshold = $nSSLCertValidityThresholdDays * 24 * 60 * 60;
						if ($timeValidTo < time() + $nSecondsRemainingThreshold)
						{
							if ($timeValidTo < time())
							{
								$this->Trace('SSL certificate expired as of '.$strDateTimeValidTo.'!');	
							}
							else
							{
								$this->Trace('SSL certificate will expire as of '.$strDateTimeValidTo.'!');									
							}
							$bResult = false;
						}
						else
						{
							$this->Trace('SSL certificate is OK.');	
						}
					}
				}
			}
			
			if ($bResult != true)
			{
				$this->SetResult(false);
				$this->Trace('TESTCASE FAILED');
			}
			return $strContent;
		}



	}

