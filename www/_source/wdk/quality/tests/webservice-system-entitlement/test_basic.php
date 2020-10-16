<?php

	require_once(GetSourceDir()."webservices_directory.inc");

	/*
	
	1. Add testentitlement user
	2. Get all privileges
	3. Set test privilege
	4. get all privileges
	5. Delete test privilege
	6. get all privileges
	7. set test and premium privileges
	8. get all privileges
	9. deleteall privileges
	10. get all privileges
	11. delete testentitlement user
	
	*/

	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		private $m_strUserWebservice;
		private $m_strEntitlementWebservice;
		private $m_strUserID;

		
		function __construct()
		{
			$this->m_strUserWebservice = "system/user";
			$this->m_strEntitlementWebservice = "system/entitlement";
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service system/entitlement",$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->RequireWebservice($this->m_strUserWebservice);
			$this->RequireWebservice($this->m_strEntitlementWebservice);
			$this->SetVerbose(false);
			return parent::CallbackInit();;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$strTestUser = "testentitlement";
			
			$consumer = new CWebServiceConsumerWebApplication($this);


		
			$this->Trace("1A. DELETE USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $strTestUser;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strUserWebservice,$arrayParams);
		
		
		
			$this->Trace("1B. ADD USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $strTestUser;
			$arrayParams["password"] = "changeme";
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strUserWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->m_strUserID = $consumer->GetResultValue("NEW_USER_ID");
			$this->Trace("New USERID = ".$this->m_strUserID."");
		
		
		
		
			$this->Trace("2. GET PRIVILEGES:");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strEntitlementWebservice ,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$arrayPrivileges = $consumer->GetResultList();
			$this->Trace($arrayPrivileges);
		
		
		
			
			$this->Trace("3a. SET TEST PRIVILEGE:");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "set";
			$arrayParams["privilege_id"] = "TEST";
			$consumer->ConsumeWebService($this->m_strEntitlementWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
			$this->Trace("3b. SET TEST PRIVILEGE (2nd time):");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "set";
			$arrayParams["privilege_id"] = "TEST";
			$consumer->ConsumeWebService($this->m_strEntitlementWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
		
			$this->Trace("4. GET PRIVILEGES (2nd time):");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strEntitlementWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$arrayPrivileges = $consumer->GetResultList();
			$this->Trace($arrayPrivileges);
		
			$this->Trace("5. DELETE TEST PRIVILEGE:");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "delete";
			$arrayParams["privilege_id"] = "TEST";
			$consumer->ConsumeWebService($this->m_strEntitlementWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
		
		
			$this->Trace("6. GET PRIVILEGES (3rd time):");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strEntitlementWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$arrayPrivileges = $consumer->GetResultList();
			$this->Trace($arrayPrivileges);
		
		
			$this->Trace("7a. SET TEST PRIVILEGE:");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "set";
			$arrayParams["privilege_id"] = "TEST";
			$consumer->ConsumeWebService($this->m_strEntitlementWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
			$this->Trace("7b. SET PREMIUM PRIVILEGE:");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "set";
			$arrayParams["privilege_id"] = "PREMIUM";
			$consumer->ConsumeWebService($this->m_strEntitlementWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
			$this->Trace("8. GET PRIVILEGES (4th time):");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strEntitlementWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$arrayPrivileges = $consumer->GetResultList();
			$this->Trace($arrayPrivileges);
		
		
		
			$this->Trace("9. DELETEALL:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "deleteall";
			$consumer->ConsumeWebService($this->m_strEntitlementWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
			$this->Trace("10. GET PRIVILEGES (5th time):");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strEntitlementWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$arrayPrivileges = $consumer->GetResultList();
			$this->Trace($arrayPrivileges);
		
		
		
		
			$this->Trace("11. DELETE USER:");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strUserWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			
			$this->SetResult(true);
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			
			if ($this->m_strUserID != "")
			{
				$arrayParams = array();
				$arrayParams["trace"] = "1";
				$arrayParams["user_id"] = $this->m_strUserID;
				$arrayParams["command"] = "delete";
				$consumer->ConsumeWebService($this->m_strUserWebservice,$arrayParams);	
			}			
			return true;
		}
		
		
	}
	
	

		
