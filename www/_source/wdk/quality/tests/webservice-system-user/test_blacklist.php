<?php

	require_once(GetSourceDir()."webservices_directory.inc");

	/*
	
	1. Delete test user(s)
	2. Check user name (using a blacklisted name)
	3. Add test user (using a blacklisted name)
	4. Check user name (using an allowed user name)
	5. Add test user 
	6. Check user name (using an occupied username)
	7. Rename test user by name (using a blacklisted name)
	8. Delete test user
	
	*/

	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		private $m_strUserID;
		
		function __construct()
		{
			$this->m_strWebservice = "system/user";
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service system/user Test Blacklist",$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->RequireWebservice($this->m_strWebservice);	
			$this->SetVerbose(true);
			return parent::CallbackInit();
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			
			
			$consumer = new CWebServiceConsumerWebApplication($this);

			$strUserName = "test-blacklist";
			$strBlacklistedUserName = "blacklisttest";

		
		

		
		
			$this->Trace("1. DELETE TEST USER:");
			
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace("");



			$this->Trace("2. Check user name (using a blacklisted name):");
			$arrayParams = array();
			$arrayParams["user_name"] = $strBlacklistedUserName;
			$arrayParams["command"] = "checkusername";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "USERNAME_BLACKLISTED")	
			{
				$this->Trace("Unexpected Errorcode: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		

			$this->Trace("3. ADD TEST USER (USING A BLACKLISTED USERNAME):");
			$arrayParams = array();
			$arrayParams["user_name"] = $strBlacklistedUserName;
			$arrayParams["password"] = "changeme";
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "USERNAME_BLACKLISTED")	
			{
				$this->Trace("Unexpected Errorcode: \"".$consumer->GetError()."\"");		
				
				if ($consumer->GetError() == "")
				{
					$this->m_strUserID = $consumer->GetResultValue("NEW_USER_ID");
					$this->Trace("New USERID = ".$this->m_strUserID);
				}
				return;	
			}
			$this->Trace("");



			$this->Trace("4. Check user name (using an allowed user name):");
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["command"] = "checkusername";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Unexpected Errorcode: \"".$consumer->GetError()."\"");
				return;	
			}
			$this->Trace("");


			$this->Trace("5. ADD USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["password"] = "changeme";
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->m_strUserID = $consumer->GetResultValue("NEW_USER_ID");
			$this->Trace("New user id = ".$this->m_strUserID);
			if ($this->m_strUserID == "")	
			{
				$this->Trace("Error: New user ID is empty!");		
				return;	
			}
			$this->Trace("");


			$this->Trace("6. Check user name (using an occupied user name):");
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["command"] = "checkusername";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "USERNAME_OCCUPIED")	
			{
				$this->Trace("Unexpected Errorcode: \"".$consumer->GetError()."\"");
				return;	
			}
			$this->Trace("");


		
			$this->Trace("7. RENAME USER (USING A BLACKLISTED USERNAME):");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["new_user_name"] = $strBlacklistedUserName;
			$arrayParams["command"] = "rename";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "USERNAME_BLACKLISTED")	
			{
				$this->Trace("Unexpected Errorcode: \"".$consumer->GetError()."\"");		
				
				if ($consumer->GetError() == "")
				{
					$this->m_strUserID = $consumer->GetResultValue("NEW_USER_ID");
					$this->Trace("New user id = ".$this->m_strUserID);
				}
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
	
	

		
