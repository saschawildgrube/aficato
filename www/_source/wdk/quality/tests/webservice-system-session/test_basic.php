<?php

	require_once(GetSourceDir()."webservices_directory.inc");

	/*
			1. CREATE: Try to get a session ID
			2. WRITE: Write some data to the session
			3. READ: Compare written data
			4. DELETE TEST USER
			5. ADD USER
			6. LOGIN: Try to authenticate a user and bind to the session
			7. READ: To check authentication status
			8. LOGOUT: Log the user out
			9. READ: To check authentication status	
	*/

	
	class CTest extends CUnitTest
	{
		private $m_strWebserviceSession;
		private $m_strWebserviceUser;
		private $m_strTestUserName; 
		private $m_strSessionID;
		private $m_strUserID;
		
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service system/session",$arrayConfig);
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();

			$this->RequireWebservice("system/session");
			$this->RequireWebservice("system/user");
		
			$this->m_strWebserviceSession = "system/session";
			$this->m_strWebserviceUser = "system/user";
			$this->m_strTestUserName = "testsession";

			
			$this->SetVerbose(true);
			
			return true;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			
			
			$consumer = new CWebServiceConsumerWebApplication($this);
		
			$this->Trace("1. CREATE: Try to get a session ID");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["command"] = "create";
			$arrayParams["owner_id"] = "www_test";
			$arrayParams["duration"] = "60";
			$consumer->ConsumeWebService($this->m_strWebserviceSession,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}	
			$this->m_strSessionID = $consumer->GetResultValue("NEW_SESSION_ID");
		
		
		
			
			$this->Trace("2. WRITE: Write some data to the session?");
			$strTestData = "This is some test data";
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["command"] = "write";
			$arrayParams["owner_id"] = "www_test";
			$arrayParams["session_id"] = $this->m_strSessionID;
			$arrayParams["data"] = $strTestData;
			$consumer->ConsumeWebService($this->m_strWebserviceSession,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}	
		
		
			$this->Trace("3. READ: Compare written data");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["command"] = "read";
			//$arrayParams["owner_id"] = "www_test";
			$arrayParams["session_id"] = $this->m_strSessionID;
			$consumer->ConsumeWebService($this->m_strWebserviceSession,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}	
			$strData = $consumer->GetResultValue("SESSION","DATA");
			$this->Trace("Test Data:   \"$strTestData\"");
			$this->Trace("Stored Data: \"$strData\"");
		
		

			$this->Trace("4. DELETE TEST USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $this->m_strTestUserName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebserviceUser,$arrayParams);
		
		
			
			
			$this->Trace("5. ADD USER:");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["user_name"] = $this->m_strTestUserName;
			$arrayParams["password"] = "1";
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strWebserviceUser,$arrayParams);
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
		
		
		
		
			$this->Trace("6. LOGIN: Try to authenticate a user and bind to the session");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["command"] = "login";
			$arrayParams["owner_id"] = "www_test";
			$arrayParams["session_id"] = $this->m_strSessionID;
			$arrayParams["user_name"] = $this->m_strTestUserName;
			$arrayParams["authenticationpayload"] = "1";
			$consumer->ConsumeWebService($this->m_strWebserviceSession,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}	
			
		
			$this->Trace("7. READ: To check authentication status");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["command"] = "read";
			//$arrayParams["owner_id"] = "www_test";
			$arrayParams["session_id"] = $this->m_strSessionID; 
			$consumer->ConsumeWebService($this->m_strWebserviceSession,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}	
			$strUserID = $consumer->GetResultValue("SESSION","USER_ID_AUTHENTICATION");
			$this->Trace("USER_ID_AUTHENTICATION: \"$strUserID\"");
		
		
			$this->Trace("8. LOGOUT: Log the user out");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["command"] = "logout";
			$arrayParams["owner_id"] = "www_test";
			$arrayParams["session_id"] = $this->m_strSessionID;
			$consumer->ConsumeWebService($this->m_strWebserviceSession,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}	
		
		
			$this->Trace("9. READ: To check authentication status");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["command"] = "read";
			//$arrayParams["owner_id"] = "www_test";
			$arrayParams["session_id"] = $this->m_strSessionID;
			$consumer->ConsumeWebService($this->m_strWebserviceSession,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}	
			$strUserID = $consumer->GetResultValue("SESSION","USER_ID_AUTHENTICATION");
			$this->Trace("USER_ID_AUTHENTICATION: \"$strUserID\"");
		
		
			

	
	
			
	
			$this->SetResult(true);
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			
			$this->Trace("DESTROY SESSION:");		
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["command"] = "destroy";
			$arrayParams["session_id"] = $this->m_strSessionID;
			//$arrayParams["owner_id"] = "www_test";
			$consumer->ConsumeWebService($this->m_strWebserviceSession,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return false;	
			}	
		
		
			$this->Trace(" DELETE USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $this->m_strTestUserName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebserviceUser,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return false;	
			}
			
			return true;
		}
		
		
	}
	
	

		
