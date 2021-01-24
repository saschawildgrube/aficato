<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	/*
	
	1. Delete test user(s)
	2. Add test user
	3. Rename test user by id
	4. Get test user
	5. Rename test user by name
	6. Get test user
	7. Delete test user
	
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
			parent::__construct("Web service system/user Rename",$arrayConfig);
		}
		
		function OnInit()
		{
			$this->RequireWebservice($this->m_strWebservice);
			return parent::OnInit();
		}
		
		function OnTest()
		{
			parent::OnTest();
			
			
			
			$consumer = new CWebServiceConsumerWebApplication($this);




			$strUserName = "test-rename";
			$strUserName2 = "test-rename-renamed";
			$strUserName3 = "test-rename-renamed-reloaded";
		
			
		
		

		
		
			$this->Trace("1. DELETE USER:");
			
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			
			
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName2;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			
		
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName3;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			
			$this->Trace("");
		
			$this->Trace("2. ADD USER:");
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
			$this->Trace("NEW USER ID = ".$this->m_strUserID);
			if ($this->m_strUserID == "")	
			{
				$this->Trace("Error: New user ID is empty!");		
				return;	
			}
			$this->Trace("");
		
			$this->Trace("3. RENAME USER BY ID:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["new_user_name"] = $strUserName2;
			$arrayParams["command"] = "rename";
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
			$strUserNameCheck = $consumer->GetResultValue("USER","USER_NAME");
			if ($strUserNameCheck != $strUserName2)	
			{
				$this->Trace("\"$strUserNameCheck\" != \"$strUserName2\"");		
				return;	
			}
			$this->Trace("");
		
		
			$this->Trace("5. RENAME USER BY NAME:");
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName2;
			$arrayParams["new_user_name"] = $strUserName3;
			$arrayParams["command"] = "rename";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
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
			$strUserNameCheck = $consumer->GetResultValue("USER","USER_NAME");
			if ($strUserNameCheck != $strUserName3)	
			{
				$this->Trace("\"$strUserNameCheck\" != \"$strUserName3\"");		
				return;	
			}
			$this->Trace("");

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
	
	

		
