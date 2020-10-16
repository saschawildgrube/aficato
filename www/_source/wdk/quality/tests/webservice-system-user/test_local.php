<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	
	/*
	
	0. delete test user
	1. Create new user
	2. Get user
	3. Set user language
	4. Get user
	5. Set user language (invalid)
	6. Get user
	7. Set user language (empty)
	8. Get user
	9. delete user
	
	
	*/

	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		
		private $m_strUserName;
		private $m_strUserID;
		
		function __construct()
		{
			$this->m_strWebservice = "system/user";			
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service system/user",$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->m_strUserName = "test-local";
			$this->SetVerbose(false);
			$this->RequireWebservice($this->m_strWebservice);			 
			return parent::CallbackInit();
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			
			
			
			
			$strLanguage1 = "DE";
			$strLanguage2 = "EN";
			$strLanguage3 = "ZZ";
			$strCountry1 = "DEU";
			$strCountry2 = "GBR";
			$strCountry3 = "ZZZ";
			$strTimezoneDefault = "UTC";
			$strTimezone2 = "Europe/London";
			$strTimezone3 = "Europe/Frankfurt";
		 
		
		
		
			$this->Trace("Test User Webservice: User Local Settings (Language, Timezone)");
		
		
			$this->Trace("0. DELETE USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $this->m_strUserName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace("");
		
		
			$this->Trace("1. ADD USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $this->m_strUserName;
			$arrayParams["password"] = "changeme";
			$arrayParams["user_language"] = $strLanguage1;
			$arrayParams["user_country"] = $strCountry1;
			//$arrayParams["timezone"] = $strTimezone1;
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->m_strUserID = $consumer->GetResultValue("NEW_USER_ID");
			$this->Trace("New USERID = ".$this->m_strUserID);
			if ($this->m_strUserID == "")	
			{
				$this->Trace("Error: New user ID is empty!");		
				return;	
			}
			$this->Trace("");
		
		
			$this->Trace("2. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strLanguage = $consumer->GetResultValue("USER","USER_LANGUAGE");
			$strCountry = $consumer->GetResultValue("USER","USER_COUNTRY");
			$strTimezone = $consumer->GetResultValue("USER","USER_TIMEZONE");
			$this->Trace("USER_LANGUAGE = $strLanguage");
			$this->Trace("USER_COUNTRY = $strCountry");
			$this->Trace("USER_TIMEZONE = $strTimezone");
			if ($strLanguage != $strLanguage1)
			{
				$this->Trace("\"$strLanguage\" != \"$strLanguage1\"");
				return;	
			}
			if ($strCountry != $strCountry1)
			{
				$this->Trace("\"$strCountry\" != \"$strCountry1\"");
				return;	
			}
			if ($strTimezone != $strTimezoneDefault)
			{
				$this->Trace("\"$strTimezone\" != \"$strTimezoneDefault\"");
				return;	
			}
			$this->Trace("");
		
			
			$this->Trace("3. SET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["user_language"] = $strLanguage2;
			$arrayParams["user_country"] = $strCountry2;
			$arrayParams["user_timezone"] = $strTimezone2;
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
		
			$this->Trace("4. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strLanguage = $consumer->GetResultValue("USER","USER_LANGUAGE");
			$strCountry = $consumer->GetResultValue("USER","USER_COUNTRY");
			$strTimezone = $consumer->GetResultValue("USER","USER_TIMEZONE");
			$this->Trace("USER_LANGUAGE = $strLanguage");
			$this->Trace("USER_COUNTRY = $strCountry");
			$this->Trace("USER_TIMEZONE = $strTimezone");
			if ($strLanguage != $strLanguage2)
			{
				$this->Trace("\"$strLanguage\" != \"$strLanguage2\"");
				return;	
			}
			if ($strCountry != $strCountry2)
			{
				$this->Trace("\"$strCountry\" != \"$strCountry2\"");
				return;	
			}
			if ($strTimezone != $strTimezone2)
			{
				$this->Trace("\"$strTimezone\" != \"$strTimezone2\"");
				return;	
			}
			$this->Trace("");
		
		
		
		
			$this->Trace("5. SET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["user_language"] = $strLanguage3;
			$arrayParams["user_country"] = $strCountry3;
			$arrayParams["user_timezone"] = $strTimezone3;
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() == "")	
			{
				$this->Trace("An error was expected");		
				return;	
			}
			$this->Trace("Error: \"".$consumer->GetError()."\"");		
			$this->Trace("");
		
		
		
			$this->Trace("6. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strLanguage = $consumer->GetResultValue("USER","USER_LANGUAGE");
			$strCountry = $consumer->GetResultValue("USER","USER_COUNTRY");
			$strTimezone = $consumer->GetResultValue("USER","USER_TIMEZONE");
			$this->Trace("USER_LANGUAGE = $strLanguage");
			$this->Trace("USER_COUNTRY = $strCountry");
			$this->Trace("USER_TIMEZONE = $strTimezone");
			if ($strLanguage != $strLanguage2)
			{
				$this->Trace("\"$strLanguage\" != \"$strLanguage2\"");
				return;	
			}
			if ($strCountry != $strCountry2)
			{
				$this->Trace("\"$strCountry\" != \"$strCountry2\"");
				return;	
			}
			if ($strTimezone != $strTimezone2)
			{
				$this->Trace("\"$strTimezone\" != \"$strTimezone2\"");
				return;	
			}
		
			
			$this->Trace("");
		
		
		
		
			$this->Trace("7. SET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["user_language"] = "";
			$arrayParams["user_country"] = "";
			$arrayParams["user_timezone"] = $strTimezoneDefault;
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
		
			$this->Trace("8. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strLanguage = $consumer->GetResultValue("USER","USER_LANGUAGE");
			$strCountry = $consumer->GetResultValue("USER","USER_COUNTRY");
			$strTimezone = $consumer->GetResultValue("USER","USER_TIMEZONE");
			$this->Trace("USER_LANGUAGE = $strLanguage");
			$this->Trace("USER_COUNTRY = $strCountry");
			$this->Trace("USER_TIMEZONE = $strTimezone");
			if ($strLanguage != "")
			{
				$this->Trace("\"$strLanguage\" != \"\"");
				return;	
			}
			if ($strCountry != "")
			{
				$this->Trace("\"$strCountry\" != \"\"");
				return;	
			}
			if ($strTimezone != $strTimezoneDefault)
			{
				$this->Trace("\"$strTimezone\" != \"$strTimezoneDefault\"");
				return;	
			}
			$this->Trace("");

			$this->SetResult(true);
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			

			$this->Trace("DELETE USER:");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return false;	
			}		
			
			return true;
		}
		
		
	}
	
	

		
