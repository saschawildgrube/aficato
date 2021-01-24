<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	require_once(GetWDKDir()."wdk_hash.inc");

	/*
	
	1. Delete test user
	2. Add test user
	3a. Set password
	3b. Authenticate via password
	4a. Set passwordhash
	4b. Authenticate via passwordhash
	
	x. Delete test user
	
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
			parent::__construct("Web service system/user Changing a Password",$arrayConfig);
		}
		
		function OnInit()
		{
			$this->m_strUserName = "test-password";
			$this->SetVerbose(false);
			$this->RequireWebservice($this->m_strWebservice);			 
			return parent::OnInit();
		}
		
		function OnTest()
		{
			parent::OnTest();
			
			$strPassword1 = "password1";
			$strPassword2 = "password2";
			
			
			
			$consumer = new CWebServiceConsumerWebApplication($this);

			$this->Trace("Test User Webservice: Setting password");
		
		
			$this->Trace("1. DELETE USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $this->m_strUserName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			
		
		
			$this->Trace("2. ADD TEST USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $this->m_strUserName;
			$arrayParams["password"] = "changeme";
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->m_strUserID = $consumer->GetResultValue("NEW_USER_ID");
			$this->Trace("New USER ID = ".$this->m_strUserID);
			if ($this->m_strUserID == "")	
			{
				$this->Trace("Error: New user ID is empty!");		
				return;	
			}
		
		
			$this->Trace("3A. SET PASSWORD:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["password"] = $strPassword1;
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
			$this->Trace("3B. AUTHENTICATE VIA PASSWORD:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["authenticationpayload"] = $strPassword1;
			$arrayParams["command"] = "authenticate";
			
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
			$this->Trace("3C. AUTHENTICATE VIA PASSWORD:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["authenticationpayload"] = "bogus";
			$arrayParams["command"] = "authenticate";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "USER_AUTHENTICATION_FAILED")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
		
		
			$this->Trace("4A. SET PASSWORD:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$strHashSeed2 = MakePasswordHashSeed();
			$arrayParams["passwordhash"] = MakePasswordHash($strPassword2,$strHashSeed2);
			$arrayParams["passwordhash_seed"] = $strHashSeed2;
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
			$this->Trace("4B. AUTHENTICATE VIA PASSWORD:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["authenticationpayload"] = MakePasswordHash($strPassword2,$strHashSeed2);
			$arrayParams["command"] = "authenticate";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
		
			$this->Trace("4C. AUTHENTICATE VIA PASSWORD:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["authenticationpayload"] = "bogus";
			$arrayParams["command"] = "authenticate";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "USER_AUTHENTICATION_FAILED")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}


			$this->SetResult(true);
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			
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
	
	

		
