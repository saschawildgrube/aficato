<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");
	require_once(GetWDKDir()."wdk_hash.inc");

	/*
	
	1. Delete / Create new user
	2. Set user
	3. Get user
	4. Authenticate User by password
	5. Authenticate User by password hash
	6. Authenticate User to provoke an error
	7. Get user and check last authentication date/time
	8. Set user inactive
	9. Authenticate User by password which should fail
	
	
	*/


	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		
		private $m_strUserName;
		private $m_strUserID;
		
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service system/user",$arrayConfig);
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();

			$this->RequireWebservice("system/user");
			
			$this->m_strWebservice = "system/user";
			$this->m_strUserName = "test";
					
			$this->SetVerbose(true);
			//$this->SetActive(false);
			
			return true;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$consumer = new CWebServiceConsumerWebApplication($this);

		
			$this->Trace("1A. DELETE USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = "test";
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace("");
		
		
			$this->Trace("1B. ADD USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = "test";
			$arrayParams["password"] = "changeme";
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
		
			
			$this->Trace("2. SET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["passwordhash_seed"] = MakePasswordHashSeed();
			$arrayParams["passwordhash"] = MakePasswordHash("1",$arrayParams["passwordhash_seed"]);
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
		
			$this->Trace("3. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strPasswordHash = $consumer->GetResultValue("USER","PASSWORDHASH");
			$strPasswordHashSeed = $consumer->GetResultValue("USER","PASSWORDHASH_SEED");
		
			$bMatch = VerifyPassword("1",$strPasswordHash,$strPasswordHashSeed);
		
			$this->Trace((($bMatch)?("MATCH!"):("MATCH FAILED!")));
			$this->Trace("");
		
			$this->Trace("4. AUTHENTICATE USER BY PASSWORD:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "authenticate";
			$arrayParams["authenticationpayload"] = "1";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
		
			$this->Trace("5. AUTHENTICATE USER BY PASSWORDHASH:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "authenticate"; 
			$arrayParams["authenticationpayload"] = MakePasswordHash("1",$strPasswordHashSeed);
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace($consumer->GetServiceOutput()) . "</pre>";
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
					
			$this->Trace("6. AUTHENTICATE USER / PROVOKE AN ERROR:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "authenticate";
			$arrayParams["authenticationpayload"] = "bogus";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "USER_AUTHENTICATION_FAILED")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\" should be USER_AUTHENTICATION_FAILED");
				return;	
			}
			$this->Trace("");

			$this->Trace("7. GET USER and check last authentication");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			
			$strDateTimeLastAuthentication = $consumer->GetResultValue("USER","LASTAUTHENTICATION_DATETIME");
			$timeLastAuthentication = GetTime($strDateTimeLastAuthentication);
			$timeNow = GetTimeNow();
			
			$this->Trace("Last Authentication: $strDateTimeLastAuthentication ($timeLastAuthentication)");
			$this->Trace("Now                : ".RenderDateTime($timeNow)." ($timeNow)");
			
			if ( ($timeNow - $timeLastAuthentication) > 10)
			{
				$this->Trace("Error: Time difference is bigger than 10 seconds.");
				return;	
			}
			
			
			
				
			$this->Trace("8. SET USER INACTIVE:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["user_active"] = "0";
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
			$this->Trace("9. AUTHENTICATE USER BY PASSWORD:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "authenticate";
			$arrayParams["authenticationpayload"] = "1";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() == "")	
			{
				$this->Trace("Error: Authentication did not fail.");		
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
	
	

		
